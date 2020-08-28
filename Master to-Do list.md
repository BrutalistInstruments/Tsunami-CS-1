
# CS1-Tsunami Sampler 
####  Master Buglist and Implementation progress
This document exist to keep record of the status of features and bugs in the firmware, hardware, and enclosure of the CS1Tsunami. 
This is mainly to help us keep track of what needs doing. 

What's here:
  - Know bugs / issues
  - Un-implemented features
  - Hardware and enclosure to-dos. (In version 1.0, you'll see nothing here)

## Bugs
#### Firmware
| Known Bug| Proposed Solution |
| ------ | ------ |
|Encoder Reading sometimes goes backwards|timer based debouncing, a bit more research|
|BPM readout for patterns is incorrect|Need to work on the BPM/sequencer algorythm a bit more|

## To-Implement
#### Firmware
| Feature |Depenencies|
| ------- |-------|
|Program Change messages change patterns|1.1|
|Midi-Out on Trigger Buttons|Version 1.1|
|TR Style Step Sequencer|1.1|
|Logarithmic Knob Reading|Verion 1.2|

#### Hardware
|Feature|Dependencies|
|----|----|
|Voltage Divider for Tsunami Serial port out|Board Revision 8|
|A2 eeprom chip tied to +5v instead of GND|Board Revision 8|
|Schematic reading of 4.7K pullups on TWI Buss|Board Revision 8|
All above hardware features are now implemented, and just need to be tested when new boards are ordered 8/5/2020


## Enclosure
|Feature|
|need to start work on aluminum enlcosure|
