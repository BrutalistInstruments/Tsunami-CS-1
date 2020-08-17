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
	factoryResetCheck(&factoryReset,&currentPattern, &currentGlobals);
	
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
//	if(currentGlobals.timerFlag) //triggers every millisecond / 16000 cycles.
//	{
		 //listenEncoders(&currentPattern, &currentGlobals); //we may not need to check this every millisecond. If we can just do these checks on 
		 //pin changes, it should be fine. and we have the specific pins to check from, so we should be good. 
//	}
	
	listenTrigButtons(&currentPattern, &currentGlobals);
	listenGPButtons(currentPattern, &currentGlobals);
	updateLEDs(currentPattern, currentGlobals);
	listenKnobs(&currentPattern, &currentGlobals);
	listenEncoders(&currentPattern, &currentGlobals);
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
	if(globalTimer%40==0) //every 40 ticks, we want to call De bounce
	{
		debounce();
	}
}