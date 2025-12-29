#include "AD9833C3.h"
// --------------

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);       // The display object
MD_AD9833	AD(AD9833_DATA, AD9833_CLK, AD9833_FSYNC);  // AD9833 (?)
MCP41010     mcp41010_pot(MCP41010_CS, AD9833_DATA, AD9833_CLK);                          // MCP41010 potentiometer

// Web and OTA server
ESPAsyncHTTPUpdateServer updateServer;
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
fauxmoESP fauxmo;
DNSServer dns;

// Global objects
// configurationObject oConfiguration;
char *configFilename = "/myconfig.json";       // JSON configuration filename
bool shouldSaveConfig = false;

configurationObject oConfiguration;

time_t now;                         // this are the seconds since Epoch (1970) - UTC
// struct tm tm;                              // the structure tm holds time information in a more convenient way


// -------
// Global variables for the frequency generator
float       AD9833_frequency;            // Frequency for AD9833 refclk=25MHz 
MD_AD9833::mode_t      AD9833_mode;                // Enum mode_t MODE_OFF, MODE_SINE, MODE_SQUARE1, MODE_SQUARE2,  MODE_TRIANGLE
uint16_t    AD9833_phase;               // Phase setting in degrees [0..3600]
uint8_t     MCP41010_value;               // MCP41010 digital potentiometer value [0-255]

// Global variable for the websocket data structure
struct json_data_struct workingParameters;

 // -----------------------------------
 // Display state objects
int mainDisplayState =0;       // Display FSM state variable
basicFrequencyStateObject stateBasicFrequency;            // Basic frequency display state


 // Variables used to step up/down amplituded and frequency from joystick
u_int8_t amplitudeStep=10;
u_int8_t frequencyStep=100;

