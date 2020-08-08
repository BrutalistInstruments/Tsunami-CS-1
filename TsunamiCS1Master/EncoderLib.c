/*

 */ 
#include <avr/io.h>
#include "globalVariables.h"
#include <avr/interrupt.h>
#define F_CPU 16000000UL
#include "OLEDLib.h"
#include "twiLib.h"
#include "knobLib.h"
#include "tsunamiLib.h"

#define topEncoderPinA 0
#define topEncoderPinB 1
#define bottomEncoderPinA 2
#define bottomEncoderPinB 3

#define topEncoderRead (PINE&0b00110000)>>4 //we want to read pins 4 and 5., and shift them down 4 spaces. 
#define bottomEncoderRead (PIND&0b0000110)>>1 //we want to read pins 3 and 2 of Port D, and shift them down one space. 


volatile uint8_t topEncoderValue = 0;  
volatile uint8_t bottomEncoderValue = 0;
uint8_t topEncoderLastValue = 0;
uint8_t bottomEncoderLastValue = 0;
//each of these numbers are going to be 2 bit values. 

volatile uint8_t encoderPortStates = 0;
volatile uint8_t topEncoderPortState = 0;
volatile uint8_t bottomEncoderPortState = 0;
volatile uint8_t bottomEncoderGrayCounter = 0; //once this reaches 4, increment the encoder. 

//if a bit in this number is high when an interrupt is called, 
//then we know we are turning that direction. If not, then set that bit. 

//we want to check 0 to 15 in binary. 
//we have general states for each one. 
//on each interrupt, we can increment the state, or decrement the state. 
//basically, there are 4 valid states for each direction, and 4 error states, that we reset our counter at, and 4 states where we do nothing. 
// incremental states go as 00 01 -> 01 11 -> 11 10 -> 10 00, or 1 -> 7 -> 14 -> 8. 
// decremental states go as 00 10 -> 10 11 -> 11 01 -> 01 00. or 2 -> 11 -> 13 -> 4.
// any time we hit 0, 5,10,15, we do nothing, and wait for the next number in the sequence (throw away the read)
// any time we hit 3,6,9, or 12 we also do nothing, since these are bounces. 

//this new code will effectively be the same for every interrupt, but only focusing on this pins of the encoder that the interrupt was made on. 
//so interrupts 
	
void initEncoders()
{
	
	//Interrupt pins - 2,3,4,5
		
	//setup rising edge detection on Int pins 2 and 3 (would maybe want all pin states if this doesn't work).
	EICRA |=(1<<ISC31)|(1<<ISC30)|(1 << ISC21)|(1 << ISC20); 
	
	//same setup on pins 4 and 5
	EICRB |=(1<<ISC51)|(1<<ISC50)|(1 << ISC41)|(1 << ISC40);
	
	//enable all 4 interrupts through masking
	EIMSK |=(1<<INT2)|(1<<INT3)|(1<<INT4)|(1<<INT5);


}
 
ISR(INT2_vect)
{
	bottomEncoderPortState = ((bottomEncoderPortState<<2) | bottomEncoderRead)&0b00001111;// get the old state of the bottom encoder, and get the new read of the bottom encoder. 
	if(bottomEncoderPortState==8)
	{
		bottomEncoderValue++;
	}else if(bottomEncoderPortState==4)
	{
		bottomEncoderValue--;
	}
	//if bottom encoder gray counter is 4, we increment bottom encoder. 
	//if it's -4, we decrement bottom encoder value. 

//this is kindof crazy, but really, the only transitions we care about for counting are 8, and 4.
//maybe we check for only those, and then increment if true?	
	
	
	/*
	if((1<<topEncoderPinB)&encoderPortStates)//this means Pin 2 is coming after pin 3
	{
		bottomEncoderValue--;
		encoderPortStates&=(1<<bottomEncoderPinB)|(1<<bottomEncoderPinA);//reset our two pins to low. 
	}
	else
	{
		encoderPortStates|=(1<<topEncoderPinA); //we want to set bit 0. 
	}
	*/
}

ISR(INT3_vect)
{
	
	bottomEncoderPortState = ((bottomEncoderPortState<<2) | bottomEncoderRead)&0b00001111;// get the old state of the bottom encoder, and get the new read of the bottom encoder.
	if(bottomEncoderPortState==8)
	{
		bottomEncoderValue++;
	}else if(bottomEncoderPortState==4)
	{
		bottomEncoderValue--;
	}
	
	/*

	if((1<<topEncoderPinA)&encoderPortStates)//this means Pin 3 is coming after pin 2
	{
		bottomEncoderValue++;
		encoderPortStates&=(1<<bottomEncoderPinA)|(1<<bottomEncoderPinB); //reset our two pins to low.
	}
	else
	{
		encoderPortStates|=(1<<topEncoderPinB); //we want to set bit 1.
	}
	*/
}

