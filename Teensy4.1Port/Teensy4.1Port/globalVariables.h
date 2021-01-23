// globalVariables.h

#ifndef _GLOBALVARIABLES_h
#define _GLOBALVARIABLES_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

	//this menu system lets us have 8 menu items per menu, and up to 8 menus. This should be more than enough room for future expansion.
	//Bits - 7 encoderA Flag, 6-4 EncoderAState, 3: EncoderBFlag, 2-0: encoderBState
	//Encoder B State: 0 = no arrow;
	//Encoder B State: 1 = arrow on screen, init state, very top
	//Encoder B State: 2 = arrow in middle of screen
	//Encoder B State: 3 = arrow at bottom of the page

#define	PreformanceModeInit 0 //case 0 000 0 000

#define	SequencerMenuInit 16 //case 0 001 0 000
#define	SequencerMenuArrow1 17 //case 0 001 0 001
#define	SequencerMenuArrow2 18 //case 0 001 0 010
#define	SequencerMenuArrow3 19//case 0 001 0 011

#define	SequencerMenuArrow1Select 25 //case 0 001 1 001
#define SequencerMenuArrow2Select 26 //case 0 001 1 010
#define SequencerMenuArrow3Select 27 //case 0 001 1 011

#define	TrackMenuInit 32 //case 0 010 0 000
#define	TrackMenuArrow1 33 //case 0 010 0 001
#define	TrackMenuArrow2 34 //case 0 010 0 010
#define	TrackMenuArrow3 35 //case 0 010 0 011
#define	TrackMenuArrow4 36 //case 0 010 0 100
#define	TrackMenuArrow5 37 //case 0 010 0 101


#define TrackMenuArrow1Select 41 //case 0 010 1 001
#define TrackMenuArrow2Select 42 //case 0 010 1 010
#define TrackMenuArrow3Select 43 //case 0 010 1 011
#define TrackMenuArrow4Select 44 //case 0 010 1 100
#define TrackMenuArrow5Select 45 //case 0 010 1 101

#define	GlobalMenuInit 48  //case 0 011 0 000
#define	GlobalMenuArrow1 49 //case 0 011 0 001
#define	GlobalMenuArrow2 50 //case 0 011 0 010
#define	GlobalMenuArrow3 51//case 0 011 0 011

#define GlobalMenuArrow1Select 57 //case 0 011 1 001
#define GlobalMenuArrow2Select 58 //case 0 011 1 010
#define GlobalMenuArrow3Select 59 //case 0 011 1 011

#define encoderChange 0
#define triggerChange 1
#define knobChange	2

//Defines for all pins on Teensy Microcontroller

#define OLEDEnable 6
#define OLEDDataCommand 26
#define OLEDData0 9
#define OLEDData1 10
#define OLEDData2 11
#define OLEDData3 12
#define OLEDData4 27
#define OLEDData5 30
#define OLEDData6 31
#define OLEDData7 32





typedef struct Pattern
{
	uint8_t outputLevelMSB[8]; //8 bytes
	uint8_t outputLevelLSB[8]; //8 bytes
	uint8_t outputPitch[8]; // 8 bytes
	uint8_t trackOutputRoute[16]; // 16 bytes
	uint8_t trackMainVolumeMSB[16]; //16 bytes
	uint8_t trackMainVolumeLSB[16]; //16 bytes 
	uint8_t trackAttackTimeMSB[16]; //16 bytes 
	uint8_t trackAttackTimeLSB[16]; //16 bytes
	uint8_t trackReleaseTimeMSB[16]; //16 bytes  
	uint8_t trackReleaseTimeLSB[16]; //16 bytes
	uint8_t trackPlayMode[16]; //16 bytes //right now, only play solo (0)and Poly (1)are supported. loop mode (4) is in the works. 
	uint8_t trackSampleMSB[16]; //16 bytes
	uint8_t trackSampleLSB[16]; //16 bytes
	uint8_t voiceLockFlag[16]; //16 bytes
	uint16_t patternBPM; //2 bytes
	uint8_t numSteps; //1 byte
	uint8_t trackSustainTimeMSB[16]; //16 bytes 
	uint8_t trackSustainTimeLSB[16]; //16 bytes
	uint8_t envelopeType[16]; //16 bytes //currently we only have 4, but eventually I'd like to have release stages triggered by "off" messages, especially for looped samples. 
	uint16_t trackSequence[64]; //128 bytes - this will be a separate page

} Pattern; //total bytes - 128 for the sequence, 251 bytes for all other data
//so 3 pages in total, 1 for sequencer data, 2 for all other data (248 bytes, room for expansion)

typedef struct Screen
{

	unsigned char screen0[9][21]; //since each screen can hold 3 bit menu states, there are 9 values that each menu can have. 
	unsigned char screen1[9][21]; //not every array needs to be initialized, but it is important for our initArray method that they be uniform. 
	unsigned char screen2[9][21];
	unsigned char screen3[9][21];
	unsigned char knobScreen[9][21];


} Screen;

typedef struct Globals
{
	uint16_t currentTrigButtons; //current state of Trig buttons.
	uint8_t currentGPButtons; //current state of GP buttons
	uint16_t currentTrigSequencer;
	uint16_t currentTrigMidi;
	uint8_t currentPatternNumber; //current pattern, between 1 and 256
	uint8_t currentStep; // current step in the sequencer
	uint8_t currentTrack; //current track being edited
	uint8_t menuState; //where the menu is currently
	uint8_t playState; //whether the sequencer is playing, stopped, or paused. 
	uint8_t factoryReset; //we may not need this in this struct, but good to have for now. 
	uint8_t buttonSwitchFlag; // could be rolled into value bits.
	uint8_t valueChangeFlag; //bit 0 -> changes in encoders, bit 1-> changes in buttons, bit2 -> changes in knobs
	uint8_t knobStatus; //top 4 bits: knob type, bottom 4 bits: knob location.

	//these are editable in global edit menu
	uint8_t midiChannel;
	uint8_t midiTrackNote[16]; //16 bytes
	uint8_t rawKnobBuffer[44]; //we write to this to collect raw knob reads
	uint8_t filteredKnobBuffer[44]; //we do math to the raw Knob buffer, and store it here, to check against current knobs with. 
	uint8_t lastFilteredKnobBuffer[44]; //we use this to check if any knob positions have changed. 
	//envelope variables
	uint32_t releaseCounter;
	uint16_t releaseTracker; //this is a bitwise tracker to keep track of which tracks still need to be releases.
	uint32_t sustainCounterArray[16]; //this will be where we store values for release times to check against the release counter.
	uint16_t clockCounter;
	uint32_t lastGlobalTimer;
	uint8_t timerFlag; //if 1, then timer has increased. if 0, timer has not increased. 


	//New Globals for Teensy port.
	uint16_t OLEDBuffer[256];
	uint8_t oledReadIndex;
	uint8_t oledWriteIndex;

}Globals;

void initBank(volatile Pattern* currentInitPattern);
void initGlobals(volatile Globals* currentGlobals, uint8_t factoryReset);

#endif

