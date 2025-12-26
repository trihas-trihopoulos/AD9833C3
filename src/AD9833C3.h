#ifndef AD9833C3_H
#define AD9833C3_H
// ------------
#pragma once
//-------------
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>

// -------------------
#include <time.h>                       // time() ctime()
#include <sys/time.h>                   // struct timeval
#include <FS.h>                   // File System Library
#include <LittleFS.h>             // SPI Flash System Library


#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <ezButton.h>
#include <MD_AD9833.h>              // AD9833 library
#include "MCP_POT_mod.h"            // Modified MCP_POT library

#include <ArduinoJson.h>          // https://github.com/bblanchon/ArduinoJson
#include <ESPAsyncWebServer.h>
#include <ESPAsyncWiFiManager.h>         //https://github.com/tzapu/WiFiManager
// #include "mylibrary/ESPAsyncHTTPUpdateServer.h"  
#include "fauxmoESP.h"
#include <ESPmDNS.h>

// -------------------
// Debugging 
#define AD9833C3_DEBUG              // Enables serial port printing of debugging info
#define LOOP_PERFORMANCE_DEBUB          // Prints debug info

// Enable serial port debug printouts
#ifdef AD9833C3_DEBUG
  #define DEBUG_PRINT(x) Serial.print(x)
  #define DEBUG_PRINTLN(x) Serial.println(x)
  #define DEBUG_PRINTF(x...) Serial.printf(x)
#else
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTLN(x)
  #define DEBUG_PRINTF(x)
#endif // AD9833C3_DEBUG

// ---------------------------------------------
// Serial and Wire (I2C)
#define SERIAL_BAUDRATE                 115200
#define LED                             8
// ----------------------------------------------
// Adafruit screen & graphics
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
#define SDA_PIN     8
#define SCL_PIN     9

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
//

#define NUMFLAKES     10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16

// ----------------------------------------------
// Rotary encoder + push button
#define ROTARY_PIN_A   0        //ky-040 clk pin, interrupt & add 100nF/0.1uF capacitors between pin & ground!!!
#define ROTARY_PIN_B   1        //ky-040 dt  pin,             add 100nF/0.1uF capacitors between pin & ground!!!
#define ROTARY_BUTTON  2        //ky-040 sw  pin, interrupt & add 100nF/0.1uF capacitors between pin & ground!!!

// ----------------------------------------------
// AD9833 functions
const uint8_t AD9833_FSYNC  = 3;     // Load pin - FSYNC
const uint8_t AD9833_DATA   = 6;     // DATA
const uint8_t AD9833_CLK    = 7;     // CLK

const uint8_t MCP41010_CS   = 10;    // Load pin
// ------------
// ------------
// My files
#include "mylibrary/esp32c3_filesystem.h"
#include "AD9833_functions.h"
#include "mylibrary/wifi_functions.h"
#include "mylibrary/ESPAsyncHTTPUpdateServer.h"
#include "mylibrary/i2c_port_address_scanner.h"
#include "graphics_functions.h"
#include "button_functions.h"
#include "webserver_functions.h"
#include "webSocketObject.h"
#include "various_functions.h"
#include "alexa_functions.h"
// Filesystem
#define FORMAT_LittleFS_IF_FAILED true
#include "configurationObject.h"

// Include global varibales
#include "global.h"
// ---------------------------------------------
#endif //AD9833C3_H