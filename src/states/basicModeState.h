#ifndef BASIC_MODE_STATE_H
#define BASIC_MODE_STATE_H
// ------------
#include "Arduino.h"

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