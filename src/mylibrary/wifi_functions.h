#ifndef WIFIFUNCTIONS_H
#define WIFIFUNCTIONS_H

#include "../AD9833C3.h"
// ------------------------------------------------------------
// Functions related to AsyncWiFiManager (https://github.com/alanswx/ESPAsyncWiFiManager/)
// -----------------------------
#define AUTO_WIFI_SSID  "AD9833C3"        // Clean autoconnect 
#define AUTO_WIFI_PSWD  "ad9833c3"      // Long password 8+ characters


extern char wm_p_host[20];     // Hostname 
extern char wm_p_port[6];      // Webserver port number

void mdns_setup();               // Setup the mDNS 

bool wifiStartClean(AsyncWiFiManager *wm);
void wifiSaveConfigCallback();
bool wifiStartFromConfigurationStructure(AsyncWiFiManager *wm);
// -------------
#endif // WIFIFUNCTIONS_H
