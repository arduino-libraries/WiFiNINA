/*
  This file is part of the WiFiNINA library.
  Copyright (c) 2018 Arduino SA. All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

/*
  Created on: Mar 6, 2024
  Author: gershnik
*/

#ifndef HEADER_WIFISOCKET_H_INCLUDED
#define HEADER_WIFISOCKET_H_INCLUDED

#include <stdint.h>

#include <IPAddress.h>

#include "utility/socket_drv.h"


/**
 * A plain socket
 * 
 * This class owns the underlying socket lifetime. It is movable and
 * move assignable but not copyable or copy assignable.
*/
class WiFiSocket {
private:
    static constexpr uint8_t s_invalidHandle = 255;
public:
    /**
     * Socket type
     * 
     * Semantics equivalent to Unix sockets SOCK_STREAM, SOCK_DGRAM etc.
    */
    enum class Type: uint8_t {
        //see lwip/sockets.h SOCK_STREAM etc. for values
        Stream  = 1,
        DGram   = 2,
        Raw     = 3
    };

    /**
     * Socket protocol
     * 
     * Semantics equivalent to Unix sockets IPPROTO_TCP etc.
     * Not all values are implemented/supported in firmware
    */
    enum Protocol : uint8_t {
        //see lwip/sockets.h IPPROTO_IP etc.
        IP      = 0,
        ICMP    = 1,
        TCP     = 6,
        UDP     = 17,
        IPv6    = 41, 
        ICMPv6  = 58,
        UDPLite = 136,
        Raw     = 255
    };

    /**
     * Socket state bitmask
     * 
     * These values can be obtained via poll()
    */
    enum State : uint8_t {
        Readable    = SocketDrv::SocketReadable,
        Writable    = SocketDrv::SocketWritable,
        ErroredOut  = SocketDrv::SocketErroredOut
    };

    /**
     * Custom error codes for lastError()
     * 
     * These represent issues communicating with NINA chip
     * and won't clash with errno codes returned from 
     * socket calls.
    */
    enum class Error : uint8_t {
        SpiFailure = SocketDrv::Failure
    };

private:
    /// Any numeric/boolean valued socket option
    template<uint32_t Name, class ExternalType, class StorageType = ExternalType> 
    class ValueOption {
        static_assert(sizeof(ExternalType) <= sizeof(StorageType));

        friend WiFiSocket;
    public:
        static constexpr uint32_t name = Name;  

        ValueOption() : m_value(0) {}
        explicit ValueOption(ExternalType val): m_value(StorageType(val)) {}

        explicit operator ExternalType() const 
            { return ExternalType(m_value);}
        ExternalType value() const 
            { return ExternalType(m_value);}

    private:
        const void * data() const { return &m_value; }
        void * data() { return &m_value; }
        uint8_t size() const { return sizeof(m_value); }

        void resize(uint8_t size) {}
    private:
        StorageType m_value;
    };

    /// Any struct-valued socket option
    template<uint32_t Name, class Struct> 
    class StructOption : public Struct {
        friend WiFiSocket;
    public:
        static constexpr uint32_t name = Name;  

        StructOption() : Struct{} {}
        StructOption(const Struct & src): Struct(src) {}

    private:
        const void * data() const { return static_cast<const Struct *>(this); }
        void * data() { return static_cast<Struct *>(this); }
        uint8_t size() const { return sizeof(Struct); }

        void resize(uint8_t size) {}
    };

public:
    struct TimeVal {
        long long	tv_sec;		
	    long	    tv_usec;
    };

    /// Available socket options types
    struct Option {
        //see lwip/sockets.h SO_REUSEADDR etc.

        /// SO_REUSEADDR option
        using ReuseAddress          = ValueOption<0x0004, bool, uint32_t>;
        /// SO_KEEPALIVE option
        using KeepAlive             = ValueOption<0x0008, bool, uint32_t>;
        /// SO_BROADCAST option
        using Broadcast             = ValueOption<0x0020, bool, uint32_t>;

