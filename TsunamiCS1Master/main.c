#include <avr/io.h>
#include "OLEDLib.h"
#include "serialLib.h"
#include "globalVariables.h"
#include "knobLib.h"
#include "ButtonLib.h"
#include "DebounceLib.h"
#include "menu.h"
#include "tsunamiLib.h"
#include "LEDLib.h"
#include "encoderLib.h"
#include "sequencerLib.h"
#include "twiLib.h"
#include "midiLib.h"
#include <avr/interrupt.h>
#include <avr/delay.h>
#define F_CPU 16000000UL

//Pattern may not need to be volatile, but I'd like to keep it around. 
volatile Pattern currentPattern;
volatile Globals currentGlobals;
volatile uint32_t globalTimer = 0;

int main(){
	uint8_t factoryReset=0; // set this to 1 if you would like to fill the eeprom with Factory data, and erase all user data.
	Screen screenBank;
	char testArray[21] = "CurrentTime:         ";
	
	initScreen();
	initButtons();
	initEncoders();
	initBank(&currentPattern);
	twi_init();
	sei();
	//factory Reset, we should turn this into a global function. 
	if(((~PINA)&0x01)&&((~PINL)&0x01))
	{//if both buttons are pressed on startup, wait 4 seconds
		outputS("FactoryReset?       ",0);
		_delay_ms(4000);

		if(((~PINA)&0x01)&&((~PINL)&0x01))
		{ 
			
			uint8_t choice = 2;
			uint8_t select = 0;
			char resetArray[21] = "yes?        no?     ";
			while(choice==2){
				outputS(resetArray,1);
				select = listenEnoderReset();
				if(select==0)
				{
					resetArray[4] = 8;
					resetArray[15] = ' ';
				}
				if(select==1)
				{
					resetArray[4] = ' ';
					resetArray[15] = 8;
				}				
				if((~PINB)&(1<<5))
				{
					choice = select; //break out of while loop, and reset, or not. 
				}
			}
			
			if(select==0) //yes was selected. 
			{
				outputS("Progress:           ",2);
				factoryReset=1;
				initGlobals(&currentGlobals, factoryReset);
				factoryResetEeprom(currentPattern);
				globalWrite(&currentGlobals);
			}
		}
		
	}
	
	initTimer();
	initGlobals(&currentGlobals, factoryReset);
	initLEDs();
	initADC();
	serialInit0();
	initMidi();
	//initEnvelopes();
	//initSequencer();
	
	
	eepromLoadPattern(&currentPattern,currentGlobals.currentPatternNumber);
	for(uint16_t i = 0; i<440; i++ ) //we need to load the FilterKnobbuffer into a stable state 
	{
		uint8_t loadSelect = i%44;
		selectKnob(loadSelect);
		updateKnob(loadSelect, &currentGlobals);
	}
	initializeKnob(&currentGlobals); //then copy it to the lastFilteredKnobBuffer. 
	globalLoad(&currentGlobals, factoryReset);
	initMenu(&screenBank, currentPattern, currentGlobals); //fills screenBank with menu strings

	//this ISR is used for Button De-Bouncing. Maybe we could put it somewhere else. 
	//TCCR2B = 1<<CS22;//using 256 from pre-scaler
	//TIMSK2 = 1<<TOIE2; //interupt on counter overflow. since we're interupting on value 256 of with a 256 pre-scaler, we're calling this function every 65,536
	//clock cycles. at 16MHz, that equates to every 0.004096, seconds, or every 4 milliseconds. We ~~~should be able to do the same thing from our global counter.

	cli(); //this may not be needed, but also may be effecting things since we're setting interrupt registers after sei has already happened. 
	sei();
	




while(1) {
	
	
	updateTimers(&currentGlobals, globalTimer); //we update our global timers here. 
	
	listenTrigButtons(&currentPattern, &currentGlobals);
	listenGPButtons(currentPattern, &currentGlobals);
	updateLEDs(currentPattern, currentGlobals);
	listenEncoders(&currentPattern, &currentGlobals);
	listenKnobs(&currentPattern, &currentGlobals);
	updateSequencer(currentPattern, &currentGlobals);
	updateScreen(&screenBank, &currentPattern, &currentGlobals);
	midiRead(currentPattern, currentGlobals);
	releaseUpdate(&currentPattern, &currentGlobals);
	//numPrinter(testArray, 11,5,currentGlobals.releaseCounter);
	//numPrinter(testArray, 11,5,(globalTimer/10)); //the only thing that should be effecting this timer, is the ISRs from the encoders. 
	//outputS(testArray,0);
	}
}

ISR(TIMER2_COMPA_vect)
{
	globalTimer++; //this counts in one order of magnitude smaller than millis : 0.0001 seconds. 
	//we don't want to do anything else here.
	if(globalTimer%40==0) //every 40 ticks, we want to call Debounce
	{
		debounce();
	}
}

//ISR(TIMER3_COMPA_vect)
//{
//	currentGlobals.releaseCounter++; //this will increase every millisecond.
	//should run for about 1000 hours before overflow, so not something we really have to worry about.
//}