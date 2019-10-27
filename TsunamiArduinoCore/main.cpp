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
#include <Midi.h>
//#include "eepromLib.h" to implement

//MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, midi0);

//globals
uint8_t midiChannel;
Pattern currentPattern;
Screen screenBank;
uint8_t menuState=0; //Bits - 7 encoderA Flag, 6-4 EncoderAState, 3: EncoderBFlag, 2-0: encoderBState
uint8_t currentStep=0;
uint16_t currentTrigButtons=0;
uint8_t currentPatternNumber = 0;
uint8_t currentTrack;

MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, midi0);
// //I'm not sure if we can put this in a library or not, but I'll just get it working here to start.
void handleNoteOn(uint8_t inChannel, uint8_t inNote, uint8_t inVelocity)
 {
   for (int i=0; i<16; i++)
   {
     if(inNote==currentPattern.midiTrackNote[i])
     {
       //trackControl(char trackNumberLSB, char trackNumberMSB, char outputNumber, char trackCommand)
       trackControl(currentPattern.trackSampleLSB[i], currentPattern.trackSampleMSB[i], currentPattern.trackOutputRoute[i], currentPattern.trackPlayMode[i]);
     }
   }

 }

void setup() {
  //update sequence
  midiChannel = 0; //for now, need to change this from load memory
	initBank(currentPattern);
	initScreen(); //gets OLED ready for data
	initButtons();
	initLEDs();
	initEncoders();
	initMenu(screenBank, currentPattern, currentPatternNumber); //fills screenBank with menu strings
	initADC();
	serialInit0();
  midi0.begin(midiChannel+1);
  midi0.setHandleNoteOn(handleNoteOn);
//  initEeprom(); //to be implemented


  TCCR2B = 1<<CS22;//using 64 from prescaler
  TIMSK2 = 1<<TOIE2;

	sei();
	 //this will be necessary on first startup, but maybe not in the actual program? maybe just something handy to have.
	//loadMemory(); //we need to load in the first struct in locarion 0 of our eeprom.


}

void loop() {
      listenTrigButtons(menuState, currentPattern, &currentTrigButtons, screenBank, &currentTrack);
  		listenGPButtons(&menuState);
  		updateLEDs(menuState, currentPattern, currentTrigButtons);
  		listenEncoders(&menuState);
  		listenKnobs(currentPattern, menuState, screenBank);
  		//updateSequence(); //to be implemented
  		updateScreen(&menuState, screenBank);
      midi0.read();
}
