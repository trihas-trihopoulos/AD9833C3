#include "AD9833C3.h"
// --------------------------------
StaticJsonDocument<128> txJsonBuffer;
StaticJsonDocument<128> rxJsonBuffer;

// -----------------------------------------
void hexdump(const void *mem, uint32_t len, uint8_t cols) 
{
	const uint8_t* src = (const uint8_t*) mem;
	Serial.printf("\n[HEXDUMP] Address: 0x%08X len: 0x%X (%d)", (ptrdiff_t)src, len, len);
	for(uint32_t i = 0; i < len; i++) {
		if(i % cols == 0) {
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
/*
  // fill status and measurements
  txJsonBuffer["channel"]        = rf_channel;
  txJsonBuffer["enable"]         = rf_enable;
*/
  // Print what is going to be sent
  DEBUG_PRINT("prepareDataForJSONTransmission:");
  serializeJsonPretty(txJsonBuffer, Serial);    // Send serial data to the serial port
  DEBUG_PRINTLN("---\n");
  serializeJson(txJsonBuffer, outputString);
  ws.textAll(outputString);   // Send the response to all clients
}
// ------------------------
// Updates variables from data in rxJsonBuffer json buffer  
void updateDataFromReceivedJson()
{
  int channel   = rxJsonBuffer["channel"];
  int enable    = rxJsonBuffer["enable"];

  DEBUG_PRINT("updateDataFromReceivedJson:");
  serializeJsonPretty(rxJsonBuffer, Serial);    // Send serial data to the serial port
  DEBUG_PRINTLN("---\n");
/*
  // toggle values
  if (channel == 1)
    switch_tv_signal();
  else
    disable_tv_signal();
*/
  prepareDataForJSONTransmission();                 // update all clients
  // client->text(response);
}
// ---------------------------
// Parses received data
void receiveDataWs(AsyncWebSocketClient * client, String request)
{
  DEBUG_PRINTLN("receiveDataWs:\n");
  DEBUG_PRINTLN(request);
  DEBUG_PRINTLN("\n---\n");


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
    updateDataFromReceivedJson(); //Update variables
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

