#include "AD9833C3.h"
// -----------------------------------------------------------------------------
// This is the web server setup
void webserver_notFound(AsyncWebServerRequest *request) 
{
    String body = (request->hasParam("body", true)) ? request->getParam("body", true)->value() : String();
// Check if fauxmo
    if (fauxmo.process(request->client(), request->method() == HTTP_GET, request->url(), body)) return;

// Handle not found request here...
    request->send(404, "text/plain", "Not found");
}

// -----------------------------------------------------------------------------
// This is the web server setup
void serverSetup() 
{
    // These two callbacks are required for gen1 and gen3 compatibility
    server.onRequestBody([](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
        if (fauxmo.process(request->client(), request->method() == HTTP_GET, request->url(), String((char *)data))) return;
        // Handle any other body request here...
    });

    // kk - Serves pages from the file system

    server.serveStatic("/", LittleFS, "/");

    server.onNotFound(webserver_notFound);

    //--------------------
    // OTA update server
    updateServer.setup(&server, "/ota", "kostas", "kostas");

    updateServer.onUpdateBegin = [](const UpdateType type, int &result)
    {
        //you can force abort the update like this if you need to:
        //result = UpdateResult::UPDATE_ABORT;        
        Serial.println("Update started : " + String(type));
    };

    updateServer.onUpdateEnd = [](const UpdateType type, int &result)
    {
        Serial.println("Update finished : " + String(type) + " result: " + String(result));
    };
    
    //--------------------
    // Register websocket server and add event handler
    ws.onEvent(onWsEvent);
    server.addHandler(&ws);

    //--------------------
    // Start the server
    server.begin();

}
// -------------------------------------------------------------

