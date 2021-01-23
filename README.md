# Dual MIDI to CV

A simple circuit to get 2 CV outs from MIDI. CV will be scaled to 1V/Oct.

I'm using a Teensy LC for development. MIDI takes one serial port, so I cannot use the serial monitor on the Arduino Uno. Teensy LC has the advantage of having multiple serial ports.

**UNDER CONSTRUCTION**

## To-do

Here are the features planned for the module.

**Add schematics (Started 23.1.2021, ONGOING)**

  * Will be updated as more components are added to the circuit.
  * Can be viewed [here.](./schematics/schematics.pdf)

**Note on/off logic (Started 18.1.2021, ONGOING)** 

  * Changed whole logic of the system. **Note On** now stores the highest and lowest key pressed.
  * **Note Off** logic still under work.
  * Need to get keys stored and removed (true&false) before creating note priorities.

**Gate signals for _Note On_ and _Note Off_ messages (Started 21.1.2021, ONGOING)**

  * Simple **DigitalWrite** turns on a **LED**, signaling a **GATE HIGH**.

**Make functions to send _Note_ values to _DAC_ (Started 23.1.2021, ONGOING)** 
  
  * **DAC** used is the **MCP4728** 4-channel **DAC**.
  * Need to learn basic usage of a **DAC**.
  * Currently only one channel sends out voltages.

**Scale and trim _DAC_ output to match _1v/Oct-scaling_ (Not started)**

**Implement _pitchbend_ (Not started)**

  * Get pitchbend value from the MIDI message -> add to existing output voltages (Vout = NoteVoltage + pitchbendVoltage).

**Add _trigger_ outputs (POSSIBLE FEATURE)**

  * Not mandatory, but could be helpful for some situations.

**Add _velocity_ output (POSSIBLE FEATURE)**

  * Works the same way as **Note On**

## Bugs/misc. notes

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

Made by: Elmo Rohula