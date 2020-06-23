/*
 * globalVariables.h
 *
 * Created: 8/13/2019 5:30:21 PM
 *  Author: Hal
 */

#include <avr/io.h>

#ifndef GLOBALVARIABLES_H_
#define GLOBALVARIABLES_H_


	//defines for menu states.
	
	//this menu system lets us have 8 menu items per menu, and up to 8 menus. More than this,
	//and I think we have a feature creep problem. 
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

	#define TrackMenuArrow1Select 41 //case 0 010 1 001
	#define TrackMenuArrow2Select 42 //case 0 010 1 010
	#define TrackMenuArrow3Select 43 //case 0 010 1 011

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

typedef struct Pattern
{
	uint8_t outputLevelMSB[8]; //8 bytes
 	uint8_t outputLevelLSB[8]; //8 bytes
 	uint8_t outputPitch[8]; // 8 bytes
 	uint8_t trackOutputRoute[16]; // 16 bytes
 	uint8_t trackMainVolumeMSB[16]; //16 bytes
 	uint8_t trackMainVolumeLSB[16]; //16 bytes //should maybe be replaces with "trackStartVolume"
 	uint8_t trackAttackTimeMSB[16]; //16 bytes //replace with "attackTime"
 	uint8_t trackAttackTimeLSB[16]; //16 bytes
 	uint8_t trackReleaseTimeMSB[16]; //16 bytes  //replace with "releaseTime"
 	uint8_t trackReleaseTimeLSB[16]; //16 bytes
 	uint8_t trackPlayMode[16]; //16 bytes //right now, only play solo (0)and Poly (1)are supported. loop mode (4) is in the works. 
 	uint8_t trackSampleMSB[16]; //16 bytes
 	uint8_t trackSampleLSB[16]; //16 bytes
 	uint8_t voiceLockFlag[16]; //16 bytes
	uint16_t patternBPM; //2 bytes
	uint8_t numSteps; //1 byte
	uint8_t trackSustainTimeMSB[16];
	uint8_t trackSustainTimeLSB[16];
	uint16_t trackSequence[64]; //128 bytes - this will be a separate page
	
	//attack time (2 8 bit integers) - replaced
	//sustain volume (aka, track volume) - this will be the Track Volume item. It won't change the track volume in the play function. 
	//sustain Time (2 8 bit integers?) - needs to be added. 
	//release time (2 8 bit integers) - replaced
	//I don't think we necessarily need release level, since we'll be stopping the track after each release stage is complete. 
	//though this may cause issues if two of the same tracks get triggered right after on another. 
	//will the first release stop the track? or should we wait until the last release is sent, and ignore the previous release comands?


} Pattern; //total bytes - 128 for the sequence, 203 bytes for all other data.
//so 3 pages in total, 1 for sequencer data, 2 for all other data (248 bytes, room for expansion)

typedef struct Screen
{

	unsigned char screen0[4][21];
	unsigned char screen1[4][21];
	unsigned char screen2[4][21];
	unsigned char screen3[4][21];
	uint8_t screen0Index;
	uint8_t screen1Index;
	uint8_t screen2Index;
	uint8_t screen3Index;
	unsigned char knobScreen[6][21]; 
	

} Screen;

typedef struct Globals
{
	uint16_t currentTrigButtons; //current state of Trig buttons.
	uint8_t currentGPButtons; //current state of GP buttons
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
	uint32_t releaseCounter; //global counter for release times.
	uint16_t releaseTracker; //this is a bitwise tracker to keep track of which tracks still need to be releases.
	uint32_t sustainCounterArray[16]; //this will be where we store values for release times to check against the release counter.

	
}Globals;

void initBank(Pattern *currentInitPattern);
void initArrays(unsigned char myArray[4][21], uint8_t lengthOfString, int stringNumber, char* myString);
void initGlobals(Globals *currentGlobals, uint8_t factoryReset);

#endif /* GLOBALVARIABLES_H_ */
