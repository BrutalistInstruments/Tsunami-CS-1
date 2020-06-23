/*
 * knobLib.h
 *
 * Created: 8/11/2019 8:00:34 AM
 *  Author: Hal
 */ 


#ifndef KNOBLIB_H_
#define KNOBLIB_H_
#include "globalVariables.h"

void selectKnob(uint8_t select);
void updateKnob(uint8_t select, Globals *currentGlobals);
void initializeKnob(Globals *currentGlobals);
void interperetKnob(uint8_t select, Pattern *currentKnobPattern, Screen *knobScreen, Globals *currentGlobals);
uint8_t startADCConversion();
void initADC();
void listenKnobs(Pattern *currentKnobPattern, Screen *currentScreen, Globals *currentGlobals);
uint8_t checkVariation(uint8_t v1, uint8_t v2);




#endif /* KNOBLIB_H_ */