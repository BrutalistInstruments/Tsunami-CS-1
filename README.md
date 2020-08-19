
# Tsunami CS-1 Readme

The Tsunami CS-1 is an open Source Audio Sample Player, based on the [Tsunami Wav Trigger by RobertSonics](https://robertsonics.com/tsunami/) and the Atmega 2560/Arduino Mega development board. 
Check out the build progress with pictures and video over at https://www.instagram.com/halford_c/

### Features:
 Tactile Control over Tsunami Wave Trigger:
- 255 Banks of 16  wav samples
- 8 Individual Assignable Outputs
- Per output volume control
- Per output pitch control (1 octave up and down)
- Volume envelope per sample
- Per Sample volume control
- Midi control of all tracks
- Midi In, Out and Through
 - Internal 64 Step Sequencer, with variable sequence length and BPM
 - 32 Voice Polyphony in mono mode, 16 in stereo mode


In this repository, you'll find:
## Schematics
We currently have schematics for the Main Control Surface, and the IO Board
## TsunamiArduinoCore
This is where the current stable build lives. Upload this through USB on your Arduino Mega in Platformio. Arudino IDE support coming soon (though not hard to convert for yourself). 
## TsunamiCS1Master
This is the development section of repository, only to be used with Atmel Studio and the Atmel Ice programmer. If you're interested In development and onboard debugging through J-tag, this is what you want to use. (currently depreciated)
## Enclosure
This has Files for laser cutting 1/8th " material to create enclosure. 
As this project progresses, new enclosure files for different fabrication facilities will be placed here.  (New enclosure files needed for new board revisions)

## BOM
Currently, our BOM is just a text file with links to carts, and component numbers. 
If any of these parts go out of stock, or are non-stocked, get in touch with us, and we'll post up a new cart link.
