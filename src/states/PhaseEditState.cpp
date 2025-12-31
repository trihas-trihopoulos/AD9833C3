#include "../AD9833C3.h"


// --------------------------------
void PhaseEditStateObject::drawPhaseEdit()
{
    char dispChar[2];

    display.clearDisplay();

    // Display frequency string
    display.setTextSize(2);             // Draw 2X-scale text
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);      // Assuming 12 pixes character height
    display.print("PHASE");
    display.drawLine(0, 15, 127, 14, SSD1306_WHITE);
  //  Serial.printf("Display str: %s\n", displayStr);
    for (int i = 0; i < length; i ++)
    {
      if (menuCursorPosition == i)
        display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // Draw 'inverse' text on cursor line
      else
        display.setTextColor(SSD1306_WHITE);
      display.setCursor(i*12, 18);
      dispChar[0]=displayStr[i];
      dispChar[1]=0;
      display.print(dispChar);
    }
  display.drawLine(0, 33, 127, 33, SSD1306_WHITE);

  if (error)
  {
    display.setCursor(0,40);
    display.setTextSize(1);             // Draw 2X-scale text
    display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
    display.print("ERROR!");
    display.setCursor(0,50);
    display.setTextColor(SSD1306_WHITE);
    display.print("max is 3600");
  }
  display.display();
}

// -----
PhaseEditStateObject::PhaseEditStateObject()
{
  DEBUG_PRINTLN("Constructor of PhaseEditStateObject");
};
// ---
uint16_t PhaseEditStateObject::updateResult()
{
  uint16_t tempResult;
  displayStr[4]=0;

  int res=sscanf(displayStr, "%d", &tempResult);
  if (res == 1)   // Conversion str->float worked
  {
    if (tempResult >3600)   //if greater than 3600, flag error
      error =true;
    else 
    {
      error = false;
      result = tempResult;
    }
  }
//  Serial.printf("Update displayStr= %s, res = %d, error=%d, tempresult:%4d, Result: %40ud\n",displayStr, res, error, tempResult, result);
  return(tempResult);
}
void PhaseEditStateObject::startupObject()
{
//  drawFunction =  (screensFunction *) drawPhaseEdit;
//  inputFunction = (inputsFunction *) manageJoystickInputs;

  // Parameters on display
  menuCursorPosition        = 0;           // Current cursor position (inverted font), selection on middle button
  result=AD9833_phase;
  sprintf(displayStr,"%04d", AD9833_phase);
  stateChange = STATE__NO_CHANGE;
  error=false;
};

// ---
// Forces redrawing 
void PhaseEditStateObject::redraw()
{
  
}
// ---
// loop function
int PhaseEditStateObject::loopObject()
{
  uint16_t tempChar;
  bool button = joystick_switches_loop();       // Read buttons
  if (button)
  {
//    Serial.printf("PhaseEditStateObject - button_pressed: %x, button_released: %x\n",button_pressed, button_released);
    switch (button_released)
    {
    case 0x1 :    // Left
      if ((menuCursorPosition >0))
        menuCursorPosition--;
      break;

    case 0x2 :    // right
      if (menuCursorPosition < (length-1))
        menuCursorPosition++;
      break;

    case 0x4 :    // Forward
      {
        tempChar = displayStr[menuCursorPosition] + 1;
        if (tempChar <= (0x30 + limits[menuCursorPosition][1]))
        {
          displayStr[menuCursorPosition] = tempChar;
        };
      };
      break;

    case 0x8 :    // Backward
      {
        tempChar = displayStr[menuCursorPosition] - 1;
        if (tempChar >= (0x30 + limits[menuCursorPosition][0]))
        {
          displayStr[menuCursorPosition] = tempChar;
        }
      };
      break;

    case 0x10 :    // Middle
      updateResult();
      // Serial.printf("Result= %f, error=%d\n",result, error);
      if (error==false)
        {
          AD9833_phase = result;
          stateChange = FSM_MAIN_MENU;  // Switch to Mode menu
          return(stateChange);
        }
      break;
    }
    drawPhaseEdit();    // Button was pressed - redraw
  }
  return(stateChange);
};

