#ifndef AMPL_EDIT_STATE_H
#define AMPL_EDIT_STATE_H
// ------------
#include "Arduino.h"

// -------
class AmplEditStateObject : public stateObject 
{
  private:

  public:

    // Parameters 
    const uint8_t length = 3;                         // 10 editable +1 characters
    const char  *EditPattern = "ddd";            // Edit pattern (d is digit, . can't be modified)
    const int   limits[8][2] = {{0,2},{0,9},{0,9}};     // for each editable character, a pair of the min and max 
    char  displayStr[6];                               // the display - edit buffer
    uint8_t valueLimits[2] ={0, 255};            // frequency limits - 0 to 12.5MHz for AD9833
    uint8_t result;
    bool error=false;

    uint8_t menuCursorPosition;                 // Current cursor position (inverted font), exit on middle button

    AmplEditStateObject();
    virtual void startupObject();
    virtual int loopObject();
    virtual void redraw();

    uint8_t updateResult();
    void drawAmplEdit();
};

#endif  //AMPL_EDIT_STATE_H