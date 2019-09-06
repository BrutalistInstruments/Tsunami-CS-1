#define F_CPU 16000000UL

#include <avr/io.h>
#include "OLEDLib.h"
#include "ButtonLib.h"
#include "EncoderLib.h"
#include "menu.h"
#include "knobLib.h"
#include "globalVariables.h"
#include "serialLib.h"
#include "LEDLib.h"
#include "MidiLib.h"
#include <util/delay.h>
#include <avr/interrupt.h>


int main(void)
{
	midiChannel = 0; //for now, need to change this from load memory
	initBank();
	initScreen();
	initButtons();
	initLEDs();
	initEncoders();
	initMenu();
	initADC();
	serialInit0();
	sei();
	 //this will be necessary on first startup, but maybe not in the actual program? maybe just something handy to have.
	//loadMemory(); //we need to load in the first struct in locarion 0 of our eeprom.
	//change for github
	while (1)
	{
		listenTrigButtons();
		listenMidi();
		listenGPButtons();
		listenMidi();
		updateLEDs();
		listenMidi();
		listenEncoders();
		listenMidi();
		listenKnobs();
		listenMidi();
		//updateSequence();
		updateScreen();
		listenMidi();
		
	}
}

