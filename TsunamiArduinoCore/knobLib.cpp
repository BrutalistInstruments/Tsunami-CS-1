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
//envelope level 1-8:16-23
//envelope time 1-8:24-31
//track volume 1-8: 32-39
//gpKnob uL:  40 (up left)
//gpKnob uR: 41 (up right)
//gpKnob dR: 42
//gpKnob dL: 43
#include <Arduino.h>
#include <avr/io.h>
#include "OLEDLib.h"
#include "tsunamiLib.h"
#include "globalVariables.h"

int checkValue = 0;
unsigned char pitchPrint[21] = "Pitch = xxx         ";
unsigned char etimePrint[21] = "Time = xxx          ";
unsigned char elevelPrint[21] = "Level = xxx         ";
unsigned char outVolumePrint[21] = "OutVolume x = xxxdb ";
unsigned char trackVolumePrint[21] = "TrackVolumex = xxxdb";
unsigned char envelopeLevelPrint[21] = "EnvelopeGainxx:xxxdb";
unsigned char envelopeTimePrint[21] = "EnvelopeTimex:xxxxMS";
uint8_t knobBuffer[44];
uint8_t checkBuffer[44];
uint8_t knobBufferCounter;
extern Screen screenBank;
extern Pattern currentPattern;

//needs no globals
uint8_t startADCConversion()
{

	ADCSRA |= (1 << ADSC); //this moves the read instruction bit to the ADC Register.
	while (ADCSRA & (1 << ADSC));
	return ADCH;

}

//needs no globals
void initADC()
{
	DDRE = 0B00111000; //init pins E5, 4, and 3 as select pins on the external mux.

	ADMUX = (1 << ADLAR);//we're using the AREF pin to reduce analog noise, and only grabbing 8 bits from the ADC
	ADCSRA = (1 <<  ADEN) | (1 <<ADPS2) | (1 << ADPS1) | (1 << ADPS0);
	ADCSRB = (1 << MUX5);
	DIDR0 = 0xff; // we should set this register to all 1s, so there is no digital input triggering.
	DIDR2 = 0xff;
	knobBufferCounter = 0;
	startADCConversion();
}
//needs no globals
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

//needs no globals
void updateKnob(uint8_t select)
{
	knobBuffer[select] = startADCConversion();
}

