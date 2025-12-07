#ifndef ROTARY_ENCODER_FUNCTIONS_H
#define GRAPHICS_FUNCTIONS_HROTARY_ENCODER_FUNCTIONS_H
//------------------------------
#include "AD9833C3.h"

extern int16_t rotary_position;

extern RotaryEncoder encoder;

void ICACHE_RAM_ATTR encoderISR();
void ICACHE_RAM_ATTR encoderButtonISR();

void encoder_setup();
void encoder_loop();

// -----------------------------
#endif          //ROTARY_ENCODER_FUNCTIONS_H
