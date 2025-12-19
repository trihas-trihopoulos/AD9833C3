#include <Arduino.h>

#include "AD9833C3.h"

// ---------------------------------------------
uint8_t pot_value = 0;
uint64_t pot_update_timer=0;
// ---------------------------------------------



void setup() 
{
    // Init serial port and clean garbage
    Serial.begin(SERIAL_BAUDRATE);
    sleep(1);
    Serial.println("\n\n\n\n ------------------------------------------------\n");
    Serial.println("started\n");

    // LED
    pinMode(LED, OUTPUT);
    digitalWrite(LED, HIGH); // Our LED has inverse logic (high for OFF, low for ON)
    
    // SCAN I2C PORTS - initialises I2C port
    Serial.printf("Scanning I2C port (SDA: %d - SCL: %d)\n", SDA_PIN, SCL_PIN);
    Wire.begin(SDA_PIN, SCL_PIN);
    check_if_exist_I2C();

    // Initialize graphics
    graphics_setup();

    // Initialise encoder switch
    encoder_setup();

// Initialise pot
    mcp41010_pot.begin();

    // Initialise AD9833
    AD.begin();
    AD.setMode(MD_AD9833::MODE_TRIANGLE);
    Serial.println("Should be in triangle mode, testing graphics");
    // Test that display is still reachable
    testscrolltext();
    
    pot_update_timer=millis();

}
// ---------------------------------------------
ulong last_printout_mills =0;
u16_t previous_rotary_value =0;

void loop() 
{
  encoder_loop();
}