/*
 * globalVariables.c
 *
 * Created: 8/16/2019 7:26:29 AM
 *  Author: Hal
 */

#include <avr/io.h>
#include "globalVariables.h"
#include <string.h>
#include <avr/delay.h>
#include "OLEDLib.h"
#include "EncoderLib.h"
#include "twiLib.h"
#define F_CPU 16000000UL


//takes an array less than 20 and fills it with blank characters
void initArrays(unsigned char myArray[9][21], int stringNumber, char* myString)
{
	uint8_t lengthOfString = strlen(myString);
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
	currentInitPattern->envelopeType[i] = 3;
	currentInitPattern->trackSustainTimeLSB[i] = 0;
	currentInitPattern->trackSustainTimeMSB[i] = 0;
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
		currentGlobals->releaseCounter = 0;
		currentGlobals->lastGlobalTimer = 0;
		currentGlobals->clockCounter = 0;
		currentGlobals->currentTrigSequencer = 0;
		currentGlobals->currentTrigMidi = 0;
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

void initTimer() //we only need to use 1 timer, and Use ISRs for that. 
{
	//we're using timer 2, because it's the highest priority 8 bit timer interupt. 
	
		//here we need to setup our timer interrupt
		TCCR2A = (1 << WGM21); //set to clear on correct compare
		TCCR2B = (1 << CS21) | (1 << CS20); // set pre-scaler to 64
		OCR2A = 50; // every 25 ticks will be 0.0001 seconds at this pre scale.
		TIMSK2 = (1 << OCIE2A); // Enable OCR0A compare interrupt
		//interrupts should now be good to go.
}

void updateTimers(Globals *currentGlobals, uint32_t currentTime)
{
	uint8_t change = 0;
	if(change=currentTime-(currentGlobals->lastGlobalTimer)) //check if there has been a change. 
	{
		currentGlobals->clockCounter = (currentGlobals->clockCounter)+change;
		currentGlobals->releaseCounter = (currentGlobals->releaseCounter)+change;
		currentGlobals->lastGlobalTimer = currentTime;
		//currentGlobals->timerFlag = 1;
	}
	//else
	//{
	//	currentGlobals->timerFlag = 0; //we may want to change this some other point in the code, like when everything reliant on this flag is complete. 
		//just so we're not wasting a conditional every time. 
	//}
	
}
void factoryResetCheck(uint8_t *factoryReset, Pattern *currentPattern, Globals *currentGlobals)
{
	if(((~PINA)&0x01)&&((~PINL)&0x01))
	{//if both buttons are pressed on startup, wait 4 seconds
		outputS("FactoryReset?       ",0);
		_delay_ms(4000);

		if(((~PINA)&0x01)&&((~PINL)&0x01))
		{
			
			uint8_t choice = 2;
			uint8_t select = 0;
			char resetArray[21] = "yes?        no?     ";
			while(choice==2){
				outputS(resetArray,1);
				select = listenEnoderReset();
				if(select==0)
				{
					resetArray[4] = 8;
					resetArray[15] = ' ';
				}
				if(select==1)
				{
					resetArray[4] = ' ';
					resetArray[15] = 8;
				}
				if((~PINB)&(1<<5))
				{
					choice = select; //break out of while loop, and reset, or not.
				}
			}
			
			if(select==0) //yes was selected.
			{
				outputS("Progress:           ",2);
				(*factoryReset)=1;
				initGlobals(currentGlobals, *factoryReset);
				factoryResetEeprom(*currentPattern);
				globalWrite(currentGlobals);
			}
		}
		
	}
	
	
}