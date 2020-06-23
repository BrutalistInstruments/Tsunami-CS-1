
# CS1-Tsunami Sampler 
####  Master Buglist and Implementation progress
This document exist to keep record of the status of features and bugs in the firmware, hardware, and enclosure of the CS1Tsunami. This is mainly to help us keep track of what needs doing. Also, We're trying to get good at markdown.

What's here:
  - Know bugs / issues
  - Un-implemented features
  - New feature updates

## Bugs
#### Firmware
| Known Bug| Proposed Solution |
| ------ | ------ |
|Encoder Reading sometimes goes backwards|timer based debouncing, a bit more research|
|Attack Envelopes sometime cutting off sound|Possible knob jitter issue? Find lowest stable attack value and make that the first possible value|

## To-Implement
#### Firmware
| Feature |Depenencies|
| ------- |-------|
|Sustain stage and Release stage|Version 0.97|
|Factory Reset in Global Menu|Version 1.0|
|Midi-Out|Version 1.1|
|Logarithmic Knob Reading|Verion 1.2|


#### Hardware
|Feature|Dependencies|
|----|----|
|Voltage Divider for Tsunami Serial port out|Board Revision 8|
|A2 eeprom chip tied to +5v instead of GND|Board Revision 8|
|Schematic reading of 4.7K pullups on TWI Buss|Board Revision 8|



## Features Complete
#### Firmware
|Feature|
|-------|
|Functioning OLED Screen|
|Reading Trigger Buttons|
|Sending Serial Data to Tsunami|
|Reading knobs, storing knob values|
|Functioning Menu|
|Midi In|
|Trigger LEDs on Performance Mode|
|Main struct, ready for storage|
|Midi notes per track assignable per pattern|
|Encoder push button menu functionality|
|GP - Buttons are functioning correctly, ready for more features |
|Sequencer is now implemented and fully functional|
|Bank storage on through I2C eeprom is fully functional, but needs to be optimized.|
|Audio and Midi ground on IO board have been separated, eliminating digital noise|
|Midi in is not fully functional, using a home-made midi library|
|Encoders are now on interrupt pins in new hardware revision, but need to be re-coded for such use. |
|GP-Button LED|
|Factory Reset in code|
|Attack envelopes|