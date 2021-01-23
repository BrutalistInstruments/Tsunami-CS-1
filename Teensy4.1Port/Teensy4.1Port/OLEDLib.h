// OLEDLib.h
#include "globalVariables.h"

#ifndef _OLEDLIB_h
#define _OLEDLIB_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

void enableCycle(volatile Globals *OLEDGlobals);
void command(uint8_t c, volatile Globals *OLEDGlobals);
void data(uint8_t d, volatile Globals* OLEDGlobals);
void initPins();
void initScreen(volatile Globals *OLEDGlobals);
void outputS(char* lineIn, int row, volatile Globals* OLEDGlobals);
void numPrinter(char* charArray, uint8_t startingPos, uint8_t numCharacters, uint16_t inputNumber);
void midiNotePrinter(char* charArray, uint8_t startingPosition, uint8_t noteNumber);

#endif

