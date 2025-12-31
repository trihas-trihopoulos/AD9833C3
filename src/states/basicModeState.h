#ifndef BASIC_MODE_STATE_H
#define BASIC_MODE_STATE_H
// ------------
#include "Arduino.h"


typedef void (screensFunction)(void);               // function pointer to screen handling function
typedef int (inputsFunction)(void);                // function pointer to inputs (buttons etc) handling function

#define MENU_MAX_ENTRIES_ON_SCREEN      4           // 12 pixels height font + 8 height footer. Y positions are 0, 13, 26, 39 and 54

// -------
class basicModeStateObject : public stateObject 
{
  private:

  public:
    // Parameters 
    uint8_t menuCursorPosition;                 // Current cursor position (inverted font), selection on middle button
    uint8_t menuDisplayStartPosition;           // First menu entry in the entries char*[]
    char    **MenuEntriesPointer;               // Pointer to the menu
    uint8_t menuLength;

    basicModeStateObject(char *MenuArray[], uint8_t MenuLength );
    virtual void startupObject();
    virtual int loopObject();
    virtual void redraw();
};
// ------------
void drawBasicModeMenu(basicModeStateObject *obj);
void drawBasicModeMenuLine(uint8_t line, char *LineStr, uint8_t cursorLine);
// ------------
#endif  //BASIC_MODE_STATE_H