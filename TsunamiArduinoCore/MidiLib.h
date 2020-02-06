#ifndef MIDILIB_H_
#define MIDILIB_H_
#include <avr/io.h>
#include "globalVariables.h"

//this library is untested
void initMidi();
void transmitMidi();
void midiRead(uint8_t midiChannel, Pattern currentPattern);
#endif /* MIDILIB_H_ */
