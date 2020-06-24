/*
 * sequencerLib.c
 *
 * Created: 3/30/2020 9:18:03 PM
 *  Author: OurBl
 */ 
#include "globalVariables.h"
#include "tsunamiLib.h"
#include <avr/io.h>
#include <avr/interrupt.h>

//should both of these be globals?
uint16_t clockCounter = 0; // how many 0.0001 seconds have passed.
//(1 degree of magnitude smaller than millis)
uint8_t currentPlayStep = 0; //this is different than the currentStep, which is for editing.
//if a real-time sequencer is implemented, they will end up being the same thing.

void initSequencer()
{
	//here we need to setup our timer interrupt
	TCCR0A = (1 << WGM01); //set to clear on correct compare
	TCCR0B = (1 << CS01) | (1 << CS00); // set pre-scaler to 64
	OCR0A = 25; // every 25 ticks will be 0.0001 seconds at this prescale.
	TIMSK0 = (1 << OCIE0A); // Enable OCR0A compare interrupt
	//interrupts should now be good to go.
}

ISR(TIMER0_COMPA_vect)
{
	clockCounter++;
	//we don't want to do anything else here.
}

void updateSequencer(Pattern sequencerPattern, Globals currentGlobals)
{
	uint16_t BPMvar = 150000/(sequencerPattern.patternBPM);

	if(clockCounter>=BPMvar && currentGlobals.playState) //if playstate is on, play next note in sequence.
	{
		//this will be where we play samples
		clockCounter=0; //clear before the loop, so we can continue counting while the sequencer plays notes.
		uint16_t parseStep = sequencerPattern.trackSequence[currentPlayStep];
		for (uint8_t sc=0; sc<16; sc++) //sequencer counter
		{//we're going to loop through all of the possible tracks, and trigger them
			if((parseStep&1)==1)
			{
				trackControl(sequencerPattern.trackSampleLSB[sc], sequencerPattern.trackSampleMSB[sc], sequencerPattern.trackOutputRoute[sc], sequencerPattern.trackPlayMode[sc]);
			}
			parseStep = parseStep>>1; //shift bits down one to check the next slot in the sequence.
		}
		currentPlayStep = currentPlayStep+1;
		if(currentPlayStep>(sequencerPattern.numSteps-1))
		{
			currentPlayStep=0; // don't play more steps than are in the sequence.
		}

	}else if(clockCounter>=BPMvar && !currentGlobals.playState)
	{ currentPlayStep=0;
		clockCounter = 0;
	}

}
