/*
 Name:		Teensy4.ino
 Created:	1/22/2021 1:04:21 PM
 Author:	OurBl
*/

// the setup function runs once when you press reset or power the board
#include "ButtonLib.h"
#include "encoderLib.h"
#include "menuLib.h"
#include "OLEDLib.h"
#include "globalVariables.h"
#include <teensyTimerTool.h>
#include <Encoder.h>
//initialize our global structs. 
volatile Pattern currentPattern;
volatile Globals currentGlobals;
volatile Screen screenBank;
TeensyTimerTool::Timer OLEDTimer;
TeensyTimerTool::Timer EncoderTimer;
TeensyTimerTool::Timer ButtonTimer;

char encoderTest[20] = "TopEncoderValue:   ";
int encoderRead1 = 0;
int encoderRead2 = 0;
int prevEncoderRead1 = 0;
int prevEncoderRead2 = 0;


void setup() {
	initBank(&currentPattern); //set bank to factory defaults
	initGlobals(&currentGlobals, 0); //set globals to factory defaults. both of these will change once the eeprom is implemented. 
	///for current testing, these will stay in here. 
	initPins();
	OLEDTimer.beginPeriodic([] {enableCycle(&currentGlobals); }, 10);
	initScreen(&currentGlobals);
	//outputS("Working Screen     ", 1, &currentGlobals);
	initMenu(&screenBank, &currentPattern, &currentGlobals);
	initEncoders();
	EncoderTimer.beginPeriodic([] {listenEncoders(&currentGlobals); }, 1000);
	ButtonTimer.beginPeriodic([] {listenButton(&currentGlobals); }, 100); //might want to make this go slower, check for response times on the buttons
	Serial.begin(9600);
}

// the loop function runs over and over again until power down or reset
void loop() {
	updateScreen(&screenBank, &currentPattern, &currentGlobals);

}