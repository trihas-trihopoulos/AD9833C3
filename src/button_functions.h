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
extern ezButton left_button;
extern ezButton right_button;
extern ezButton forward_button;
extern ezButton backward_button;
extern ezButton middle_button;

extern int left_button_state;
extern int right_button_state;
extern int forward_button_state;
extern int backward_button_state;
extern int middle_button_state;


extern int previous_left_button_state    ;
extern int previous_right_button_state   ;
extern int previous_forward_button_state ;
extern int previous_backward_button_state;
extern int previous_middle_button_state  ;

void joystick_switches_loop();
void joystick_switches_setup();
// ---------------------------------------------------
#endif          // BUTTON_FUNCTIONS_H

