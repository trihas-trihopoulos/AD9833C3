#include "alexa_functions.h"
// ------------------------------------------------
void alexa_command(unsigned char device_id, const char * device_name, bool state, unsigned char value) 
{
    // Callback when a command from Alexa is received. 
    // You can use device_id or device_name to choose the element to perform an action onto (relay, LED,...)
    // State is a boolean (ON/OFF) and value a number from 0 to 255 (if you say "set kitchen light to 50%" you will receive a 128 here).
    // Just remember not to delay too much here, this is a callback, exit as soon as possible.
    // If you have to do something more involved here set a flag and process it in your main loop.

    // For the example we are turning the same LED on and off regardless fo the device triggered or the value
    // digitalWrite(LED_BUILTIN, !state); // we are nor-ing the state because our LED has inverse logic.
/*
    if (0 == device_id) switch_tv_signal();
    if (1 == device_id) disable_tv_signal();
*/    
    DEBUG_PRINTF("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);

};
// ------------------------------------------------
void fauxmo_setup()
{
    fauxmo.createServer(false);     // Set fauxmoESP to not create an internal TCP server and redirect requests to the server on the defined port
    fauxmo.setPort(80);             // This is required for gen3 devices
    fauxmo.enable(true);            // You have to call enable(true) once you have a WiFi connection    

    // Add virtual devices
/*
    fauxmo.addDevice(structConfiguration.ant01);
	fauxmo.addDevice(structConfiguration.ant02);
*/
    // ----------------------------------------------------------------------
    fauxmo.onSetState(alexa_command);
}
