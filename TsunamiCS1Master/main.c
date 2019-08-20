#define F_CPU 16000000UL

#include <avr/io.h>
#include "OLEDLib.h"
#include "ButtonLib.h"
#include "EncoderLib.h"
#include "menu.h"
#include "knobLib.h"
#include "globalVariables.h"
#include "serialLib.h"
#include <util/delay.h>
#include <avr/interrupt.h>

int main(void)
{
	initScreen();
	initButtons();
	initEncoders();
	initMenu();
	initADC();
	serialInit0();
	sei();
	initBank(); //this will be necessary on first startup, but maybe not in the actual program? maybe just something handy to have.
	//loadMemory(); //we need to load in the first struct in locarion 0 of our eeprom.
	//making an edit for GIT
	//more comments to see if we can cause the same problem
	while (1)
	{
		listenTrigButtons();
		listenGPButtons();
		listenEncoders();
		listenKnobs();
		//listenMidi();
		//updateSequence();
		updateScreen();
		
	}
}

