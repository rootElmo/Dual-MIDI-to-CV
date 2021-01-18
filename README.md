# Dual MIDI to CV

A simple circuit to get 2 CV outs from MIDI.

**UNDER CONSTRUCTION**

## To-do

**Note on/off logic**

Assign variables/flags to notes.

  * If both notes are pressed, and the either is released, the released note should turn off, not the one which was on first.
  * Polivoks-style note priority logic; if both notes are on, and a new note is pressed which is higher than the lowest note -> lowest note stays on, higher note gets changed to new note
