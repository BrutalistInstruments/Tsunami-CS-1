#define F_CPU 16000000UL

#include "OLEDLib.h"
#include "ButtonLib.h"
#include "EncoderLib.h"
#include "menu.h"
#include "knobLib.h"
#include "globalVariables.h"
#include "serialLib.h"
#include "LEDLib.h"
#include "MidiLib.h"
#include "debounceLib.h"
void setup() {
  // put your setup code here, to run once:
  midiChannel = 0; //for now, need to change this from load memory
  initBank();
//  initScreen();
  initButtons();
  initLEDs();
  initEncoders();
  initMenu();
  initADC();
  serialInit0();
  TCCR0B = 1<<CS02;
  TIMSK0 = 1<<TOIE0;
  
  
  sei();

}

void loop() {
  // put your main code here, to run repeatedly:
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
  //  updateScreen();
    listenMidi();

}
