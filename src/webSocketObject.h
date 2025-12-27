#ifndef WEB_SOCKET_H
#define WEB_SOCKET_H
// ----------------------------------
void receiveDataWs(AsyncWebSocketClient * client, String request);
void prepareDataForJSONTransmission();
void hexdump(const void *mem, uint32_t len, uint8_t cols) ;
void receiveDataWs(AsyncWebSocketClient * client, String request);
void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len);

void updateBasicParametersFromReceivedJson();

// -------------------------------
// Commands for the web socket
#define    JSON_COMMAND_SEND_UPDATE             0            // Update browser with the current workingParameters
#define    JSON_COMMAND_BASIC_PARAMETERS        1            // Update workingParameters with the JSON contents

struct json_data_struct         // Structure with the variables passed over websocket interface
{
    int         command;              // Ask ESP32 specific data (when originating from browser, respose contents when originating from ESP32)

    float       AD9833_frequency;               // Frequency for AD9833 refclk=25MHz 
    int         AD9833_mode;                    // Enum mode_t MODE_OFF, MODE_SINE, MODE_SQUARE1, MODE_SQUARE2,  MODE_TRIANGLE
    int         AD9833_phase;                   // Phase setting in degrees [0..3600]
    int         MCP41010_value;                 // MCP41010 digital potentiometer value [0-255]

    float       startfrequency;                 // Frequency sweep
    float       endfrequency;
    float       stepfrequency;
    long        frequencystepTime;              // in millisecondes

    int         startAmplitude;                 // Frequency sweep
    int         stopAmplitude;
    int         stepAmplitude;
    long        amplitudeStepTime;              // in millisecondes

    int         sweepMode;                      // 0 - no sweep, 1 - frequency sweep, 2 - Amplitude seep, 3 - both Frequency and Amplitude (inner loop:amplitude)
};

void InitialiseWorkingParameters();

#endif