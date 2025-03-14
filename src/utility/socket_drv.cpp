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

#include <stdint.h>
#include <stddef.h>
#include <errno.h>

#include "socket_drv.h"
#include "spi_drv.h"

extern "C" {
    #include "wifi_spi.h"
    #include "wl_types.h"
    #include "debug.h"
}

namespace {
    class SelectSlave {
    public:
        SelectSlave() {
            SpiDrv::waitForSlaveReady();
            SpiDrv::spiSlaveSelect();
        }
        ~SelectSlave() {
            SpiDrv::spiSlaveDeselect();
        }

        SelectSlave(const SelectSlave &) = delete;
        SelectSlave & operator=(const SelectSlave &) = delete;
    };

    struct LastError {
        bool present = false;
        uint8_t value = 0;

        void reset() 
            { this->present = false; }
        void operator=(uint8_t val) {
            this->present = true;
            this->value = val;
        }
        operator bool() const 
            { return this->present; }
    };
}

static LastError g_lastError = {};


uint8_t SocketDrv::socket(uint8_t type, uint8_t proto) {

    g_lastError.reset();
    if (!SpiDrv::initialized)
		SpiDrv::begin();
	

    uint8_t cmd = SOCKET_SOCKET_CMD;
    {
        SelectSlave sel;
        
        // Send Command
        int commandSize = 4;
        SpiDrv::sendCmd(cmd, PARAM_NUMS_2);
        SpiDrv::sendParam(&type, sizeof(type), NO_LAST_PARAM);
        commandSize += (1 + sizeof(type));
        SpiDrv::sendParam(&proto, sizeof(proto), LAST_PARAM);
        commandSize += (1 + sizeof(proto));

        // pad to multiple of 4
        while (commandSize % 4) {
            SpiDrv::readChar();
            commandSize++;
        }
    }
    {
        //Wait the reply elaboration
        SelectSlave sel;
    
        // Wait for reply
        uint8_t data = 0;
        uint8_t dataLen = 0;
        if (!SpiDrv::waitResponseCmd(cmd, PARAM_NUMS_1, &data, &dataLen)) {
            WARN("error waitResponse");
            g_lastError = SocketDrv::Failure;
            return 255;
        }

        if (data != 255) //255  on remote failure
            g_lastError = 0;
        return data; 
    }
}

bool SocketDrv::close(uint8_t s) {

    g_lastError.reset();
    if (!SpiDrv::initialized)
		SpiDrv::begin();

    uint8_t cmd = SOCKET_CLOSE_CMD;
    {
        SelectSlave sel;
        
        // Send Command
        int commandSize = 4;
        SpiDrv::sendCmd(cmd, PARAM_NUMS_1);
        SpiDrv::sendParam(&s, sizeof(s), LAST_PARAM);
        commandSize += (1 + sizeof(s));

        // pad to multiple of 4
        while (commandSize % 4) {
            SpiDrv::readChar();
            commandSize++;
        }
    }

    {
        //Wait the reply elaboration
        SelectSlave sel;
    
        // Wait for reply
        uint8_t data = 0;
        uint8_t dataLen = 0;
        if (!SpiDrv::waitResponseCmd(cmd, PARAM_NUMS_1, &data, &dataLen)) {
            WARN("error waitResponse");
            g_lastError = SocketDrv::Failure;
            return false;
        }

        if (data != 0) {
            g_lastError = 0;
            return true;
        }
        return false;
    }
}

uint8_t SocketDrv::lastError() {

    if (g_lastError)
        return g_lastError.value;

    if (!SpiDrv::initialized)
		SpiDrv::begin();

    uint8_t cmd = SOCKET_ERRNO_CMD;
    {
        SelectSlave sel;
        
        // Send Command
        SpiDrv::sendCmd(cmd, PARAM_NUMS_0);
    }
    {
        //Wait the reply elaboration
        SelectSlave sel;

        // Wait for reply
        uint8_t data = 0;
        uint8_t dataLen = 0;
        if (!SpiDrv::waitResponseCmd(cmd, PARAM_NUMS_1, &data, &dataLen)) {
            WARN("error waitResponse");
            g_lastError = SocketDrv::Failure;
            return SocketDrv::Failure;
        }

        g_lastError = data;
        return data;
    }
}

