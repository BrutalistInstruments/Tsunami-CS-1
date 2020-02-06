/*
 * menu.c
 *
 * Created: 8/13/2019 5:54:51 PM
 *  Author: Hal
 */
#include <Arduino.h>
#include <avr/io.h>
#include <string.h> //do I need these?
#include <stdio.h> //I don't think I do.
#include "globalVariables.h"
#include "OLEDLib.h"

//globals we probably don't need anymore
char midiNote[3] = "C-0";
extern Pattern currentPattern; //maybe this should be in our init function?
extern uint8_t midiChannel;
extern uint8_t currentPatternNumber;
extern Screen screenBank;

//globals we actually need:
uint8_t prevMenuState;



//I don't like using a global extern here, instead of a passed pointer,
//but I can't seem to get the struct to stay in scope.
void initMenu(Screen initTheScreen, Pattern initPattern, uint8_t initPatternNumber)
{

//screen0
// = {"Performance Mode    ","Pattern:            ","BPM: xxx            ","Stop                "}
initArrays(screenBank.screen0,8,1,"Pattern:");
initArrays(screenBank.screen0,4,2,"BPM:");
initArrays(screenBank.screen0,4,3,"Stop");
initArrays(screenBank.screen0,16,0,"Performance Mode");
//screen1
// = {"Sequence Edit       ","Pattern:            ","Steps:              ","Step number:        "}; //this will eventually be 5 once we implement naming of samples.
initArrays(screenBank.screen1,8,1,"Pattern:");
initArrays(screenBank.screen1,6,2,"Steps:");
initArrays(screenBank.screen1,12,3,"Step number:");
initArrays(screenBank.screen1,13,0,"Sequence Edit");
//screen2
//= {"Track Settings      ","Track:              ","Play Mode           ","OutRoute            "};
initArrays(screenBank.screen2,6,1,"Track:");
initArrays(screenBank.screen2,9,2,"PlayMode:");
initArrays(screenBank.screen2,9,3,"OutRoute:");
initArrays(screenBank.screen2,14,0,"Track Settings");
//screen3
// = {"Global Settings     ","Midi Channel: xx    ", "Midi trig:  "};
initArrays(screenBank.screen3,13,1,"Midi Channel:");
initArrays(screenBank.screen3,18,2,"Midi trig:           ");
initArrays(screenBank.screen3,20,3,"                     ");
initArrays(screenBank.screen3,15,0,"Global Settings");

numPrinter(screenBank.screen0[2],5,3, currentPattern.patternBPM);
numPrinter(screenBank.screen3[1],14,2, (midiChannel+1));
numPrinter(screenBank.screen0[1], 9, 3, initPatternNumber+1);
numPrinter(screenBank.screen1[1], 9, 3, initPatternNumber+1); //pattern index is 0 through 255, but when displayed
numPrinter(screenBank.screen1[2], 7, 2, currentPattern.numSteps);
numPrinter(screenBank.screen1[3], 13, 2, (currentStep+1));



	for(uint8_t i=0;i<4; i++ )
	{
		outputS(screenBank.screen0[i],i);
	}
}

