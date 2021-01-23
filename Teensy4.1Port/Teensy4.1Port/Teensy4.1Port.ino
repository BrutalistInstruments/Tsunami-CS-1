/*
 Name:		Teensy4.ino
 Created:	1/22/2021 1:04:21 PM
 Author:	OurBl
*/

// the setup function runs once when you press reset or power the board
#include "OLEDLib.h"
#include "globalVariables.h"
//initialize our global structs. 
volatile Pattern currentPattern;
volatile Globals currentGlobals;
volatile Screen screenBank;
IntervalTimer OLEDIntervalTimer; //we may be able to save these in our global struct.

//OLEDIntervalTimer.update(5);


void setup() {
	initBank(&currentPattern); //set bank to factory defaults
	initGlobals(&currentGlobals, 0); //set globals to factory defaults. both of these will change once the eeprom is implemented. 
	initPins();
	//begin intervalTimer here
	initScreen(&currentGlobals);
	///for current testing, these will stay in here. 

}

// the loop function runs over and over again until power down or reset
void loop() {

}