#include "../AD9833C3.h"


// --------------------------------
void freqEditStateObject::drawFreqEdit()
{
    char dispChar[2];

    display.clearDisplay();

    // Display frequency string
    display.setTextSize(2);             // Draw 2X-scale text
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);      // Assuming 12 pixes character height
    display.print("FREQUENCY");
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
    display.print("max is 12.5MHz");
  }
  display.display();
}

// -----
freqEditStateObject::freqEditStateObject()
{
  DEBUG_PRINTLN("Constructor of freqEditStateObject");
};
// ---
float freqEditStateObject::updateResult()
{
  float tempResult;
  displayStr[8]=0;

  int res=sscanf(displayStr, "%f", &tempResult);
  if (res == 1)   // Conversion str->float worked
  {
    if (tempResult >12500000)   //if greater than 12.5 MHz, flag error
      error =true;
    else 
    {
      error = false;
      result = tempResult;
    }
  }
  // Serial.printf("Update displayStr= %s, res = %d, error=%d, tempresult:%f, Result: %8.2f\n",displayStr, res, error, tempResult, result);
  return(tempResult);
}
void freqEditStateObject::startupObject()
{
//  drawFunction =  (screensFunction *) drawFreqEdit;
//  inputFunction = (inputsFunction *) manageJoystickInputs;

  // Parameters on display
  menuCursorPosition        = 0;           // Current cursor position (inverted font), selection on middle button
  result=AD9833_frequency;
  sprintf(displayStr,"%08.0f", AD9833_frequency);
  stateChange = STATE__NO_CHANGE;
  error=false;
};

// ---
// Forces redrawing 
void freqEditStateObject::redraw()
{
  
}
// ---
// loop function
int freqEditStateObject::loopObject()
{
  uint8_t tempChar;
  bool button = joystick_switches_loop();       // Read buttons
  if (button)
  {
//    Serial.printf("freqEditStateObject - button_pressed: %x, button_released: %x\n",button_pressed, button_released);
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
//        Serial.printf("menuCursorPosition: %d, original char:%x, updated char: %x, lim[%d,%d]\n",menuCursorPosition,displayStr[menuCursorPosition],displayStr[menuCursorPosition]+1,limits[menuCursorPosition][0],limits[menuCursorPosition][1] );
        if (tempChar <= (0x30 + limits[menuCursorPosition][1]))
        {
          displayStr[menuCursorPosition] = tempChar;
        };
      };
      break;

    case 0x8 :    // Backward
      {
        tempChar = displayStr[menuCursorPosition] - 1;
//        Serial.printf("menuCursorPosition: %d, original char:%c, updated char: %c",menuCursorPosition,displayStr[menuCursorPosition],displayStr[menuCursorPosition]+1 );
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
          AD9833_frequency = result;
          stateChange = FSM_MAIN_MENU;  // Switch to Mode menu
          return(stateChange);
        }
      break;
    }
    drawFreqEdit();    // Button was pressed - redraw
  }
  return(stateChange);
};

