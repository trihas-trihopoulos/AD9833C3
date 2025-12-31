#include "AD9833C3.h"
// --------------------------------
StaticJsonDocument<512> txJsonBuffer;
StaticJsonDocument<512> rxJsonBuffer;

// -----------------------------------------
void hexdump(const void *mem, uint32_t len, uint8_t cols) 
{
	const uint8_t* src = (const uint8_t*) mem;
	Serial.printf("\n[HEXDUMP] Address: 0x%08X len: 0x%X (%d)", (ptrdiff_t)src, len, len);
	for(uint32_t i = 0; i < len; i++) 
  {
		if(i % cols == 0) 
    {
			Serial.printf("\n[0x%08X] 0x%08X: ", (ptrdiff_t)src, i);
		}
		Serial.printf("%02X ", *src);
		src++;
	}
	Serial.printf("\n");
}
// -----------------------------------------------
void prepareDataForJSONTransmission()
{
  String outputString;
  txJsonBuffer.clear();   // Clear the json document 

  txJsonBuffer["cmd"]           =  workingParameters.command;                               // 1 - reports the AD9833 parameters only
  txJsonBuffer["ADfreq"]        =  workingParameters.AD9833_frequency;                      // One kHz
  txJsonBuffer["ADmode"]        =  workingParameters.AD9833_mode;                           // Enum mode_t MODE_OFF, MODE_SINE, MODE_SQUARE1, MODE_SQUARE2,  MODE_TRIANGLE
  txJsonBuffer["ADphas"]        =  workingParameters.AD9833_phase;                          // Phase setting in degrees [0..3600]
  txJsonBuffer["MCPval"]        =  workingParameters.MCP41010_value;                        // MCP41010 digital potentiometer value [0-255]
  txJsonBuffer["stFreq"]        =  workingParameters.startfrequency;                        // Frequency sweep
  txJsonBuffer["enFreq"]        =  workingParameters.endfrequency;
  txJsonBuffer["stFreq"]        =  workingParameters.stepfrequency;
  txJsonBuffer["fstTim"]        =  workingParameters.frequencystepTime;                     // in millisecondes
  txJsonBuffer["stAmpl"]        =  workingParameters.startAmplitude;                        // Frequency sweep
  txJsonBuffer["stpAmp"]        =  workingParameters.stopAmplitude;
  txJsonBuffer["stepAm"]        =  workingParameters.stepAmplitude;
  txJsonBuffer["aStpTm"]        =  workingParameters.amplitudeStepTime;                     // in millisecondes
  txJsonBuffer["swMode"]        =  workingParameters.sweepMode=0;                           // 0 - no sweep, 1 - frequency sweep, 2 - Amplitude seep, 3 - both Frequency and Amplitude (inner loop:amplitude)

  // Print what is going to be sent
  DEBUG_PRINT("prepareDataForJSONTransmission:");
  serializeJsonPretty(txJsonBuffer, Serial);    // Send serial data to the serial port
  DEBUG_PRINTLN("---\n");
  serializeJson(txJsonBuffer, outputString);
  ws.textAll(outputString);   // Send the response to all clients

  // Update display (will be done in the loop)   drawSignalGeneratorParameters();
  
}
// ---------------------------
// Parses received data
void receiveDataWs(AsyncWebSocketClient * client, String request)
{
  DEBUG_PRINTLN("receiveDataWs:");
  DEBUG_PRINTLN(request);
  
  rxJsonBuffer.clear();   // Clear the json document 
  DeserializationError error = deserializeJson(rxJsonBuffer, request);

  // Test if parsing succeeds.
  if (error) 
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
  else
  {
    // -----------------------
    // Start parsing by first grabbing the command
    int command = rxJsonBuffer["cmd"];
    switch (command)
    {
      case  JSON_COMMAND_SEND_UPDATE:
        prepareDataForJSONTransmission();               // Send current parameters to browser
        break;
      case  JSON_COMMAND_BASIC_PARAMETERS:
        updateBasicParametersFromReceivedJson();               // Send current parameters to browser
        break;

    }

  }
  // ++++++++++++++++++++=
  // String response = String(millis(), DEC);
}

