/*************************************************** 
  This is an example for the  VS1053 Codec Breakout
with mega, sd card and lcd display


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



  
void setup() {
  Serial.begin(9600);
  Wire.begin();

  lcd.init();                      // initialize the lcd
  lcd.backlight();
  lcd.clear();
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.println(F("VS1053_SD_I2C_LCD_ON_Mega_ok_1.3_VR.ino"));
  
  Serial.println(F("VS1053_SD_I2C_LCD_ON_Mega_ok_1.3_VR.ino"));
  delay(5000);
  lcd.clear();

    Serial.print("Initializing SD card...");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin 
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output 
  // or the SD library functions will not work. 
    int CS=10;
   
   pinMode(CS, OUTPUT);
   bool ok=true;
   
  if (!SD.begin(chipSelect, 11,12 , 13)) {
    Serial.println("NO Card Present!!!!!!!!!!!!_VR");
   ok=false;
  }


  if (! musicPlayer.begin()) { // initialise the music player
     Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
     ok=false;}

  if (ok==true) {
      
  Serial.println(F("VS1053 found"));
   Serial.println(F("SD found"));
    Serial.println("Card Present _VR initialization done.");
  // list files
  
  }
  
 // printDirectory(SD.open("/"), 0);
  Serial.println("\nFiles found on the card (name, date and size in bytes): ");
  root.openRoot(volume);
  
  // list all files in the card with date and size
  //root.ls(LS_R | LS_DATE | LS_SIZE);
  
    // list files
  //printDirectory(SD.open("/"), 0);

  
 
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.println(F("Playing Test tone"));
  
//http://www.bajdi.com/lcsoft-vs1053-mp3-module/#comment-33773
/*You can detect RTMIDI mode after hardware/software reset by checking AUDATA. 
//If you see 44100/44101, RTMIDI has been activated,
 and you can write GPIO_DDR=3 and GPIO_ODATA=0, then give software reset to 
 boot into normal decoding mode.”
For information the address of GPIO_DDR is 0xC017 and the address of GPIO_ODATA is 0xC019.

//Set musicplayer in MP3 Mode 
//musicPlayer.sciWrite(VS1053_REG_WRAMADDR, VS1053_GPIO_DDR);
//musicPlayer.sciWrite(VS1053_REG_WRAM, 0x0003);
//musicPlayer.GPIO_digitalWrite(0x0000);
*/
uint16_t GPIO_DDR = 0xc017;
uint16_t GPIO_ODATA = 0xc019;

musicPlayer.sciWrite( GPIO_DDR, 3 ); // GPIO DDR register
musicPlayer.sciWrite( GPIO_ODATA, 0 ); // GPIO ODATA register

delay (100);

//musicPlayer.sciWrite(SCI_MODE, SM_LINE1 | SM_SDINEW | SM_RESET); // soft reset
//
//delay (100);
 // Set volume for left, right channels. lower numbers == louder volume!
musicPlayer.setVolume(190,190);
musicPlayer.sineTest(0x44, 500);

musicPlayer.softReset();

Serial.print(F("SampleRate "));
Serial.println(musicPlayer.sciRead(VS1053_REG_AUDATA));
//“You can detect RTMIDI mode after hardware/software reset by checking AUDATA.
//If you see 44100/44101, RTMIDI has been activated, if SampleRate=8000 Mp3 work fine,and you can write GPIO_DDR=3 and GPIO_ODATA=0, 
//then give software reset to boot into normal decoding mode.”
//For information the address of GPIO_DDR is 0xC017 and the address of GPIO_ODATA is 0xC019.



  
  musicPlayer.softReset();
  

  
  // Timer interrupts are not suggested, better to use DREQ interrupt!
  //musicPlayer.useInterrupt(VS1053_FILEPLAYER_TIMER0_INT); // timer int

  // If DREQ is on an interrupt pin (on uno, #2 or #3) we can do background
  // audio playing
  //musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);  // DREQ int
  

   
  // Play one file, don't return until complete
  Serial.println(F("Playing track 003"));
 
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.println(F("Playing track 03"));
  musicPlayer.playFullFile("/03.mp3");

  // Play one file, don't return until complete
  Serial.println(F("Playing track 004"));
 
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.println(F("Playing track 04"));
  musicPlayer.playFullFile("/04.mp3");
   
//  // Play another file in the background, REQUIRES interrupts!
//  Serial.println(F("Playing track 04"));
//  lcd.clear();
//  lcd.setCursor(0, 0);
//  lcd.println(F("Playing track 04"));
//  musicPlayer.startPlayingFile("/04.mp3");
  
}

void loop() {
  // File is playing in the background
//  if (musicPlayer.stopped()) {
//    Serial.println("Done playing music");
//    while (1) {
//      delay(10);  // we're done! do nothing...
//    }
//  }
//  if (Serial.available()) {
//    char c = Serial.read();
//    
//    // if we get an 's' on the serial console, stop!
//    if (c == 's') {
//      musicPlayer.stopPlaying();
//    }
//    
//    // if we get an 'p' on the serial console, pause/unpause!
//    if (c == 'p') {
//      if (! musicPlayer.paused()) {
//        Serial.println("Paused");
//        musicPlayer.pausePlaying(true);
//      } else { 
//        Serial.println("Resumed");
//        musicPlayer.pausePlaying(false);
//      }
//    }
//  }
//
//  delay(100);
}


/// File listing helper
void printDirectory(File dir, int numTabs) {
   while(true) {
     
     File entry =  dir.openNextFile();
     if (! entry) {
       // no more files
       //Serial.println("**nomorefiles**");
       break;
     }
     for (uint8_t i=0; i<numTabs; i++) {
       Serial.print('\t');
     }
     Serial.print(entry.name());
     if (entry.isDirectory()) {
       Serial.println("/");
       printDirectory(entry, numTabs+1);
     } else {
       // files have sizes, directories do not
       Serial.print("\t\t");
       Serial.println(entry.size(), DEC);
     }
     entry.close();
   }

}
