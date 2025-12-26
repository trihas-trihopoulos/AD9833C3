#ifndef CONFIGURATIONOBJECT_H
#define CONFIGURATIONOBJECT_H
#include "AD9833C3.h"

// ------------------------------------------------------------
#define CONFIGURATION_STRUCTURE_LENTGH 10              // How many parameters in structConfigJsonEntry AND config_data_struct!

// Description of the config file entries
struct structConfigJsonEntry
{
  char name[16];  // json key (max 16 chars)
  int type;       // 0=string, 1=integer
  int length;     // for strings, the maximum length 
  void *whereToWrite;
};

// ------------------------------------------------------------
// Contains the infomation stored in the json file (wif_ssid and wifi_pass only ofr info, storem in EEPROM by wifimanager)
struct config_data_struct
{
      char ssid[20];
      char pass[20];

      char ip[16];            // Static IP
      char gw[16];            // default gateway
      char sn[16];            // subnet mask
      char dns[16];           // dns 

      char host[20];      // hostname for MDNS
      int  port;          // web server port

      int mode;           // mode (sine/triangle etc)
      char freq[24];      // Frequency
};

// ----------------------------------------------------------------------------------
// This class writes/reads a configuration file from the file system

class configurationObject 
{
  private:

  public:
    bool fileExistsFlag;          // Configuration file flag - is it available in the file system?

    // ---------------------------
    bool setup(char *configurationFileName);                                   // Initialises the confg 

    bool esp32_readConfigurationFile(char *configurationFileName);    // Reads teh configuration file
    bool esp32_writeConfigurationFile(char *configurationFileName);   // Writes the configuration file
    void esp32_printConfigurationobject();                            // Dump the configuration structure on teh Serial port
};


extern struct config_data_struct structConfiguration;

#endif // CONFIGURATIONOBJECT_H