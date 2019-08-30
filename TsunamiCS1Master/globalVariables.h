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
	uint8_t midiTrackNote[16];
	uint16_t patternBPM;
	uint8_t numSteps;
	
} Pattern;

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
uint8_t midiChannel;
uint8_t currentPatternNumber;
uint8_t currentStep;
uint8_t currentTrack;
uint8_t encoderAFlag;
uint8_t encoderBFlag;

//screen 0
char screen0[4][20];
// = {"Performance Mode    ","Pattern:            ","BPM: xxx            ","Stop                ","Play                "  };
uint8_t screen0Index;

//screen 1:
char screen1[4][20];
// = {"Sequence Edit       ","Pattern:            ","Steps:              ","Step number:        "}; //this will eventually be 5 once we implement naming of samples.
uint8_t screen1Index;

//screen 2:
char screen2[4][20]; 
//= {"Track Settings      ","Track:              ","Play Mode           ","OutRoute            "};
uint8_t screen2Index;

//screen 3:
char screen3[4][20];
// = {"Global Settings     ","Midi Channel: xx    ", "Midi trigger Notes  ","(put triggered note)"};
uint8_t screen3Index;


void initBank();
void initArrays(char myArray[4][20], uint8_t lengthOfString, int stringNumber, char* myString);




#endif /* GLOBALVARIABLES_H_ */