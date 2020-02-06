#include <Arduino.h>
#include <globalVariables.h>
#include <tsunamiLib.h>

uint16_t clockCounter = 0; // how many 0.0001 seconds have passed.
//(1 degree of magnitude smaller than millis)
uint8_t currentPlayStep = 0; //this is different than the currentStep, which is for editing.
//if a real-time sequencer is implemented, they will end up being the same thing.

void initSequencer()
{
  //here we need to setup our timer interupt, and set playback to stop.
  TCCR0A = (1 << WGM01); //set to clear on correct comapare
  TCCR0B = (1 << CS01) | (1 << CS00); // set pre-scaler to 64
  OCR0A = 25; // every 25 ticks will be 0.0001 seconds at this prescale.
  TIMSK0 = (1 << OCIE0A); // Enable OCR0A conpare inturupt
  //interupts should now be good to go.
}

ISR(TIMER0_COMPA_vect)
{
  clockCounter++;
  //we don't want to do anything else here.
}



void updateSequencer(Pattern sequencerPattern, uint8_t playState)
{
  uint16_t BPMvar = 150000/(sequencerPattern.patternBPM);
  //we also need a play button variable, but lets just see if it works without it.

  if(clockCounter>=BPMvar && playState) //if playstate is on, play next note in sequence.
  {
    //this will be where we play sampmles
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



  }else if(clockCounter>=BPMvar && !playState)
  { currentPlayStep=0;
    clockCounter = 0;

  }

}
