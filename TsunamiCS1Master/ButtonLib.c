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

uint8_t buttonsCurrentCycle;
uint16_t lastFullBits = 0;
uint8_t currentTrig;

char buttonTest[20] = "Pressed: xx       ";


void initButtons()
{
	//this will initialize all of the buttons on the front panel
	//main trigger buttons
	PORTA = 0xFF;
	PORTL = 0xFF;

	//GPButtons
	PORTB = 0B00011111;
	
	//what ports are the encoder buttons on?
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
				trackControl(currentPattern.trackSampleLSB[bc], currentPattern.trackSampleMSB[bc], currentPattern.trackOutputRoute[bc], currentPattern.trackPlayMode[bc]);
				//trackControl(3,0,0,1);
				numPrinter(buttonTest,9, 2,bc);
				outputS(buttonTest, 2);
				break;
				
				case 1:
				//turn on step number, or turn off step number.
				//step sequencer mode.
				break;
				
				case 2: 
				//select track for sample assignment
				break;
				
				case 3: 
				//same as 0
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



}