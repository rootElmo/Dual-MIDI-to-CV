//////////////////////////////
//  CREATED BY ELMO ROHULA  //
//////////////////////////////

// Dual MIDI to CV
// version 0.0.1
// A Dual MIDI IN device

#include <MIDI.h>
#include <Wire.h>
#include "mcp4728.h"

#define LEDboard 13
#define gateLED 12
#define maxNotes 87

mcp4728 myDac = mcp4728(0);

MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, midiIn); // Creates a custom instance of MIDI

/*
 * TODO here: clean the mess, remove unused variables etc.
 */

byte data;
int topNote;

bool notesList[128] = {0};

void setup(){
    pinMode(LEDboard, OUTPUT);
    pinMode(gateLED, OUTPUT);
    midiIn.setHandleNoteOn(noteOn); // Function called, when Note On message received
    midiIn.setHandleNoteOff(noteOff); // same as above but for Note Off message
    midiIn.begin();
    myDac.begin();
    myDac.vdd(5000);
    Serial.begin(9600);
    
    topNote = 0;
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
        notesList[pitch] = false; // some devices use Note On with 0 velocity as Note Off
        return;
    } else if (pitch < 0 || pitch > maxNotes) {
        return;
    } else {
        notesList[pitch] = true; // set note to active notes list
        handleNote();
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

    int notesActive = 0;
    
    if (notesList[pitch] == false) {
        return;
    } else if (notesList[pitch] == true) {
        notesList[pitch] = false;
    }

    for (int i = 0; i < 128; i++) {   
        if (notesList[pitch] == true) {
            notesActive++;    
        }    
    }
    if (notesActive != 0) {
        return;
    } else if (notesActive == 0){
        digitalWrite(gateLED, LOW);    
    }
    handleNote();
    
}

void handleNote() {
    // get highest active note and "play" it.
    // Highest pressed note has priority over all other notess
    topNote = 0;
    bool noteActive = false;
    
    for (int i = 0; i < 128; i++) {
        if (notesList[i]) {
            topNote = i;
            noteActive = true; 
        }
    }
    if (noteActive == true) { // if active notes, play highest active, set gate on
        Serial.print("GATE HIGH || TOP NOTE: ");
        Serial.println(topNote);
        sendToDac(topNote);
        digitalWrite(gateLED, HIGH);
    } /*else { // if no active notes, set gate off
        Serial.print("GATE LOW || TOP NOTE: ");
        Serial.println(topNote);
        digitalWrite(gateLED, LOW);
    }*/
}

/*
int getLowestNote() {
    // DO NOT USE, TO  BE REMOVED
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
*/

void sendToDac(int note) {
    float mVPerNote = 47.069f;
    unsigned int mVToDac = (unsigned int) note * mVPerNote;
    if (mVToDac >= 4095) {
        mVToDac = 4095;
    }
    myDac.voutWrite(0, mVToDac);
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