bool SocketDrv::bind(uint8_t s, uint16_t port) {

    g_lastError.reset();
    if (!SpiDrv::initialized)
		SpiDrv::begin();

    uint8_t cmd = SOCKET_BIND_CMD;
    {
        SelectSlave sel;
        
        // Send Command
        int commandSize = 4;
        SpiDrv::sendCmd(cmd, PARAM_NUMS_2);
        SpiDrv::sendParam(&s, sizeof(s), NO_LAST_PARAM);
        commandSize += (1 + sizeof(s));
        SpiDrv::sendParam(port, LAST_PARAM);
        commandSize += (1 + sizeof(port));

        // pad to multiple of 4
        while (commandSize % 4) {
            SpiDrv::readChar();
            commandSize++;
        }
    }
    {
        //Wait the reply elaboration
        SelectSlave sel;
    
        // Wait for reply
        uint8_t data = 0;
        uint8_t dataLen = 0;
        if (!SpiDrv::waitResponseCmd(cmd, PARAM_NUMS_1, &data, &dataLen)) {
            WARN("error waitResponse");
            g_lastError = SocketDrv::Failure;
            return false;
        }

        if (data != 0) {
            g_lastError = 0;
            return true;
        }
        return false;
    }
}

bool SocketDrv::listen(uint8_t s, uint8_t backlog) {
    g_lastError.reset();
    if (!SpiDrv::initialized)
		SpiDrv::begin();

    uint8_t cmd = SOCKET_LISTEN_CMD;
    {
        SelectSlave sel;
        
        // Send Command
        int commandSize = 4;
        SpiDrv::sendCmd(cmd, PARAM_NUMS_2);
        SpiDrv::sendParam(&s, sizeof(s), NO_LAST_PARAM);
        commandSize += (1 + sizeof(s));
        SpiDrv::sendParam(&backlog, sizeof(backlog), LAST_PARAM);
        commandSize += (1 + sizeof(backlog));

        // pad to multiple of 4
        while (commandSize % 4) {
            SpiDrv::readChar();
            commandSize++;
        }
    }
    {
        //Wait the reply elaboration
        SelectSlave sel;
    
        // Wait for reply
        uint8_t data = 0;
        uint8_t dataLen = 0;
        if (!SpiDrv::waitResponseCmd(cmd, PARAM_NUMS_1, &data, &dataLen)) {
            WARN("error waitResponse");
            g_lastError = SocketDrv::Failure;
            return false;
        }

        if (data != 0) {
            g_lastError = 0;
            return true;
        }
        return false;
    }
}

uint8_t SocketDrv::accept(uint8_t s, arduino::IPAddress & remoteIpAddress, uint16_t & remotePort) {
    g_lastError.reset();
    if (!SpiDrv::initialized)
		SpiDrv::begin();

    uint8_t cmd = SOCKET_ACCEPT_CMD;
    {
        SelectSlave sel;
        
        // Send Command
        int commandSize = 4;
        SpiDrv::sendCmd(cmd, PARAM_NUMS_1);
        SpiDrv::sendParam(&s, sizeof(s), LAST_PARAM);
        commandSize += (1 + sizeof(s));
        
        // pad to multiple of 4
        while (commandSize % 4) {
            SpiDrv::readChar();
            commandSize++;
        }
    }
    {
        //Wait the reply elaboration
        SelectSlave sel;
    
        // Wait for reply
        uint8_t resultSocket;
        uint8_t remoteIpBuf[4];
        uint16_t remotePortTmp;
        tParam params[PARAM_NUMS_3] = { 
            {0, (char*)&resultSocket}, 
            {0, (char*)&remoteIpBuf},  
            {0, (char*)&remotePortTmp}
        };
        if (!SpiDrv::waitResponseParams(cmd, PARAM_NUMS_3, params)) {
            WARN("error waitResponse");
            g_lastError = SocketDrv::Failure;
            return 255;
        }

        if (resultSocket != 255) {
            g_lastError = 0;
            remoteIpAddress = IPAddress(arduino::IPv4, remoteIpBuf); //buf is in network order
            remotePort = remotePortTmp; //port is host order
        }
        return resultSocket;
    }
}

