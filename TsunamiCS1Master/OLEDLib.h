/*
 * OLEDLib.h
 *
 * Created: 6/26/2019 5:47:25 PM
 *  Author: Hal
 */ 


#ifndef OLEDLIB_H_
#define OLEDLIB_H_

void enableCycle();
void send8bit(uint8_t value);
void command(uint8_t c);
void data(uint8_t d);
void initScreen();
void outputS(char* lineIn, int row);
void numPrinter(char* charArray,uint8_t startingPos, uint8_t numCharacters, uint16_t inputNumber);


#endif /* OLEDLIB_H_ */