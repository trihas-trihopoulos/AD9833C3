#include "AD9833C3.h"
// --------------------------

uiScreenObject::uiScreenObject(screensFunction sfunc, inputsFunction ifunc, long st, bool en)
{
    drawFunction = sfunc;
    inputFunction = ifunc;
    timeToStayOnScreen = st;
    enabled = en;
};
uiScreenObject::uiScreenObject()
{
    drawFunction = NULL;
    inputFunction = NULL;
    overlayFunction = NULL;
    timeToStayOnScreen = 0;
    enabled = false;
}
// --------------------------
uiScreensManager::uiScreensManager()
{
    NoOfScreens = 0;
};

// --------------------------
void DrawCharacterSet() 
{
  display.clearDisplay();

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  // Not all the characters will fit on the display. This is normal.
  // Library will draw what it can and the rest will be clipped.
  for(int16_t i=0; i<256; i++) {
    if(i == '\n') display.write(' ');
    else          display.write(i);
  }
}
// ---
void DrawCharacterSetInputHandler() 
{
    Serial.println("InputHandler");
}

// ---------------------------
uiScreenObject uobj(DrawCharacterSet, DrawCharacterSetInputHandler, (long) 10000, (bool) true);
