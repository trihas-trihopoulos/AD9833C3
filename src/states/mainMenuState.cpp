#include "../AD9833C3.h"
// --------------------------------
void drawmainMenuLine(uint8_t line, char *LineStr, uint8_t cursorLine)
{
  display.setCursor(0, line*14);      // Assuming 12 pixes character height
  display.setTextSize(2);             // Draw 2X-scale text
  if (cursorLine == line)
    display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // Draw 'inverse' text on cursor line
  else
    display.setTextColor(SSD1306_WHITE);
  display.print(LineStr); 
}
// --------------------------------
void drawmainMenu(mainMenuStateObject *obj)
{
  char frstr[10];
  int cline = obj->menuDisplayStartPosition;

  display.clearDisplay();
  for (
    int lines = cline; 
          (lines < cline + MENU_MAX_ENTRIES_ON_SCREEN) && 
          (lines < (obj->menuLength));
    lines ++)
    drawmainMenuLine((lines - cline), (char *)obj->MenuEntriesPointer[lines], obj->menuCursorPosition);
  display.display();
}
// -----
mainMenuStateObject::mainMenuStateObject(char *me[], uint8_t ln )
{
  MenuEntriesPointer = me;
  menuLength = ln;
};
// ---
void mainMenuStateObject::startupObject()
{
  drawFunction =  (screensFunction *) drawmainMenu;
  inputFunction = (inputsFunction *) manageJoystickInputs;

  // Parameters on display
  menuCursorPosition        = 0;           // Current cursor position (inverted font), selection on middle button
  menuDisplayStartPosition  = 0;           // First menu entry in the entries char*[]
};
// ---
// Forces redrawing 
void mainMenuStateObject::redraw()
{
}
// ---
// loop function
int mainMenuStateObject::loopObject()
{
  bool button = joystick_switches_loop();       // Read buttons
  if (button)
  {
//    Serial.printf("button_pressed: %x, button_released: %x\n",button_pressed, button_released);
    switch (button_released)
    {
    case 0x1 :    // Left
      break;
    case 0x2 :    // right
      break;
    case 0x4 :    // Forward
      if (menuCursorPosition != 0)    // we are still inside the screen
        menuCursorPosition --;
      else
      {
        if (menuDisplayStartPosition > 0)
          menuDisplayStartPosition --;
      }
      break;
    // ----
    case 0x8 :    // Backward
      if (
        (menuCursorPosition < MENU_MAX_ENTRIES_ON_SCREEN - 1) &&
        ((menuDisplayStartPosition + menuCursorPosition) < (menuLength-1))
        )
        menuCursorPosition ++;
      else
      {
        if (menuDisplayStartPosition < (menuLength - MENU_MAX_ENTRIES_ON_SCREEN) )
          menuDisplayStartPosition ++;
      }
      break;
    // ----
    case 0x10 :    // Middle button pressed
      int entry = menuDisplayStartPosition + menuCursorPosition;
//      Serial.printf("entry:%d\n",entry);
      switch (entry)
      {
        case 0: 
          stateChange = FSM_PARAMETERS_MENU;  // Switch to Mode menu
        break;

        case 4: 
          stateChange = FSM_BASIC_SCREEN;  // Switch to Basic screen
        break;
      
        default:
          break;
      };
      break;
    // ----

    }
    drawmainMenu(this);    // Button was pressed - redraw
  }
  return(stateChange);
};

