#ifndef MIDILIB_H_
#define MIDILIB_H_
#include <avr/io.h>
#include "globalVariables.h"


void initMidi();
void transmitMidi();
void midiRead(Pattern currentPattern, Globals currentGlobals);
#endif /* MIDILIB_H_ */
