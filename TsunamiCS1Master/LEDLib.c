/*
LED Info:
ShiftRegisterPins:
Data: PG5
Clock: PG0
Latch: PG2

bits to write: 21
1 8bit and 2 16 bits shifted in should work alright.

 */

#include <avr/io.h>
#include "globalVariables.h"
#include "OLEDLib.h" //for debugging purposes only

uint16_t holdTrig = 0;
extern uint8_t encoderAValue;
extern uint16_t currentTrigButtons;
extern Pattern currentPattern;
extern uint8_t currentStep;

void initLEDs()
{
	DDRG = 0B00000111; //outputs on G5,2, and 0.

}
void parseLEDs(uint16_t LEDInput, uint8_t gpButtonInput) //this should not be a 16 bit int, or we need an additional 8 bits
{
	uint16_t trigLEDParse = LEDInput;
	uint8_t gpParse = gpButtonInput; //we might not need these variables if they revert after they fall out of scope. These might be 2 whole wasted clock cycles.
	PORTG &= (~(1 << PG2)); //set latch low for data input
	for(uint8_t j=0; j<7; j++)
	{
		PORTG |= (1 << PG0); //turn clock pin high
		if(gpParse&0x80)
		{
			PORTG |= (1 << PG1); //send current 1's place bit to the data pin
		}
		else
		{
			PORTG &= (~(1 << PG1));
		}
		PORTG &= (~(1 << PG0)); //turn clock pin low.
		gpParse = gpParse << 1;
	}
	
	for(uint8_t i = 0; i<17; i++)
	{
		PORTG |= (1 << PG0); //turn clock pin high
		if(trigLEDParse&32768)
		{
			PORTG |= (1 << PG1); //send current 1's place bit to the data pin
		}
		else
		{
			PORTG &= (~(1 << PG1));
		}
		PORTG &= (~(1 << PG0)); //turn clock pin low.
		trigLEDParse = trigLEDParse << 1;
	}
	
	
	PORTG |= (1 << PG2); //latch pin high
}

void updateLEDs(Pattern ledCurrentPattern, Globals currentGlobals)
{
	uint8_t shiftedState = currentGlobals.menuState >> 4; //this will get rid of EncoderB
	switch(shiftedState)
	{
		case 0:
		parseLEDs(currentGlobals.currentTrigButtons, currentGlobals.currentGPButtons);
		break;

		case 1:
		parseLEDs(ledCurrentPattern.trackSequence[currentGlobals.currentStep], currentGlobals.currentGPButtons);
		break;

		case 2:
		parseLEDs(currentGlobals.currentTrigButtons, currentGlobals.currentGPButtons);
		break;

		case 3:
		parseLEDs(currentGlobals.currentTrigButtons, currentGlobals.currentGPButtons);
		break;
	}

}