ISR(INT4_vect)
{
	
	if((1<<bottomEncoderPinB)&encoderPortStates)//this means Pin 4 is coming after pin 5
	{
		topEncoderValue++;
		encoderPortStates&=(1<<topEncoderPinA)|(1<<topEncoderPinB); //reset our two pins to low.
	}
	else
	{
		encoderPortStates|=(1<<bottomEncoderPinA); //we want to set bit 2.
	}
}

ISR(INT5_vect)
{
	
	if((1<<bottomEncoderPinA)&encoderPortStates)//this means Pin 3 is coming after pin 2
	{
		topEncoderValue--;
		encoderPortStates&=(1<<topEncoderPinA)|(1<<topEncoderPinB); //reset our two pins to low.
	}
	else
	{
		encoderPortStates|=(1<<bottomEncoderPinB); //we want to set bit 3.
	}
}

void listenEncodersNew(Pattern *currentPattern, Globals *currentGlobals)
{
	//this will happen every millisecond. 
	
}

void listenEncoders(Pattern *currentPattern, Globals *currentGlobals)
{
	if(topEncoderValue!=topEncoderLastValue)
	{
		currentGlobals->menuState = ((topEncoderValue%4)<<4);
		bottomEncoderValue = 0; 
		bottomEncoderLastValue = 0; 
		currentGlobals->valueChangeFlag |= (1<<encoderChange);
		topEncoderLastValue = topEncoderValue;
	}
	
	if(bottomEncoderValue!=bottomEncoderLastValue)
	{
		 uint8_t menuSub = bottomEncoderValue - bottomEncoderLastValue;
		 currentGlobals->valueChangeFlag |= (1<<encoderChange);
		 			//if this value is negative, we increase the menu.
		 			//if positive, we decrement the menu.
		switch(currentGlobals->menuState)
		{
			case PreformanceModeInit:
			if(menuSub==1)
			{
				currentGlobals->currentPatternNumber = (currentGlobals->currentPatternNumber) + 1;
				if(currentGlobals->currentPatternNumber==0)
				{
					currentGlobals->currentPatternNumber=255;
				}
				
			}else if(menuSub==255) //we don't want to hit this when switching menus after changing from the sequencer. 
			{
				currentGlobals->currentPatternNumber = (currentGlobals->currentPatternNumber) - 1;
				if(currentGlobals->currentPatternNumber==255)
				{
					currentGlobals->currentPatternNumber=0;
				}
			}
			eepromLoadPattern(currentPattern, currentGlobals->currentPatternNumber);
			break;
			
			case SequencerMenuArrow1:			
			if(menuSub==1)
			{
				currentGlobals->menuState = SequencerMenuArrow3;
			}else
			{
				currentGlobals->menuState = SequencerMenuArrow2;
			}
			break;
			
			case SequencerMenuArrow1Select:
			if(menuSub==1)
			{
				currentGlobals->currentPatternNumber = (currentGlobals->currentPatternNumber) + 1;
				if(currentGlobals->currentPatternNumber==0)
				{
					currentGlobals->currentPatternNumber=255;
				}
		
			}else
			{
				currentGlobals->currentPatternNumber = (currentGlobals->currentPatternNumber) - 1;
				if(currentGlobals->currentPatternNumber==255)
				{
					currentGlobals->currentPatternNumber=0;
				}	
			}
			eepromLoadPattern(currentPattern, currentGlobals->currentPatternNumber);
			//this keeps the knob reads that will happen right after the eeprom load to write over the loaded values.
			//Now when you load a pattern, the saved positions of the knobs are unchanged until you turn a knob. 
			break;
			
			case SequencerMenuArrow2:
			if(menuSub==1)
			{
				currentGlobals->menuState = SequencerMenuArrow1;
			}else
			{
				currentGlobals->menuState = SequencerMenuArrow3;
			}
			break;
			
			case SequencerMenuArrow2Select:
			if(menuSub==1)
			{
				currentPattern->numSteps++;
				if(currentPattern->numSteps>64)
				{//we only have 64 spaces to write patterns into
					currentPattern->numSteps=64;
				}
			}else
			{
				currentPattern->numSteps--;
				if(currentPattern->numSteps<1)
				{//we can't have a 0 step pattern. 
					currentPattern->numSteps=1;
				}
			}
			break;
			
			case SequencerMenuArrow3:;
			if(menuSub==1)
			{
				currentGlobals->menuState = SequencerMenuArrow2;
			}else
			{
				currentGlobals->menuState = SequencerMenuArrow1;
			}
			break;
			
			case SequencerMenuArrow3Select:
			//change current step number
			if(menuSub==1)
			{
				currentGlobals->currentStep++;
				if(currentGlobals->currentStep>=currentPattern->numSteps)
				{//our ceiling is the maximum number of steps.
					currentGlobals->currentStep=(currentPattern->numSteps)-1;
				}
			}else
			{
				currentGlobals->currentStep--;
				if(currentGlobals->currentStep>currentPattern->numSteps)
				{//do no write notes to step 0.
					currentGlobals->currentStep=0;
				}				
			}
			break;
			
			case TrackMenuArrow1:;
			if(menuSub==1)
			{
				currentGlobals->menuState = TrackMenuArrow5;
			}else
			{
				currentGlobals->menuState = TrackMenuArrow2;
			}
			break;
			
			case TrackMenuArrow1Select:;
			uint16_t currentSample = ((currentPattern->trackSampleMSB[currentGlobals->currentTrack])<<8)|(currentPattern->trackSampleLSB[currentGlobals->currentTrack]);
			if(menuSub==1)
			{
				currentSample++;
				if(currentSample>4096)
				{//maximum number of indexable samples.
					currentSample=4096;
				}
			}else
			{//no samples in space 0.
				currentSample--;
				if(currentSample<1)
				{
					currentSample=1;
				}
			}
			currentPattern->trackSampleLSB[currentGlobals->currentTrack] = (currentSample&0x00FF);//we want to lob off the top 8 bits, just in case. May be unnecessary.
			currentPattern->trackSampleMSB[currentGlobals->currentTrack] = (currentSample>>8); 
			break;
			
			case TrackMenuArrow2:
			if(menuSub==1)
			{
				currentGlobals->menuState = TrackMenuArrow1;
			}else
			{
				currentGlobals->menuState = TrackMenuArrow3;
			}
			break;
			
			case TrackMenuArrow2Select:
			//change play mode of currently selected track
			//since we only have two play modes currently,
			//we only need to flip the play mode.
			currentPattern->trackPlayMode[currentGlobals->currentTrack] = (!(currentPattern->trackPlayMode[currentGlobals->currentTrack]))&(0b00000001);
			//flip all bits, mask for first bit. 
			break;
			
			case TrackMenuArrow3:
			if(menuSub==1)
			{
				currentGlobals->menuState = TrackMenuArrow2;
			}else
			{
				currentGlobals->menuState = TrackMenuArrow4;
			}
			break;
			
			case TrackMenuArrow3Select:
			
			if(menuSub==1)
			{
				//indexing might be an issue here, don't remember if this is 0 indexed or not. 
				(currentPattern->trackOutputRoute[currentGlobals->currentTrack]) = (currentPattern->trackOutputRoute[currentGlobals->currentTrack])+1 ;
				if(currentPattern->trackOutputRoute[currentGlobals->currentTrack]>7)
				{
					currentPattern->trackOutputRoute[currentGlobals->currentTrack]=7;
				}
			}else
			{
				(currentPattern->trackOutputRoute[currentGlobals->currentTrack]) = (currentPattern->trackOutputRoute[currentGlobals->currentTrack])-1;
				if(currentPattern->trackOutputRoute[currentGlobals->currentTrack]>7)
				{
					currentPattern->trackOutputRoute[currentGlobals->currentTrack]=0;
				}
			}
			break;
			
			case TrackMenuArrow4:
			if(menuSub==1)
			{
				currentGlobals->menuState = TrackMenuArrow3;
			}else
			{
				currentGlobals->menuState = TrackMenuArrow5;
			}
			break;
			
			case TrackMenuArrow4Select:
			if(menuSub==1)
			{
				(currentPattern->envelopeType[currentGlobals->currentTrack])++;
				if((currentPattern->envelopeType[currentGlobals->currentTrack])>3)
				{
					(currentPattern->envelopeType[currentGlobals->currentTrack])=3;
				}
			}else
			{
				(currentPattern->envelopeType[currentGlobals->currentTrack])--;
				if((currentPattern->envelopeType[currentGlobals->currentTrack])>3)
				{
					(currentPattern->envelopeType[currentGlobals->currentTrack])=0;
				}
			}
			if(currentPattern->envelopeType[currentGlobals->currentTrack]==0||currentPattern->envelopeType[currentGlobals->currentTrack]==2) //AR or A
			{
				setTrackVolume(currentPattern->trackSampleLSB[currentGlobals->currentTrack], currentPattern->trackSampleMSB[currentGlobals->currentTrack],255,186);
			}else
			{
				setTrackVolume(currentPattern->trackSampleLSB[currentGlobals->currentTrack], currentPattern->trackSampleMSB[currentGlobals->currentTrack],
				currentPattern->trackMainVolumeLSB[currentGlobals->currentTrack],currentPattern->trackMainVolumeMSB[currentGlobals->currentTrack]);		
			}
			break;
			
			case TrackMenuArrow5:
			if(menuSub==1)
			{
				currentGlobals->menuState = TrackMenuArrow4;
			}else
			{
				currentGlobals->menuState = TrackMenuArrow1;
			}
			break;
			
			case TrackMenuArrow5Select:;
			uint16_t sustainTime = (currentPattern->trackSustainTimeLSB[currentGlobals->currentTrack])|((currentPattern->trackSustainTimeMSB[currentGlobals->currentTrack])<<8);	
			if(menuSub==1)
			{
				if((currentGlobals->currentGPButtons)&(0x04))
				{
					sustainTime++;
				}
				else
				{
					sustainTime = sustainTime+236;
				}
				
				if(sustainTime>60000)
				{
					sustainTime = 60000;
				}
			}else
			{
				if((currentGlobals->currentGPButtons)&(0x04))
				{
					sustainTime--;
				}else
				{	
					sustainTime = sustainTime - 236;
				}
				
				if(sustainTime>60000)
				{
					sustainTime = 0;
				}
			}
			currentPattern->trackSustainTimeMSB[currentGlobals->currentTrack] = (sustainTime>>8);
			currentPattern->trackSustainTimeLSB[currentGlobals->currentTrack] = sustainTime; //upper bits will be truncated.

			break;
			
			case GlobalMenuArrow1:
			if(menuSub==1)
			{
				currentGlobals->menuState = GlobalMenuArrow1;
			}else
			{
				currentGlobals->menuState = GlobalMenuArrow2;
			}
			break;
			
			//this may need to be in the range 0 to 15, and displayed with + 1.
			case GlobalMenuArrow1Select:
			if(menuSub==1)
			{ 
				//increment midi number
				currentGlobals->midiChannel = (currentGlobals->midiChannel)+1;
				if(currentGlobals->midiChannel>15)
				{
					currentGlobals->midiChannel=15;
				}
			}
			else
			{
				//decrement midi number
				currentGlobals->midiChannel = (currentGlobals->midiChannel)-1;
				if(currentGlobals->midiChannel>254)
				{
					currentGlobals->midiChannel=0;
				}
			}
			break;
			
			case GlobalMenuArrow2:
			if(menuSub==1)
			{
				currentGlobals->menuState = GlobalMenuArrow1;
			}else
			{
				currentGlobals->menuState = GlobalMenuArrow2;
			}
			break;
			
			case GlobalMenuArrow2Select:;
			//change midi note for selected track.
			//midi range is 0 to 127.
			uint8_t currentMidiNote = currentGlobals->midiTrackNote[currentGlobals->currentTrack];
			if(menuSub==1)
			{
				//not 100% sure how to set this up.
				currentMidiNote++;
				if(currentMidiNote>127)
				{
					currentMidiNote = 127;
				}
			}else
			{
				currentMidiNote--;
				if(currentMidiNote>128)
				{
					currentMidiNote = 0;
				}
			}
			currentGlobals->midiTrackNote[currentGlobals->currentTrack] = currentMidiNote;
			break;
			
			case GlobalMenuArrow3:
			//currently unreachable, no settings live here atm		
			if(menuSub==1)
			{
				currentGlobals->menuState = GlobalMenuArrow2;
			}else
			{
				currentGlobals->menuState = GlobalMenuArrow1;
			}
			break;
			
			case GlobalMenuArrow3Select:
			//nothing to put here yet.
			break;
		}
		bottomEncoderLastValue = bottomEncoderValue;
		//menuSub=0;
	}
}
uint8_t listenEnoderReset()
{
	uint8_t returnMe = 2;
	if(topEncoderValue!=topEncoderLastValue){
	returnMe = topEncoderValue%2; //should be 0 or 1.
	}
	return returnMe;
}

