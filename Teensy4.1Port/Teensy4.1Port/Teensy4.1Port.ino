/*
 Name:		Teensy4.ino
 Created:	1/22/2021 1:04:21 PM
 Author:	OurBl
*/

// the setup function runs once when you press reset or power the board
#include "OLEDLib.h"
#include "globalVariables.h"
#include <teensyTimerTool.h>
//initialize our global structs. 
volatile Pattern currentPattern;
volatile Globals currentGlobals;
volatile Screen screenBank;
TeensyTimerTool::Timer OLEDTimer;


void setup() {
	initBank(&currentPattern); //set bank to factory defaults
	initGlobals(&currentGlobals, 0); //set globals to factory defaults. both of these will change once the eeprom is implemented. 
	///for current testing, these will stay in here. 
	initPins();
	OLEDTimer.beginPeriodic([] {enableCycle(&currentGlobals); }, 6);
	initScreen(&currentGlobals);
	

}

// the loop function runs over and over again until power down or reset
void loop() {

}