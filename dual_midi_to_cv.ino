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

mcp4728 myDac = mcp4728(0);

MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, midiIn); // Creates a custom instance of MIDI

/*
 * TODO here: clean the mess, remove unused variables etc.
 */

byte data;
bool maxNotes;
int notesActive, topNote, bottomNote;
int notesListIndex = 0;
int notesList[22] = {0};

bool noteActive[128] = {0};

void setup(){
    pinMode(LEDboard, OUTPUT);
    pinMode(gateLED, OUTPUT);
    midiIn.setHandleNoteOn(noteOn); // Function called, when Note On message received
    midiIn.setHandleNoteOff(noteOff); // same as above but for Note Off message
    midiIn.begin();
    myDac.begin();
    myDac.vdd(5000);
    Serial.begin(9600);
    
    notesActive = 0;
    topNote = 0;
    bottomNote = 0;
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
        noteActive[pitch] = false; // some devices use Note On with 0 velocity as Note Off
    } else {
        noteActive[pitch] = true; // set note to active notes list
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

    if (noteActive[pitch] != true) {
        return;
    } else {
        noteActive[pitch] = false;
        playNote();
    }
    
}

void playNote() {
    // get highest active note and "play" it.
    // Highest pressed note has priority over all other notess
    topNote = getHighestNote();

    if (topNote == 0) { // if no active notes, set gate off
        Serial.print("GATE LOW || TOP NOTE: ");
        Serial.println(topNote);
        digitalWrite(gateLED, LOW);
    } else { // if active notes, play highest active
        Serial.print("GATE HIGH || TOP NOTE: ");
        Serial.println(topNote);
        sendToDac(topNote);
        digitalWrite(gateLED, HIGH);
        
    }
    /*
    Serial.print(" || BOTTOM NOTE: ");
    Serial.println(bottomNote);
    */
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
    // checks active note list for highest note and returns it
    int max = 0;
    for (int i = 0; i < 128; i++) {
        if (noteActive[i]) {
            if (i > max) {
            max = i;  
            }
        }
    }
    /*
    Serial.print("MAX: ");
    Serial.println(max);
    */
    return max;
}

void sendToDac(int note) {
    float mV = 85.3125f * note;
    if (mV >= 4095) {
        mV = 4095;
    }

    myDac.voutWrite(0, mV);
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
