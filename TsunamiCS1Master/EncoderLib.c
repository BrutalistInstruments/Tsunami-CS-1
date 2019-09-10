/*
this library is made with help from this article:
https://www.best-microcontroller-projects.com/rotary-encoder.html 
 */ 
#include <avr/io.h>
#include "globalVariables.h"
#include <avr/interrupt.h>

static uint8_t prevNextCodeA = 0;
static uint8_t prevNextCodeB = 0;
static uint16_t storeA=0;
static uint16_t storeB=0;
static int8_t rotEncoderTable[] = {0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0};
	
void initEncoders()
{
	//this is where we set the encoder pins to where they need to be
PORTH = 0B01111000; // enable internal pullup resistors.

}

void pollEncoder0(uint8_t *encoderC0)
{
	
		prevNextCodeA <<=2;
		if(~PINH&(1<<PH6))
		{
			prevNextCodeA |= 0x02;
		}
		if(~PINH&(1<<PH5))
		{
			prevNextCodeA |= 0x01;
		}
		prevNextCodeA &= 0x0f;
		
		if(rotEncoderTable[prevNextCodeA])
		{
			storeA <<=4;
			storeA |= prevNextCodeA;
			if((storeA&255)==0x2b)
			{
				*encoderC0 = (*encoderC0)-1;
			}
			if((storeA&255)==0x17)
			{
				*encoderC0 = (*encoderC0)+1;
			}			
		}
}

void pollEncoder1(uint8_t *encoderC1)
{
	
	prevNextCodeB <<=2;
	if(~PINH&(1<<PH4))
	{
		prevNextCodeB |= 0x02;
	}
	if(~PINH&(1<<PH3))
	{
		prevNextCodeB |= 0x01;
	}
	prevNextCodeB &= 0x0f;
	
	if(rotEncoderTable[prevNextCodeB])
	{
		storeB <<=4;
		storeB |= prevNextCodeB;
		if((storeB&255)==0x2b)
		{
			*encoderC1 = (*encoderC1)-1;
		}
		if((storeB&255)==0x17)
		{
			*encoderC1 = (*encoderC1)+1;
		}
	}
}


void listenEncoders()
{
	//this is where we update the encoder variables.
	pollEncoder0(&encoderAValue); //testing out this lookup table method
	pollEncoder1(&encoderBValue);


}

