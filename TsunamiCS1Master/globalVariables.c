/*
 * globalVariables.c
 *
 * Created: 8/16/2019 7:26:29 AM
 *  Author: Hal
 */ 

#include <avr/io.h>
#include "globalVariables.h"

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

}