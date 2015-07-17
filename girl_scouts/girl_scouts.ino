/*
  Groove Sensor:
  Pin 1 (Black Wire) > Arduino Gnd
  Pin 3 (Red wire) - Arduino +5v
  Pin 4 (Yellow wire) - Arduino Digital pin 2

  SD Card:
  MOSI - pin 11
  MISO - pin 12
  CLK  - pin 13
  CS   - pin 9
  
  LCD Wiring:
  Pin 1 - Gnd
  Pin 2 - +5v
  Pin 3 - Output from Pot
  Pin 4 - (RS) Arduino pin 3
  Pin 5 - (RW) Gnd
  Pin 6 - EN to Arduino pin 4
  Pin 7 -
  Pin 8 -
  Pin 9 -
  Pin 10 -
  Pin 11 - (DB4) Arduino pin 5
  Pin 12 - (DB5) Arduino pin 6
  Pin 13 - (DB6) Arduino pin 7
  Pin 14 - (DB7) Arduino pin 8
  Pin 15 - +5v
  Pin 16 - Gnd
*/

// include SD libraries
#include <SPI.h>
#include <SD.h>

// include libraries
#include <Wire.h>
#include "RTClib.h"

// initialize library
RTC_DS1307 RTC;

const int chipSelect = 9; // chip to communicate with SD module

File dataFile;

int pin = 2;
unsigned long duration;
unsigned long starttime;
unsigned long sampletime_ms = 30000;//sampe 30s ;
unsigned long lowpulseoccupancy = 0;
float ratio = 0;
float concentration = 0;

void setup() {
  // set up the LCD's number of columns and rows:
  pinMode(6, INPUT);
  if(initSD()){
  }
  Serial.begin(9600);
  Wire.begin();
  RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));

  // Make sure the clock is running
  if (! RTC.isrunning()) {
    Serial.println("RTC NOT running");
  }  
  starttime = millis();
}

void loop() {
  DateTime now = RTC.now();

  duration = pulseIn(pin, LOW);
  lowpulseoccupancy = lowpulseoccupancy + duration;

  if ((millis() - starttime) > sampletime_ms) //if the sampel time == 30s
  {
    ratio = lowpulseoccupancy / (sampletime_ms * 10.0); // Integer percentage 0=>100
    concentration = 1.1 * pow(ratio, 3) - 3.8 * pow(ratio, 2) + 520 * ratio + 0.62; // using spec sheet curve
    String dataString = ""; // create blank string to write to SD
  
    dataString += lowpulseoccupancy;
    dataString += ",";
    dataString += ratio;
    dataString += ",";
    dataString += concentration;
    dataString += ",";
    dataString += now.month();
    dataString += "/";
    dataString += now.day();
    dataString += ",";
    dataString += now.hour();
    dataString += ":";    
    dataString += now.minute();    
    dataString += ":";
    dataString += now.second();    

    dataFile.println(dataString);  // write to the SD
    dataFile.flush();   //

    lowpulseoccupancy = 0;
    starttime = millis();
  }
}
boolean initSD() {
  // this is the default slave select.  must be set to output
  pinMode(SS, OUTPUT); 

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    // don't do anything more:
    while (1) ;
  }
  // Open up the file we're going to log to!
  dataFile = SD.open("datafile.txt", FILE_WRITE);
  if (!dataFile) {
    // Wait forever since we cant write data
    while (1) ;
  }
  return true;
}
