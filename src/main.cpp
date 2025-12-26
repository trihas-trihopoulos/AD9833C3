#include <Arduino.h>

#include "AD9833C3.h"

// ---------------------------------------------
uint8_t pot_value = 0;
uint64_t pot_update_timer=0;
// ---------------------------------------------

// ---------------------------------------------

void setup() 
{
    // Init serial port and clean garbage
    Serial.begin(SERIAL_BAUDRATE);
    sleep(1);
    Serial.println("\n\n\n\n ------------------------------------------------\n");
    Serial.println("AD9833C3 v1.0 started\n");

    // Onboard LED
    pinMode(LED, OUTPUT);
    digitalWrite(LED, HIGH); // Our LED has inverse logic (high for OFF, low for ON)
    
    // Initialise encoder switch
    joystick_switches_setup();

    // SCAN I2C PORTS - initialises I2C port
    Serial.printf("Scanning I2C port (SDA: %d - SCL: %d)\n", SDA_PIN, SCL_PIN);
    Wire.begin(SDA_PIN, SCL_PIN);
    check_if_exist_I2C();

    // Initialize graphics
    graphics_setup();

    // Initialise digital potentiometer
    mcp41010_pot.begin();

    // Initialise signal generator AD9833
    AD.begin();

    // Filesystem
    if(!LittleFS.begin(FORMAT_LittleFS_IF_FAILED))
    {
        Serial.println("LittleFS Mount Failed");
        return;
    }

    AsyncWiFiManager *wm = new AsyncWiFiManager(&server,&dns);

    // Attempt to read the configuration"));
    bool configFileFound = false;       // config file found and parsed
    configFileFound = oConfiguration.esp32_readConfigurationFile(configFilename);
    if (configFileFound)
    {
        AsyncWiFiManager *wm = new AsyncWiFiManager(&server,&dns);
        oConfiguration.esp32_printConfigurationobject();
        wifiStartFromConfigurationStructure(wm);
    }
    else
    {
        AsyncWiFiManager *wm = new AsyncWiFiManager(&server,&dns);
        wm->resetSettings();     // Clear WM settings
        Serial.println(F("Using WiFiManager setup"));
        wifiStartClean(wm);
    }
  
    // WiFi is active so...    
    mdns_setup();           // Setup mDNS
    serverSetup();          // Web and update server setup 
    
    fauxmo_setup();         // Alexa setup

    // =================================================
    // TEST CODE
    // =================================================
    Serial.println("Test code");
    // Test that display is still reachable
    testscrolltext();
    
    pot_update_timer=millis();

    // filesystem_test_setup();      // Test filesystem *TO BE REMOVED*
    // ---------------------------------------------

    // ---------------------------------------------

// ---------------------------------------------

}

float               freq= 10000;
MD_AD9833::mode_t   md = MD_AD9833::MODE_SINE;
uint16_t            ph=0;
uint8_t             pot=0;
//----
void loop() 
{
    int i = 0;
    joystick_switches_loop();
  
    if (freq > 500000)
        freq= 10000;
        
    md = MD_AD9833::MODE_SINE;
    print_signal_generator_parameters();
    for (i =0; i<256;i++)
    {
        AD9833_phase = ph;
        AD9833_frequency = freq * (i%10);
        AD9833_mode = md;
        MCP41010_value = pot+i;
        set_signal_generator_parameters();
        delay(50);
    }

    delay(1000);

    md = MD_AD9833::MODE_SQUARE1;
    if (freq > 500000)
        freq= 10000;

    print_signal_generator_parameters();
    for (i =0; i<256;i++)
    {
        AD9833_phase = ph;
        AD9833_frequency = freq * (i%10);
        AD9833_mode = md;
        MCP41010_value = pot+i;
        set_signal_generator_parameters();
        delay(50);
    }

    // ----
    md = MD_AD9833::MODE_TRIANGLE;
    if (freq > 500000)
        freq= 10000;

    print_signal_generator_parameters();
    for (i =0; i<256;i++)
    {
        AD9833_phase = ph;
        AD9833_frequency = freq * (i%10);
        AD9833_mode = md;
        MCP41010_value = pot+i;
        set_signal_generator_parameters();
        delay(50);
    }
   delay(1000); 

}