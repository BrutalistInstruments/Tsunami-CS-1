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
| Encoder reading issues | debouncing encoder pins, or interupts on falling edge |
| Encoder switch bouncing | debouncing decoder switch, or falling edge detection |
| Midi in dropping bytes | use arduino midi library |

## To-Implement
#### Firmware
| Feature |Depenencies|
| ------- |-------|
|Midi-Out|Midi-In functioning correctly|
|Spi-bus Library|- - -|
|Bank Storage|Spi-Bus Library|
|GP-Button Functionallity|GP section of Button Library|
|Sequencer|Variable Timer interupts|
|Variable BPM Sequencing|Sequencer|
|Logarithmic Knob Reading|Knob Library|
|GP-Button LEDs|LED output Library|

#### Hardware
|Feature|Dependencies|
|----|----|
|LowPass Filter on audio Outputs|Re-Design of IO Board|

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
|Trigger LEDs on Preformance Mode|
|Main struct, ready for storage|