bool SocketDrv::connect(uint8_t s, const arduino::IPAddress & ipAddress, uint16_t port) {

    if (ipAddress.type() != arduino::IPv4) {
        g_lastError = EINVAL;
        return false;
    }
    auto ipv4Address = uint32_t(ipAddress);

    g_lastError.reset();
    if (!SpiDrv::initialized)
		SpiDrv::begin();

    uint8_t cmd = SOCKET_CONNECT_CMD;

    {
        SelectSlave sel;
        
        // Send Command
        int commandSize = 4;
        SpiDrv::sendCmd(cmd, PARAM_NUMS_3);
        SpiDrv::sendParam(&s, sizeof(s), NO_LAST_PARAM);
        commandSize += (1 + sizeof(s));
        SpiDrv::sendParam((uint8_t *)&ipv4Address, sizeof(ipv4Address), NO_LAST_PARAM);
        commandSize += (1 + sizeof(ipv4Address));
        SpiDrv::sendParam(port, LAST_PARAM);
        commandSize += (1 + sizeof(port));
        
        // pad to multiple of 4
        while (commandSize % 4) {
            SpiDrv::readChar();
            commandSize++;
        }
    }
    {
        //Wait the reply elaboration
        SelectSlave sel;
    
        // Wait for reply
        uint8_t data = 0;
        uint8_t dataLen = 0;
        if (!SpiDrv::waitResponseCmd(cmd, PARAM_NUMS_1, &data, &dataLen)) {
            WARN("error waitResponse");
            g_lastError = SocketDrv::Failure;
            return false;
        }

        if (data != 0) {
            g_lastError = 0;
            return true;
        }
        return false;
    }
}

int32_t SocketDrv::send(uint8_t s, const void * buf, uint16_t size) {
    g_lastError.reset();
    if (!SpiDrv::initialized)
		SpiDrv::begin();

    //keep the size well below 4096 which seems to be SPI_MAX_DMA_LEN
    if (size > 4000)
        size = 4000;

    uint8_t cmd = SOCKET_SEND_CMD;

    {
        SelectSlave sel;
        
        // Send Command
        int commandSize = 4;
        SpiDrv::sendCmd(cmd, PARAM_NUMS_2);
        SpiDrv::sendBuffer(&s, sizeof(s), NO_LAST_PARAM);
        commandSize += (2 + sizeof(s));
        SpiDrv::sendBuffer((uint8_t *)buf, size, LAST_PARAM);
        commandSize += (2 + size);
        
        // pad to multiple of 4
        while (commandSize % 4) {
            SpiDrv::readChar();
            commandSize++;
        }
    }
    {
        //Wait the reply elaboration
        SelectSlave sel;
    
        // Wait for reply
        uint8_t data[2] = {};
        uint8_t dataLen = 0;
        if (!SpiDrv::waitResponseCmd(cmd, PARAM_NUMS_1, data, &dataLen)) {
            WARN("error waitResponse");
            g_lastError = SocketDrv::Failure;
            return -1;
        }

        uint16_t ret = (uint16_t(data[0]) << 8) | uint16_t(data[1]);
        if (ret)
            g_lastError = 0;
        return ret;
    }
}

