#ifndef BASIC_MENU_STATE_H
#define BASIC_MENU_STATE_H
// ------------
#include "Arduino.h"


typedef void (screensFunction)(void);               // function pointer to screen handling function
typedef int (inputsFunction)(void);                // function pointer to inputs (buttons etc) handling function

#define MENU_MAX_ENTRIES_ON_SCREEN      4           // 12 pixels height font + 8 height footer. Y positions are 0, 13, 26, 39 and 54

// -------
class mainMenuStateObject : public stateObject 
{
  private:

  public:
    // Parameters 
    uint8_t menuCursorPosition;                 // Current cursor position (inverted font), selection on middle button
    uint8_t menuDisplayStartPosition;           // First menu entry in the entries char*[]
    char    **MenuEntriesPointer;               // Pointer to the menu
    uint8_t menuLength;

    mainMenuStateObject(char *MenuArray[], uint8_t MenuLength );
    virtual void startupObject();
    virtual int loopObject();
    virtual void redraw();
};
// ------------
void drawmainMenu(mainMenuStateObject *obj);
// ------------
#endif  //BASIC_MENU_STATE_H