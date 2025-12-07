#include <Arduino.h>

#include "AD9833C3.h"

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
}
// ---------------------------------------------

void loop() 
{
  // sleep(5);
  // Serial.println("5secs passed");
  encoder_loop();
  sleep(1);
}