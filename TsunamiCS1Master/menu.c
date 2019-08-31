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
		encoderBValue = encoderBValue%4;
		//this needs some work...
		uint8_t menuMoveArrow = encoderBValue - prevEncoderBValue; //this tells us whether we need to move up or down.
		switch(encoderAValue)
		{
			
			case 1:
			if(encoderBFlag)
			{
				switch (screen1Index)
				{
					case 1:
				if(menuMoveArrow==1)
				{
					currentPatternNumber = currentPatternNumber + 1;
					numPrinter(screen1[1],9,3,currentPatternNumber);
					outputS(screen1[1], 1);
				}else
				{
					currentPatternNumber = currentPatternNumber - 1;
					numPrinter(screen1[1],9,3,currentPatternNumber);
					outputS(screen1[1], 1);
				}
				break;
				
				case 2:
				if(menuMoveArrow==1){
				currentPattern.numSteps = currentPattern.numSteps+1;
					if(currentPattern.numSteps>64)
					{
						currentPattern.numSteps = 64;
					}
					numPrinter(screen1[2], 6, 2, currentPattern.numSteps);
					outputS(screen1[2], 2);
				}else
				{
					
					currentPattern.numSteps = currentPattern.numSteps-1;
					if(currentPattern.numSteps<1)
					{
						currentPattern.numSteps= 1;
			
					}
					numPrinter(screen1[2], 6, 2, currentPattern.numSteps);
					outputS(screen1[2],2);
				}
				break;
				
				case 3:
				if(menuMoveArrow==1)
				{
					currentStep = currentStep+1;
					if(currentStep>(currentPattern.numSteps)-1)
					{
						currentStep = (currentPattern.numSteps)-1;
					}
					numPrinter(screen1[3], 13, 2, currentStep+1);
					outputS(screen1[3], 3);
					
				}else
				{
					currentStep = currentStep-1;
					if(currentStep==255)
					{
						currentStep = 0;
					}
					numPrinter(screen1[3], 13, 2, currentStep+1);
					outputS(screen1[3], 3);
				}
				
				break;
				}
			}else{
			if(menuMoveArrow==1)
			{
				
				screen1Index++;
				if(screen1Index>3)
				{
					screen1Index = 3;
				}
				screen1[screen1Index][19] = '<';
				screen1[screen1Index-1][19] = ' ';
				outputS(screen1[screen1Index], screen1Index);
				outputS(screen1[screen1Index-1], screen1Index-1);
			
			
			}else 
			{
				screen1Index--;
				if(screen1Index>250||screen1Index==0)
				{
					screen1Index = 1;
				}
				screen1[screen1Index][19] = '<';
				screen1[screen1Index+1][19] = ' ';
				outputS(screen1[screen1Index], screen1Index);
				outputS(screen1[screen1Index+1], screen1Index+1);
			
			}
			}
			
			break;
			
			case 2:
			if(encoderBFlag)
			{
				switch (screen2Index)
				{
					if(menuMoveArrow==1)
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
					
					case 2:
					if(menuMoveArrow==1)
					{
						currentPattern.trackPlayMode[currentTrack] = (currentPattern.trackPlayMode[currentTrack])+1;
						if(currentPattern.trackPlayMode[currentTrack]>6)
						{
							currentPattern.trackPlayMode[currentTrack] = 6;
						}
						//do printing poly or solo stuff here.
						
						outputS(screen2[2], 2);
						
					}else
					{
						currentPattern.trackPlayMode[currentTrack] = (currentPattern.trackPlayMode[currentTrack])+1;
						if(currentPattern.trackPlayMode[currentTrack]==255)
						{
							currentPattern.trackPlayMode[currentTrack] = 0;
						}
						//do printing poly or solo stuff here
						outputS(screen2[2], 2);
					}
					break;
					
					case 3:
					if(menuMoveArrow==1)
					{
						currentPattern.trackOutputRoute[currentTrack] = (currentPattern.trackOutputRoute[currentTrack])+1;
						if(currentPattern.trackOutputRoute[currentTrack]>7)
						{
							currentPattern.trackOutputRoute[currentTrack] = 7;
						}
						numPrinter(screen2[3], 10, 2, (currentPattern.trackOutputRoute[currentTrack])+1);
						outputS(screen2[3],3);
						
					}else
					{
						currentPattern.trackOutputRoute[currentTrack] = (currentPattern.trackOutputRoute[currentTrack])-1;
						if(currentPattern.trackOutputRoute[currentTrack]==255)
						{
							currentPattern.trackOutputRoute[currentTrack] = 0;
						}
						numPrinter(screen2[3], 10, 2, (currentPattern.trackOutputRoute[currentTrack])+1);
						outputS(screen2[3],3);
						
					}
					
					break;
					
				}
				
			}else{
			if(menuMoveArrow==1)
			{
				screen2Index++;
				if(screen2Index>3)
				{
					screen2Index = 3;
				}
				screen2[screen2Index][19] = '<';
				screen2[screen2Index-1][19] = ' ';
				outputS(screen2[screen2Index], screen2Index);
				outputS(screen2[screen2Index-1], screen2Index-1);
				
				
			}else
			{
				screen2Index--;
				if(screen2Index>250||screen2Index==0) //this should account for any negative numbers from overflow.
				{
					screen2Index = 1;
				}
				screen2[screen2Index][19] = '<';
				screen2[screen2Index+1][19] = ' ';
				outputS(screen2[screen2Index], screen2Index);
				outputS(screen2[screen2Index+1], screen2Index+1);
				
			}
			}
			
			/*
			
			*/
			break;
			
			case 3:
			
				//this is the functionality if encoder B flag is not pressed
				if(menuMoveArrow==1)
				{
					if(encoderBFlag)
					{
						switch(screen3Index)
						{
							case 1:
							midiChannel = midiChannel+1;
							if(midiChannel>15)
							{
								midiChannel = 15;
							}
							numPrinter(screen3[1], 14, 2, (midiChannel+1));
							outputS(screen3[1], 1);
							break;
							
						}
					}else
					{
						screen3Index++;
						if(screen3Index>3)
						{
							screen3Index = 3;
						}
						screen3[screen3Index][19] = '<';
						screen3[screen3Index-1][19] = ' ';
						outputS(screen3[screen3Index], screen3Index);
						outputS(screen3[screen3Index-1], screen3Index-1);
					}
					
					
				}else
				{
					if(encoderBFlag)
					{
						switch(screen3Index)
						{
							case 1:
							midiChannel = midiChannel-1;
							if(midiChannel==255)
							{
								midiChannel = 0;
							}
							numPrinter(screen3[1], 14, 2, (midiChannel+1));
							outputS(screen3[1], 1);
							break;
							
						}
					}else{
					screen3Index--;
					if(screen3Index>250||screen3Index==0) //this should account for any negative numbers from overflow.
					{
						screen3Index = 1;
					}
					screen3[screen3Index][19] = '<';
					screen3[screen3Index+1][19] = ' ';
					outputS(screen3[screen3Index], screen3Index);
					outputS(screen3[screen3Index+1], screen3Index+1);
				}
					}
				
			
			break;
					
		}
		prevEncoderBValue = encoderBValue;
	}
	
	//top encoder
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
		numPrinter(screen1[2], 6, 2, currentPattern.numSteps);
		numPrinter(screen1[3], 13, 2, (currentStep+1));
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