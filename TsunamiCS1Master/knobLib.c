/*
 * knobLib.c
 * //inspiration taken from bastl instruments 60 knobs
 * Created: 8/11/2019 8:00:19 AM
 *  Author: Hal
 */ 

//and another comment for git testing
//knob index:
//output 1-8: 0-7
//pitch 1-8:8-15
//envelope time 1-8:16-23
//envelope level 1-8:24-31
//track volume 1-8: 32-39
//gpKnob uL:  40 (up left)
//gpKnob uR: 41 (up right)
//gpKnob dR: 42
//gpKnob dL: 43
#include <avr/io.h>
#include "OLEDLib.h"
#include "tsunamiLib.h"

#include "globalVariables.h"
int checkValue = 0;
char pitchPrint[20] = "Pitch = xxx         ";


uint8_t startADCConversion()
{
	
	ADCSRA |= (1 << ADSC); //this moves the read instruction bit to the ADC Register.
	while (ADCSRA & (1 << ADSC));
	return ADCH; //this is the top 8 bits of the 10 bit ADC Read.
	
}

void initADC()
{
	DDRE = 0B00111000; //init pins E5, 4, and 3 as select pins on the external mux.
	
	ADMUX = (1 << ADLAR);//we're using the AREF pin to reduce analog noise, and only grabbing 8 bits from the ADC
	ADCSRA = (1 <<  ADEN) | (1 <<ADPS2) | (1 << ADPS1) | (1 << ADPS0);
	ADCSRB = (1 << MUX5);
	DIDR0 = 0xff; // we should set this register to all 1s, so there is no digital input triggering.
	DIDR2 = 0xff;
	startADCConversion();
}

void selectKnob(uint8_t select)
{ 
	select = select%44; //accounts for overflows, may be unnecessary
	if(select<40)
	{
		ADCSRB = (1 << MUX5);
		uint8_t muxSelect = select%8; //this should produce a number between 1 and 7.
		uint8_t tempMuxSelect = muxSelect;

	//we need to set the internal multiplxer
		uint8_t internalMuxSelect = select/8;
		
		ADMUX = internalMuxSelect|(1 << ADLAR);   
		startADCConversion();//this should throw away our first read after the mux changover.
		//we might need to worry about the delay, but that's fine.
		
		//we put our select0 and select2 pins in the opposite ports, so we need to do some math to fix that.
		//easy stupid fix:
		switch (tempMuxSelect)
		{
			case 1: muxSelect = 4;
			break;
			
			case 3:	muxSelect = 6;
			break;
			
			case 4: muxSelect = 1;
			break;
			
			case 6: muxSelect = 3;
			break;
		}

	//then the external multiplexer
		PORTE = (muxSelect << 3);
	
	}else
	{ //we only have to change the ADC Register, since these knobs are wired directly into our microcontroller.
		switch (select){
		case 40:
		ADMUX = 5|(1 << ADLAR);
		startADCConversion();
		break;
		
		case 41:
		ADMUX = 6|(1 << ADLAR);
		startADCConversion();
		break;
		
		case 42:
		ADMUX = 7|(1 << ADLAR);
		startADCConversion();
		break;
		
		case 43:
		//remember, this knob is in port A0.
		ADMUX = 0|(1 << ADLAR);
		ADCSRB = (0 << MUX5); 
		startADCConversion();
		break;
		}
	
	}
	
}

void updateKnob(uint8_t select)
{
for (int i = 3; i > 0; i--) {
	knobBuffer[i][select] = knobBuffer[i - 1][select];//shift everyone up one.
}
knobBuffer[0][select] = startADCConversion(); 

}

void interperetKnob(uint8_t select)
{//this function will compare outputs, and write to our struct.
	select = select%44;
	float volumeDivisor = 3.1875;
	checkValue = (knobBuffer[0][select])+(knobBuffer[1][select])+(knobBuffer[2][select])+(knobBuffer[3][select]);
	checkValue = (checkValue >> 2);
	if (select<40)
	{
		uint8_t positionSelect = select%8;
		uint8_t bankSwitch = select/8;
		
		//int16_t currentOutVoulume = ((currentPattern.outputLevelMSB[positionSelect]<<8)|(currentPattern.outputLevelLSB));
		//this should be a regular integer between -70 and +10
		//int16_t negCheckValue = (checkValue / volumeDivisor)-70; //we need negative check values here, so this is what we have to do I guess?
		
		switch (bankSwitch)
		{
			case 0:
			//int16_t currentOutVoulume = ((currentPattern.outputLevelMSB[positionSelect]<<8)|(currentPattern.outputLevelLSB)); 
			//this should be a regular integer between -70 and +10
			//int16_t negCheckValue = (checkValue / volumeDivisor)-70; //we need negative check values here, so this is what we have to do I guess?
			//if(currentOutVoulume!=negCheckValue)
			//{
			//	currentPattern.outputLevelMSB = (negCheckValue>>8);
			//	currentPattern.outputLevelLSB = (negCheckValue) //will this just throw away the top 8 bits?
				//then output to screen.
		//	}
			break;
			
 			case 1:
 			if(currentPattern.outputPitch[positionSelect]!=checkValue)
 			{
 				(currentPattern.outputPitch[positionSelect]) = checkValue;
				 if(encoderAValue == 0)
				 {
					 pitchPrint[5] = (positionSelect+49);
					 numPrinter(pitchPrint,8,3,currentPattern.outputPitch[positionSelect]);
					 outputS(pitchPrint, 3);
				 }
				 outputSampleRate(positionSelect, 0, currentPattern.outputPitch[positionSelect]);
				 
 			}
			
 			break;
// 			
// 			case 2:
// 			if(eTime[positionSelect]!=checkValue)
// 			{
// 				(eTime[positionSelect]) = checkValue;
// 			}
// 			break;
// 			
// 			case 3:
// 			if(eLevel[positionSelect]!=checkValue)
// 			{
// 				(eLevel[positionSelect]) = checkValue;
// 			}
// 			break;
// 			
// 			case 4:
// 			if(trackVolume[positionSelect]!=checkValue)
// 			{
// 				(trackVolume[positionSelect]) = (checkValue/volumeDivisor);
// 			}
// 			break;
		
		}
		
		
	}else
	{
		switch (select)
		{
// 			case 40:
// 			if(gpKnob0!=checkValue)
// 			{
// 				gpKnob0 = checkValue;
// 			}
// 			break;
// 			
// 			case 41:
// 			if(gpKnob1!=checkValue)
// 			{
// 				gpKnob1 = checkValue;
// 			}
// 			break;
// 			
// 			case 42:
// 			if(gpKnob2!=checkValue)
// 			{
// 				gpKnob2 = checkValue;
// 			}
// 			break;
// 			
// 			case 43:
// 			if(gpKnob3!=checkValue)
// 			{
// 				gpKnob3 = checkValue;
// 			}
// 			break;
		}
		
	}


}

void listenKnobs()
{
	for(uint8_t loopCounter = 0; loopCounter<44; loopCounter++)
	{
		selectKnob(loopCounter);
		updateKnob(loopCounter);
		interperetKnob(loopCounter);
	}


}

