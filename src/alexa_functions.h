#ifndef ALEXA_FUNCTIONS_H
#define ALEXA_FUNCTIONS_H

// -------------
#include "AD9833C3.h"

extern void     fauxmo_setup();
extern void     alexa_command(unsigned char device_id, const char * device_name, bool state, unsigned char value);
// -------------
#endif //ALEXA_FUNCTIONS_H