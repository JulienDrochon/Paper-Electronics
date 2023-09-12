//Analog pin settings variables
int analogPin = 0;
int raw = 0;
int Vin = 5;
float Vout = 0;
float R1 = 1000;
float R2 = 0;
float buffer = 0;

// mp3 includes
#include <SPI.h>
#include <SdFat.h>
#include <FreeStack.h>
#include <SFEMP3Shield.h>

// mp3 variables
SFEMP3Shield MP3player;
byte result;
int lastPlayed = 0;
uint8_t volume = 0;

// sd card instantiation
SdFat sd;

// define LED_BUILTIN for older versions of Arduino
#ifndef LED_BUILTIN
#define LED_BUILTIN 13
#endif



void setup() {
  Serial.begin(57600);

  Serial.println("Bare Conductive Touch MP3 player");

  if (!sd.begin(SD_SEL, SPI_HALF_SPEED)) sd.initErrorHalt();

  result = MP3player.begin();
  MP3player.setVolume(volume, volume);

  if (result != 0) {
    Serial.print("Error code: ");
    Serial.print(result);
    Serial.println(" when trying to start MP3 player");
  }

  MP3player.playTrack(0);


}

void loop() {
  raw = analogRead(analogPin);
  if (raw) {
    buffer = raw * Vin;
    Vout = (buffer) / 1024.0;
    buffer = (Vin / Vout) - 1;
    R2 = R1 * buffer;

    // map the the resitive value of A0 range to 0..254 (max range for MP3player.setVolume())
    //0 and 12000 values are depending of your slider. You have to adkust tese values depending your slider
    uint8_t thisOutput = (uint8_t)map(R2, 0,12000,0,254);

    // if((uint8_t)lastProx!=prox){ // only update volume if the value has changed
    // output the mapped value to the LED
     MP3player.setVolume(thisOutput, thisOutput);
  }
}
