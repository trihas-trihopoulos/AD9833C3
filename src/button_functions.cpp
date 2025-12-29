
#include "AD9833C3.h"
// ---------------------------------------------------------
#define NO_OF_BUTTONS    5
#define BUTTON_DEBOUNCE_TIME    50

uint8_t button_pressed  = 0;
uint8_t button_released = 0;
uint8_t previous_button_pressed  = 0;
uint8_t previous_button_released = 0;

// --------------
ezButton buttonArray[] = 
{
  ezButton(L_BUTTON_PIN, INTERNAL_PULLUP),
  ezButton(R_BUTTON_PIN, INTERNAL_PULLUP),
  ezButton(F_BUTTON_PIN, INTERNAL_PULLUP),
  ezButton(B_BUTTON_PIN, INTERNAL_PULLUP),
  ezButton(M_BUTTON_PIN, INTERNAL_PULLUP)
};
// ---------------------------------------------------------
// setup function
void joystick_switches_setup()
{
    for(int i =0;i<NO_OF_BUTTONS;i++)
        buttonArray[i].setDebounceTime(BUTTON_DEBOUNCE_TIME);
}

// ---------------------------------------------------------
// loop function
bool joystick_switches_loop()
{
    for(int i =0;i<NO_OF_BUTTONS;i++)
        buttonArray[i].loop();

    // Hold old values
    previous_button_pressed     = button_pressed;
    previous_button_released    = button_released;
    button_pressed  = 0;
    button_released = 0;

    for(int i =0; i<NO_OF_BUTTONS; i++)
    {
        if (buttonArray[i].isPressed()) 
            button_pressed  |= (1 << i);
        if (buttonArray[i].isReleased()) 
            button_released |= (1 << i);
    }

    if  (
            (button_pressed    !=  previous_button_pressed)  ||
            (button_released   !=  previous_button_released)
        )
        return(true);  // notify  of change
    else 
        return(false);
}





