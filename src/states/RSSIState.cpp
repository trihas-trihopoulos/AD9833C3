#include "../AD9833C3.h"
// --------------------------------
// Draws the graph axis and ticks
void RSSIStateObject::drawAxis()
{
  int i;
  int tickStep = (graph_max_rssi - graph_min_rssi) / 5 + 1;
  // X axis
  display.drawLine(X_AXIS_STARTX, X_AXIS_STARTY, X_AXIS_ENDX, X_AXIS_ENDY, SSD1306_WHITE);      
  for(i=X_AXIS_STARTX;i<X_AXIS_ENDX;i+=X_AXIS_TICK_STEP)
    display.drawLine(i, X_AXIS_STARTY, i, X_AXIS_STARTY + X_AXIS_TICK_LENGTH, SSD1306_WHITE);   // Draw ticks

  // Y axis
  display.drawLine(Y_AXIS_STARTX, Y_AXIS_STARTY, Y_AXIS_ENDX, Y_AXIS_ENDY, SSD1306_WHITE);      
  for(i=Y_AXIS_STARTY; i < Y_AXIS_ENDY;i+=Y_AXIS_TICK_STEP)
    display.drawLine( Y_AXIS_STARTX - Y_AXIS_TICK_LENGTH, i, Y_AXIS_STARTX, i, SSD1306_WHITE);   // Draw ticks

};

// --------------------------------
void RSSIStateObject::drawRSSI()
{
  int i;
  char tempstr[5];

  display.clearDisplay();

  // Print Y axis labels
  display.setTextSize(1);             // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  sprintf(tempstr,"%3d", graph_max_rssi);
  display.print(tempstr);
  display.setCursor(0,X_AXIS_STARTY-10);
  sprintf(tempstr,"%3d", graph_min_rssi);
  display.print(tempstr);
  // draw axis
  drawAxis();
  

  // draw the graph
  // display.fillRect(X_AXIS_STARTX+1,0, X_AXIS_ENDX ,Y_AXIS_ENDY-2 , SSD1306_BLACK);      // Clear graph, draw it again
  for(i=X_AXIS_STARTX+1; i< (X_AXIS_STARTX + last_data_index+1); i++ )
  {
    float yvalue = (abs(data[i-X_AXIS_STARTX-1]) - abs(graph_max_rssi)) *yscale_factor;
    // Serial.printf("d[%d]=%3d, ", i-X_AXIS_STARTX-1,data[i-X_AXIS_STARTX-1]);
    // Serial.printf("%4.1f,",yvalue);
    display.drawPixel(i, yvalue, SSD1306_WHITE);
  }
  //Serial.println("\n");

  // Display current RSSI
  sprintf(strRSSI, "%3d", last_rssi);
  strRSSI[3]=0;
  // Serial.printf("strrssi:%s\n",strRSSI);
  display.setTextSize(1);             // Draw 2X-scale text

  display.fillRect(0, 56, display.width(), display.height(), SSD1306_BLACK);      // SSD1306_INVERSE
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 56);
  display.print("RSSI:");
  display.print(strRSSI);
  display.display();
}

// -----
RSSIStateObject::RSSIStateObject()
{
  DEBUG_PRINTLN("Constructor of RSSIStateObject");
};
// ---
void RSSIStateObject::startupObject()
{
  // drawFunction =  (screensFunction *) drawRSSI;
  // inputFunction = (inputsFunction *) manageJoystickInputs;

  // Parameters on display
  last_data_index = 0;
  error = false;
  last_measurement_mills=0;
  for(int i=0; i < ARR_LEN ; i++)   // Initialize array
    data[i]=0;
  // redraw();       // Clear display and draw axes
  // drawRSSI();
};

// ---
// Forces redrawing
void RSSIStateObject::redraw()
{
}
// ---
// loop function
int RSSIStateObject::updateRSSI()
{
  int i;

  int wifi_status = WiFi.status();
  // Serial.printf("wifistatus=%d\n", wifi_status);

  if ( wifi_status!= WL_CONNECTED)
  {
    error = true;
    // Serial.println("--> Update RSSI - not connected");
    return(-1);
  }
  else
    error = false;
  
  last_rssi = WiFi.RSSI();
  // Serial.printf("RSSI: %d\n", last_rssi);

  if(last_data_index == (ARR_LEN - 1))
  {
    data[last_data_index] = last_rssi;
    // Serial.printf("Array full: %d\n", last_data_index);
    for(int i=0;i<(ARR_LEN-1); i++)
      data[i]=data[i+1];
    // memmove((void *)data,(void *)(data+sizeof(int)), sizeof(int)*(ARR_LEN-2) );       // Move the array left by one
  }
  else
  {
    data[last_data_index] = last_rssi;
    // Serial.printf("Array not full: %d\n", last_data_index);
  }

  // find min and max
  max_rssi = last_rssi;
  min_rssi = last_rssi;
  for(i=0; i< last_data_index; i++)
  {
    if (max_rssi<data[i])
      max_rssi = data[i];
    if (min_rssi>data[i])
      min_rssi = data[i];
  }
  long number_of_ticks=4;
  graph_max_rssi = max_rssi + (abs(max_rssi % 5));      // Round up to 5 db
  graph_min_rssi = min_rssi - (5 - abs((min_rssi % 5)));
  float dom   = (Y_AXIS_ENDY - Y_AXIS_STARTY - 1);
  float denom = (graph_max_rssi - graph_min_rssi);
  yscale_factor = abs(dom/denom);
  // Serial.printf("last_data_index:=%d, min_rssi:%d, max_rssi:%d\n", last_data_index, min_rssi, max_rssi);
  // Serial.printf("graph_min_rssi:%d, graph_max_rssi:%d, yscale:%f\n", graph_min_rssi, graph_max_rssi, yscale_factor);
  if(last_data_index != (ARR_LEN - 1))
    last_data_index +=1;
  return(last_rssi);
}
// -----
int RSSIStateObject::loopObject()
{
  ulong curmillis = millis();
  if ((curmillis - last_measurement_mills) > RSSI_MEASURMENT_PERIOD)
  {
    last_measurement_mills= curmillis;
    updateRSSI();
    if (error==false)
      drawRSSI();  
  }
  
  bool button = joystick_switches_loop();       // Read buttons
  if (button)
  {
//    Serial.printf("RSSIStateObject - button_pressed: %x, button_released: %x\n",button_pressed, button_released);
    switch (button_released)
    {
    case 0x1 :    // Left
      break;

    case 0x2 :    // right
      break;

    case 0x4 :    // Forward
      {
      };
      break;

    case 0x8 :    // Backward
      {
      };
      break;

    case 0x10 :    // Middle
      stateChange = FSM_MAIN_MENU;  // Switch to Mode menu
      break;
    }
    // drawRSSI();    // Button was pressed - redraw
  }
  return(stateChange);
};

