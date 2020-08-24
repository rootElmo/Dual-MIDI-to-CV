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
  Serial.begin(9600);

  blink();
  Serial.println("Tämä on setupissa oleva println");
  
  notesActive = 0;
}

void loop() {
  midiIn.read();
}

void noteOn(byte channel, byte pitch, byte vel){
  Serial.print("Nuotti: ");
  Serial.println(pitch);
  if (vel <= 0) {
    // noteOff
    return;
  } else {
    switch(notesActive){
      case 0:
        // new note -> 1st note
        // notesActive == 1
        Serial.println("Eka nuotti päällä!");
        notesActive = 1;
        blink();
        break;
      case 1:
        // 1st note -> 2nd note
        // new note -> 1st note
        // notesActive = 2
        Serial.println("Toka nuotti päällä!");
        blink();
        notesActive = 2;
        break;
      case 2:
        // replace note flagged 2nd note ->
        // 1st note -> 2nd note
        // new note -> 1st note
        // no change to 'notesActive'
        blink();
        Serial.println("Maksimimäärä nuotteja päällä!");
        break;
    }
  }
}

void blink(){
  int loops = 3;
  for (int i = 0; i < loops; i++) {
      digitalWrite(LEDboard, HIGH);
      delay(200);
      digitalWrite(LEDboard, LOW);
      delay(200);
    }
  }

void noteOff(byte channel, byte pitch, byte vel){
  switch(notesActive) {
      case 0:
        break;
        Serial.println("Ei nuotteja pois päältä");
      case 1:
        // simply turn off note
        Serial.println("Eka nuotti pois päältä!");
        notesActive = 0;
        break;
      case 2:
        // turn off 2nd note
        Serial.println("Toka nuotti pois päältä!");
        notesActive = 1;
        break;
    }
}
