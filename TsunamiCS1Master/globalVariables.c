/*
 * globalVariables.c
 *
 * Created: 8/16/2019 7:26:29 AM
 *  Author: Hal
 */ 

#include <avr/io.h>
#include "globalVariables.h"

void initArrays(char myArray[4][20], uint8_t lengthOfString, int stringNumber, char* myString)
{
	uint8_t charLeft = 20 - lengthOfString;
	uint8_t currentIndex = 0;
	for(currentIndex; currentIndex<lengthOfString; currentIndex++)
	{
		myArray[stringNumber][currentIndex] = myString[currentIndex];
		
	}
	
	for(charLeft; charLeft>0; charLeft--)
	{
		
		myArray[stringNumber][currentIndex] = ' ';
		currentIndex++;
	}

}

void initBank()
{
	for(uint8_t i = 0; i<16; i++)
	{
		currentPattern.trackSampleLSB[i] = i+1;
	}
	
	//assign midi notes C3 to E4
	currentPattern.midiTrackNote[0] = 0x30;
	currentPattern.midiTrackNote[1] = 0x31;
	currentPattern.midiTrackNote[2] = 0x32;
	currentPattern.midiTrackNote[3] = 0x33;
	currentPattern.midiTrackNote[4] = 0x34;
	currentPattern.midiTrackNote[5] = 0x35;
	currentPattern.midiTrackNote[6] = 0x36;
	currentPattern.midiTrackNote[7] = 0x37;
	currentPattern.midiTrackNote[8] = 0x38;
	currentPattern.midiTrackNote[9] = 0x39;
	currentPattern.midiTrackNote[10] = 0x3a;
	currentPattern.midiTrackNote[11] = 0x3b;
	currentPattern.midiTrackNote[12] = 0x3c;
	currentPattern.midiTrackNote[13] = 0x3d;
	currentPattern.midiTrackNote[14] = 0x3e;
	currentPattern.midiTrackNote[15] = 0x3f;
	currentPattern.patternBPM = 120;
	currentPatternNumber = 1;
	currentStep = 0;
	currentPattern.numSteps = 16;
	//screen0
	// = {"Performance Mode    ","Pattern:            ","BPM: xxx            ","Stop                "}
	initArrays(screen0,8,1,"Pattern:");
	initArrays(screen0,4,2,"BPM:");
	initArrays(screen0,4,3,"Stop");
	initArrays(screen0,16,0,"Performance Mode");
	//screen1
	// = {"Sequence Edit       ","Pattern:            ","Steps:              ","Step number:        "}; //this will eventually be 5 once we implement naming of samples.
	initArrays(screen1,8,1,"Pattern:");
	initArrays(screen1,6,2,"Steps:");
	initArrays(screen1,12,3,"Step number:");
	initArrays(screen1,13,0,"Sequence Edit");
	//screen2
	//= {"Track Settings      ","Track:              ","Play Mode           ","OutRoute            "};
	initArrays(screen2,6,1,"Track:");
	initArrays(screen2,9,2,"PlayMode:");
	initArrays(screen2,9,3,"OutRoute:");
	initArrays(screen2,14,0,"Track Settings");
	//screen3
	// = {"Global Settings     ","Midi Channel: xx    ", "Midi trigger Notes  ","(put triggered note)"};
	initArrays(screen3,13,1,"Midi Channel:");
	initArrays(screen3,18,2,"Midi Trigger Notes");
	initArrays(screen3,20,3,"(trigger Notes here)");
	initArrays(screen3,15,0,"Global Settings");	



}

