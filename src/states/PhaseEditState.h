#ifndef PHASE_EDIT_STATE_H
#define PHASE_EDIT_STATE_H
// ------------
#include "Arduino.h"

// -------
class PhaseEditStateObject : public stateObject 
{
  private:

  public:

    // Parameters 
    const uint16_t length = 4;                                 // 10 editable +1 characters
    const char  *EditPattern = "dddd";                        // Edit pattern (d is digit, . can't be modified)
    const int   limits[8][2] = {{0,3},{0,9},{0,9},{0,9}};     // for each editable character, a pair of the min and max 
    char  displayStr[6];                                      // the display - edit buffer
    uint16_t valueLimits[2] ={0, 3600};            // phase limits - 0 to 3600 for AD9833
    uint16_t result;
    bool error=false;

    uint16_t menuCursorPosition;                 // Current cursor position (inverted font), exit on middle button

    PhaseEditStateObject();
    virtual void startupObject();
    virtual int loopObject();
    virtual void redraw();

    uint16_t updateResult();
    void drawPhaseEdit();
};

#endif  //PHASE_EDIT_STATE_H