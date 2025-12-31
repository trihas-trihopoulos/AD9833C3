#include "../AD9833C3.h"


// --------------------------------
void drawBasicModeMenuLine(uint8_t line, char *LineStr, uint8_t cursorLine)
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
void drawBasicModeMenu(basicModeStateObject *obj)
{
  char frstr[10];
  int cline = obj->menuDisplayStartPosition;

  display.clearDisplay();
  for (
    int lines = cline; 
          (lines < cline + MENU_MAX_ENTRIES_ON_SCREEN) && 
          (lines < (obj->menuLength));
    lines ++)
  
    drawBasicModeMenuLine((lines - cline), (char *)obj->MenuEntriesPointer[lines], obj->menuCursorPosition);
  display.display();
}

// -----
basicModeStateObject::basicModeStateObject(char *me[], uint8_t ln )
{
  DEBUG_PRINTLN("Constructor of basicModeStateObject");
  MenuEntriesPointer = me;
  menuLength = ln;
};
// ---
void basicModeStateObject::startupObject()
{
  DEBUG_PRINTLN("startupObject of basicModeStateObject");
  drawFunction =  (screensFunction *) drawBasicModeMenu;
  inputFunction = (inputsFunction *) manageJoystickInputs;

  // Parameters on display
  menuCursorPosition        = 0;           // Current cursor position (inverted font), selection on middle button
  menuDisplayStartPosition  = 0;           // First menu entry in the entries char*[]
//   Serial.printf("menu printout\n");
//  for(int i =0; i<menuLength; i++)
//    Serial.printf("menu startupObject i:%d - %s\n", i, MenuEntriesPointer[i]); 
};

// ---
// Forces redrawing 
void basicModeStateObject::redraw()
{
  
}
// ---
// loop function
int basicModeStateObject::loopObject()
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
    case 0x10 :    // Middle
      int entry = menuDisplayStartPosition + menuCursorPosition;
//      Serial.printf("entry:%d\n",entry);
      switch (entry)
      {
        case 0: 
        case 1: 
        case 2: 
        case 3: 
        case 4: 
          AD9833_mode = convertIntToAD9833Mode(entry);
          break;
        case 5: 
          stateChange = FSM_MAIN_MENU;  // Switch to Mode menu
        break;
      }
      stateChange = FSM_MAIN_MENU;
      break;
    }
    drawBasicModeMenu(this);    // Button was pressed - redraw
  }
  return(stateChange);
};

