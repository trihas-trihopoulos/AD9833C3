#ifndef MAIN_SCREEN_STATE_H
#define MAIN_SCREEN_STATE_H
// ------------
#include "Arduino.h"


typedef void (screensFunction)(void);               // function pointer to screen handling function
typedef int (inputsFunction)(void);                // function pointer to inputs (buttons etc) handling function

// -------
class mainScreenStateObject : public stateObject {
  private:
    // Parameters on display
    float       onDisplay_AD9833_frequency;            // Frequency for AD9833 refclk=25MHz 
    MD_AD9833::mode_t      onDisplay_AD9833_mode;                // Enum mode_t MODE_OFF, MODE_SINE, MODE_SQUARE1, MODE_SQUARE2,  MODE_TRIANGLE
    uint16_t    onDisplay_AD9833_phase;               // Phase setting in degrees [0..3600]
    uint8_t     onDisplay_MCP41010_value;               // MCP41010 digital potentiometer value [0-255]
    
  public:
    mainScreenStateObject();
    screensFunction *drawFunction=NULL;                  // Called for screen drawing
    inputsFunction  *inputFunction=NULL;
    virtual void startupObject();
    virtual int loopObject();
    virtual void redraw();
};
// ------------
void drawSignalGeneratorParameters(void);
// ------------
#endif  //MAIN_SCREEN_STATE_H