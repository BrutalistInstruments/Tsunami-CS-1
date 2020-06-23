/*
 * serialLib.h
 *
 * Created: 8/9/2019 10:59:59 AM
 *  Author: Hal
 */ 
#include <avr/io.h>

#ifndef SERIALLIB_H_
#define SERIALLIB_H_

void appendSerial0(char c);
void serialWrite0(char c[], uint8_t messageLength);
void serialInit0();
char getChar();


#endif /* SERIALLIB_H_ */