// =============================
// Main handler 
void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len)
{ 
  AwsFrameInfo * info = (AwsFrameInfo*)arg;
  String msg = "";  
  
  switch(type)
  {
    case WS_EVT_CONNECT:
      Serial.printf("ws[%s][%u] connect\n", server->url(), client->id());
      client->ping();
      break;

    case WS_EVT_DISCONNECT:
      Serial.printf("ws[%s][%u] disconnect: %u\n", server->url(), client->id());
      break;

    case WS_EVT_ERROR:
      Serial.printf("ws[%s][%u] error(%u): %s\n", server->url(), client->id(), *((uint16_t*)arg), (char*)data);
      break;

    case WS_EVT_PONG:
      Serial.printf("ws[%s][%u] pong[%u]: %s\n", server->url(), client->id(), len, (len)?(char*)data:"");
      break;
  
  
    case  WS_EVT_DATA:
      if(info->final && info->index == 0 && info->len == len)
      {
        if(info->opcode == WS_TEXT)
        {
          for(size_t i=0; i < info->len; i++) 
          {
            msg += (char) data[i];
          }
        } 
        else 
        {
          char buff[3];
          for(size_t i=0; i < info->len; i++) 
          {
            sprintf(buff, "%02x ", (uint8_t) data[i]);
            msg += buff ;
          }
        }

        if(info->opcode == WS_TEXT)
          receiveDataWs(client, msg);
      } 
      else 
      {
        //message is comprised of multiple frames or the frame is split into multiple packets
        if(info->opcode == WS_TEXT)
        {
          for(size_t i=0; i < len; i++) 
          {
            msg += (char) data[i];
          }
        } 
        else 
        {
          char buff[3];
          for(size_t i=0; i < len; i++) 
          {
            sprintf(buff, "%02x ", (uint8_t) data[i]);
            msg += buff ;
          }
        }
        Serial.printf("%s\n",msg.c_str());

        if((info->index + len) == info->len)
        {
          if(info->final){
            if(info->message_opcode == WS_TEXT)
              receiveDataWs(client, msg);
          }
        }
      };
      break;
  }
}

// ==========================================================================
// Intialise the working parameters structure - also passed over websocket interface
void InitialiseWorkingParameters()
{
    workingParameters.command = 0;                          // 1 - reports the AD9833 parameters only

    workingParameters.AD9833_frequency=1000;                // One kHz
    workingParameters.AD9833_mode= MD_AD9833::MODE_OFF;     // Enum mode_t MODE_OFF, MODE_SINE, MODE_SQUARE1, MODE_SQUARE2,  MODE_TRIANGLE
    workingParameters.AD9833_phase=0;                       // Phase setting in degrees [0..3600]
    workingParameters.MCP41010_value=0;                     // MCP41010 digital potentiometer value [0-255]

    workingParameters.startfrequency=1000;                  // Frequency sweep
    workingParameters.endfrequency=10000;
    workingParameters.stepfrequency=100;
    workingParameters.frequencystepTime=100;                // in millisecondes

    workingParameters.startAmplitude=0;                     // Frequency sweep
    workingParameters.stopAmplitude=255;
    workingParameters.stepAmplitude=5;
    workingParameters.amplitudeStepTime=10;                 // in millisecondes
    workingParameters.sweepMode=0;                          // 0 - no sweep, 1 - frequency sweep, 2 - Amplitude seep, 3 - both Frequency and Amplitude (inner loop:amplitude)
}
// ------------------------
// Updates variables from data in rxJsonBuffer json buffer  
void updateBasicParametersFromReceivedJson()
{
  int mode = rxJsonBuffer["ADmode"];;
  
  workingParameters.AD9833_mode = convertIntToAD9833Mode(mode);

  workingParameters.command             = rxJsonBuffer["cmd"];
  workingParameters.AD9833_frequency    = atof(rxJsonBuffer["ADfreq"]);
 
  workingParameters.AD9833_phase        = atoi(rxJsonBuffer["ADphas"]);
  workingParameters.MCP41010_value      = atoi(rxJsonBuffer["MCPval"]);

  AD9833_phase        = workingParameters.AD9833_phase;
  AD9833_frequency    = workingParameters.AD9833_frequency;
  AD9833_mode         = (MD_AD9833::mode_t) workingParameters.AD9833_mode;
  MCP41010_value      = workingParameters.MCP41010_value & 0x00FF;
  set_signal_generator_parameters();
  print_signal_generator_parameters();

  DEBUG_PRINT("updateBasicParametersFromReceivedJson:");

  prepareDataForJSONTransmission();
}

// -------------------------------
// Update the clients from the actual parameters
void updateWebSocketClients()
{
  workingParameters.AD9833_phase                     =   AD9833_phase     ;                       
  workingParameters.AD9833_frequency                 =   AD9833_frequency ;   
  workingParameters.AD9833_mode                      =   AD9833_mode      ;                   
  workingParameters.MCP41010_value                   =   MCP41010_value   ;           


  prepareDataForJSONTransmission();
}

/*
  // toggle values
  if (channel == 1)
    switch_tv_signal();
  else
    disable_tv_signal();

  prepareDataForJSONTransmission();                 // update all clients
  // client->text(response);
*/

/*
"cmd"  
"ADfreq"
"ADmode"
"ADphas"
"MCPval"
"stFreq"
"enFreq"
"stFreq"
"fstTim"
"stAmpl"
"stpAmp"
"stepAm"
"aStpTm"
"swMode"
*/