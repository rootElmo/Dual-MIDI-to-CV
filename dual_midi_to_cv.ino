//////////////////////////////
//  CREATED BY ELMO ROHULA  //
//////////////////////////////

// Dual MIDI to CV
// version 0.0.1
// A Dual MIDI IN device


// BUGS:


// FIXED BUGS:
// - 1. When two notes are active and top one is released
// - both notes don't revert to bottom note's value.
// - Program doesn't recognice note off ???
// - FOUND DATE: 19.1.2021

// ----> in "Note Off" switch, case "2", it checked if the
// MIDI is not equal to top OR bottom note.
// Since either was always true, since Note Off
// was received only by one note, the program would misbehave.
// Now it checks if the MIDI
// Note Off is not equal to top note AND bottom note.
// FIXING DATE: 18.1.2021

// - 2. Top note isn't changing if 2 notes are already active.
// - FOUND DATE: 19.1.2021
//
// ----> Problem with keyboard "ghosting". Note priority works correct
#include <MIDI.h>

#define LEDboard 13

MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, midiIn);

byte data;
bool maxNotes;
int notesActive;
int notesListIndex = 0;
int notesList[22] = {0};

bool noteActive[128] = {0};

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
  Serial.print("MIDI NOTE ON: channel: ");
  Serial.print(channel);
  Serial.print(", pitch: ");
  Serial.print(pitch);
  Serial.print(", velocity: ");
  Serial.println(vel);

  // Function called, when "Note On" received from MIDI
  
  if (vel <= 0) {
    noteActive[pitch] = false;
  } else {
    noteActive[pitch] = true;
    playNote();
  }
  
}

void noteOff(byte channel, byte pitch, byte vel){
  Serial.print("MIDI NOTE OFF: channel: ");
  Serial.print(channel);
  Serial.print(", pitch: ");
  Serial.print(pitch);
  Serial.print(", velocity: ");
  Serial.println(vel);
  // Function called, when "Note Off" received from MIDI
  
}

void playNote() {
  int topNote = getHighestNote();
  int bottomNote = getLowestNote();

  Serial.print("TOP NOTE: ");
  Serial.print(topNote);
  Serial.print(" || BOTTOM NOTE: ");
  Serial.println(bottomNote);
}

int getLowestNote() {
  int min = 128;
  for (int i = 0; i < 128; i++) {
    if (noteActive[i]) {
        if (i < min) {
          min = i;
        }
      }
    }
  Serial.print("MIN: ");
  Serial.println(min);
  return min;
}

int getHighestNote() {
  int max = 0;
  for (int i = 0; i < 128; i++) {
    if (noteActive[i]) {
        if (i > max) {
          max = i;  
        }
      }
    }
  Serial.print("MAX: ");
  Serial.println(max);
  return max;
}

void blinkOn(){
  digitalWrite(LEDboard, HIGH);
}

void blinkOff(){
  digitalWrite(LEDboard, LOW);
}

void printNotes(int n1, int n2, int nActive) {
  Serial.print("TOP NOTE: ");
  Serial.print(n1);
  Serial.print(" || BOTTOM NOTE: ");
  Serial.print(n2);
  Serial.print(" || NOTES ACTIVE: ");
  Serial.println(nActive);
}
