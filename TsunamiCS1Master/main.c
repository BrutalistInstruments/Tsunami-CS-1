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


int main(){

//globals
//uint8_t midiChannel=0;
Pattern currentPattern;
Screen screenBank;
Globals currentGlobals;
uint8_t factoryReset=0; // set this to 1 if you would like to fill the eeprom with Factory data, and erase all user data.
	
	initScreen();
	initGlobals(&currentGlobals, factoryReset);  
	initButtons();
	initLEDs();
	initEncoders();
	initADC();
	serialInit0();
	initMidi();
	//initEnvelopes();
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

	//what were these for? Some timer interrupt somewhere?
	TCCR2B = 1<<CS22;//using 64 from pre-scaler
	TIMSK2 = 1<<TOIE2;

	sei();	

while(1) {
	
	
	listenTrigButtons(&currentPattern, &currentGlobals);
	listenGPButtons(currentPattern, &currentGlobals);
	updateLEDs(currentPattern, currentGlobals);
	listenEncoders(&currentPattern, &currentGlobals);
	listenKnobs(&currentPattern, &screenBank, &currentGlobals);
	updateSequencer(currentPattern, currentGlobals);
	updateScreen(&screenBank, &currentPattern, &currentGlobals);
	midiRead(currentPattern, currentGlobals);
	//releaseCheck(releaseCounter, &releaseTracker, releaseCounterArray[16],currentPattern);

	}
}