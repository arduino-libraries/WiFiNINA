#include "WiFiStorage.h"

WiFiStorageFile WiFiStorageClass::open(const char *filename) {
	WiFiStorageFile file(filename);
	file.size();
	return file;
}
