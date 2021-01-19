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
bool noteActive;
bool maxNotes;
int notesActive, topNote, bottomNote;
int notesListIndex = 0;
int notesList[22] = {0};

void setup(){
  pinMode(LEDboard, OUTPUT);
  midiIn.setHandleNoteOn(noteOn);
  midiIn.setHandleNoteOff(noteOff);
  midiIn.begin();
  Serial.begin(9600);

  Serial.println("Setup println, I provide no actual value to debugging");

  bottomNote = 0;
  topNote = 0;
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
    // noteOff
    return;
  } else {
    switch(notesActive){
      case 0:
        // new note -> both notes playing: monophonic
        notesList[notesListIndex] = pitch;
        notesListIndex ++;
        
        topNote = pitch;
        bottomNote = pitch;
        
        notesActive ++;
        blinkOn();
        printNotes(topNote, bottomNote, notesActive);
        break;
      case 1:

        if (pitch == bottomNote) { // no changes, stays monophonic
          // (possible feature?) -> RE-TRIGGER both top and bottom note
          // requires module to have trigger outputs
          
          blinkOn();
          break;  
        } else if (pitch > bottomNote) { // new note > old note -> duophonic: top note gets new value
          notesList[notesListIndex] = pitch;
          notesListIndex ++;
          
          topNote = pitch;
          notesActive ++;
          
          blinkOn();
          printNotes(topNote, bottomNote, notesActive);
          break;
        } else {  // new note < old note -> duophonic: bottom note gets new value
          notesList[notesListIndex] = pitch;
          notesListIndex ++;
          
          bottomNote = pitch;
          notesActive ++;

          printNotes(topNote, bottomNote, notesActive);
          blinkOn();
          break;
        }

      case 2:

        if (pitch == topNote || pitch == bottomNote) { // no changes, stays duophonic
          // (possible feature?) -> RE-TRIGGER
          // requires module to have trigger outputs

          blinkOn();
          break;
        } else if (pitch > bottomNote) { // new note > bottom note -> change top note
          
          notesList[notesListIndex] = pitch;
          notesListIndex ++;
          
          topNote = pitch;

          printNotes(topNote, bottomNote, notesActive);

          blinkOn();
          break;
        } else if (pitch < bottomNote) { // new note < bottom note -> change bottom note

          notesList[notesListIndex] = pitch;
          notesListIndex ++;
          
          bottomNote = pitch;

          printNotes(topNote, bottomNote, notesActive);

          blinkOn();
          break;
        }
    }
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

  switch(notesActive) {
      case 0:
        break;
      case 1:

        if (pitch != topNote || pitch != bottomNote) {
          
          break;
        } else if (pitch == topNote || pitch == bottomNote) {

          for (int i = notesListIndex; i >= 0; i--) {
            if (notesList[i] != 0) {
                notesList[i] = 0;
                notesListIndex --;
              }
          }
            
          notesActive = 0;
          topNote = 0;
          bottomNote = 0;
  
          printNotes(topNote, bottomNote, notesActive);
          
          blinkOff();
          break;
        }
        
      case 2:

        if (pitch != bottomNote && pitch != topNote) { // Note turned off isn't either active one

          for (int i = notesListIndex; i >= 0; i--) {
            if (notesList[i] != 0) {
                notesList[i] = 0;
                notesListIndex --;
              }
          }
          
          break;
        } else if (pitch == bottomNote) { // bottom note is turned off -> monophonic

          for (int i = notesListIndex; i >= 0; i--) {
              
            }
          
          bottomNote = topNote;
          notesActive --;

          printNotes(topNote, bottomNote, notesActive);

          break;
        } else if (pitch == topNote) { // bottom note is turned off -> monophonic
          topNote = bottomNote;
          notesActive --;

          printNotes(topNote, bottomNote, notesActive);

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

void printNotes(int n1, int n2, int nActive) {
    Serial.print("TOP NOTE: ");
    Serial.print(n1);
    Serial.print(" || BOTTOM NOTE: ");
    Serial.print(n2);
    Serial.print(" || NOTES ACTIVE: ");
    Serial.println(nActive);
  }
