/*
 * ButtonLib.c
 *
 * Created: 7/31/2019 4:31:55 PM
 *  Author: Hal
 */ 

#include "globalVariables.h"
#include "EncoderLib.h"
#include "tsunamiLib.h"
#include "OLEDLib.h"
#include <avr/io.h>
#include "DebounceLib.h"

uint8_t buttonsCurrentCycle;
uint16_t lastFullBits = 0;
uint8_t currentTrig;

ISR(TIMER0_OVF_vect)
{
	debounce();
}

void initButtons()
{
	//this will initialize all of the buttons on the front panel
	//main trigger buttons
	PORTA = 0xFF;
	PORTL = 0xFF;

	//GPButtons and Encoder buttons
	PORTB = 0B01111111;
	
}


void listenTrigButtons()
{
	buttonsCurrentCycle = (PINL^255); //^ = bitwise XOR operation.
	currentTrigButtons = (buttonsCurrentCycle << 8) | (PINA^255);
	if(currentTrigButtons!=lastFullBits) //we do read the buttons every cycle, but we don't need to update everything base on the buttons if they haven't changed.
	{
		lastFullBits = currentTrigButtons;
		uint16_t fullBitsParse = currentTrigButtons;
		//play sounds, if that is the switch case on the encoder
	//updateLEDs
	for(uint8_t bc = 0; bc<16; bc++)//bc for buttonCounter
	{
		currentTrig = (fullBitsParse&1);
		if(currentTrig)
		{
			switch (encoderAValue)
			{
				
				//for "perfrmance mode", we should just use the default case, and only have code for the cases where things are outside of that use case.
				case 0: //performance mode
				//we trigger a sound here based on the location of bc
				trackControl(currentPattern.trackSampleLSB[bc], currentPattern.trackSampleMSB[bc], currentPattern.trackOutputRoute[bc], currentPattern.trackOutputRoute[bc]);
				break;
				
				case 1:
				currentPattern.trackSequence[currentStep] ^=currentTrigButtons;
				//turn on step number, or turn off step number.
				//step sequencer mode.
				break;
				
				case 2: ;
				//select track for sample assignment
				uint16_t currentSample = (currentPattern.trackSampleMSB[bc]<<8)|(currentPattern.trackSampleLSB[bc]);
				currentTrack = bc;
				numPrinter(screen2[1], 7, 2, (bc+1));
				numPrinter(screen2[1], 10, 4, currentSample);
				outputS(screen2[1], 1);
				switch (currentPattern.trackPlayMode[bc])
				{
					case 0:
					screen2[2][10] = 'P';
					screen2[2][11] = 'o';
					screen2[2][12] = 'l';
					screen2[2][13] = 'y';
					break;
					
					case 1:
					screen2[2][10] = 'S';
					screen2[2][11] = 'o';
					screen2[2][12] = 'l';
					screen2[2][13] = 'o';
					break;
					
					//these additional cases will be for loops and other stuff. have not decided on how to deal with them yet.
					case 2:
					break;
					
					case 3:
					break;
				}
				outputS(screen2[2], 2);
				numPrinter(screen2[3], 10, 2, (currentPattern.trackOutputRoute[bc]+1));
				outputS(screen2[3], 3);
				
				
				//trackControl(currentPattern.trackSampleLSB[bc], currentPattern.trackSampleMSB[bc], currentPattern.trackOutputRoute[bc], currentPattern.trackOutputRoute[bc]);
				break;
				
				case 3:; 
				//should these be assignable per pattern? maybe should take them out of global settings, or change that
				uint16_t currentMidiNote = currentPattern.midiTrackNote[bc];
				currentTrack = bc;
				numPrinter(screen3[2], 11, 2, currentTrack);
				numPrinter(screen3[2], 14, 2, currentMidiNote);
				outputS(screen3[2], 2);
				break;
				
				default:
				//this should be the same as case 0;
				break;
			}
		
		
		}
		
		fullBitsParse = fullBitsParse>>1;
	}
	}


}

void listenGPButtons() // are the encoder buttons here also?
{

	if(button_down(1 << PB5))
	{ //top encoder button
	encoderAFlag = ~encoderAFlag;
		
	}
	
	if(button_down(1<<PB6))
	{//bottom encoder button
		encoderBFlag = ~encoderBFlag;
		
	}
}
