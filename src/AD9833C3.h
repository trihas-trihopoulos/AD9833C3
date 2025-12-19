#ifndef AD9833C3_H
#define AD9833C3_H
// ------------
#pragma once
//-------------
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <RotaryEncoder.h>          // Libray handling the rotary encoder + switch
#include <MD_AD9833.h>              // AD9833 library
#include "MCP_POT_mod.h"            // Modified MCP_POT library

// ---------------------------------------------
// Serial and Wire (I2C)
#define SERIAL_BAUDRATE                 115200
#define LED                             8
#define SDA_PIN     8
#define SCL_PIN     9

// ----------------------------------------------
// Adafruit screen & graphics
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
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
#include "global.h"
#include "i2c_port_address_scanner.h"
#include "graphics_functions.h"
#include "rotary_encoder_functions.h"

// ---------------------------------------------
#endif //AD9833C3_H