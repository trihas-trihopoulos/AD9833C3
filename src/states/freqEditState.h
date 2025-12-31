#ifndef FREQ_EDIT_STATE_H
#define FREQ_EDIT_STATE_H
// ------------
#include "Arduino.h"


typedef void (screensFunction)(void);               // function pointer to screen handling function
typedef int (inputsFunction)(void);                // function pointer to inputs (buttons etc) handling function

#define MENU_MAX_ENTRIES_ON_SCREEN      4           // 12 pixels height font + 8 height footer. Y positions are 0, 13, 26, 39 and 54

// -------
class freqEditStateObject : public stateObject 
{
  private:

  public:

    // Parameters 
    const uint8_t length = 8;                         // 10 editable +1 characters
    const char  *EditPattern = "dddddddd";            // Edit pattern (d is digit, . can't be modified)
    const int   limits[8][2] = {{0,1},{0,9},{0,9},{0,9},{0,9},{0,9},{0,9},{0,9}};     // for each editable character, a pair of the min and max 
    char  displayStr[12];                               // the display - edit buffer
    float valueLimits[2] ={0.0, 12500000.0};            // frequency limits - 0 to 12.5MHz for AD9833
    float result;
    bool error=false;

    uint8_t menuCursorPosition;                 // Current cursor position (inverted font), exit on middle button

    freqEditStateObject();
    virtual void startupObject();
    virtual int loopObject();
    virtual void redraw();

    float updateResult();
    void drawFreqEdit();
};

#endif  //FREQ_EDIT_STATE_H