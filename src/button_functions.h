#ifndef BUTTON_FUNCTIONS_H
#define BUTTON_FUNCTIONS_H
// ---------------------------------------------------
// https://espressif-docs.readthedocs-hosted.com/projects/arduino-esp32/en/latest/api/timer.html

#define L_BUTTON_PIN    0
#define R_BUTTON_PIN    1
#define F_BUTTON_PIN    2
#define B_BUTTON_PIN    4
#define M_BUTTON_PIN    5

// ---------------------------------------------------
extern uint8_t button_pressed;
extern uint8_t button_released;
extern uint8_t previous_button_pressed;
extern uint8_t previous_button_released;

bool joystick_switches_loop();
void joystick_switches_setup();
// ---------------------------------------------------
#endif          // BUTTON_FUNCTIONS_H

