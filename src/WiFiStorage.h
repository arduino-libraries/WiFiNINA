#include "utility/wifi_drv.h"

class WiFiStorageFile;

class WiFiStorageClass
{
public:
	static bool begin();

	static WiFiStorageFile open(const char *filename);
	static WiFiStorageFile open(String filename);

	static bool exists(const char *filename) {
		size_t len;
		return (WiFiDrv::existsFile(filename, strlen(filename), &len) > 0);
	}
	static bool exists(const char *filename, size_t* len) {
		return (WiFiDrv::existsFile(filename, strlen(filename), len) > 0);
	}
	static bool remove(const char *filename) {
		WiFiDrv::deleteFile(filename, strlen(filename));
		return true;
	}
	static bool read(const char *filename, size_t offset, uint8_t* buffer, size_t buffer_len) {
		WiFiDrv::readFile(filename, strlen(filename), offset, buffer, buffer_len);
		return true;
	}
	static bool write(const char *filename, size_t offset, uint8_t* buffer, size_t buffer_len) {
		WiFiDrv::writeFile(filename, strlen(filename), offset, buffer, buffer_len);
		return true;
	}
	static bool download(const char* url, const char *filename) {
		WiFiDrv::downloadFile(url, strlen(url), filename, strlen(filename));
		return true;
	}

    static bool remove(String filename) {
    	return remove(filename.c_str());
	}
	static bool read(String filename, size_t offset, uint8_t* buffer, size_t buffer_len) {
		return read(filename.c_str(), offset, buffer, buffer_len);
	}
	static bool write(String filename, size_t offset, uint8_t* buffer, size_t buffer_len) {
		return write(filename.c_str(), offset, buffer, buffer_len);
	}
    static bool download(String url, String filename) {
    	return download(url.c_str(), filename.c_str());
    }
};

extern WiFiStorageClass WiFiStorage;


class WiFiStorageFile
{
public:
	constexpr WiFiStorageFile(const char* _filename) : filename(_filename) { }
	operator bool() {
		return WiFiStorage.exists(filename, &length);
	}
	uint32_t read(void *buf, uint32_t rdlen) {
		if (offset + rdlen > length) {
			if (offset >= length) return 0;
			rdlen = length - offset;
		}
		WiFiStorage.read(filename, offset, (uint8_t*)buf, rdlen);
		offset += rdlen;
		return rdlen;
	}
	uint32_t write(const void *buf, uint32_t wrlen) {
		WiFiStorage.write(filename, offset, (uint8_t*)buf, wrlen);
		offset += wrlen;
		return wrlen;
	}
	void seek(uint32_t n) {
		offset = n;
	}
	uint32_t position() {
		return offset;
	}
	uint32_t size() {
		WiFiStorage.exists(filename, &length);
		return length;
	}
	uint32_t available() {
		WiFiStorage.exists(filename, &length);
		return length - offset;
	}
	void erase() {
		offset = 0;
		WiFiStorage.remove(filename);
	}
	void flush();
	void close() {
		offset = 0;
	}
protected:
	friend class WiFiStorageClass;
	size_t offset = 0;
	size_t length = 0;
	const char* filename;
};
