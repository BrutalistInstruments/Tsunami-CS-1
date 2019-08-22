/*
 * globalVariables.h
 *
 * Created: 8/13/2019 5:30:21 PM
 *  Author: Hal
 */ 


//this is where our main structs and global variables will live, so that all other libraries can see them.
#include <avr/io.h>

#ifndef GLOBALVARIABLES_H_
#define GLOBALVARIABLES_H_

typedef struct Pattern
{//default levels
	uint8_t outputLevelMSB[8];
 	uint8_t outputLevelLSB[8];
 	uint8_t outputPitch[8];
 	uint8_t trackOutputRoute[16];
 	uint8_t trackMainVolumeMSB[16];
 	uint8_t trackMainVolumeLSB[16];
 	uint8_t trackFadeGainMSB[16];
 	uint8_t trackFadeGainLSB[16];
 	uint8_t trackFadeTimeMSB[16];
 	uint8_t trackFadeTimeLSB[16];
 	uint8_t trackPlayMode[16];
 	uint8_t trackSampleMSB[16];
 	uint8_t trackSampleLSB[16];
 	uint8_t voiceLockFlag[16];
 	uint16_t trackSequenceMSB[64];
} Pattern; //what is the size of this struct?

uint8_t knobBuffer[44];
uint8_t checkBuffer[44];
uint8_t knobBufferCounter;
Pattern currentPattern;
uint8_t encoderAValue;
uint8_t encoderBValue;
uint8_t prevEncoderAValue;
uint8_t prevEncoderBValue;
uint16_t currentTrigButtons;
uint8_t currentGPButtons;

void initBank();




#endif /* GLOBALVARIABLES_H_ */