int32_t SocketDrv::recv(uint8_t s, void * buf, uint16_t size) {
    g_lastError.reset();
    if (!SpiDrv::initialized)
		SpiDrv::begin();

    uint8_t cmd = SOCKET_RECV_CMD;

    {
        SelectSlave sel;
        
        // Send Command
        int commandSize = 4;
        SpiDrv::sendCmd(cmd, PARAM_NUMS_2);
        SpiDrv::sendParam(&s, sizeof(s), NO_LAST_PARAM);
        commandSize += (1 + sizeof(s));
        SpiDrv::sendParam((uint8_t*)&size, sizeof(size), LAST_PARAM);
        commandSize += (1 + sizeof(size));
        
        // pad to multiple of 4
        while (commandSize % 4) {
            SpiDrv::readChar();
            commandSize++;
        }
    }
    {
        //Wait the reply elaboration
        SelectSlave sel;
    
        // Wait for reply
        uint16_t dataLen = 0;
        if (!SpiDrv::waitResponseData16(cmd, (uint8_t*)buf, &dataLen)) {
            WARN("error waitResponse");
            g_lastError = SocketDrv::Failure;
            return -1;
        }

        if (dataLen)
            g_lastError = 0;
        return dataLen;
    }
}

int32_t SocketDrv::sendTo(uint8_t s, const void * buf, uint16_t size, 
                          const arduino::IPAddress & ipAddress, uint16_t port) {

    if (ipAddress.type() != arduino::IPv4) {
        g_lastError = EINVAL;
        return false;
    }
    auto ipv4Address = uint32_t(ipAddress);
    uint8_t portNetOrder[2] = {uint8_t(port >> 8), uint8_t(port)};
    
    g_lastError.reset();
    if (!SpiDrv::initialized)
		SpiDrv::begin();

    //keep the size well below 4096 which seems to be SPI_MAX_DMA_LEN
    if (size > 4000)
        size = 4000;

    uint8_t cmd = SOCKET_SENDTO_CMD;

    {
        SelectSlave sel;
        
        // Send Command
        int commandSize = 4;
        SpiDrv::sendCmd(cmd, PARAM_NUMS_4);
        SpiDrv::sendBuffer(&s, sizeof(s), NO_LAST_PARAM);
        commandSize += (2 + sizeof(s));
        SpiDrv::sendBuffer((uint8_t *)&ipv4Address, sizeof(ipv4Address), NO_LAST_PARAM);
        commandSize += (2 + sizeof(ipv4Address));
        SpiDrv::sendBuffer(portNetOrder, sizeof(portNetOrder), NO_LAST_PARAM);
        commandSize += (2 + sizeof(portNetOrder));
        SpiDrv::sendBuffer((uint8_t *)buf, size, LAST_PARAM);
        commandSize += (2 + size);
        
        // pad to multiple of 4
        while (commandSize % 4) {
            SpiDrv::readChar();
            commandSize++;
        }
    }
    {
        //Wait the reply elaboration
        SelectSlave sel;
    
        // Wait for reply
        uint8_t data[2] = {};
        uint8_t dataLen = 0;
        if (!SpiDrv::waitResponseCmd(cmd, PARAM_NUMS_1, data, &dataLen)) {
            WARN("error waitResponse");
            g_lastError = SocketDrv::Failure;
            return -1;
        }

        uint16_t ret = (uint16_t(data[0]) << 8) | uint16_t(data[1]);
        if (ret)
            g_lastError = 0;
        return ret;
    }
}

int32_t SocketDrv::recvFrom(uint8_t s, void * buf, uint16_t size, 
                            arduino::IPAddress & remoteIpAddress, uint16_t & remotePort) {
    g_lastError.reset();
    if (!SpiDrv::initialized)
		SpiDrv::begin();

    //keep the size well below 4096 which seems to be SPI_MAX_DMA_LEN
    if (size > 4000)
        size = 4000;

    uint8_t cmd = SOCKET_RECVFROM_CMD;

    {
        SelectSlave sel;
        
        // Send Command
        int commandSize = 4;
        SpiDrv::sendCmd(cmd, PARAM_NUMS_2);
        SpiDrv::sendParam(&s, sizeof(s), NO_LAST_PARAM);
        commandSize += (1 + sizeof(s));
        SpiDrv::sendParam((uint8_t*)&size, sizeof(size), LAST_PARAM);
        commandSize += (1 + sizeof(size));
        
        // pad to multiple of 4
        while (commandSize % 4) {
            SpiDrv::readChar();
            commandSize++;
        }
    }
    {
        //Wait the reply elaboration
        SelectSlave sel;
    
        // Wait for reply
        uint8_t remoteIpBuf[4];
        uint16_t remotePortTmp;
        tDataParam params[PARAM_NUMS_3] = { 
            {0, (char*)&remoteIpBuf},  
            {0, (char*)&remotePortTmp},
            {0, (char*)buf}
        };
        if (!SpiDrv::waitResponseParams(cmd, PARAM_NUMS_3, params)) {
            WARN("error waitResponse");
            g_lastError = SocketDrv::Failure;
            return -1;
        }

        if (params[2].dataLen) {
            g_lastError = 0;
            remoteIpAddress = IPAddress(arduino::IPv4, remoteIpBuf); //buf is in network order
            remotePort = remotePortTmp; //port is in host order
        }

        return params[2].dataLen;
    }
}
    

