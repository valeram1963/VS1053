# VS1053 MP3 Player

/*************************************************** 
  This is an example for the  VS1053 Codec Breakout
with mega, sd card sheld ( old IC chip version) and lcd display


/***************************************************/

#include <SPI.h>
#include <SD.h>
#include <Arduino.h>


File myFile;
// set up variables using the SD utility library functions:
Sd2Card card;
SdVolume volume;
SdFile root;
//https://www.arduino.cc/en/Tutorial/LibraryExamples/Listfiles#hardware-required
// change this to match your SD shield or module;
//     Arduino Ethernet shield: pin 4
//     Adafruit SD shields and modules: pin 10
//     Sparkfun SD shield: pin 8
const int chipSelect = 10;


// include lcd display
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);
#define MAX_BRIGHTNESS 255

// include SPI, MP3 and SD libraries
#include <Adafruit_VS1053.h>



// define the pins used
#define CLK 52 //13       // SPI Clock, shared with SD card
#define MISO 50 //12      // Input data, from VS1053/SD card
#define MOSI 51 //11      // Output data, to VS1053/SD card
// Connect CLK, MISO and MOSI to hardware SPI pins. 
// See http://arduino.cc/en/Reference/SPI "Connections"

// These are the pins used for the breakout example
#define BREAKOUT_RESET  47 //9      // VS1053 reset pin (output)
#define BREAKOUT_CS     49 //10     // VS1053 chip select pin (output)
#define BREAKOUT_DCS    46 //8      // VS1053 Data/command select pin (output)
// These are the pins used for the music maker shield
#define SHIELD_RESET  47//-1      // VS1053 reset pin (unused!)
#define SHIELD_CS     49//7      // VS1053 chip select pin (output)
#define SHIELD_DCS    46//6      // VS1053 Data/command select pin (output)

// These are common pins between breakout and shield
#define CARDCS 10 //4     // Card chip select pin must be equal to CSS or chipSelect
// DREQ should be an Int pin, see http://arduino.cc/en/Reference/attachInterrupt
#define DREQ 48 //3       // VS1053 Data request, ideally an Interrupt pin
// create Breakout-example object!
Adafruit_VS1053_FilePlayer musicPlayer =  Adafruit_VS1053_FilePlayer(BREAKOUT_RESET, BREAKOUT_CS, BREAKOUT_DCS, DREQ, CARDCS);

/*************************************************** This is an example for the VS1053 Codec Breakout with mega, sd card sheld ( old IC chip version) and lcd display

/***************************************************/

