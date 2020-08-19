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
#include "twiLib.h"

//These are declared here instead of in the functions they are used to save clock cycles. 
//we only initialized them once, and hold in memory, instead of allocating every time one of these functions are called. 
//(at least I think that's how C works)
uint8_t buttonsCurrentCycle;
uint16_t lastFullBits = 0;
uint8_t currentTrig;

//ISR(TIMER2_OVF_vect)
//{
//	debounce();
//}

void initButtons()
{
	//this will initialize all of the buttons on the front panel
	//main trigger buttons
	PORTA = 0xFF;
	PORTL = 0xFF;

	//GPButtons and Encoder buttons
	PORTB = 0b01111111; 

}

void listenTrigButtons(Pattern *buttonCurrentPattern, Globals *currentGlobals)
{
	buttonsCurrentCycle = (PINL^255); //^ = bitwise XOR operation.
	(currentGlobals->currentTrigButtons) = (buttonsCurrentCycle << 8) | (PINA^255);
	if(currentGlobals->currentTrigButtons!=lastFullBits) //we do read the buttons every cycle, but we don't need to update everything base on the buttons if they haven't changed.
	{
		lastFullBits = currentGlobals->currentTrigButtons;
		uint16_t fullBitsParse = currentGlobals->currentTrigButtons;
		//play sounds, if that is the switch case on the encoder
		//updateLEDs
		for(uint8_t bc = 0; bc<16; bc++)//bc for buttonCounter
		{
			currentTrig = (fullBitsParse&1);
			if(currentTrig)
			{
				uint8_t encoderAstate = currentGlobals->menuState >> 4;
				switch (encoderAstate)
				{
	
					//for "performance mode", we should just use the default case, and only have code for the cases where things are outside of that use case.
					case 0: //performance mode
					//we trigger a sound here based on the location of bc
					playTrack(buttonCurrentPattern, currentGlobals,bc);
					break;

					case 1:
					buttonCurrentPattern->trackSequence[currentGlobals->currentStep] ^= currentGlobals->currentTrigButtons;
					//turn on step number, or turn off step number.
					//step sequencer mode.
					break;

					//we want this functionality for both case 2 and case 3
					case 2:
					case 3:;
					//select track for sample assignment
					//uint16_t currentSample = (buttonCurrentPattern->trackSampleMSB[bc]<<8)|(buttonCurrentPattern->trackSampleLSB[bc]);
					currentGlobals->currentTrack = bc;
					currentGlobals->valueChangeFlag |=(1<<triggerChange);
					playTrack(buttonCurrentPattern, currentGlobals,bc);
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

void listenGPButtons(Pattern currentPattern, Globals *currentGlobals) //may need to be a pointer
{

	if(button_down(1 << PB5))
	{ //top encoder button
		if(currentGlobals->menuState>>4==3)
		{
			globalWrite(currentGlobals);
		}else
		{
			eepromSavePattern(currentPattern, currentGlobals->currentPatternNumber);
		}
		
	}

	uint8_t encoderSwitchMask = 0b00001000;
	if(button_down(1<<PB6))
		{//bottom encoder button
			uint8_t encoderBCheck = currentGlobals->menuState&encoderSwitchMask;
			if(encoderBCheck)
			{
				currentGlobals->menuState &=0b11110111;//turn off the encoderBFlag

			}else
			{
				currentGlobals->menuState |=0b00001000; //turn on the encoderBFlag bit
			}
		}
	uint8_t playButtonMask = 0b0000001; //we could probably make a define for both of these masks.
	uint8_t playStateCheck = currentGlobals->playState & playButtonMask;
	if(button_down(1<<PB4))
	{
		if(playStateCheck)
		{
			currentGlobals->playState=0; //playstate is on, turn it off
			currentGlobals->currentGPButtons &=(~0x20); //turn the first bit
			
		}else
		{
			currentGlobals->playState=1;
			currentGlobals->currentGPButtons |= 0x20; //turn on the first bit
		}

	} //not sure which button this is
	uint8_t trackButtonMask = 0b00000001;
	uint8_t trackStateCheck = (currentGlobals->buttonSwitchFlag) & trackButtonMask;
	if(button_down(1<<PB0))
	{
		if(trackStateCheck)
		{
			currentGlobals->buttonSwitchFlag = 0;
			currentGlobals->currentGPButtons &=(~0x02);
		}else
		{
			currentGlobals->buttonSwitchFlag = 1;
			currentGlobals->currentGPButtons |=0x02;
		}
	}
	uint8_t fineButtonMask = 4;
	uint8_t fineStateCheck = (currentGlobals->currentGPButtons) & fineButtonMask;
	if(button_down(1<<PB1))
	{		
		if(fineStateCheck)
		{
			
			currentGlobals->currentGPButtons &=(~0x04);
		}else
		{
			currentGlobals->currentGPButtons |=0x04;
		}
		
	}
}
