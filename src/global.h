#ifndef GLOBAL_H
#define GLOBAL_H
// ------------
#include "AD9833C3.h"


extern Adafruit_SSD1306 display;        // The display object
extern MD_AD9833	AD;                 // AD9833 instance
extern MCP_POT     mcp41010_pot;        // MCP41010 pot

extern ESPAsyncHTTPUpdateServer updateServer;       // OTA server
extern AsyncWebServer server;                       // Asynch web server
extern DNSServer dns;
extern AsyncWebSocket ws;
extern fauxmoESP fauxmo;

extern time_t now;
// extern   struct tm* timeInfo;
// extern struct tm tm;    

//--------------------
// Global objects and variables
extern configurationObject oConfiguration;
extern bool shouldSaveConfig;                       // If the wifi or other configuration, has been changed
extern char *configFilename;                        // JSON configuration filename

// ------------
extern float       AD9833_frequency;            // Frequency for AD9833 refclk=25MHz 
extern MD_AD9833::mode_t      AD9833_mode;     // Enum mode_t MODE_OFF, MODE_SINE, MODE_SQUARE1, MODE_SQUARE2,  MODE_TRIANGLE
extern uint16_t    AD9833_phase;               // Phase setting in degrees [0..3600]
extern uint8_t     MCP41010_value;               // MCP41010 digital potentiometer value [0-255]

extern struct json_data_struct workingParameters;
// ------------
#endif  //GLOBAL_H
