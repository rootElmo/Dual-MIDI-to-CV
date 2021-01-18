# Dual MIDI to CV

A simple circuit to get 2 CV outs from MIDI. CV will be scaled to 1V/Oct.

**UNDER CONSTRUCTION**

## To-do

Here are the features planned for the module.

**Add schematics (Not started)**

**Note on/off logic (Done as of 18.1.2021)** 

Assign variables/flags to notes.

  * If both notes are pressed, and the either is released, the released note should turn off, not the one which was on first.
  * Polivoks-style note priority logic; if both notes are on, and a new note is pressed which is higher than the lowest note -> lowest note stays on, higher note gets changed to new note.

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


Made by: Elmo Rohula