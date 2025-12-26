#ifndef WEBSERVER_FUNCTIONS_H
#define WEBSERVER_FUNCTIONS_H

#include "AD9833C3.h"

extern void webserver_notFound(AsyncWebServerRequest *request);
extern void serverSetup();
extern void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len);


#endif //WEBSERVER_FUNCTIONS_H
