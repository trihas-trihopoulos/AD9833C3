#ifndef ESP32C3_FILESYSTEM_H
#define ESP32C3_FILESYSTEM_H
// ------------
#include "esp_err.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_littlefs.h"

void esp32c3_setup_filesystem();
bool esp32c3_deleteFile(char *configurationFileName);

void esp32c3_readFileToSerial(char *path);
void esp32c3_initialiseFileSystem();

// ------------
void filesystem_test_setup();
void testFileIO(fs::FS &fs, const char * path);
void deleteFile2(fs::FS &fs, const char * path);
void writeFile2(fs::FS &fs, const char * path, const char * message);
void deleteFile(fs::FS &fs, const char * path);
void renameFile(fs::FS &fs, const char * path1, const char * path2);
void appendFile(fs::FS &fs, const char * path, const char * message);
void writeFile(fs::FS &fs, const char * path, const char * message);
void readFile(fs::FS &fs, const char * path);
void removeDir(fs::FS &fs, const char * path);
void createDir(fs::FS &fs, const char * path);
void listDir(fs::FS &fs, const char * dirname, uint8_t levels);
void filesystem_info();
// ------------
#endif // ESP32C3_FILESYSTEM_H