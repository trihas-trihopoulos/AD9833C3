#include "AD9833C3.h"
//------------------------------

int16_t rotary_position = 0;

RotaryEncoder encoder(ROTARY_PIN_A, ROTARY_PIN_B, ROTARY_BUTTON);

// ---------------
void ICACHE_RAM_ATTR encoderISR()                                            //interrupt service routines need to be in ram
{
  encoder.readAB();
}
// ---------------
void ICACHE_RAM_ATTR encoderButtonISR()
{
  encoder.readPushButton();
}

// ---------------
void encoder_setup()
{
  encoder.begin();                                                           //set encoders pins as input & enable built-in pullup resistors

  attachInterrupt(digitalPinToInterrupt(ROTARY_PIN_A),  encoderISR,       CHANGE);  //call encoderISR()       every high->low or low->high changes
  attachInterrupt(digitalPinToInterrupt(ROTARY_BUTTON), encoderButtonISR, FALLING); //call encoderButtonISR() every high->low              changes

  Serial.begin(115200);
}
// ---------------
void encoder_loop()
{
  if (rotary_position != encoder.getPosition())
  {
    rotary_position = encoder.getPosition();
    Serial.println(rotary_position);
  }
  
  if (encoder.getPushButton() == true) Serial.println(F("PRESSED"));         //(F()) saves string to flash & keeps dynamic memory free
}