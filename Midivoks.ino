//////////////////////////////
//  CREATED BY ELMO ROHULA  //
//////////////////////////////

// MidiVoks
// version 0.1.0
// A MIDI IN device for the Polivoks synthesizer

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
  Serial2.begin(9600);

  blink();
  Serial2.println("Tämä on setupissa oleva println");
  
  notesActive = 0;
}

void loop() {
  midiIn.read();
}

void noteOn(byte channel, byte pitch, byte vel){
  if (vel <= 0) {
    // noteOff
    return;
  } else {
    switch(notesActive){
      case 0:
        // new note -> 1st note
        // notesActive == 1
        Serial2.println("Eka nuotti päällä!");
        notesActive = 1;
        blink();
        break;
      case 1:
        // 1st note -> 2nd note
        // new note -> 1st note
        // notesActive = 2
        Serial2.println("Toka nuotti päällä!");
        blink();
        notesActive = 2;
        break;
      case 2:
        // replace note flagged 2nd note ->
        // 1st note -> 2nd note
        // new note -> 1st note
        // no change to 'notesActive'
        blink();
        Serial2.println("Maksimimäärä nuotteja päällä!");
        break;
    }
  }
}

void blink(){
    digitalWrite(LEDboard, HIGH);
    delay(50);
    digitalWrite(LEDboard, LOW);
  }

void noteOff(byte channel, byte pitch, byte vel){
  switch(notesActive) {
      case 0:
        break;
        Serial2.println("Ei nuotteja pois päältä");
      case 1:
        // simply turn off note
        Serial2.println("Eka nuotti pois päältä!");
        notesActive = 0;
        break;
      case 2:
        // turn off 2nd note
        Serial2.println("Toka nuotti pois päältä!");
        notesActive = 1;
        break;
    }
}
