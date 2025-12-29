
#include "AD9833C3.h"

// ---------------------------------------------------------
ezButton left_button(L_BUTTON_PIN, INTERNAL_PULLUP);
ezButton right_button(R_BUTTON_PIN, INTERNAL_PULLUP);
ezButton forward_button(F_BUTTON_PIN, INTERNAL_PULLUP);
ezButton backward_button(B_BUTTON_PIN, INTERNAL_PULLUP);
ezButton middle_button(M_BUTTON_PIN, INTERNAL_PULLUP);

int left_button_state     = 1;
int right_button_state    = 1;
int forward_button_state  = 1;
int backward_button_state = 1;
int middle_button_state   = 1;

int previous_left_button_state     = 1 ;
int previous_right_button_state    = 1 ;
int previous_forward_button_state  = 1 ;
int previous_backward_button_state = 1 ;
int previous_middle_button_state   = 1 ;

// ---------------------------------------------------------        
// ---------------------------------------------------------
// setup function
void joystick_switches_setup()
{
    left_button.setDebounceTime(50);
    right_button.setDebounceTime(50);
    forward_button.setDebounceTime(50);
    backward_button.setDebounceTime(50);
    middle_button.setDebounceTime(50);
}

// ---------------------------------------------------------
// loop function
void joystick_switches_loop()
{
    left_button.loop();
    right_button.loop();
    forward_button.loop();
    backward_button.loop();
    middle_button.loop();


    left_button_state       =     left_button.getState();
    right_button_state      =     right_button.getState();
    forward_button_state    =     forward_button.getState();
    backward_button_state   =     backward_button.getState();
    middle_button_state     =     middle_button.getState();

    if (previous_left_button_state     != left_button_state    ) Serial.printf("left_button_state     %d\n",left_button_state    );
    if (previous_right_button_state    != right_button_state   ) Serial.printf("right_button_state    %d\n",right_button_state   );
    if (previous_forward_button_state  != forward_button_state ) Serial.printf("forward_button_state  %d\n",forward_button_state );
    if (previous_backward_button_state != backward_button_state) Serial.printf("backward_button_state %d\n",backward_button_state);
    if (previous_middle_button_state   != middle_button_state  ) Serial.printf("middle_button_state   %d\n",middle_button_state  );

    previous_left_button_state     = left_button_state    ;
    previous_right_button_state    = right_button_state   ;
    previous_forward_button_state  = forward_button_state ;
    previous_backward_button_state = backward_button_state;
    previous_middle_button_state   = middle_button_state  ;
}





