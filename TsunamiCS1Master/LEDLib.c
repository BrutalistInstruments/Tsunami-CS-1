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

void initLEDs()
{
	DDRG = 0B00100101; //outputs on G5,2, and 0.

}

void updateLEDs()
{
	if(encoderAValue==0){
	uint16_t trigLEDParse = currentTrigButtons;
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

}