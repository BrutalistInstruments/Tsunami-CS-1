/*
 * globalVariables.c
 *
 * Created: 8/16/2019 7:26:29 AM
 *  Author: Hal
 */

#include <avr/io.h>
#include "globalVariables.h"

//extern Pattern currentPattern;

void initArrays(unsigned char myArray[4][21], uint8_t lengthOfString, int stringNumber, char* myString)
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
//why are we not using the currentInitPattern that's being passed in?
void initBank(Pattern *currentInitPattern)
{
	for(uint8_t i = 0; i<16; i++)
  {
    (*currentInitPattern).trackSampleLSB[i] = i+1;
    (*currentInitPattern).trackPlayMode[i] = 0x01;
  }
  (*currentInitPattern).midiTrackNote[0] = 0x30;
  (*currentInitPattern).midiTrackNote[1] = 0x31;
  (*currentInitPattern).midiTrackNote[2] = 0x32;
  (*currentInitPattern).midiTrackNote[3] = 0x33;
  (*currentInitPattern).midiTrackNote[4] = 0x34;
  (*currentInitPattern).midiTrackNote[5] = 0x35;
  (*currentInitPattern).midiTrackNote[6] = 0x36;
  (*currentInitPattern).midiTrackNote[7] = 0x37;
  (*currentInitPattern).midiTrackNote[8] = 0x38;
  (*currentInitPattern).midiTrackNote[9] = 0x39;
  (*currentInitPattern).midiTrackNote[10] = 0x3a;
  (*currentInitPattern).midiTrackNote[11] = 0x3b;
  (*currentInitPattern).midiTrackNote[12] = 0x3c;
  (*currentInitPattern).midiTrackNote[13] = 0x3d;
  (*currentInitPattern).midiTrackNote[14] = 0x3e;
  (*currentInitPattern).midiTrackNote[15] = 0x3f;
  (*currentInitPattern).patternBPM = 120;
  (*currentInitPattern).numSteps = 16;
}
