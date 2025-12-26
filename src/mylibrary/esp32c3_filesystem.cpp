#include "../AD9833C3.h"
// -----------------------------------------
#define FORMAT_LittleFS_IF_FAILED true
// -----------------------------------------


// Initialises LittleFS

void  esp32c3_setup_filesystem()
{
  DEBUG_PRINTLN(F("Staring LittleFS filesystem"));
  bool fsStatus = LittleFS.begin();
  if (fsStatus == false) 
    esp32c3_initialiseFileSystem();
  LittleFS.begin();
};
//-----------------------------------------------------------------
//  Deletes the configuration file from the file system
bool esp32c3_deleteFile(char *configurationFileName)
{
    bool fileExistsFlag = false;

    DEBUG_PRINTLN("Running deleteFile\n");
    fileExistsFlag = LittleFS.exists(configurationFileName);
    
    // if yes, delete it
    if (fileExistsFlag) {
        DEBUG_PRINTLN(F("deleteFile: file was deleted"));
        LittleFS.remove(configurationFileName);
    }
    else
        DEBUG_PRINTLN(F("deleteFile: File not found"));
    
    return(fileExistsFlag);
}

//-----------------------------------------------------------------
// Prints contents of the filesystem directory to serial
// 
void esp32c3_readFileToSerial(char *path) 
{
  Serial.printf("Reading file: %s\r\n", path);

  File file = LittleFS.open(path, "r");
  if (!file || file.isDirectory()) {
    Serial.println("- failed to open file for reading");
    return;
  }

  Serial.println("- read from file:");
  while (file.available()) {
    Serial.write(file.read());
  }
  file.close();
}

//-----------------------------------------------------------------
// *** clean FS, for testing
// LittleFS.format();
// 
void esp32c3_initialiseFileSystem() 
{
    Serial.println(F("Formating filesystem"));
    LittleFS.format();
    ESP.restart(); //
}



