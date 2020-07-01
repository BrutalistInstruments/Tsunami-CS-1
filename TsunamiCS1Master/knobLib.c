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
#include <avr/io.h>
#include "OLEDLib.h"
#include "tsunamiLib.h"
#include "MidiLib.h"
#include "globalVariables.h"
#include "knobLib.h"


uint8_t NegativeOffset = 71;
float volumeDivisor = 3.135;

//uint8_t checkValue = 0;
//uint8_t knobBuffer[44];
//uint8_t checkBuffer[44];
uint8_t knobBufferCounter;



uint8_t startADCConversion()
{
	ADCSRA |= (1 << ADSC); //this moves the read instruction bit to the ADC Register.
	while (ADCSRA & (1 << ADSC));
	return ADCH; //this is the top 8 bits of the 10 bit ADC Read.	
}

void initADC()
{
	DDRF |= 0B00000111; //init pins F2, 1, and 0 as select pins on the external mux.
	
	ADMUX = (1 << ADLAR);//we're using the AREF pin to reduce analog noise, and only grabbing 8 bits from the ADC
	ADCSRA = (1 <<  ADEN) | (1 <<ADPS2) | (1 << ADPS1) | (1 << ADPS0);
	ADCSRB = (1 << MUX5);
	DIDR0 = 0xff; // we should set this register to all 1s, so there is no digital input triggering.
	DIDR2 = 0xff;
	knobBufferCounter = 0;
	startADCConversion();
}