//globals needed here:
//-current pattern
//
void interperetKnob(uint8_t select, Pattern currentKnobPattern, uint8_t knobMenuState, Screen knobScreen)
{//this function will compare outputs, and write to our struct.
//I don't think we need this operation.
	select = select%44;
	float volumeDivisor = 3.1875; //This should be a define

	//Change from FIR filter to IIR filter.
     checkBuffer[select] += ((knobBuffer[select]-checkBuffer[select])/2);


	if (select<40)
	{
		uint8_t positionSelect = select%8;
		uint8_t bankSwitch = select/8;
		switch (bankSwitch)
		{

			case 0:
			{
			int16_t currentOutVoulume = ((currentPattern.outputLevelMSB[positionSelect]<<8)|(currentPattern.outputLevelLSB[positionSelect]));
			//this should be a regular integer between -70 and +10
			int16_t negCheckValue = (checkBuffer[select] / volumeDivisor)-70; //we need negative check values here, so this is what we have to do I guess?
			if(currentOutVoulume!=negCheckValue)
			{
				currentPattern.outputLevelLSB[positionSelect] = (negCheckValue);
				if(negCheckValue>(-1))
				{
					currentPattern.outputLevelMSB[positionSelect] = 0;
					//just hard coding this for now until we make a function.
					outVolumePrint[14] = 48;
					outVolumePrint[16] = (currentPattern.outputLevelLSB[positionSelect]%10)+48;
					outVolumePrint[15] = ((currentPattern.outputLevelLSB[positionSelect]%100)/10)+48;
				}else
				{
					currentPattern.outputLevelMSB[positionSelect] = 255;
					outVolumePrint[14] = '-';
					outVolumePrint[15] = ((((currentPattern.outputLevelLSB[positionSelect]^255)+1)%100)/10)+48; //negative 8 bit numbers: flip every bit and add 1.
					outVolumePrint[16] = (((currentPattern.outputLevelLSB[positionSelect]^255)+1)%10)+48;
				}
				//then output to screen.
				outVolumePrint[10] = positionSelect + 49;
				outputS(outVolumePrint, 3);
				setOutputVolume(currentPattern.outputLevelLSB[positionSelect], currentPattern.outputLevelMSB[positionSelect], positionSelect);
			}
		}
			break;

 			case 1:
			{
 			if(currentPattern.outputPitch[positionSelect]!=(checkBuffer[select]^128))
 			{
				//why are we even checking the menu here? I think this is unnecessary
 				(currentPattern.outputPitch[positionSelect]) = (checkBuffer[select]^128);
				//uint8_t encoderAValue = knobMenuState>>4;
				 //if(encoderAValue == 0)
				// {
					 pitchPrint[5] = (positionSelect+49);
					 if(currentPattern.outputPitch[positionSelect]>>7)
					 {
						 //again, hard coding.
						 pitchPrint[7] = '-';
						 numPrinter(pitchPrint, 8, 3, (currentPattern.outputPitch[positionSelect]^255));

					 }else{
						 pitchPrint[7] = '+';
					 numPrinter(pitchPrint,8,3,currentPattern.outputPitch[positionSelect]);}
					 outputS(pitchPrint, 3);
				 //}
				 outputSampleRate(positionSelect, 0, currentPattern.outputPitch[positionSelect]);

 			}
}
 			break;

 			case 2:
			{
			int16_t currentEnvelopeVolume = ((currentPattern.trackFadeGainMSB[positionSelect]<<8)|(currentPattern.trackFadeGainLSB[positionSelect]));
			int16_t negCheckValueEnvelope = (checkBuffer[select] / volumeDivisor)-70; //we need negative check values here, so this is what we have to do I guess?
			if(currentEnvelopeVolume!=negCheckValueEnvelope)
			{
				currentPattern.trackFadeGainLSB[positionSelect] = (negCheckValueEnvelope);
				if(negCheckValueEnvelope>(-1))
				{
					currentPattern.trackFadeGainMSB[positionSelect] = 0;
					//just hard coding this for now until we make a function.
					envelopeLevelPrint[15] = 48;
					envelopeLevelPrint[17] = (currentPattern.trackFadeGainLSB[positionSelect]%10)+48;
					envelopeLevelPrint[16] = ((currentPattern.trackFadeGainLSB[positionSelect]%100)/10)+48;
				}else
				{
					currentPattern.trackFadeGainMSB[positionSelect] = 255;
					envelopeLevelPrint[15] = '-';
					envelopeLevelPrint[16] = ((((currentPattern.trackFadeGainLSB[positionSelect]^255)+1)%100)/10)+48; //negative 8 bit numbers: flip every bit and add 1.
					envelopeLevelPrint[17] = (((currentPattern.trackFadeGainLSB[positionSelect]^255)+1)%10)+48;
				}
				//then output to screen.
				envelopeLevelPrint[13] = positionSelect + 49;
				outputS(envelopeLevelPrint, 3);
				//nothing to "set", since envelopes are triggered after a sound is playing.
			}
		}
			break;

 			case 3:
			{
 			if(currentPattern.trackFadeTimeMSB[positionSelect]!=checkBuffer[select])
 			{
 				(currentPattern.trackFadeTimeMSB[positionSelect]) = checkBuffer[select];
				 numPrinter(envelopeTimePrint,14,4,currentPattern.trackFadeTimeMSB[positionSelect]);
				 envelopeTimePrint[12] = positionSelect+49;
				 outputS(envelopeTimePrint, 3);
 			}
		}
 			break;

 			case 4:
			{
 			int16_t currentTrackVolume = ((currentPattern.trackMainVolumeMSB[positionSelect]<<8)|(currentPattern.trackMainVolumeLSB[positionSelect]));
 			int16_t negCheckValueTrack = (checkBuffer[select] / volumeDivisor)-70; //we need negative check values here, so this is what we have to do I guess?
 			if(currentTrackVolume!=negCheckValueTrack)
 			{
	 			currentPattern.trackMainVolumeLSB[positionSelect] = (negCheckValueTrack);
	 			if(negCheckValueTrack>(-1))
	 			{
		 			currentPattern.trackMainVolumeMSB[positionSelect] = 0;
		 			//just hard coding this for now until we make a function.
		 			trackVolumePrint[15] = 48;
		 			trackVolumePrint[17] = (currentPattern.trackMainVolumeLSB[positionSelect]%10)+48;
		 			trackVolumePrint[16] = ((currentPattern.trackMainVolumeLSB[positionSelect]%100)/10)+48;
	 			}else
	 			{
		 			currentPattern.trackMainVolumeMSB[positionSelect] = 255;
		 			trackVolumePrint[15] = '-';
		 			trackVolumePrint[16] = ((((currentPattern.trackMainVolumeLSB[positionSelect]^255)+1)%100)/10)+48; //negative 8 bit numbers: flip every bit and add 1.
		 			trackVolumePrint[17] = (((currentPattern.trackMainVolumeLSB[positionSelect]^255)+1)%10)+48;
	 			}
	 			//then output to screen.
	 			trackVolumePrint[11] = positionSelect + 49;
	 			outputS(trackVolumePrint, 3);
	 			setTrackVolume(currentPattern.trackSampleLSB[positionSelect], currentPattern.trackSampleMSB[positionSelect],
				 currentPattern.trackMainVolumeLSB[positionSelect], currentPattern.trackMainVolumeMSB[positionSelect]);
 			}
		}
 			break;

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
 			case 42:
 			if(currentPattern.patternBPM!=checkBuffer[select]+30)
			{
 				currentPattern.patternBPM = checkBuffer[select]+30;
				uint8_t encoderAValue = knobMenuState>>4;
				if(encoderAValue==0){
				 numPrinter(screenBank.screen0[2], 5, 3, currentPattern.patternBPM);
				 outputS(screenBank.screen0[2], 2);
				}
 			}
 			break;
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

//does not need global variables
void listenKnobs(Pattern currentKnobPattern, uint8_t knobMenuState, Screen knobScreen)
{
	for(uint8_t loopCounter = 0; loopCounter<44; loopCounter++)
	{
		selectKnob(loopCounter);
		updateKnob(loopCounter);
		interperetKnob(loopCounter, currentKnobPattern, knobMenuState, knobScreen);
	}


}
