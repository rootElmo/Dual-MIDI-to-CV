# Dual MIDI to CV

A simple circuit to get 2 CV outs from MIDI. CV will be scaled to 1V/Oct.

I'm using a Teensy LC for development. MIDI takes one serial port, so I cannot use the serial monitor on the Arduino Uno. Teensy LC has the advantage of having multiple serial ports.

Inspired by [midi2cv by elkayem](https://github.com/elkayem/midi2cv). (Basic functionality and operational logic).

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

**Scale and trim _DAC_ output to match _1v/Oct-scaling_ (Started 23.1.2021, ONGOING)**

  * Tracking is pretty accurate over multiple octaves. Some deviation can be seen. Could be remedied by hard-coding a scale factor and tweaking it, or by placing a trim potentiometer to the negative feedback loop of the scale amplifier op-amp.

**Implement _pitchbend_ (Not started)**

  * Get pitchbend value from the MIDI message -> add to existing output voltages (Vout = NoteVoltage + pitchbendVoltage).

**Add _trigger_ outputs (POSSIBLE FEATURE)**

  * Not mandatory, but could be helpful for some situations.

**Add _velocity_ output (POSSIBLE FEATURE)**

  * Should work the same way as **Note On**

## Notes

Made by: Elmo Rohula