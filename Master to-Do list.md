
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

## To-Implement
#### Firmware
| Feature |Depenencies|
| ------- |-------|
|Midi-Out|Midi-In functioning correctly|
|Logarithmic Knob Reading|Knob Library|
|GP-Button LEDs|LED output Library|
|Envelop Knob Functionality|Knob Library|

#### Hardware
|Feature|Dependencies|
|----|----|
|Encoder Pin Interrupts|Encoders need to be changed so that each have 1 interrupt pin, instead of EncoderA having 2.|
|Resistors on the 2 lines of the SPI bus connecting them to VCC|Rev5 of CS-PCB|
|Move Tsunami power and serial pin Headers|

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