        /// SO_ACCEPTCONN option
        using AcceptsConnections    = ValueOption<0x0002, bool, uint32_t>;
        /// SO_RCVBUF option
        using RecvBufferSize        = ValueOption<0x1002, uint32_t>;
        /// SO_SNDTIMEO option
        using SendTimeout           = StructOption<0x1005, TimeVal>; 
        /// SO_RCVTIMEO option
        using RecvTimeout           = StructOption<0x1006, TimeVal>;
        /// SO_ERROR option
        using GetAndClearError      = ValueOption<0x1007, uint8_t, uint32_t>;
        /// SO_TYPE option
        using SocketType            = ValueOption<0x1008, WiFiSocket::Type, uint32_t>;
        /// SO_NO_CHECK option
        using NoUdpChecksum         = ValueOption<0x100a, bool, uint32_t>;
    };

private:
    enum class IOControl : uint32_t {
        //see lwip/sockets.h
        NRead           = 0x4004667F, //FIONREAD
        NonBlockingIO   = 0x8004667E  //FIONBIO
    };
public:
    /**
     * Retrieves error (if any) of the last method call
     * 
     * Last error is always set, whether the call failed or succeeded.
     * 
     * The returned value is either a standard errno value from
     * the underlying socket call or one of the Error enumeration values
     * Their ranges are guaranteed to be distinct.
     * In case of success the value is 0.
    */
    static uint8_t lastError()
        { return SocketDrv::lastError(); }

    /// Creates an invalid socket
    WiFiSocket() = default;

    /**
     * Creates a socket
     * 
     * This is equivalent to socket() API call.
     * In case of failure the socket is created as invalid which
     * can be tested via operator bool.
    */
    WiFiSocket(Type type, Protocol proto):
        m_handle(SocketDrv::socket(uint8_t(type), uint8_t(proto)))
    {}

    /// Closes the socket
    ~WiFiSocket() {
        close();
    }

    /**
     * Manually close the socket
     * 
     * This makes this object an invalid socket
    */ 
    void close();

    /**
     * Moving a socket
     * 
     * The source socket is left in an invalid state
    */
    WiFiSocket(WiFiSocket && src): m_handle(src.m_handle) { 
        src.m_handle = s_invalidHandle; 
    }

    /**
     * Move-assigning a socket
     * 
     * The source socket is left in an invalid state
    */
    WiFiSocket & operator=(WiFiSocket && src) {
        if (this != &src) {
            close();
            m_handle = src.m_handle;
            src.m_handle = s_invalidHandle; 
        }
        return *this;
    }

    /**
     * Tests whether the socket is invalid.
     * 
     * A socket is in an invalid state when it represents "no socket".
     * A valid socket never becomes invalid unless it is moved out or closed. 
     * Similarly an invalid socket never becomes valid
     * unless moved-in from a valid socket.
     * 
    */
    explicit operator bool() const 
        { return m_handle != s_invalidHandle; }

    /**
     * Binds a socket to given port
     * @returns success flag. Check lastError() for more information about failure
    */
    bool bind(uint16_t port)
        { return SocketDrv::bind(m_handle, port); }

    /**
     * Starts listening for incoming connections
     * @returns success flag. Check lastError() for more information about failure
    */
    bool listen(uint8_t backlog)
        { return SocketDrv::listen(m_handle, backlog); }

    /**
     * Accepts an incoming connection
     * 
     * @returns a valid socket, if successful or invalid otherwise. Check lastError()
     * for more information about the failure.  
     * 
     * @param remoteIpAddress if successful populated by the address of the remote client
     * @param remotePort if successful populated by the port of the remote client
    */
    WiFiSocket accept(arduino::IPAddress & remoteIpAddress, uint16_t & remotePort)
        { return WiFiSocket(SocketDrv::accept(m_handle, remoteIpAddress, remotePort)); }

    /**
     * Connects a socket to remote endpoint
     * 
     * @returns success flag. Check lastError() for more information about failure
     * @param ipAddress host to connect to
     * @param port port to connect to
    */
    bool connect(const arduino::IPAddress & ipAddress, uint16_t port)
        { return SocketDrv::connect(m_handle, ipAddress, port); }


