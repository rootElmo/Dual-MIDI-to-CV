# Dual MIDI to CV

_**This project is currently on hold as I work on my Bachelor's thesis. I need the Teensy LC I used in this project for the work done on the thesis. Expect to see me back on this project around May of 2021, sooner if possible :). The project is now as it was in January of 2021 before I started working on my thesis.**_



A simple circuit to get 2 CV outs from MIDI. CV will be scaled to 1V/Oct.

I'm using a Teensy LC for development. MIDI takes one serial port, so I cannot use the serial monitor on the Arduino Uno. Teensy LC has the advantage of having multiple serial ports.

Inspired by [midi2cv by elkayem](https://github.com/elkayem/midi2cv). (Basic functionality and operational logic).

Uses the following libraries:

[mcp4728 by Neuroelec/Neurostar](https://code.google.com/archive/p/neuroelec/downloads), related forum post [here](https://forum.arduino.cc/index.php?topic=51842.0).

[Arduino MIDI Library by Francois Best](https://github.com/FortySevenEffects/arduino_midi_library)

**UNDER CONSTRUCTION**

## To-do

Here are the features planned for the module.

**Add schematics (Started 23.1.2021, ONGOING)**

  * Will be updated as more components are added to the circuit.
  * Can be viewed [here.](./schematics/schematics.pdf)

**Note on/off logic (Started 18.1.2021, ONGOING)** 

**Gate signals for _Note On_ and _Note Off_ messages (Started 21.1.2021, ONGOING)**

  * 1st channel **gate signal** works, need to add second one for duophonic mode.
  * **Monophonic** and **Polivoks**-style note priorities working.

**Make functions to send _Note_ values to _DAC_ (Started 23.1.2021, ONGOING)** 
  
  * **DAC** used is the **MCP4728** 4-channel **DAC**.
  * Currently both channels send appropriate voltages to the scaling op-amp!

**Scale and trim _DAC_ output to match _1v/Oct-scaling_ (Started 23.1.2021, ONGOING)**

  * Tracking is pretty accurate over multiple octaves. Some deviation can be seen. Could be remedied by hard-coding a scale factor and tweaking it, or by placing a trim potentiometer to the negative feedback loop of the scale amplifier op-amp.
  * Voltage variation could be fixed with calibration mode

**Implement _pitchbend_ (Not started)**

  * Get pitchbend value from the MIDI message -> add to existing output voltages (Vout = NoteVoltage + pitchbendVoltage).

**Implement changing of MIDI channel (Started 28.1.2021, ONGOING)**

  * Done via a "programming button"
  * Hold down button for a certain period of time to enter programming mode
  * Send _Note On_ from the desired channel. After saving the channel, device will only listen to that MIDI channel.

**Add _trigger_ outputs (POSSIBLE FEATURE)**

  * Not mandatory, but could be helpful for some situations.

**Add _velocity_ output (POSSIBLE FEATURE)**

  * Should work the same way as **Note On**

**Add calibration mode (POSSIBLE FEATURE)**

## Notes

Made by: Elmo Rohula