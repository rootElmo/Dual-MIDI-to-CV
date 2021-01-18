//////////////////////////////
//  CREATED BY ELMO ROHULA  //
//////////////////////////////

// Dual MIDI to CV
// version 0.0.1
// A Dual MIDI IN device

#include <MIDI.h>

#define LEDboard 13

MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, midiIn);

byte data;
bool noteActive;
bool maxNotes;
int notesActive;

void setup(){
  pinMode(LEDboard, OUTPUT);
  midiIn.setHandleNoteOn(noteOn);
  midiIn.setHandleNoteOff(noteOff);
  midiIn.begin();
  Serial.begin(9600);

  Serial.println("Setup println, I provide no actual value to debugging");
  
  notesActive = 0;
}

void loop() {
  midiIn.read();
}

void noteOn(byte channel, byte pitch, byte vel){
  Serial.print("Note: ");
  Serial.print(pitch);
  Serial.print(", ");
  if (vel <= 0) {
    // noteOff
    return;
  } else {
    switch(notesActive){
      case 0:
        // new note -> 1st note
        // notesActive == 1
        Serial.println("First note on");
        notesActive = 1;
        blinkOn();
        break;
      case 1:
        // 1st note -> 2nd note
        // new note -> 1st note
        // notesActive = 2
        Serial.println("Second note on");
        blinkOn();
        notesActive = 2;
        break;
      case 2:
        // replace note flagged 2nd note ->
        // 1st note -> 2nd note
        // new note -> 1st note
        // no change to 'notesActive'
        blinkOn();
        Serial.println("Maximum number of notes on");
        break;
    }
  }
}

void blinkOn(){
    digitalWrite(LEDboard, HIGH);
  }

void blinkOff(){
    digitalWrite(LEDboard, LOW);
  }

void noteOff(byte channel, byte pitch, byte vel){
  switch(notesActive) {
      case 0:
        break;
        Serial.println("No notes on");
      case 1:
        // simply turn off note
        Serial.println("First note off");
        notesActive = 0;
        blinkOff();
        break;
      case 2:
        // turn off 2nd note
        Serial.println("Second note off");
        notesActive = 1;
        blinkOff();
        break;
    }
}
