#ifndef WEB_SOCKET_H
#define WEB_SOCKET_H
// ----------------------------------
void receiveDataWs(AsyncWebSocketClient * client, String request);
void updateDataFromReceivedJson();
void prepareDataForJSONTransmission();
void hexdump(const void *mem, uint32_t len, uint8_t cols) ;
void receiveDataWs(AsyncWebSocketClient * client, String request);
void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len);
#endif