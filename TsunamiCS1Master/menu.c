/*
 * menu.c
 *
 * Created: 8/13/2019 5:54:51 PM
 *  Author: Hal
 */ 

#include <avr/io.h>
#include <string.h>
#include <stdio.h>
#include "globalVariables.h"
#include "OLEDLib.h"


//screen 0
char screen0[5][20] = {"Performance Mode    ","Pattern:            ","BPM: xxx            "
,"Stop                ","Play                "  };
uint8_t screen0Index = 0;

//screen 1:
char screen1[5][20] = {"Sequence Edit       ","Pattern:            ","Steps  :            "
,"Step number:        ","                    "};
uint8_t screen1Index = 0;

//screen 2:
char screen2[4][20] = {"Track Settings      ","Track:              ",
"Play Mode           ","OutRoute            "};
uint8_t screen2Index = 0;

//screen 3:
char screen3[4][20] = {"Global Settings     ","Midi Channel: xx    ", "Midi trigger Notes  ","(put triggered note)"};
uint8_t screen3Index = 0;

void initMenu()
{

numPrinter(screen0[2],5,3, currentPattern.patternBPM);
numPrinter(screen3[1],14,2, (midiChannel+1));
numPrinter(screen0[1], 9, 3, currentPatternNumber);
numPrinter(screen1[1], 9, 3, currentPatternNumber);
numPrinter(screen0[2], 5, 3, currentPattern.patternBPM);

for(uint8_t i=0;i<4; i++ ){
	outputS(screen0[i],i);
}


}

void updateScreen()
{
	if(prevEncoderBValue!=encoderBValue)
	{
		//encoderBValue = encoderBValue%5;
		//this needs some work...
//		uint8_t menuMoveArrow = encoderBValue - prevEncoderBValue; //this tells us whether we need to move up or down.
		switch(encoderAValue)
		{
			case 0:
// 			if(menuMoveArrow==1)
// 			{
// 				screen0Index++;
// 				if(screen0Index>4)
// 				{
// 					screen0Index = 4;
// 				}
// 				screen0[screen0Index][19] = '<';
// 				screen0[screen0Index-1][19] = ' ';
// 				outputS(screen0[screen0Index], screen0Index);
// 				outputS(screen0[screen0Index-1], screen0Index-1);
// 				
//			}else
//			{
// 				screen0Index--;
// 				if(screen0Index<1)
// 				{
// 					screen0Index = 1;
// 				}
// 				screen0[screen0Index][19] = '<';
// 				screen0[screen0Index+1][19] = ' ';
// 				outputS(screen0[screen0Index], screen0Index);
// 				outputS(screen0[screen0Index+1], screen0Index+1);
			
			
			//}
			
			break;
			
			case 1:
			break;
			
			case 2:
			if(encoderBValue-prevEncoderBValue==1)
			{
				currentPattern.trackSampleLSB[currentTrack] = (currentPattern.trackSampleLSB[currentTrack])+ 1; 
				uint16_t currentSample = (currentPattern.trackSampleMSB[currentTrack]<<8)|(currentPattern.trackSampleLSB[currentTrack]);
				numPrinter(screen2[1], 7, 2, (currentTrack+1));
				numPrinter(screen2[1], 10, 4, currentSample);
				outputS(screen2[1], 1);
	
			}else
			{
				currentPattern.trackSampleLSB[currentTrack] = (currentPattern.trackSampleLSB[currentTrack])- 1;
				uint16_t currentSample = (currentPattern.trackSampleMSB[currentTrack]<<8)|(currentPattern.trackSampleLSB[currentTrack]);
				numPrinter(screen2[1], 7, 2, (currentTrack+1));
				numPrinter(screen2[1], 10, 4, currentSample);
				outputS(screen2[1], 1);
			}
			
			break;
			
			case 3:
			break;
			
		}
		prevEncoderBValue = encoderBValue;
	}
	if(prevEncoderAValue!=encoderAValue){
	encoderAValue = encoderAValue%4;
	switch (encoderAValue)
	{
		case 0:
		numPrinter(screen0[2], 5, 3, currentPattern.patternBPM);
		for(uint8_t i=0;i<4; i++ ){
			outputS(screen0[i],i);
		}
		screen0Index = 0;
		
		break;
		
		case 1:
		for(uint8_t i=0;i<4; i++ ){
 		outputS(screen1[i],i);
		}
		screen1Index = 0;
 		break;
		 
		case 2:
		for(uint8_t i=0;i<4; i++ ){
			outputS(screen2[i],i);
		}
		screen2Index = 0;
		break;
		 
		case 3:
		for(uint8_t i=0;i<4; i++ ){
			outputS(screen3[i],i);
		}
		screen3Index = 0;
		break;
		 
		 
	}
	prevEncoderAValue = encoderAValue;
	
	}


}