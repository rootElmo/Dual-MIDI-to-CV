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
#define gate1LED 12
#define gate2LED 11
#define maxNotes 87
#define progBtn 15

mcp4728 myDac = mcp4728(0);

MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, midiIn); // Creates a custom instance of MIDI

byte data;
int topNote, bottomNote, activeNotes;
int notePriority, noteAssignment;
int8_t noteOrder[20] = {0}, orderIndx = {0};

bool notesList[88] = {0};

void setup(){
    pinMode(LEDboard, OUTPUT);
    pinMode(gate1LED, OUTPUT);
    pinMode(gate2LED, OUTPUT);
    pinMode(progBtn, INPUT);
    midiIn.setHandleNoteOn(noteOn); // Function called, when Note On message received
    midiIn.setHandleNoteOff(noteOff); // Function called, when Note Off message received
    midiIn.begin();
    myDac.begin();
    myDac.vdd(5000);
    Serial.begin(9600);
    
    topNote = 0;
    bottomNote = 0;
    notePriority = 1; // 0 = polivoks, 1 = monophonic, 2 = duophonic
    noteAssignment = 1; // 0 = diamond, 1 = last note
}

void loop() {
    midiIn.read();
    readBtn();
}

void noteOn(byte channel, byte pitch, byte vel){
    printMidiOn(channel, pitch, vel);
    // Function called, when "Note On" received from MIDI
    
    if (pitch < 0 || pitch > maxNotes) {
       return; 
    } else if (vel <= 0) {
        notesList[pitch] = false; // some devices use Note On with 0 velocity as Note Off
    } else if (notesList[pitch] == true) {
        return; // re-trigger trigger outputs here
    } else {
        notesList[pitch] = true;
    }
    if (noteAssignment == 1) {
        if (notesList[pitch] == true) {
            orderIndx = (orderIndx + 1) % 20;
            noteOrder[orderIndx] = pitch;
        } 
    }
    handleNote();
}

void noteOff(byte channel, byte pitch, byte vel){
    printMidiOff(channel, pitch, vel);
    // Function called, when "Note Off" received from MIDI
    
    if (notesList[pitch] == false) {
        return;
    } else if (notesList[pitch] == true) {
        notesList[pitch] = false;
    }
    handleNote();
}

void handleNote() {
    // get highest active note and "play" it.
    // Highest pressed note has priority over all other notess

    // For loops below part of "diamond" note assignment. In the future, break note assignments
    // to different functions. Can be like this for now
    switch (notePriority) {
        case 0: // Polivoks-style note priority
            topNote = topDiamondNote();
            bottomNote = bottomDiamondNote();
            
            if (topNote == bottomNote) {
                sendToDac(topNote, bottomNote, 1);
                allGates(true);
            } else if (topNote != bottomNote) {
                sendToDac(topNote, bottomNote, 2);
                allGates(true);
            }

        break;
        case 1: // Monophonic note priority
            // ONLY LAST NOTE ASSIGNMENT
            int8_t noteIndx;
  
            for (int i=0; i<20; i++) {
                noteIndx = noteOrder[mod(orderIndx-i, 20)];
                if (notesList[noteIndx] == true) {
                    sendToDac(noteIndx, noteIndx, 1);
                    allGates(true);
                  return;
                }
            }
            allGates(false); // All notes are off
        break;
        case 2: // Duophonic note priority
            
        break;
    }
}

int topDiamondNote() {
    int note = 0;
    for (int i = 0; i < 88; i++) { // top note loop
        if (notesList[i]) {
            note = i;
        }
    }
    return note;    
}

int bottomDiamondNote() {
    int note = 0;
    for (int i = 88; i >= 0; i--) {
        if (notesList[i]) {
            note = i;   
        }
    }
    return note;
}

void allGates(bool i) {
    if (i == true) {
        digitalWrite(gate1LED, HIGH);
        digitalWrite(gate2LED, HIGH);
    } else {
        digitalWrite(gate1LED, LOW);
        digitalWrite(gate2LED, LOW);
    }
}

void sendToDac(int note1, int note2, int dacAmnt) {
    
    float mVPerNote = 47.069f;
    unsigned int mVToDacOut0 = (unsigned int) note1 * mVPerNote;
    unsigned int mVToDacOut1 = (unsigned int) note2 * mVPerNote;
    
    if (mVToDacOut0 >= 4095) {
        mVToDacOut0 = 4095;
    }
    if (mVToDacOut1 >= 4095) {
        mVToDacOut1 = 4095;
    }

    if (dacAmnt == 1) {
        myDac.voutWrite(0, mVToDacOut0);
        myDac.voutWrite(1, mVToDacOut0);
    } else if (dacAmnt == 2){
        myDac.voutWrite(0, mVToDacOut0);
        myDac.voutWrite(1, mVToDacOut1);
    }
}

int mod(int a, int b) {
    int r = a % b;
    return r < 0 ? r + b : r;
}

void readBtn() {
    if (digitalRead(progBtn) == LOW) {
        Serial.println("Prog btn pressed!");
    }
}

void blinkOn(){
    digitalWrite(LEDboard, HIGH);
}

void blinkOff(){
    digitalWrite(LEDboard, LOW);
}

void printMidiOn(byte ch, byte p, byte vel) {
    Serial.print("MIDI NOTE ON: channel: ");
    Serial.print(ch);
    Serial.print(", pitch: ");
    Serial.print(p);
    Serial.print(", velocity: ");
    Serial.println(vel);
}

void printMidiOff(byte ch, byte p, byte vel) {
    Serial.print("MIDI NOTE OFF: channel: ");
    Serial.print(ch);
    Serial.print(", pitch: ");
    Serial.print(p);
    Serial.print(", velocity: ");
    Serial.println(vel);
}
