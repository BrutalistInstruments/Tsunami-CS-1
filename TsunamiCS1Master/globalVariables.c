/*
 * globalVariables.c
 *
 * Created: 8/16/2019 7:26:29 AM
 *  Author: Hal
 */ 

#include <avr/io.h>
#include "globalVariables.h"

void initBank()
{
	for(uint8_t i = 0; i<16; i++)
	{
		currentPattern.trackSampleLSB[i] = i+1;
	}


}