uint8_t SocketDrv::ioctl(uint8_t s, uint32_t code, void * buf, uint8_t bufSize) {

    g_lastError.reset();
    if (!SpiDrv::initialized)
		SpiDrv::begin();

    uint8_t cmd = SOCKET_IOCTL_CMD;

    {
        SelectSlave sel;
        
        // Send Command
        int commandSize = 4;
        SpiDrv::sendCmd(cmd, PARAM_NUMS_3);
        SpiDrv::sendParam(&s, sizeof(s), NO_LAST_PARAM);
        commandSize += (1 + sizeof(s));
        SpiDrv::sendParam((uint8_t*)&code, sizeof(code), NO_LAST_PARAM);
        commandSize += (1 + sizeof(code));
        SpiDrv::sendParam((uint8_t *)buf, bufSize, LAST_PARAM);
        commandSize += (1 + bufSize);
        
        // pad to multiple of 4
        while (commandSize % 4) {
            SpiDrv::readChar();
            commandSize++;
        }
    }
    {
        //Wait the reply elaboration
        SelectSlave sel;
    
        // Wait for reply
        uint8_t dataLen = 0;
        if (!SpiDrv::waitResponseCmd(cmd, PARAM_NUMS_1, (uint8_t*)buf, &dataLen)) {
            WARN("error waitResponse");
            g_lastError = SocketDrv::Failure;
            return 0;
        }

        if (dataLen != 0)
            g_lastError = 0;
        return dataLen;
    }
}

SocketDrv::SocketState SocketDrv::poll(uint8_t s) {
    g_lastError.reset();
    if (!SpiDrv::initialized)
		SpiDrv::begin();

    uint8_t cmd = SOCKET_POLL_CMD;

    {
        SelectSlave sel;
        
        // Send Command
        int commandSize = 4;
        SpiDrv::sendCmd(cmd, PARAM_NUMS_1);
        SpiDrv::sendParam(&s, sizeof(s), LAST_PARAM);
        commandSize += (1 + sizeof(s));
        
        // pad to multiple of 4
        while (commandSize % 4) {
            SpiDrv::readChar();
            commandSize++;
        }
    }
    {
        //Wait the reply elaboration
        SelectSlave sel;
    
        // Wait for reply
        uint8_t data = 0;
        uint8_t dataLen = 0;
        if (!SpiDrv::waitResponseCmd(cmd, PARAM_NUMS_1, &data, &dataLen)) {
            WARN("error waitResponse");
            g_lastError = SocketDrv::Failure;
            return SocketDrv::SocketPollFailed;
        }

        auto ret = SocketState(data);
        if (ret != SocketDrv::SocketPollFailed)
            g_lastError = 0;
        return ret;
    }
}

