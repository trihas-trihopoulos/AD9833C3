#ifndef VARIOUS_FUNCTIONS_H
#define VARIOUS_FUNCTIONS_H

// -------------
#include "AD9833C3.h"

MD_AD9833::mode_t convertIntToAD9833Mode(int mode);

int manageJoystickInputs(void);                             // Checks the inputs and returns an event

// -------------
#endif //VARIOUS_FUNCTIONS_H
