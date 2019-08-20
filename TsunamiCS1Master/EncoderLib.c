/*
 * EncoderLib.c
 *
 * Created: 7/31/2019 4:57:56 PM
 *  Author: Hal
 */ 
#include <avr/io.h>
#include "globalVariables.h"

void initEncoders()
{
	//this is where we set the encoder pins to where they need to be
PORTH = 0B01111000; // enable internal pullup resistors.

}

void pollEncoder0(uint8_t *encoderC0)
{
	if(~PINH&(1<<PH6))
	{
		if(~PINH&(1<<PH5))
		{
			//left turn?
			*encoderC0= (*encoderC0)+1;
		}
		else
		{
			*encoderC0= (*encoderC0)-1;
		}
		//_delay_ms(2); // seems un-necesary with while loop
		while(~PINH&(1<<PH6)){} // this does make things smoother, but we need to be sure we have interupts for important things.
	}


}

void pollEncoder1(uint8_t *encoderC1)
{
	if(~PINH&(1<<PH3))
	{
		if(~PINH&(1<<PH4))
		{
			//left turn?
			*encoderC1=(*encoderC1)-1;
		}
		else
		{
			*encoderC1=(*encoderC1)+1;
		}
		//_delay_ms(2); // seems un-necesary with while loop
		while(~PINH&(1<<PH3)){} // this does make things smoother, but we need to be sure we have interupts for important things.
	}
}


void listenEncoders()
{
	//this is where we update the encoder variables.
	pollEncoder0(&encoderAValue);
	pollEncoder1(&encoderBValue);


}