    /**
     * Sends data to remote endpoint
     * 
     * @return the amount of data actually sent or -1 on failure. Check lastError() 
     * for more information about failure. The type of the return value is int32_t 
     * to accommodate -1. When non-negative it will never be bigger than the size parameter.
    */
    int32_t send(const void * buf, uint16_t size);

    /**
     * Receives data from remote endpoint
     * 
     * @return the amount of data actually read or -1 on failure. Check lastError() 
     * for more information about failure. The type of the return value is int32_t 
     * to accommodate -1. When non-negative it will never be bigger than the size parameter.
    */
    int32_t recv(void * buf, uint16_t size);

    /**
     * Sends data to remote endpoint
     * 
     * @return the amount of data actually sent or -1 on failure. Check lastError() 
     * for more information about failure. The type of the return value is int32_t 
     * to accommodate -1. When non-negative it will never be bigger than the size parameter.
    */
    int32_t sendTo(const void * buf, uint16_t size, const arduino::IPAddress & ipAddress, uint16_t port);

    /**
     * Receives data from remote endpoint
     * 
     * @return the amount of data actually read or -1 on failure. Check lastError() 
     * for more information about failure. The type of the return value is int32_t 
     * to accommodate -1. When non-negative it will never be bigger than the size parameter.
    */
    int32_t recvFrom(void * buf, uint16_t size, arduino::IPAddress & remoteIpAddress, uint16_t & remotePort);
    
    /** 
     * Sets the socket into non-blocking or blocking mode
     * 
     * This is equivalent to ioctl(...FIONBIO...)
     * 
     * @returns success flag. Check lastError() for more information about failure
    */
    bool setNonBlocking(bool val);

    /** 
     * Retrieves the number of bytes available for reading.
     * 
     * This is equivalent to ioctl(...FIONREAD...)
     * 
     * @returns success flag. Check lastError() for more information about failure
    */
    int32_t availableToRead() const;

    /**
     * Retrieves current socket state bitmask
     * 
     * See State enum for possible flags. This call is similar in semantics to 
     * calling select() on the socket.
     * 
     * @returns success flag. Check lastError() for more information about failure
     * 
    */
    bool poll(State & state) const;

    /**
     * Sets a socket option
     * 
     * Only a small subset of SO_SOCKET options are supported.
     * These are represented by strongly typed values of types
     * declared in Option struct.
     * 
     * For example here is how to set ReuseAddress option:
     * 
     * ```
     *   if (socket.setOption(WiFiSocket::Option::ReuseAddress(true))) {
     *       ...
     *   }
     * ```
     * @tparam Option one of types declared in WiFiSocket::Option 
     * @param option value to set
     * @returns success flag. Check lastError() for more information about failure
    */
    template<class Option>
    inline bool setOption(const Option & opt) {
        return SocketDrv::setsockopt(m_handle, opt.name, opt.data(), opt.size());
    }

    /**
     * Reads a socket option
     * 
     * Only a small subset of SO_SOCKET options are supported.
     * These are represented by strongly typed values of types
     * declared in Option struct.
     * 
     * For example here is how to read ReuseAddress option:
     * 
     * ```
     *   WiFiSocket::Option::ReuseAddress reuseAddress;
     *   if (socket.setOption(reuseAddress)) {
     *       ...
     *   }
     * ```
     * 
     * @tparam Option one of types declared in WiFiSocket::Option 
     * @param option value to fill in
     * @returns success flag. Check lastError() for more information about failure
    */
    template<class Option>
    inline bool getOption(Option & opt) {
        uint8_t size = opt.size();
        bool ret = SocketDrv::getsockopt(m_handle, opt.name, opt.data(), size);
        if (ret)
            opt.resize(size);
        return ret;
    }

    bool getPeerName(arduino::IPAddress & remoteIpAddress, uint16_t & remotePort)
        { return SocketDrv::getPeerName(m_handle, remoteIpAddress, remotePort); }

    /**
     * Retrieves underlying socket handle
     * 
     * This is for debugging purposes only. 
    */
    uint8_t handle() const 
        { return m_handle; }
private:
    explicit WiFiSocket(uint8_t handle): m_handle(handle)
    {}
private:
    uint8_t m_handle = s_invalidHandle;
};

#endif