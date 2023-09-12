// touch  includes
#include <MPR121.h>
#include <Wire.h>
#define MPR121_ADDR 0x5C
#define MPR121_INT 4
// serial port communication
//#include <SoftwareSerial.h>
//SoftwareSerial mySerial(4, 5); // RX, TX

#define holdKey true  // set this to false if you want to have a single quick keystroke
// true means the key is pressed and released when you press and release the electrode respectively


// define LED_BUILTIN for older versions of Arduino
#ifndef LED_BUILTIN
#define LED_BUILTIN 13
#endif

int minValuesSensors[12] = {579, 579, 579, 579, 579, 579, 579, 579, 579, 579, 579, 579};
int maxValuesSensors[12] = {640, 640, 640, 640, 640, 640, 640, 640, 640, 640, 640, 640};

void setup()
{
  Serial.begin(9600);
  // Serial.begin(9600);
  // delay(5000);

  pinMode(LED_BUILTIN, OUTPUT);

  //while (!Serial) ;
  if (!MPR121.begin(MPR121_ADDR)) {
    //Serial.println("error setting up MPR121");
    switch (MPR121.getError()) {
      case NO_ERROR:
        Serial.println("no error");
        break;
      case ADDRESS_UNKNOWN:
        Serial.println("incorrect address");
        break;
      case READBACK_FAIL:
        Serial.println("readback failure");
        break;
      case OVERCURRENT_FLAG:
        Serial.println("overcurrent on REXT pin");
        break;
      case OUT_OF_RANGE:
        Serial.println("electrode out of range");
        break;
      case NOT_INITED:
        Serial.println("not initialised");
        break;
      default:
        Serial.println("unknown error");
        break;
    }
    while (1);
  }

  MPR121.setInterruptPin(MPR121_INT);

  MPR121.updateFilteredData();
}

void loop() {
  MPR121.updateAll();
  //Serial.println( MPR121.getFilteredData(0));
  for (int i = 0; i < 12; i++) {
    Serial.println(i);
    Serial.println( MPR121.getFilteredData(i));

  }

  delay(10);

}

// functions below are little helpers based on using the SoftwareSerial
// as a MIDI stream input to the VS1053 - all based on stuff from Nathan Seidle

//Send a MIDI note-on message.  Like pressing a piano key
//channel ranges from 0-15
void noteOn(byte channel, byte note, byte attack_velocity) {
  talkMIDI( (0x90 | channel), note, attack_velocity);
}

//Send a MIDI note-off message.  Like releasing a piano key
void noteOff(byte channel, byte note, byte release_velocity) {
  talkMIDI( (0x80 | channel), note, release_velocity);
}

//Plays a MIDI note. Doesn't check to see that cmd is greater than 127, or that data values are less than 127
void talkMIDI(byte cmd, byte data1, byte data2) {
  digitalWrite(ledPin, HIGH);
  mySerial.write(cmd);
  mySerial.write(data1);

  //Some commands only have one data byte. All cmds less than 0xBn have 2 data bytes
  //(sort of: http://253.ccarh.org/handout/midiprotocol/)
  if ( (cmd & 0xF0) <= 0xB0)
    mySerial.write(data2);

  digitalWrite(ledPin, LOW);
}
