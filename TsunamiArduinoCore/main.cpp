#include <Arduino.h>
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


//globals
uint8_t midiChannel;
Pattern currentPattern;
Screen screenBank;
uint8_t menuState=0; //Bits - 7 encoderA Flag, 6-4 EncoderAState, 3: EncoderBFlag, 2-0: encoderBState
uint8_t currentStep=0; //current Step is 0 indexed. ex: a 16 step sequece is array location 0 to 15.
uint16_t currentTrigButtons=0;
uint8_t currentPatternNumber = 0;
uint8_t currentTrack;
uint8_t playState=0; //0 for stop, 1 for play. maybe we implement 2 for pause later or something for pausing a sequence?
uint8_t factoryReset=0; // set this to 1 if you would like to fill the eeprom with Fatory data, and erase all user data.
uint8_t buttonSwitchFlag = 0;


void setup() {
  //update sequence
  midiChannel = 0; //for now, need to change this from load memory
	//initBank(&currentPattern);
	initScreen(); //gets OLED ready for data
	initButtons();
	initLEDs();
	initEncoders();
	initADC();
	serialInit0();
  initMidi();

  initSequencer();
twi_init();
if(factoryReset==1)
{
  //do factory reset here.
  factoryResetEeprom(currentPattern);
  factoryReset = 0;
  //we need to write this in a specific place in the eeprom, and check it in the if statement.
}
currentPattern = eepromLoadPattern(1);

initMenu(screenBank, currentPattern, currentPatternNumber); //fills screenBank with menu strings

  TCCR2B = 1<<CS22;//using 64 from prescaler
  TIMSK2 = 1<<TOIE2;

	sei();
	 //this will be necessary on first startup, but maybe not in the actual program? maybe just something handy to have.
	//loadMemory(); //we need to load in the first struct in locarion 0 of our eeprom.


}

void loop() {
      listenTrigButtons(menuState, &currentPattern, &currentTrigButtons, screenBank, &currentTrack, currentStep);
  		listenGPButtons(&menuState, &playState, &buttonSwitchFlag);
      updateLEDs(menuState, currentPattern, currentTrigButtons, currentStep);
      listenEncoders(&menuState, &currentStep);
  		listenKnobs(currentPattern, menuState, screenBank, buttonSwitchFlag);
  		updateSequencer(currentPattern, playState);
  		updateScreen(&menuState, screenBank);
      midiRead(midiChannel, currentPattern);
}
