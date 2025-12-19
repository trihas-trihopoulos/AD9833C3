#include "AD9833C3.h"
// --------------

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);       // The display object
MD_AD9833	AD(AD9833_DATA, AD9833_CLK, AD9833_FSYNC);  // AD9833 (?)
MCP_POT     mcp41010_pot(MCP41010_CS, AD9833_DATA, AD9833_CLK);                          // MCP41010 potentiometer
