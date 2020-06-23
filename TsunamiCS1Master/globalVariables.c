/*
 * globalVariables.c
 *
 * Created: 8/16/2019 7:26:29 AM
 *  Author: Hal
 */

#include <avr/io.h>
#include "globalVariables.h"

//takes an array less than 20 and fills it with blank characters
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

void initBank(Pattern *currentInitPattern)
{
	for(uint8_t i = 0; i<16; i++)
  {
    currentInitPattern->trackSampleLSB[i] = i+1;
    currentInitPattern->trackPlayMode[i] = 0x01;
  }
  
  for(uint8_t j = 0; j<64; j++)
  {
	  currentInitPattern->trackSequence[j] = 0; //start with an empty sequence. 
 
  }
  //We need to take these and put them in global. 
  currentInitPattern->patternBPM = 120;
  currentInitPattern->numSteps = 16;
}

void initGlobals(Globals *currentGlobals, uint8_t factoryReset)
{
		currentGlobals->currentTrigButtons=0; //current state of Trig buttons.
		currentGlobals->currentGPButtons=0; //current state of GP buttons
		currentGlobals->currentPatternNumber=0; //current pattern, between 1 and 256
		currentGlobals->currentStep=0; // current step in the sequencer
		currentGlobals->currentTrack=0; //current track being edited
		currentGlobals->menuState=0; //where the menu is currently
		currentGlobals->playState=0; //whether the sequencer is playing, stopped, or paused.
		currentGlobals->factoryReset=0; //we may not need this in this struct, but good to have for now.
		currentGlobals->buttonSwitchFlag=0; // could be rolled into value bits.
		currentGlobals->valueChangeFlag=0; //bit 0 -> changes in encoders, bit 1-> changes in buttons, bit2 -> changes in knobs
		currentGlobals->knobStatus=0; //top 4 bits: knob type, bottom 4 bits: knob location.
		
		if(factoryReset==1)
		{
			currentGlobals->midiChannel=0;
			currentGlobals->midiTrackNote[0] = 0x24;
			currentGlobals->midiTrackNote[1] = 0x25;
			currentGlobals->midiTrackNote[2] = 0x26;
			currentGlobals->midiTrackNote[3] = 0x27;
			currentGlobals->midiTrackNote[4] = 0x28;
			currentGlobals->midiTrackNote[5] = 0x29;
			currentGlobals->midiTrackNote[6] = 0x2a;
			currentGlobals->midiTrackNote[7] = 0x2b;
			currentGlobals->midiTrackNote[8] = 0x2c;
			currentGlobals->midiTrackNote[9] = 0x2d;
			currentGlobals->midiTrackNote[10] = 0x2e;
			currentGlobals->midiTrackNote[11] = 0x2f;
			currentGlobals->midiTrackNote[12] = 0x30;
			currentGlobals->midiTrackNote[13] = 0x31;
			currentGlobals->midiTrackNote[14] = 0x32;
			currentGlobals->midiTrackNote[15] = 0x33;
		}
}