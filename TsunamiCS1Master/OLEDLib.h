/*
 * OLEDLib.h
 *
 * Created: 6/26/2019 5:47:25 PM
 *  Author: Hal
 */ 


#ifndef OLEDLIB_H_
#define OLEDLIB_H_

#include <avr/io.h>

void enableCycle();
void send8bit(uint8_t value);
void command(uint8_t c);
void data(uint8_t d);
void initScreen();
void outputS(unsigned char* lineIn, int row);
void numPrinter(unsigned char* charArray,uint8_t startingPos, uint8_t numCharacters, uint16_t inputNumber);
void midiNotePrinter(char* charArray, uint8_t startingPosition, uint8_t noteNumber);


#endif /* OLEDLIB_H_ */