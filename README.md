# Dual MIDI to CV

A simple circuit to get 2 CV outs from MIDI. CV will be scaled to 1V/Oct.

I'm using a Teensy LC for development. MIDI takes one serial port, so I cannot use the serial monitor on the Arduino Uno. Teensy LC has the advantage of having multiple serial ports.

**UNDER CONSTRUCTION**

## To-do

Here are the features planned for the module.

**Add schematics (Not started)**

**Note on/off logic (Under work)** 

When top or bottom note is released and there are still "keys pressed", the code doesn't remember them being pressed. Instead of the top or bottom note getting new values from the keys still "on", the note simply turns off.

  * program doesn't remember pressed keys.
  * pressed keys need to be stored and checked upon on **Note Off**

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