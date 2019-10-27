/*
 * globalVariables.h
 *
 * Created: 8/13/2019 5:30:21 PM
 *  Author: Hal
 */

#include <avr/io.h>

#ifndef GLOBALVARIABLES_H_
#define GLOBALVARIABLES_H_

typedef struct Pattern
{
	uint8_t outputLevelMSB[8]; //8 bytes
 	uint8_t outputLevelLSB[8]; //8 bytes
 	uint8_t outputPitch[8]; // 8 bytes
 	uint8_t trackOutputRoute[16]; // 16 bytes
 	uint8_t trackMainVolumeMSB[16]; //16 bytes
 	uint8_t trackMainVolumeLSB[16]; //16 bytes
 	uint8_t trackFadeGainMSB[16]; //16 bytes
 	uint8_t trackFadeGainLSB[16]; //16 bytes
 	uint8_t trackFadeTimeMSB[16]; //16 bytes
 	uint8_t trackFadeTimeLSB[16]; //16 bytes
 	uint8_t trackPlayMode[16]; //16 bytes
 	uint8_t trackSampleMSB[16]; //16 bytes
 	uint8_t trackSampleLSB[16]; //16 bytes
 	uint8_t voiceLockFlag[16]; //16 bytes
 	uint16_t trackSequence[64]; //128 bytes - this will be a seperate page
	uint8_t midiTrackNote[16]; //16 bytes
	uint16_t patternBPM; //2 bytes
	uint8_t numSteps; //1 byte

} Pattern; //total bytes - 128 for the sequence, 219 bytes for all other data.
//so 3 pages in total, 1 for sequencer data, 2 for all other data (248 bytes, room for expansion

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


} Screen;

extern uint16_t currentTrigButtons;
extern uint8_t currentGPButtons;

extern uint8_t currentPatternNumber;
extern uint8_t currentStep;
extern uint8_t currentTrack;

void initBank(Pattern currentInitPattern);
void initArrays(unsigned char myArray[4][21], uint8_t lengthOfString, int stringNumber, char* myString);
#endif /* GLOBALVARIABLES_H_ */
