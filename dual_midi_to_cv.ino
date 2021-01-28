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
#define progBtn 15

mcp4728 myDac = mcp4728(0);

MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, midiIn); // Creates a custom instance of MIDI

byte data;
int topNote, bottomNote;
int notePriority;

bool notesList[88] = {0};

void setup(){
    pinMode(LEDboard, OUTPUT);
    pinMode(gateLED, OUTPUT);
    pinMode(progBtn, INPUT);
    midiIn.setHandleNoteOn(noteOn); // Function called, when Note On message received
    midiIn.setHandleNoteOff(noteOff); // same as above but for Note Off message
    midiIn.begin();
    myDac.begin();
    myDac.vdd(5000);
    Serial.begin(9600);
    
    topNote = 0;
    bottomNote = 0;
    notePriority = 1; // 0 = polivoks, 1 = monophonic, 2 = duophonic
}

void loop() {
    midiIn.read();
    readBtn();
}

void noteOn(byte channel, byte pitch, byte vel){
    printMidiOn(channel, pitch, vel);
    // Function called, when "Note On" received from MIDI
    
    if (vel <= 0) {
        notesList[pitch] = false; // some devices use Note On with 0 velocity as Note Off
    } else if (pitch < 0 || pitch > maxNotes) {
        return;
    } else if (notesList[pitch] == true) {
        return; // re-trigger trigger outputs here
    } else {
        notesList[pitch] = true; // set note to active notes list
    }
    handleNote();
}

void noteOff(byte channel, byte pitch, byte vel){
    printMidiOff(channel, pitch, vel);
    // Function called, when "Note Off" received from MIDI

    int notesActive = 0;
    
    if (notesList[pitch] == false) {
        return;
    } else if (notesList[pitch] == true) {
        notesList[pitch] = false;
    }
    for (int i = 0; i < 88; i++) {   
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
    
    bool topNoteActive = false;
    bool bottomNoteActive = false;

    // For loops below part of "diamond" note assignment. In the future, break note assignments
    // to different functions. Can be like this for now
    
    for (int i = 0; i < 88; i++) { // top note loop
        if (notesList[i]) {
            topNote = i;
            topNoteActive = true; 
        }
    }
    for (int i = 88; i >= 0; i--) {
        if (notesList[i]) {
            bottomNote = i;
            bottomNoteActive = true;    
        }
    }
    Serial.print("BOTTOM NOTE: ");
    Serial.println(bottomNote);

    switch (notePriority) {
        case 0: // Polivoks-style note priority
            if (topNoteActive == true) {
                if (bottomNoteActive == false) { // if top is on and bottom off, send top note to both CV outs
                    sendToDac(topNote, bottomNote, 1);
                    digitalWrite(gateLED, HIGH);
                } else if (bottomNoteActive == true) {
                    if (topNote == bottomNote) { // if both notes are on, but the same value, send same value to both CV outs
                        sendToDac(topNote, bottomNote, 1); // write to DAC outs: Top note, Bottom note, Channel amount
                        digitalWrite(gateLED, HIGH);
                    } else { // if top and bottom notes are on, but different value, send values to corresponding CV outs
                        sendToDac(topNote, bottomNote, 2); // write to DAC outs: Top note, Bottom note, Channel amount
                        digitalWrite(gateLED, HIGH);
                    }
                }
            }
        break;
        case 1: // Monophonic note priority
            if (topNoteActive == true) {
                sendToDac(topNote, topNote, 1); // write to DAC outs: Top note, Bottom note, Channel amount
                digitalWrite(gateLED, HIGH); // Both channels get top note's value
            }
        break;
        case 2: // Duophonic note priority
        break;
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
