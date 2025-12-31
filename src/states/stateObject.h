
#ifndef STATE_OBJECT_H
#define STATE_OBJECT_H
// ------------
#include <Arduino.h>

typedef void (screensFunction)(void);               // function pointer to screen handling function
typedef int (inputsFunction)(void);                // function pointer to inputs (buttons etc) handling function

// -------
class stateObject
{
  private:

  public:
    bool        running;
    ulong       startTime;
    int         stateChange;

    stateObject();
    screensFunction *drawFunction=NULL;                  // Called for screen drawing
    inputsFunction  *inputFunction=NULL;

    virtual void redraw();
    virtual void startupObject();
    virtual int loopObject();
};

// ------------
#endif  //STATE_OBJECT_H