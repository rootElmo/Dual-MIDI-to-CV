# Dual MIDI to CV

A simple circuit to get 2 CV outs from MIDI. CV will be scaled to 1V/Oct.

I'm using a Teensy LC for development. MIDI takes one serial port, so I cannot use the serial monitor on the Arduino Uno. Teensy LC has the advantage of having multiple serial ports.

**UNDER CONSTRUCTION**

## To-do

Here are the features planned for the module.

**Add schematics (Not started)**

**Note on/off logic (Under work)** 

Changed whole logic of the system. **Note On** now stores the highest and lowest key pressed.

  * **Note Off** logic still under work.
  * Need to get keys stored and removed (true&false) before creating note priorities.

**Gate signals for _Note On_ and _Note Off_ messages (Not started)**

  * Simple **DigitalWrite** should be enough. Voltages can be amplified with op-amps.

**Make functions to send _Note_ values to _DAC_ (Not started)** 
  
  * **DAC** will be either 2 or 4 channel, **12-bit**.
  * Need to learn basic usage of a **DAC**.

**Scale and trim _DAC_ output to match _1v/Oct-scaling_ (Not started)**

**Implement pitchbend (Not started)**

  * Math is required (yuck).

**Add trigger outputs (POSSIBLE FEATURE)**

  * Not mandatory, but could be helpful for some situations.

**Add _velocity_ output (POSSIBLE FEATURE)**


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