bool SocketDrv::setsockopt(uint8_t s, uint32_t optionName, const void * option, uint8_t optLen) {
    g_lastError.reset();
    if (!SpiDrv::initialized)
		SpiDrv::begin();

    uint8_t cmd = SOCKET_SETSOCKOPT_CMD;

    {
        SelectSlave sel;
        
        // Send Command
        int commandSize = 4;
        SpiDrv::sendCmd(cmd, PARAM_NUMS_3);
        SpiDrv::sendParam(&s, sizeof(s), NO_LAST_PARAM);
        commandSize += (1 + sizeof(s));
        SpiDrv::sendParam((uint8_t*)&optionName, sizeof(optionName), NO_LAST_PARAM);
        commandSize += (1 + sizeof(optionName));
        SpiDrv::sendParam((uint8_t *)option, optLen, LAST_PARAM);
        commandSize += (1 + optLen);
        
        // pad to multiple of 4
        while (commandSize % 4) {
            SpiDrv::readChar();
            commandSize++;
        }
    }
    {
        //Wait the reply elaboration
        SelectSlave sel;
    
        // Wait for reply
        uint8_t data = 0;
        uint8_t dataLen = 0;
        if (!SpiDrv::waitResponseCmd(cmd, PARAM_NUMS_1, &data, &dataLen)) {
            WARN("error waitResponse");
            g_lastError = SocketDrv::Failure;
            return false;
        }

        if (data != 0) {
            g_lastError = 0;
            return true;
        }
        return false;
    }
}

bool SocketDrv::getsockopt(uint8_t s, uint32_t optionName, void * option, uint8_t & optLen) {
    g_lastError.reset();
    if (!SpiDrv::initialized)
		SpiDrv::begin();

    uint8_t cmd = SOCKET_GETSOCKOPT_CMD;

    {
        SelectSlave sel;
        
        // Send Command
        int commandSize = 4;
        SpiDrv::sendCmd(cmd, PARAM_NUMS_3);
        SpiDrv::sendParam(&s, sizeof(s), NO_LAST_PARAM);
        commandSize += (1 + sizeof(s));
        SpiDrv::sendParam((uint8_t*)&optionName, sizeof(optionName), NO_LAST_PARAM);
        commandSize += (1 + sizeof(optionName));
        SpiDrv::sendParam((uint8_t *)&optLen, sizeof(optLen), LAST_PARAM);
        commandSize += (1 + sizeof(optLen));
        
        // pad to multiple of 4
        while (commandSize % 4) {
            SpiDrv::readChar();
            commandSize++;
        }
    }
    {
        //Wait the reply elaboration
        SelectSlave sel;
    
        // Wait for reply
        if (!SpiDrv::waitResponseCmd(cmd, PARAM_NUMS_1, (uint8_t *)option, &optLen)) {
            WARN("error waitResponse");
            g_lastError = SocketDrv::Failure;
            return false;
        }

        if (optLen != 0) {
            g_lastError = 0;
            return true;
        }
        return false;
    }
}

bool SocketDrv::getPeerName(uint8_t s, arduino::IPAddress & remoteIpAddress, uint16_t & remotePort) {
    g_lastError.reset();
    if (!SpiDrv::initialized)
		SpiDrv::begin();

    uint8_t cmd = SOCKET_GETPEERNAME_CMD;

    {
        SelectSlave sel;
        
        // Send Command
        int commandSize = 4;
        SpiDrv::sendCmd(cmd, PARAM_NUMS_1);
        SpiDrv::sendParam(&s, sizeof(s), LAST_PARAM);
        commandSize += (1 + sizeof(s));
        
        // pad to multiple of 4
        while (commandSize % 4) {
            SpiDrv::readChar();
            commandSize++;
        }
    }
    {
        //Wait the reply elaboration
        SelectSlave sel;
    
        // Wait for reply
        uint8_t res;
        uint8_t remoteIpBuf[4];
        uint16_t remotePortTmp;
        tParam params[PARAM_NUMS_3] = { 
            {0, (char*)&res}, 
            {0, (char*)&remoteIpBuf},  
            {0, (char*)&remotePortTmp}
        };
        if (!SpiDrv::waitResponseParams(cmd, PARAM_NUMS_3, params)) {
            WARN("error waitResponse");
            g_lastError = SocketDrv::Failure;
            return -1;
        }

        if (res) {
            g_lastError = 0;
            remoteIpAddress = IPAddress(arduino::IPv4, remoteIpBuf); //buf is in network order
            remotePort = remotePortTmp; //port is in host order
            return true;
        }

        return false;
    }
}