//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
void listDir(fs::FS &fs, const char * dirname, uint8_t levels)
{
    Serial.printf("Listing directory: %s\r\n", dirname);

    File root = fs.open(dirname);
    if(!root)
    {
        Serial.println("- failed to open directory");
        return;
    }
    if(!root.isDirectory())
    {
        Serial.println(" - not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory())
        {
            Serial.print("  DIR : ");

#ifdef CONFIG_LittleFS_FOR_IDF_3_2
            Serial.println(file.name());
#else
            Serial.print(file.name());
            time_t t= file.getLastWrite();
            struct tm * tmstruct = localtime(&t);
            Serial.printf("  LAST WRITE: %d-%02d-%02d %02d:%02d:%02d\n",(tmstruct->tm_year)+1900,( tmstruct->tm_mon)+1, tmstruct->tm_mday,tmstruct->tm_hour , tmstruct->tm_min, tmstruct->tm_sec);
#endif

            if(levels)
            {
                listDir(fs, file.name(), levels -1);
            }
        } 
        else 
        {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("  SIZE: ");

#ifdef CONFIG_LittleFS_FOR_IDF_3_2
            Serial.println(file.size());
#else
            Serial.print(file.size());
            time_t t= file.getLastWrite();
            struct tm * tmstruct = localtime(&t);
            Serial.printf("  LAST WRITE: %d-%02d-%02d %02d:%02d:%02d\n",(tmstruct->tm_year)+1900,( tmstruct->tm_mon)+1, tmstruct->tm_mday,tmstruct->tm_hour , tmstruct->tm_min, tmstruct->tm_sec);
#endif
        }
        file = root.openNextFile();
    }
}

//-----------------------------------------------------------------
void createDir(fs::FS &fs, const char * path)
{
    Serial.printf("Creating Dir: %s\n", path);
    if(fs.mkdir(path))
    {
        Serial.println("Dir created");
    } 
    else 
    {
        Serial.println("mkdir failed");
    }
}

//-----------------------------------------------------------------
void removeDir(fs::FS &fs, const char * path)
{
    Serial.printf("Removing Dir: %s\n", path);
    if(fs.rmdir(path))
    {
        Serial.println("Dir removed");
    } else 
    {
        Serial.println("rmdir failed");
    }
}

//-----------------------------------------------------------------
void readFile(fs::FS &fs, const char * path)
{
    Serial.printf("Reading file: %s\r\n", path);

    File file = fs.open(path);
    if(!file || file.isDirectory())
    {
        Serial.println("- failed to open file for reading");
        return;
    }

    Serial.println("- read from file:");
    while(file.available())
    {
        Serial.write(file.read());
    }
    file.close();
}
//-----------------------------------------------------------------
void writeFile(fs::FS &fs, const char * path, const char * message)
{
    Serial.printf("Writing file: %s\r\n", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file)
    {
        Serial.println("- failed to open file for writing");
        return;
    }
    if(file.print(message))
    {
        Serial.println("- file written");
    }
    else 
    {
        Serial.println("- write failed");
    }
    file.close();
}

//-----------------------------------------------------------------
void appendFile(fs::FS &fs, const char * path, const char * message)
{
    Serial.printf("Appending to file: %s\r\n", path);

    File file = fs.open(path, FILE_APPEND);
    if(!file)
    {
        Serial.println("- failed to open file for appending");
        return;
    }
    if(file.print(message))
    {
        Serial.println("- message appended");
    }
    else 
    {
        Serial.println("- append failed");
    }
    file.close();
}

//-----------------------------------------------------------------
void renameFile(fs::FS &fs, const char * path1, const char * path2)
{
    Serial.printf("Renaming file %s to %s\r\n", path1, path2);
    if (fs.rename(path1, path2)) 
    {
        Serial.println("- file renamed");
    } 
    else 
    {
        Serial.println("- rename failed");
    }
}

//-----------------------------------------------------------------
void deleteFile(fs::FS &fs, const char * path)
{
    Serial.printf("Deleting file: %s\r\n", path);
    if(fs.remove(path))
    {
        Serial.println("- file deleted");
    } 
    else 
    {
        Serial.println("- delete failed");
    }
}

//-----------------------------------------------------------------
// SPIFFS-like write and delete file, better use #define CONFIG_LittleFS_SPIFFS_COMPAT 1
void writeFile2(fs::FS &fs, const char * path, const char * message)
{
    if(!fs.exists(path))
    {
      if (strchr(path, '/')) 
      {
        Serial.printf("Create missing folders of: %s\r\n", path);
        char *pathStr = strdup(path);
        if (pathStr) 
        {
          char *ptr = strchr(pathStr, '/');
          while (ptr) {
            *ptr = 0;
            fs.mkdir(pathStr);
            *ptr = '/';
            ptr = strchr(ptr+1, '/');
          }
        }
        free(pathStr);
      }
    }

    Serial.printf("Writing file to: %s\r\n", path);
    File file = fs.open(path, FILE_WRITE);
    if(!file)
    {
        Serial.println("- failed to open file for writing");
        return;
    }
    if(file.print(message))
    {
        Serial.println("- file written");
    } 
    else 
    {
        Serial.println("- write failed");
    }
    file.close();
}

//-----------------------------------------------------------------
void deleteFile2(fs::FS &fs, const char * path)
{
    Serial.printf("Deleting file and empty folders on path: %s\r\n", path);

    if(fs.remove(path))
    {
        Serial.println("- file deleted");
    } 
    else 
    {
        Serial.println("- delete failed");
    }

    char *pathStr = strdup(path);
    if (pathStr) 
    {
        char *ptr = strrchr(pathStr, '/');
        if (ptr) 
        {
            Serial.printf("Removing all empty folders on path: %s\r\n", path);
        }
        while (ptr) 
        {
            *ptr = 0;
            fs.rmdir(pathStr);
            ptr = strrchr(pathStr, '/');
        }
        free(pathStr);
    }
}

//-----------------------------------------------------------------
void testFileIO(fs::FS &fs, const char * path)
{
    Serial.printf("Testing file I/O with %s\r\n", path);

    static uint8_t buf[512];
    size_t len = 0;
    File file = fs.open(path, FILE_WRITE);
    if(!file)
    {
        Serial.println("- failed to open file for writing");
        return;
    }

    size_t i;
    Serial.print("- writing" );
    uint32_t start = millis();
    for(i=0; i<2048; i++)
    {
        if ((i & 0x001F) == 0x001F){
          Serial.print(".");
        }
        file.write(buf, 512);
    }
    Serial.println("");
    uint32_t end = millis() - start;
    Serial.printf(" - %u bytes written in %u ms\r\n", 2048 * 512, end);
    file.close();

    file = fs.open(path);
    start = millis();
    end = start;
    i = 0;
    if(file && !file.isDirectory())
    {
        len = file.size();
        size_t flen = len;
        start = millis();
        Serial.print("- reading" );
        while(len){
            size_t toRead = len;
            if(toRead > 512)
            {
                toRead = 512;
            }
            file.read(buf, toRead);
            if ((i++ & 0x001F) == 0x001F)
            {
              Serial.print(".");
            }
            len -= toRead;
        }
        Serial.println("");
        end = millis() - start;
        Serial.printf("- %u bytes read in %u ms\r\n", flen, end);
        file.close();
    } 
    else 
    {
        Serial.println("- failed to open file for reading");
    }
}
//-----------------------------------------------------------------
void filesystem_info()
{
  size_t total = 0, used = 0;
  Serial.println("\n--- ESP32-C3 Partition Table ---");

  // Iterate through all "data" partitions
  esp_partition_iterator_t it = esp_partition_find(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_ANY, NULL);
  
  while (it != NULL) 
  {
    const esp_partition_t* partition = esp_partition_get(it);
    Serial.printf("Label: %s | Type: 0x%02x | SubType: 0x%02x | Size: %d KB\n", 
                  partition->label, partition->type, partition->subtype, partition->size / 1024);
    it = esp_partition_next(it);
  }
  esp_partition_iterator_release(it);


  int ret = esp_littlefs_info("spiffs", &total, &used);
  if (ret != ESP_OK) 
  {
      Serial.printf("Failed to get LittleFS partition information (%s)", esp_err_to_name(ret));
  } 
  else 
  {
    Serial.printf("\n\nMain partition size: total: %d, used: %d\n", total, used);
  }
}
//-----------------------------------------------------------------
void filesystem_test_setup()
{
  Serial.begin(115200);
  if(!LittleFS.begin(FORMAT_LittleFS_IF_FAILED))
  {
      Serial.println("LittleFS Mount Failed");
      return;
  }
  
  listDir(LittleFS, "/", 0);
	createDir(LittleFS, "/mydir");
	writeFile(LittleFS, "/mydir/hello2.txt", "Hello2");
  //writeFile(LittleFS, "/mydir/newdir2/newdir3/hello3.txt", "Hello3");
  writeFile2(LittleFS, "/mydir/newdir2/newdir3/hello3.txt", "Hello3");
	listDir(LittleFS, "/", 3);
	deleteFile(LittleFS, "/mydir/hello2.txt");
  //deleteFile(LittleFS, "/mydir/newdir2/newdir3/hello3.txt");
  deleteFile2(LittleFS, "/mydir/newdir2/newdir3/hello3.txt");
	removeDir(LittleFS, "/mydir");
	listDir(LittleFS, "/", 3);
  writeFile(LittleFS, "/hello.txt", "Hello ");
  appendFile(LittleFS, "/hello.txt", "World!\r\n");
  readFile(LittleFS, "/hello.txt");
  renameFile(LittleFS, "/hello.txt", "/foo.txt");
  readFile(LittleFS, "/foo.txt");
  deleteFile(LittleFS, "/foo.txt");
  testFileIO(LittleFS, "/test.txt");
  deleteFile(LittleFS, "/test.txt");

  filesystem_info();
  Serial.println( "Test complete" );
}

