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
| Encoder reading issues | Interupt enable on encoderA, need to make hardware revision for encoderB|
| Encoder switch bouncing | debouncing decoder switch, or falling edge detection |
| Midi in dropping bytes | use arduino midi library |

## To-Implement
#### Firmware
| Feature |Depenencies|
| ------- |-------|
|Midi-Out|Midi-In functioning correctly|
|I2C Library|- - -|
|Bank Storage|I2C Library|
|GP-Button Functionallity|GP section of Button Library|
|Sequencer|Variable Timer interupts|
|Variable BPM Sequencing|Sequencer|
|Logarithmic Knob Reading|Knob Library|
|GP-Button LEDs|LED output Library|
|Envelop Knob Functionallity|Knob Library|

#### Hardware
|Feature|Dependencies|
|----|----|
|LowPass Filter on audio Outputs|Re-Design of IO Board|
|Encoder Pin Interupts|Encoders need to be changed so that each have 1 interupt pin, instead of EncoderA having 2.|
|Resistors on the 2 lines of the SPI bus connecting them to VCC|Rev5 of CS-PCB|

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
|Midi notes per track assinable per pattern|