void selectKnob(uint8_t select)
{ 
	select = select%44; //accounts for overflows, may be unnecessary
	if(select<40)
	{
		ADCSRB = (1 << MUX5);
		uint8_t muxSelect = select%8; //this should produce a number between 1 and 7.
		//uint8_t tempMuxSelect = muxSelect;

	//we need to set the internal multiplxer
		uint8_t internalMuxSelect = select/8;
		
		ADMUX = internalMuxSelect|(1 << ADLAR);   
		startADCConversion();//this should throw away our first read after the mux changover.
		

		//then the external multiplexer
		PORTF = muxSelect;
	
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

void updateKnob(uint8_t select, Globals *currentGlobals)
{
	//IIR filter.
	currentGlobals->rawKnobBuffer[select] = startADCConversion(); //raw reads
		currentGlobals->filteredKnobBuffer[select] = 
		currentGlobals->filteredKnobBuffer[select] + ((currentGlobals->rawKnobBuffer[select]-currentGlobals->filteredKnobBuffer[select])/2); //reads with math done to them
}
void initializeKnob(Globals *currentGlobals)
{
	//we do this after we fill the knob buffer at startup. 
	for(int i = 0; i<44; i++){
	currentGlobals->lastFilteredKnobBuffer[i] = currentGlobals->filteredKnobBuffer[i];
	}
}

void interperetKnob(uint8_t select, Pattern *currentKnobPattern, Globals *currentGlobals)
{//this function will compare outputs, and write to our struct.
	select = select%44;
	
	if (select<40)
	{
		uint8_t positionSelect = select%8;
		uint8_t positionSelectTracks = select%8; //this seems redundant looking at it. Maybe there is a clever way we can avoid this?
		uint8_t bankSwitch = select/8;
		if(((currentGlobals->buttonSwitchFlag)&0x01)==1) //we only want to check bit 1 of the GP buttons. We might want to check other values later. 
		{
			positionSelectTracks=positionSelectTracks+8;
		}
		
		switch (bankSwitch){

			uint8_t prevRead = 0;
			uint8_t newRead = 0;
			case 0:; //outputVolume
			
			//int16_t currentOutVoulume = ((currentKnobPattern->outputLevelMSB[positionSelect]<<8)|(currentKnobPattern->outputLevelLSB[positionSelect])); 
			//this should be a regular integer between -70 and +10
			prevRead = currentGlobals->lastFilteredKnobBuffer[select];
			newRead = currentGlobals->filteredKnobBuffer[select];

			if(checkVariation(newRead,prevRead)>2)
			{
				
				int16_t negCheckValue = (currentGlobals->filteredKnobBuffer[select] / volumeDivisor)-NegativeOffset;
				currentGlobals->valueChangeFlag |= (1<<knobChange); //if knob change bit is already set, this should be fine.
				currentGlobals->knobStatus = (bankSwitch<<4)|positionSelect; //we don't want to | this, we just want to set it equal, so the screen only updates the last value 
				
				currentKnobPattern->outputLevelLSB[positionSelect] = (negCheckValue);
				if(negCheckValue>(-1))
				{
					currentKnobPattern->outputLevelMSB[positionSelect] = 0;	
				}else
				{
					currentKnobPattern->outputLevelMSB[positionSelect] = 255;
				}
				setOutputVolume(currentKnobPattern->outputLevelLSB[positionSelect], currentKnobPattern->outputLevelMSB[positionSelect], positionSelect);
				currentGlobals->lastFilteredKnobBuffer[select] = currentGlobals->filteredKnobBuffer[select];
			}
			break;
			
 			case 1: //pitch
 			if(currentGlobals->lastFilteredKnobBuffer[select]!=(currentGlobals->filteredKnobBuffer[select]))
 			{
				currentGlobals->valueChangeFlag |= (1<<knobChange); //if knob change bit is already set, this should be fine.
				currentGlobals->knobStatus = (bankSwitch<<4)|positionSelect; //we don't want to | this, we just want to set it equal, so the screen only updates the last value
 				currentKnobPattern->outputPitch[positionSelect] = (currentGlobals->filteredKnobBuffer[select]^128);
				 currentGlobals->lastFilteredKnobBuffer[select] = currentGlobals->filteredKnobBuffer[select];
				outputSampleRate(positionSelect, 0, currentKnobPattern->outputPitch[positionSelect]);
 			}
 			break;
 			
 			case 2:; //attackEnvelope
			if(currentGlobals->lastFilteredKnobBuffer[select]!=currentGlobals->filteredKnobBuffer[select])
			{
				uint16_t totalAttackTime = currentKnobPattern->trackAttackTimeLSB[positionSelectTracks]|((currentKnobPattern->trackAttackTimeMSB[positionSelectTracks])<<8);
				currentGlobals->valueChangeFlag |= (1<<knobChange); //if knob change bit is already set, this should be fine.
				currentGlobals->knobStatus = (bankSwitch<<4)|positionSelect; //we don't want to | this, we just want to set it equal, so the screen only updates the last value	
				
				if(currentGlobals->currentGPButtons&0x04) 
				{ //if "fine" is on:
						totalAttackTime = totalAttackTime+((currentGlobals->filteredKnobBuffer[select])-(currentGlobals->lastFilteredKnobBuffer[select])); 
				}else
				{
					totalAttackTime = ((currentGlobals->filteredKnobBuffer[select])-1)*238;
				}
				if(totalAttackTime<20)
				{
					totalAttackTime = 20; 
				}
				currentKnobPattern->trackAttackTimeMSB[positionSelectTracks] = ((totalAttackTime)>>8);
				currentKnobPattern->trackAttackTimeLSB[positionSelectTracks] = (totalAttackTime); //this should truncate the top 8 bits. 

				currentGlobals->lastFilteredKnobBuffer[select] = currentGlobals->filteredKnobBuffer[select];
			}
			break;
 			
 			case 3: //release Envelope
 			if(currentGlobals->lastFilteredKnobBuffer[select]!=currentGlobals->filteredKnobBuffer[select])
 			{
				 uint16_t totalReleaseTime = currentKnobPattern->trackReleaseTimeLSB[positionSelectTracks]|((currentKnobPattern->trackReleaseTimeMSB[positionSelectTracks])<<8);
				currentGlobals->valueChangeFlag |= (1<<knobChange); //if knob change bit is already set, this should be fine.
				currentGlobals->knobStatus = (bankSwitch<<4)|positionSelect; //we don't want to | this, we just want to set it equal, so the screen only updates the last value
				if(currentGlobals->currentGPButtons&0x04)
				{ //if "fine" is on:
					totalReleaseTime = totalReleaseTime+((currentGlobals->filteredKnobBuffer[select])-(currentGlobals->lastFilteredKnobBuffer[select]));
				}else
				{
					totalReleaseTime = ((currentGlobals->filteredKnobBuffer[select])-1)*238;
				}
				if(totalReleaseTime<20)
				{
					totalReleaseTime = 20;
				}
				currentKnobPattern->trackReleaseTimeMSB[positionSelectTracks] = ((totalReleaseTime)>>8);
				currentKnobPattern->trackReleaseTimeLSB[positionSelectTracks] = (totalReleaseTime);	
				
				currentGlobals->lastFilteredKnobBuffer[select] = currentGlobals->filteredKnobBuffer[select];
 			}
 			break;
 			
 			case 4:;
 			prevRead = currentGlobals->lastFilteredKnobBuffer[select];
			newRead = currentGlobals->filteredKnobBuffer[select];
 			if(checkVariation(newRead,prevRead)>2)
 			{
				int16_t negCheckValueTrack = (currentGlobals->filteredKnobBuffer[select] / volumeDivisor)-NegativeOffset;
				currentGlobals->valueChangeFlag |= (1<<knobChange); //if knob change bit is already set, this should be fine.
				currentGlobals->knobStatus = (bankSwitch<<4)|positionSelect; //we don't want to | this, we just want to set it equal, so the screen only updates the last value
				
	 			currentKnobPattern->trackMainVolumeLSB[positionSelectTracks] = (negCheckValueTrack);
	 			if(negCheckValueTrack>(-1))
	 			{
		 			currentKnobPattern->trackMainVolumeMSB[positionSelectTracks] = 0;
	 			}else
	 			{
		 			currentKnobPattern->trackMainVolumeMSB[positionSelectTracks] = 255;
	 			}

				
				if(currentKnobPattern->envelopeType[positionSelectTracks]==1||currentKnobPattern->envelopeType[positionSelectTracks]==3) //set track volume directly if Envelope mode is only release, or none. 
				{
	 				setTrackVolume(currentKnobPattern->trackSampleLSB[positionSelectTracks], currentKnobPattern->trackSampleMSB[positionSelectTracks],
					currentKnobPattern->trackMainVolumeLSB[positionSelectTracks], currentKnobPattern->trackMainVolumeMSB[positionSelectTracks]);
				}
				 currentGlobals->lastFilteredKnobBuffer[select] = currentGlobals->filteredKnobBuffer[select]; 
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
			 
			 //we need to do a bit more filtering here. Not sure if that's happening here, or in the actual knob read.
			 if(currentGlobals->lastFilteredKnobBuffer[select]!=(currentGlobals->filteredKnobBuffer[select]))
			{//not sure if this works here, but we're going to try it. 
				
				currentGlobals->valueChangeFlag |= (1<<knobChange); //if knob change bit is already set, this should be fine.
				currentGlobals->knobStatus = (5<<4); //since all other pot banks are 0-4, the next ones will be 5-8. We should maybe figure out a better system for this, 
				//maybe some defines?				
 				currentKnobPattern->patternBPM = currentGlobals->filteredKnobBuffer[select];
				currentGlobals->lastFilteredKnobBuffer[select] = currentGlobals->filteredKnobBuffer[select];
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

void listenKnobs(Pattern *currentKnobPattern, Globals *currentGlobals)
{
	for(uint8_t loopCounter = 0; loopCounter<44; loopCounter++)
	{
		selectKnob(loopCounter);
		updateKnob(loopCounter, currentGlobals);
		interperetKnob(loopCounter,currentKnobPattern, currentGlobals);
	}
}

uint8_t checkVariation(uint8_t v1, uint8_t v2) //this is used to check the difference between 2 knob reads, and give how far appart they are.
{
	uint8_t returnMe=0;
	if(v1>v2)
	{
		returnMe = v1-v2;
	}
	else
	{
		returnMe = v2-v1;
	}
	return returnMe;
}