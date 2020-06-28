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


int main(){
	uint8_t factoryReset=0; // set this to 1 if you would like to fill the eeprom with Factory data, and erase all user data.
	Screen screenBank;
	
	initScreen();
	initButtons();
	initEncoders();
	//factory Reset;
	if(((~PINA)&0x01)&&((~PINL)&0x01))
	{//if both buttons are pressed on startup, wait 4 seconds
		outputS("FactoryReset?       ",0);
		_delay_ms(4000);
		if(((~PINA)&0x01)&&((~PINL)&0x01))
		{
			outputS("yes?  no?           ",1);
			//then do stuff here to cause factory reset. 
			//probably use some while loop structure to make it happen. 
		}
		
	}
	initGlobals(&currentGlobals, factoryReset);  
	initLEDs();
	initEncoders();
	initADC();
	serialInit0();
	initMidi();
	initEnvelopes();
	initSequencer();
	twi_init();
	initBank(&currentPattern);
	
	//this should be a global function. 
	if(factoryReset==1)
	{
		//do factory reset here.
		factoryResetEeprom(currentPattern);
		globalWrite(&currentGlobals);
	}

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
	TCCR2B = 1<<CS22;//using 64 from pre-scaler
	TIMSK2 = 1<<TOIE2;


	sei();	




while(1) {
	
	
	listenTrigButtons(&currentPattern, &currentGlobals);
	listenGPButtons(currentPattern, &currentGlobals);
	updateLEDs(currentPattern, currentGlobals);
	listenEncoders(&currentPattern, &currentGlobals);
	listenKnobs(&currentPattern, &currentGlobals);
	updateSequencer(currentPattern, currentGlobals);
	updateScreen(&screenBank, &currentPattern, &currentGlobals);
	midiRead(currentPattern, currentGlobals);
	releaseUpdate(&currentPattern, &currentGlobals);

	}
}

ISR(TIMER3_COMPA_vect)
{
	currentGlobals.releaseCounter++; //this will increase every millisecond.
	//should run for about 1000 hours before overflow, so not something we really have to worry about.
}