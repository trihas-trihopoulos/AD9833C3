#include "../AD9833C3.h"         // See https://github.com/tzapu/AsyncWiFiManager


char wm_p_host[20]="ad9833c3";
char wm_p_port[6] ="8080";

// ------------------
void mdns_setup()
{
  // Initialize mDNS
  if (!MDNS.begin(structConfiguration.host))   // Set the hostname to "esp32.local"
  {
    Serial.println("Error setting up MDNS responder!");
    while(1)  delay(1000);
  }
  Serial.println("mDNS responder started");

}
// ---------------------------------------------------------------
// Zero config WiFI manager setup - 
// Called when no config.json found or first time setup or prolonged errors

// Called when the AsyncWiFiManager gets into configuration mode
void wifi_configModeCallback(AsyncWiFiManager *myAsyncWiFiManager) 
{
  Serial.println(F("Entered wifi config mode"));
  Serial.println(WiFi.softAPIP());
  //if auto generated SSID, print it
  Serial.println(myAsyncWiFiManager->getConfigPortalSSID());
  shouldSaveConfig = false;
  //  Entered config mode, make led toggle faster
  //  onboardLed = new myLedClass(LED_BUILTIN, 100, 400) ;                  // LED_BUILTIN flashes for 100ms every 500ms
}
// ------------------------------------
// This gets called when custom parameters have been set AND a connection has been established.
//callback notifying us of the need to save config
void wifiSaveConfigCallback() 
{
  Serial.println(F("Saving config"));
  shouldSaveConfig = true;
}

// ------------------------------------
// Delete any old information and start clean.
bool wifiStartClean(AsyncWiFiManager *wm)
{
  bool res = false;
  Serial.println(F("WiFi starting clean"));
  wm->resetSettings();   // Clear any settings
  deleteFile(LittleFS, configFilename); // Delete the configuration file 
  wm->setConfigPortalTimeout(600);    // break after 10 minutes - reboot
//  wm->setClass("invert");      // set dark theme
//  wm->setShowStaticFields(true);
//  wm->setShowDnsFields(true);

  wm->setAPCallback(wifi_configModeCallback);
  wm->setSaveConfigCallback(wifiSaveConfigCallback);            // Set callback 

  
  AsyncWiFiManagerParameter custom_hostname("hostname", "Hostname", "ad9833c3", 20);
  wm->addParameter(&custom_hostname);

  AsyncWiFiManagerParameter custom_port("port", "Web server port", "80", 6);
  wm->addParameter(&custom_port);

  AsyncWiFiManagerParameter custom_mode("mode", "Waveform mode", "0", 25);
  wm->addParameter(&custom_mode);

  AsyncWiFiManagerParameter custom_freq("freq", "Start frequency", "1000", 25);
  wm->addParameter(&custom_freq);


  Serial.print("Use SSID :");     // Print these on serial port...
  Serial.println( AUTO_WIFI_SSID );
  Serial.print("Use password :");
  Serial.println( AUTO_WIFI_PSWD );

  res = wm->autoConnect(AUTO_WIFI_SSID, AUTO_WIFI_PSWD);    // Autoconnect
  if(!res) 
    {
      Serial.println(F("Failed to connect, rebooting in 10s"));
      delay(10000);   // Wait 3 seconds and rebooot
      ESP.restart(); // is this correct behaviour?
    } 
    else 
    {   
      // confirm that parameters neeed to be saved (from callback)
      if (shouldSaveConfig)
      {
        strcpy(structConfiguration.host, custom_hostname.getValue());
        Serial.println(custom_port.getValue());
        structConfiguration.port= atoi(custom_port.getValue());

        Serial.println(structConfiguration.host);
        Serial.println(structConfiguration.port);

        strcpy(structConfiguration.ssid, WiFi.SSID().c_str());
        strcpy(structConfiguration.pass, WiFi.psk().c_str());

        strcpy(structConfiguration.ip, WiFi.localIP().toString().c_str());
        strcpy(structConfiguration.sn, WiFi.subnetMask().toString().c_str());
        strcpy(structConfiguration.gw, WiFi.gatewayIP().toString().c_str());
        strcpy(structConfiguration.dns, WiFi.dnsIP().toString().c_str());

        structConfiguration.mode = atoi(custom_mode.getValue());   //  custom_ant01.getValue().toString().c_str()
        structConfiguration.port = atoi(custom_port.getValue());   //  custom_ant01.getValue().toString().c_str()
        strcpy(structConfiguration.freq, custom_freq.getValue());

        oConfiguration.esp32_printConfigurationobject();
        oConfiguration.esp32_writeConfigurationFile(configFilename);
      }        

      shouldSaveConfig = false;
      Serial.println("connected\n\n"); //connected to the WiFi, parameters were saved in the wifiSaveConfigCallback callback
      // Release myLedClass instance
      // onboardLed->stop();
      // onboardLed->deleteTimer();
      // delete onboardLed;
      // onboardLed = NULL;
    }
  return res;
}

// ------------------------------------
// Start with the configuration file data.
bool wifiStartFromConfigurationStructure(AsyncWiFiManager *wm)
{
// AsyncWiFiManager.setSTAStaticIPConfig(IPAddress(192,168,0,99), IPAddress(192,168,0,1), IPAddress(255,255,255,0)); // optional DNS 4th argument
  //set static ip
  IPAddress _ip, _gw, _sn, _dns;

  

  _ip.fromString(structConfiguration.ip);
  _gw.fromString(structConfiguration.gw);
  _sn.fromString(structConfiguration.sn);
  _dns.fromString(structConfiguration.dns);

  wm->setSTAStaticIPConfig(_ip, _gw, _sn, _dns);
  wm->setMinimumSignalQuality();              // Set signalquality as per AsyncWiFiManager example 

  // Try to connect, if it fails restart after 30 seconds
  if (!(wm->autoConnect(structConfiguration.ssid, structConfiguration.pass)))
  {
    Serial.println(F("timeout - failed to connect"));
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    ESP.restart();
    delay(30000);
  }
  return true;
}