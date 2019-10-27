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

uint16_t holdTrig = 0;
extern uint8_t encoderAValue;
extern uint16_t currentTrigButtons;
extern Pattern currentPattern;

void initLEDs()
{
	DDRG = 0B00100101; //outputs on G5,2, and 0.

}
void parseLEDs(uint16_t LEDInput)
{
	uint16_t trigLEDParse = LEDInput;
	PORTG &= (~(1 << PG2)); //set latch low for data input
	for(uint8_t i = 0; i<17; i++)
	{
		PORTG |= (1 << PG0); //turn clock pin high
		if(trigLEDParse&32768)
		{
			PORTG |= (1 << PG5); //send current 1's place bit to the data pin
		}
		else
		{
			PORTG &= (~(1 << PG5));
		}
		PORTG &= (~(1 << PG0)); //turn clock pin low.
		trigLEDParse = trigLEDParse << 1;
	}
	PORTG |= (1 << PG2); //latch pin high
}

void updateLEDs(uint8_t ledMenuState, Pattern ledCurrentPattern, uint16_t ledCurrentTrigButtons)
{
	uint8_t shiftedState = ledMenuState >> 4; //this will get rid of EncoderB
	switch(shiftedState)
	{
		case 0:
		parseLEDs(ledCurrentTrigButtons);
		break;

		case 1:
		parseLEDs(ledCurrentPattern.trackSequence[currentStep]);
		break;

		case 2:
		parseLEDs(ledCurrentTrigButtons);
		break;

		case 3:
		parseLEDs(ledCurrentTrigButtons);
		break;
	}

}