void updateScreen(uint8_t *menuStateChange, Screen menuScreen)
{
	//Bits - 7 encoderA Flag, 6-4 EncoderAState, 3: EncoderBFlag, 2-0: encoderBState
	if(*menuStateChange!=prevMenuState){
	switch(*menuStateChange)
	{
		//I just printed out every number in the 7 bit range, since we won't be
		//using encoderAflag at this moment in time.

		//Encoder B State: 0 = no arrow;
		//Encoder B State: 1 = arrow on screen, init state, very top
		//Encoder B State: 2 = arrow in middile of screen
		//Encoder B State: 3 = arrow at bottom of the page

		case 0: //inital state
		outputS(menuScreen.screen0[0], 0);
		outputS(menuScreen.screen0[1], 1);
		outputS(menuScreen.screen0[2], 2);
		outputS(menuScreen.screen0[3], 3);
		break;

//case 1:
//this case does nothing, since there can be no B states in screen0 of encoder A
//the next state will be 0 001 0 000 / or 0 011 0 000
//break;


case 16: //0 001 0 000
//this is EncoderA state 1, so Sequencer Menu.
//possible state changes:
//0 000 0 000 - Backwards 1 step on encoder A
//0 010 0 000 - forward 1 step on encoder A
//0 001 0 001 - imediatly go to this state.
outputS(menuScreen.screen1[0], 0);
outputS(menuScreen.screen1[1], 1);
outputS(menuScreen.screen1[2], 2);
outputS(menuScreen.screen1[3], 3);
//should we print the arrow on the screen right away?
//I think we should.
//or maybe we imidiately move to state 17?
*menuStateChange = 17; //we may need to make this a pointer?
//no break, go strait to state 17

case 17:
menuScreen.screen1[1][19]= '<';
menuScreen.screen1[2][19] = ' ';
menuScreen.screen1[3][19] = ' ';
outputS(menuScreen.screen1[1], 1);
outputS(menuScreen.screen1[2], 2);
outputS(menuScreen.screen1[3], 3);
break;

case 18:
menuScreen.screen1[1][19]= ' ';
menuScreen.screen1[2][19] = '<';
menuScreen.screen1[3][19] = ' ';
outputS(menuScreen.screen1[1], 1);
outputS(menuScreen.screen1[2], 2);
outputS(menuScreen.screen1[3], 3);
break;

case 19:
menuScreen.screen1[1][19]= ' ';
menuScreen.screen1[2][19] = ' ';
menuScreen.screen1[3][19] = '<';
outputS(menuScreen.screen1[1], 1);
outputS(menuScreen.screen1[2], 2);
outputS(menuScreen.screen1[3], 3);
break;

case 25:
//this is case 17, but with the encoderBFlag active.
break;

case 26:
//this is case 18, but with the encoderBFlag active.
break;

case 27:
//this is case 19, but with the encoderBFlag active.
break;

case 32:
outputS(menuScreen.screen2[0], 0);
outputS(menuScreen.screen2[1], 1);
outputS(menuScreen.screen2[2], 2);
outputS(menuScreen.screen2[3], 3);
*menuStateChange = 33;

case 33:
menuScreen.screen2[1][19]= '<';
menuScreen.screen2[2][19] = ' ';
menuScreen.screen2[3][19] = ' ';
outputS(menuScreen.screen2[1], 1);
outputS(menuScreen.screen2[2], 2);
outputS(menuScreen.screen2[3], 3);
break;

case 34:
menuScreen.screen2[1][19]= ' ';
menuScreen.screen2[2][19] = '<';
menuScreen.screen2[3][19] = ' ';
outputS(menuScreen.screen2[1], 1);
outputS(menuScreen.screen2[2], 2);
outputS(menuScreen.screen2[3], 3);

break;

case 35:
menuScreen.screen2[1][19]= ' ';
menuScreen.screen2[2][19] = ' ';
menuScreen.screen2[3][19] = '<';
outputS(menuScreen.screen2[1], 1);
outputS(menuScreen.screen2[2], 2);
outputS(menuScreen.screen2[3], 3);
break;


case 41:
//case 33, but with EncoderBFlag
break;

case 42:
//case 34, but with EncoderBFlag`
break;

case 43:
//case 35, but with EncoderBFlag
break;

case 48:
outputS(menuScreen.screen3[0], 0);
outputS(menuScreen.screen3[1], 1);
outputS(menuScreen.screen3[2], 2);
outputS(menuScreen.screen3[3], 3);
*menuStateChange = 49;

case 49:
menuScreen.screen3[1][19]= '<';
menuScreen.screen3[2][19] = ' ';
menuScreen.screen3[3][19] = ' ';
outputS(menuScreen.screen3[1], 1);
outputS(menuScreen.screen3[2], 2);
outputS(menuScreen.screen3[3], 3);
break;

case 50:
menuScreen.screen3[1][19]= ' ';
menuScreen.screen3[2][19] = '<';
menuScreen.screen3[3][19] = ' ';
outputS(menuScreen.screen3[1], 1);
outputS(menuScreen.screen3[2], 2);
outputS(menuScreen.screen3[3], 3);
break;

case 51:
menuScreen.screen3[1][19]= ' ';
menuScreen.screen3[2][19] = ' ';
menuScreen.screen3[3][19] = '<';
outputS(menuScreen.screen3[1], 1);
outputS(menuScreen.screen3[2], 2);
outputS(menuScreen.screen3[3], 3);
break;

case 57:
//this is case49, but with EncoderBFlag
break;

case 58:
//this is case50, but with EncoderBFlag
break;

case 59:
//this is case51, but with EncoderBFlag
break;


	}
	prevMenuState = menuStateChange;
}
}
