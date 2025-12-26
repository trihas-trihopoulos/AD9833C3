#include "AD9833C3.h"
// ---------------------------------------------------------------
// Contains the run time configuration 
struct config_data_struct structConfiguration;

// JSON file config entries definitions - any change MUST be reflected in *struct config_data_struct*
struct structConfigJsonEntry structConfigJsonEntry[] = 
{
  {"ssid",      0, 20,  &structConfiguration.ssid},
  {"pass",      0, 20,  &structConfiguration.pass},
  {"ip",        0, 16,  &structConfiguration.ip},
  {"gw",        0, 16,  &structConfiguration.gw},
  {"sn",        0, 16,  &structConfiguration.sn},
  {"dns",       0, 16,  &structConfiguration.dns},
  {"host",      0, 20,  &structConfiguration.host},
  {"port",      1, 7,   &structConfiguration.port},           // Will get converted to integer, 6 is the string length
  {"mode",      1, 7,   &structConfiguration.mode},           // AD9833 mode (triangle, sine etc)
  {"freq",      0, 25,  &structConfiguration.freq}           // Startup frequency
};

// ---------------------------------------------------------------------------------------------
// Reads the configuration file into  ConfigurationDoc, returns true if file exists
bool configurationObject::esp32_readConfigurationFile(char *configurationFileName)
{
      bool fileExistsFlag=false;
      bool errorparsing = false;    // Error found while parsing parameters

      Serial.println(F("Reading config file:"));

      // Check first if configurationfile exists
      DEBUG_PRINT("configurationObject::readConfigurationFile - Filename: ");
      DEBUG_PRINTLN(configurationFileName);
      fileExistsFlag = LittleFS.exists(configurationFileName);

      // if yes, try to read it
      if (fileExistsFlag)
      {
            DEBUG_PRINTLN(F("File exists, opening for reading"));
            File fhandle = LittleFS.open(configurationFileName, "r");
            if (fhandle)
            { // If file exists
                  Serial.println(F("opened config file"));
                  size_t size = fhandle.size();
                  // Allocate a buffer to store contents of the file.
                  std::unique_ptr<char[]> buf(new char[size]);

                  fhandle.readBytes(buf.get(), size);
                  DynamicJsonDocument json(1024);
                  DeserializationError error = deserializeJson(json, buf.get());
                  serializeJson(json, Serial); // Dump config file to serial port
                  if (!error)
                  {
                        Serial.println(F("\nParsing json"));

                        for (int i =0; i < CONFIGURATION_STRUCTURE_LENTGH; i++)
                        {
                              if (json.containsKey(structConfigJsonEntry[i].name))
                              {
                                    switch (structConfigJsonEntry[i].type)
                                    {
                                    case 0:                       /// String
                                          strlcpy((char *)structConfigJsonEntry[i].whereToWrite, json[structConfigJsonEntry[i].name], 20);
                                          break;
                                    case 1: 
                                          *(int *)structConfigJsonEntry[i].whereToWrite = json[structConfigJsonEntry[i].name];
                                          break;

                                    default:
                                          errorparsing = true;
                                          break;
                                    }
                              }
                        }
                        fhandle.close(); // close the file anyhow
                  }

                  
                  if (errorparsing || error)
                  {
                        Serial.println(F("Failed parsing config file, deleting it and restarting"));
                        if (error)
                              Serial.println(error.c_str());
                        deleteFile(LittleFS, configurationFileName); // Atttempt to delete the file
                        fileExistsFlag = false;
                        delay(1000);
                        ESP.restart(); // restart - with no config file it will go to wifimanager initial setup
                  }
            }
      }
      return fileExistsFlag;
}

// ---------------------------------------------------------------------------------------------
// Prints the configuration structure

void configurationObject::esp32_printConfigurationobject()
{
      Serial.println(F("Configuration object:"));
      for (int i =0; i < CONFIGURATION_STRUCTURE_LENTGH; i++)
      {
            switch (structConfigJsonEntry[i].type)
            {
            case 0:                             // String parameter
                  Serial.print(structConfigJsonEntry[i].name);
                  Serial.print(F(" :\t\t"));
                  Serial.println((char *)structConfigJsonEntry[i].whereToWrite);
                  break;
            
            case 1:                             // Integer 
                  Serial.print(structConfigJsonEntry[i].name);
                  Serial.print(F(" :\t\t"));
                  Serial.println(*(int *)structConfigJsonEntry[i].whereToWrite);
                  break;
            
            default:
                  break;
            }
      }

}
// ---------------------------------------------------------------------------------------------
// Reads the configuration file and initialises dependent variables
bool configurationObject::setup(char *configurationFileName)
{
      fileExistsFlag = esp32_readConfigurationFile(configurationFileName);
      return(fileExistsFlag);
};

//-----------------------------------------------------------------
// Writes the configuration parameters to the filesystem - dumps configuration structure
bool configurationObject::esp32_writeConfigurationFile(char *configurationFileName)
{
      File f;
      bool fileExistsFlag = false;
      String ConfigurationJSON="";              // Set configuration String to empty

      DynamicJsonDocument ConfigurationDoc(1024);

      Serial.println(F("Writing configuration file"));

      Serial.println(F("Configuration object:"));
      for (int i =0; i < CONFIGURATION_STRUCTURE_LENTGH; i++)
      {
            switch (structConfigJsonEntry[i].type)
            {
            case 0:                             // String parameter
                  ConfigurationDoc[structConfigJsonEntry[i].name] = (char *)structConfigJsonEntry[i].whereToWrite;
                  break;
            
            case 1:                             // Integer 
                  ConfigurationDoc[structConfigJsonEntry[i].name] = (int) *(int *)structConfigJsonEntry[i].whereToWrite;
                  break;
            
            default:
                  break;
            }
      }
      // serialise json
      serializeJson(ConfigurationDoc, ConfigurationJSON);
      // serializeJsonPretty(ConfigurationDoc, Serial);              // Send it also to Serial port

      // Write it to file 
      f = LittleFS.open(configurationFileName, "w");
      if (f)
      {
            serializeJson(ConfigurationDoc, f);
            DEBUG_PRINTLN(F("writeConfigurationFile: writing the config.json")); // print it on serial port
            serializeJsonPretty(ConfigurationDoc, Serial);
            Serial.println();
            fileExistsFlag = true;
      }
      else
      {
            DEBUG_PRINTLN(F("Failed writing to file")); // print it on serial port
            fileExistsFlag = false;
      }
      f.close();
      return fileExistsFlag;
}
