#include "AD9833C3.h"
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
    // pinMode(MCP41010_CS, OUTPUT);
    // mcp41010_pot.setSWSPIdelay(50);

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
    mdns_setup();                                   // Setup mDNS
    serverSetup();                                  // Web and update server setup 
    
    fauxmo_setup();                                 // Alexa setup

    InitialiseWorkingParameters();                  // Initialise working parameters

    // =================================================
    // TEST CODE
    // =================================================
    Serial.println("Test code");
    
    statemainScreen.startupObject();    // main screen, thus startupObject is called here
    mainDisplayState = FSM_BASIC_SCREEN;

    statemainMenu           = new mainMenuStateObject(mainMenuStrings, mainMenuStrings_LENGTH);
    stateParametersMenu     = new ParametersMenuStateObject(basicParametersMenuStrings, basicParametersMenuStrings_LENGTH);
    stateModeMenu           = new basicModeStateObject(basicModeMenuStrings, basicModeMenuStrings_LENGTH);
    stateEditFrequency      = new freqEditStateObject();
    stateEditAmplitude      = new AmplEditStateObject();
    stateEditPhase          = new PhaseEditStateObject();
}


//----
void loop() 
{
    int delta = 0;

    switch (mainDisplayState)
    {
    // -----------------
    case FSM_BASIC_SCREEN:
        delta = statemainScreen.loopObject();
        if (delta)  // Branch to a different state
        {
            switch (delta)
            {
            case FSM_MAIN_MENU:                    // Switch to MENU
                statemainScreen.stateChange = STATE__NO_CHANGE;
                
                statemainMenu->startupObject();     // Basic screen is the main screen, thus startupObject is called here
                mainDisplayState = FSM_MAIN_MENU;  // Menu is on the screen
                drawmainMenu(statemainMenu);      // Force initial menu drawing
                break;
           
            default:
                break;
            }
        }
        break;
    // -----------------
    case FSM_MAIN_MENU:
        delta = statemainMenu->loopObject();
        if (delta)  // Branch to a different state
        {
            switch (delta)
            {
                case FSM_PARAMETERS_MENU:
                    statemainMenu->stateChange = STATE__NO_CHANGE;
                    stateParametersMenu->startupObject();      // Going to parameters meni
                    mainDisplayState = FSM_PARAMETERS_MENU;    // Switch to mode menu on screen FSM_PARAMETERS_MENU
                    break;

                case FSM_BASIC_SCREEN:
                    statemainMenu->stateChange = STATE__NO_CHANGE;
                    statemainScreen.redraw();     // Basic screen is the main screen, redraw to update
                    mainDisplayState = FSM_BASIC_SCREEN;  // Menu is on the screen
                    break;
                default:
                    break;
            }
        }
        break;
    // -----------------
    case FSM_PARAMETERS_MENU:
        delta = stateParametersMenu->loopObject();
        if (delta)  // Branch to a different state
        {
            switch (delta)
            {
                case FSM_MODE_MENU:
                    stateParametersMenu->stateChange = STATE__NO_CHANGE;
                    stateModeMenu->startupObject();          // Basic screen is the main screen, thus startupObject is called here
                    mainDisplayState = FSM_MODE_MENU;    // Switch to mode menu on screen FSM_PARAMETERS_MENU
                    break;
                case FSM_FREQUENCY_EDIT:
                    stateParametersMenu->stateChange = STATE__NO_CHANGE;
                    stateEditFrequency->startupObject();          // goig to edit frequency
                    mainDisplayState = FSM_FREQUENCY_EDIT;    // Switch to mode menu on screen FSM_PARAMETERS_MENU
                    break;
                case FSM_AMPLITUDE_EDIT:
                    stateParametersMenu->stateChange = STATE__NO_CHANGE;
                    stateEditAmplitude->startupObject();          // goig to edit frequency
                    mainDisplayState = FSM_AMPLITUDE_EDIT;    // Switch to mode menu on screen FSM_PARAMETERS_MENU
                    break;
                case FSM_PHASE_EDIT:
                    stateParametersMenu->stateChange = STATE__NO_CHANGE;
                    stateEditPhase->startupObject();          // goig to edit frequency
                    mainDisplayState = FSM_PHASE_EDIT;    // Switch to mode menu on screen FSM_PARAMETERS_MENU
                    break;
                case FSM_MAIN_MENU:
                    stateParametersMenu->stateChange = STATE__NO_CHANGE;
                    statemainMenu->startupObject();        // Basic screen is the main screen, thus startupObject is called here
                    mainDisplayState = FSM_MAIN_MENU;       // Menu is on the screen
                    break;
            }
        }
        break;      // FSM_MODE_MENU
    // -----------------
    case FSM_MODE_MENU:
        delta = stateModeMenu->loopObject();
        if (delta)  // Branch to a different state
        {
            switch (delta)
            {
                case FSM_MAIN_MENU:
                    stateModeMenu->stateChange = STATE__NO_CHANGE;
                    statemainMenu->startupObject();        // Basic screen is the main screen, thus startupObject is called here
                    mainDisplayState = FSM_MAIN_MENU;       // Menu is on the screen
                    break;
                default:
                    break;

            }
        }
        break;      // FSM_MODE_MENU
    // -----------------
    case FSM_FREQUENCY_EDIT:
        delta = stateEditFrequency->loopObject();
        if (delta)  // Branch to a different state
        {
            switch (delta)
            {
                case FSM_MAIN_MENU:
                    stateEditFrequency->stateChange = STATE__NO_CHANGE;
                    statemainMenu->startupObject();         // Basic screen is the main screen, thus startupObject is called here
                    mainDisplayState = FSM_MAIN_MENU;       // Menu is on the screen
                    break;
                default:
                    break;
            }
        }
        break;      //FSM_FREQUENCY_EDIT
    // -----------------
    case FSM_AMPLITUDE_EDIT:
        delta = stateEditAmplitude->loopObject();
        if (delta)  // Branch to a different state
        {
            switch (delta)
            {
                case FSM_MAIN_MENU:
                    stateEditAmplitude->stateChange = STATE__NO_CHANGE;
                    statemainMenu->startupObject();         // Basic screen is the main screen, thus startupObject is called here
                    mainDisplayState = FSM_MAIN_MENU;       // Menu is on the screen
                    break;
                default:
                    break;
            }
        }
        break;      //FSM_FREQUENCY_EDIT
    // -----------------
    case FSM_PHASE_EDIT:
        delta = stateEditPhase->loopObject();
        if (delta)  // Branch to a different state
        {
            switch (delta)
            {
                case FSM_MAIN_MENU:
                    stateEditPhase->stateChange = STATE__NO_CHANGE;
                    statemainMenu->startupObject();         // Basic screen is the main screen, thus startupObject is called here
                    mainDisplayState = FSM_MAIN_MENU;       // Menu is on the screen
                    break;
                default:
                    break;
            }
        }
        break;      //FSM_FREQUENCY_EDIT
    // -----------------
    default:
        break;
    }

    if (delta !=0 )   // State change
        updateWorkingParameters();


    delayMicroseconds(10);      // Breathing space ?
} 



/*

    
 filesystem_test_setup();      // Test filesystem *TO BE REMOVED*
 display.clearDisplay();
 uobj.drawFunction();
 display.display();
 Serial.println("Done drawing character set");
 delay(1000);

 drawSignalGeneratorParameters();
 pot_update_timer=millis();

 // ---------------------------------------------



*/