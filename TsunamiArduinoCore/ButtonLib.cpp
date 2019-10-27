/*
 * ButtonLib.c
 *
 * Created: 7/31/2019 4:31:55 PM
 *  Author: Hal
 */

#include "globalVariables.h"
#include "tsunamiLib.h"
#include "OLEDLib.h"
#include <avr/io.h>
#include "DebounceLib.h"
//#include "eepromLib.h" //to implement

uint8_t buttonsCurrentCycle;
uint16_t lastFullBits = 0;
uint8_t currentTrig;
extern uint8_t currentGPButtons;
extern Screen screenBank;
extern Pattern currentPattern;

ISR(TIMER2_OVF_vect)
{
	debounce();
}

//does not need globals
void initButtons()
{
	//this will initialize all of the buttons on the front panel
	//main trigger buttons
	PORTA = 0xFF;
	PORTL = 0xFF;

	//GPButtons and Encoder buttons
	PORTB = 0B01111111;

}

//we might need globals here?
//encoder values
//current pattern
//currentTrig buttons?
//need to check on pointer stuff here.
void listenTrigButtons(uint8_t buttonMenuState, Pattern buttonCurrentPattern, uint16_t *buttonsCurrentTrig, Screen buttonScreen, uint8_t *buttonCurrentTrack)
{
	buttonsCurrentCycle = (PINL^255); //^ = bitwise XOR operation.
	(*buttonsCurrentTrig) = (buttonsCurrentCycle << 8) | (PINA^255);
	if((*buttonsCurrentTrig)!=lastFullBits) //we do read the buttons every cycle, but we don't need to update everything base on the buttons if they haven't changed.
	{
		lastFullBits = (*buttonsCurrentTrig);
		uint16_t fullBitsParse = (*buttonsCurrentTrig);
		//play sounds, if that is the switch case on the encoder
	//updateLEDs
	for(uint8_t bc = 0; bc<16; bc++)//bc for buttonCounter
	{
		currentTrig = (fullBitsParse&1);
		if(currentTrig)
		{
			uint8_t encoderAstate = buttonMenuState >> 4;
			switch (encoderAstate)
			{

				//for "perfrmance mode", we should just use the default case, and only have code for the cases where things are outside of that use case.
				case 0: //performance mode
				//we trigger a sound here based on the location of bc
				trackControl(buttonCurrentPattern.trackSampleLSB[bc], buttonCurrentPattern.trackSampleMSB[bc], buttonCurrentPattern.trackOutputRoute[bc], buttonCurrentPattern.trackOutputRoute[bc]);
				break;

				case 1:
				buttonCurrentPattern.trackSequence[currentStep] ^=currentTrigButtons;
				//turn on step number, or turn off step number.
				//step sequencer mode.
				break;

				case 2: ;
				//select track for sample assignment
				uint16_t currentSample = (buttonCurrentPattern.trackSampleMSB[bc]<<8)|(buttonCurrentPattern.trackSampleLSB[bc]);
				*buttonCurrentTrack = bc;
				//we need to make "spoof" screens to avoid globals here.
				numPrinter(screenBank.screen2[1], 7, 2, (bc+1));
				numPrinter(screenBank.screen2[1], 10, 4, currentSample);
				outputS(screenBank.screen2[1], 1);
				trackControl(buttonCurrentPattern.trackSampleLSB[bc], buttonCurrentPattern.trackSampleMSB[bc], buttonCurrentPattern.trackOutputRoute[bc], buttonCurrentPattern.trackOutputRoute[bc]);



				switch (buttonCurrentPattern.trackPlayMode[bc])
				{
					case 0:
					screenBank.screen2[2][10] = 'P';
					screenBank.screen2[2][11] = 'o';
					screenBank.screen2[2][12] = 'l';
					screenBank.screen2[2][13] = 'y';
					break;

					case 1:
					screenBank.screen2[2][10] = 'S';
					screenBank.screen2[2][11] = 'o';
					screenBank.screen2[2][12] = 'l';
					screenBank.screen2[2][13] = 'o';
					break;

					//these additional cases will be for loops and other stuff. have not decided on how to deal with them yet.
					case 2:
					break;

					case 3:
					break;
				}
				outputS(screenBank.screen2[2], 2);
				numPrinter(screenBank.screen2[3], 10, 2, (buttonCurrentPattern.trackOutputRoute[bc]+1));
				outputS(screenBank.screen2[3], 3);


				//trackControl(currentPattern.trackSampleLSB[bc], currentPattern.trackSampleMSB[bc], currentPattern.trackOutputRoute[bc], currentPattern.trackOutputRoute[bc]);
				break;

				case 3:;
				//should these be assignable per pattern? maybe should take them out of global settings, or change that
				/*
				uint16_t currentMidiNote = currentPattern.midiTrackNote[bc];
				currentTrack = bc;
				numPrinter(screen3[2], 11, 2, currentTrack);
				numPrinter(screen3[2], 14, 2, currentMidiNote);
				outputS(screen3[2], 2);
				*/
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

void listenGPButtons(uint8_t *gpMenuState) //may need to be a pointer
{

	if(button_down(1 << PB5))
	{ //top encoder button
		//I think we'll be using this button for saving,
		//so no need to do any state changes atm
		//writePattern(currentPattern, currentPatternNumber); //to implement


	}

	uint8_t encoderSwitchMask = 0b00001000;
	if(button_down(1<<PB6))
	{//bottom encoder button
	uint8_t encoderBCheck = *gpMenuState&encoderSwitchMask;
	if(encoderBCheck)
	{
		*gpMenuState &=0b11110111;//turn off the encoderBFlag

	}else
	{
		*gpMenuState |=0b00001000; //turn on the encoderBFlag bit
	}
	}
}
