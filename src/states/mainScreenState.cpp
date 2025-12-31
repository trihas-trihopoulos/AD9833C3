#include "../AD9833C3.h"

// --------------------------------
void drawSignalGeneratorParameters(void)
{
  char frstr[10];

  display.clearDisplay();

  display.setTextSize(1);             // Normal 1:1 pixel scale

  display.setCursor(0,0);             // Start at row 8
  display.setTextColor(SSD1306_WHITE);
  display.print("M:");

  if (AD9833_mode == 0)               // OFF
    display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // Draw 'inverse' text
  display.print("OFF");
  display.setTextColor(SSD1306_WHITE);
  display.print("|");

  if (AD9833_mode == 1)               // SINEWAVE
    display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // Draw 'inverse' text
  display.print("SIN");
  display.setTextColor(SSD1306_WHITE);
  display.print("|");

  if (AD9833_mode == 2)               // SQUARE1
    display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // Draw 'inverse' text
  display.print("SQ1");
  display.setTextColor(SSD1306_WHITE);
  display.print("|");

  if (AD9833_mode == 3)               // SQUARE2
    display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // Draw 'inverse' text
  display.print("SQ2");
  display.setTextColor(SSD1306_WHITE);

  if (AD9833_mode == 4)               // TRIANGLE
    display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // Draw 'inverse' text
  display.print("TRI");
  display.setTextColor(SSD1306_WHITE);
  display.print("|");


  display.drawLine(0, 9, 127, 9, SSD1306_WHITE);

  display.setCursor(0,11);             // Start at row 
  display.setTextSize(2);             // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.print(F("F:")); 

  sprintf(frstr,"%08.0f",AD9833_frequency);
  display.println(frstr);

  display.drawLine(0, 29, 127, 29, SSD1306_WHITE);

  display.setCursor(0,31);             // Start at row 
  display.setTextSize(2);             // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.print(F("A:")); 
  sprintf(frstr,"%03d",MCP41010_value);
  display.print(frstr);

  int lineLength = MCP41010_value / 2;
  display.drawLine(0, 49, lineLength, 49, SSD1306_WHITE);
  display.drawLine(0, 50, lineLength, 50, SSD1306_WHITE);
  display.drawLine(0, 51, lineLength, 51, SSD1306_WHITE);

  display.setCursor(0,56);             // Start at row 8
  display.setTextSize(1);             // Draw 2X-scale text
  display.print(F(" P:")); 
  sprintf(frstr,"%04d",AD9833_phase);
  display.println(frstr);

  display.display();
  // Update websocket clients
  updateWebSocketClients();
}
// -----
// -----
mainScreenStateObject::mainScreenStateObject()
{
  DEBUG_PRINTLN("Constructor of mainScreenStateObject");
};
// ---
void mainScreenStateObject::startupObject()
{
  DEBUG_PRINTLN("startupObject of mainScreenStateObject");
  drawFunction =  (screensFunction *) drawSignalGeneratorParameters;
  inputFunction = (inputsFunction *) manageJoystickInputs;
  onDisplay_AD9833_frequency = -1.0;  // Force update on first run
};


// ---
// Forces redrawing 
void mainScreenStateObject::redraw()
{
  onDisplay_AD9833_frequency = -1;    
}
// ---
// loop function
int mainScreenStateObject::loopObject()
{
  bool button = joystick_switches_loop();       // Read buttons
  if (button)
  {
    Serial.printf("button_pressed: %x, button_released: %x\n",button_pressed, button_released);
    switch (button_released)
    {
    case 0x1 :    // Left
      if (MCP41010_value >= amplitudeStep)
        MCP41010_value = MCP41010_value - amplitudeStep;
      break;
    case 0x2 :    // right
      if (255 - MCP41010_value >= amplitudeStep)
        MCP41010_value = MCP41010_value + amplitudeStep;
      break;
    case 0x4 :    // Forward
      AD9833_frequency = AD9833_frequency + frequencyStep;
      break;
    case 0x8 :    // Backward
      AD9833_frequency = AD9833_frequency - frequencyStep;
      break;
    case 0x10 :    // Middle
      Serial.println("Middle button pressed");
      stateChange = FSM_MAIN_MENU;

      break;
    default:
      break;
    }

    if (AD9833_frequency < 0)
      AD9833_frequency = 0.0;
    if (AD9833_frequency > 12500000)
      AD9833_frequency = 12500000;
  
    Serial.printf("AD9833_frequency: %08.2f, MCP41010_value: %d\n", AD9833_frequency, MCP41010_value);
  }

  // If updates from websocket, update screen
  if  (
        (onDisplay_AD9833_frequency != AD9833_frequency) ||
        (onDisplay_AD9833_mode != AD9833_mode) ||
        (onDisplay_AD9833_phase != AD9833_phase) ||
        (onDisplay_MCP41010_value != MCP41010_value)
      )
  {
  DEBUG_PRINTLN("loopObject of mainScreenStateObject");
    onDisplay_AD9833_frequency = AD9833_frequency;
    onDisplay_AD9833_mode = AD9833_mode;
    onDisplay_AD9833_phase = AD9833_phase;
    onDisplay_MCP41010_value = MCP41010_value;
    drawFunction();
  }

  return(stateChange);
};
