/*
 * menu.c
 *
 * Created: 8/13/2019 5:54:51 PM
 *  Author: Hal
 */
#include <avr/io.h>
#include "globalVariables.h"
#include "OLEDLib.h"

#define selectBit 0xF7
char midiNote[4] = "C-0";


uint8_t prevMenuState; //do we need this anymore?

//I don't like using a global extern here, instead of a passed pointer,
//but I can't seem to get the struct to stay in scope.
void initMenu(Screen *initTheScreen, Pattern currentPattern, Globals currentGlobals)
{

//screen0
initArrays(initTheScreen->screen0,0,"Performance Mode");
initArrays(initTheScreen->screen0,1,"Pattern:");
initArrays(initTheScreen->screen0,2,"BPM:");
initArrays(initTheScreen->screen0,3,"Stop");

//screen1
initArrays(initTheScreen->screen1,0,"Sequence Edit");
initArrays(initTheScreen->screen1,1,"Pattern:");
initArrays(initTheScreen->screen1,2,"Steps:");
initArrays(initTheScreen->screen1,3,"Step number:");

//screen2
initArrays(initTheScreen->screen2,0,"Track Settings");
initArrays(initTheScreen->screen2,1,"Track:");
initArrays(initTheScreen->screen2,2,"PlayMode:");
initArrays(initTheScreen->screen2,3,"OutRoute:");
initArrays(initTheScreen->screen2,4,"EnvelopeMode:");
initArrays(initTheScreen->screen2,5,"SustainTime:      S");

//screen3
initArrays(initTheScreen->screen3,1,"Midi Channel:");
initArrays(initTheScreen->screen3,2,"Midi trig   :");
initArrays(initTheScreen->screen3,3," ");
initArrays(initTheScreen->screen3,0,"Global Settings");

//init all of the knob arrays:
initArrays(initTheScreen->knobScreen,0,"OutVolume x : xxxdb");//string 0 is outVolume
initArrays(initTheScreen->knobScreen,1,"Pitch : xxx");//string 1 is pitch
initArrays(initTheScreen->knobScreen,2,"AttackTime  :      S"); //string 2 is Envelope gain
initArrays(initTheScreen->knobScreen,3,"ReleaseTimexx:xx xxx"); //string 3 is Envelop Time
initArrays(initTheScreen->knobScreen,4,"TrackVolume xx:xxxdb"); //string 4 is track Level.
//initArrays(initTheScreen->knobScreen,5,"BPM: ");
//we might want to put in one of these for BPM, but I'm not sure. 

numPrinter(initTheScreen->screen0[2],5,3, currentPattern.patternBPM);
numPrinter(initTheScreen->screen3[1],14,2, (currentGlobals.midiChannel)+1);
numPrinter(initTheScreen->screen0[1], 9, 3, (currentGlobals.currentPatternNumber)+1);
numPrinter(initTheScreen->screen1[1], 9, 3, (currentGlobals.currentPatternNumber)+1);
numPrinter(initTheScreen->screen1[2], 7, 2, currentPattern.numSteps);
numPrinter(initTheScreen->screen1[3], 13, 2, (currentGlobals.currentStep)+1);
 
	for(uint8_t i=0;i<4; i++ )
	{
		outputS(initTheScreen->screen0[i],i);
	}
}

//this method fills all the relevant screens once we load a new pattern. 
void reInitMenuOnLoad(Screen *initTheScreen, Pattern *currentPattern, Globals *currentGlobals)
{
	numPrinter(initTheScreen->screen0[2],5,3, currentPattern->patternBPM);
	numPrinter(initTheScreen->screen1[2], 7, 2, currentPattern->numSteps);
	numPrinter(initTheScreen->screen1[1],9,3,(currentGlobals->currentPatternNumber)+1);
	numPrinter(initTheScreen->screen0[1],9,3,(currentGlobals->currentPatternNumber)+1);
}

void updateScreen(Screen *menuScreen, Pattern *currentPattern, Globals *currentGlobals)
{
	
	if((currentGlobals->valueChangeFlag)&(1<<encoderChange))//check if encoder bit is high
	{ 
		currentGlobals->valueChangeFlag = currentGlobals->valueChangeFlag&(0xFF&(0<<encoderChange));//set encoder bit low, and carry our whatever encoder change has occurred. 
		//we need to debug this to make sure it's doing what we think it's doing.
		switch(currentGlobals->menuState)
		{
			case PreformanceModeInit: //initial state
			reInitMenuOnLoad(menuScreen, currentPattern, currentGlobals);
			outputS(menuScreen->screen0[0], 0);
			outputS(menuScreen->screen0[1], 1);
			outputS(menuScreen->screen0[2], 2);
			outputS(menuScreen->screen0[3], 3);
			break;

			case SequencerMenuInit:
			outputS(menuScreen->screen1[0], 0);
			outputS(menuScreen->screen1[1], 1);
			outputS(menuScreen->screen1[2], 2);
			outputS(menuScreen->screen1[3], 3);
			currentGlobals->menuState = SequencerMenuArrow1; 
			
			
			case SequencerMenuArrow1:
			menuScreen->screen1[1][19]= 8;
			menuScreen->screen1[2][19] = ' ';
			menuScreen->screen1[3][19] = ' ';
			outputS(menuScreen->screen1[1], 1);
			outputS(menuScreen->screen1[2], 2);
			outputS(menuScreen->screen1[3], 3);
			break;
			
			case SequencerMenuArrow1Select:
			reInitMenuOnLoad(menuScreen, currentPattern, currentGlobals);
			outputS(menuScreen->screen1[1],1);
			break;
	
	
			case SequencerMenuArrow2:
			menuScreen->screen1[1][19]= ' ';
			menuScreen->screen1[2][19] = 8;
			menuScreen->screen1[3][19] = ' ';
			outputS(menuScreen->screen1[1], 1);
			outputS(menuScreen->screen1[2], 2);
			outputS(menuScreen->screen1[3], 3);
			break;
	
			case SequencerMenuArrow2Select:
			numPrinter(menuScreen->screen1[2],7,2,currentPattern->numSteps);
			outputS(menuScreen->screen1[2],2);
			break;
	
			case SequencerMenuArrow3:
			menuScreen->screen1[1][19]= ' ';
			menuScreen->screen1[2][19] = ' ';
			menuScreen->screen1[3][19] = 8;
			outputS(menuScreen->screen1[1], 1);
			outputS(menuScreen->screen1[2], 2);
			outputS(menuScreen->screen1[3], 3);
			break;
		
			case SequencerMenuArrow3Select:
			numPrinter(menuScreen->screen1[3],14,2,(currentGlobals->currentStep)+1); //these are 0 indexed, so we need to add 1 to the display.
			outputS(menuScreen->screen1[3],3);
			break;
	

			case TrackMenuInit:
			outputS(menuScreen->screen2[0], 0);
			outputS(menuScreen->screen2[1], 1);
			outputS(menuScreen->screen2[2], 2);
			outputS(menuScreen->screen2[3], 3);
			currentGlobals->menuState = TrackMenuArrow1;
		
			case TrackMenuArrow1:
			menuScreen->screen2[1][19]= 8;
			menuScreen->screen2[2][19] = ' ';
			menuScreen->screen2[3][19] = ' ';
			outputS(menuScreen->screen2[1], 1);
			outputS(menuScreen->screen2[2], 2);
			outputS(menuScreen->screen2[3], 3);
			break;
		
			case TrackMenuArrow1Select:;
			uint16_t trackSample = (currentPattern->trackSampleMSB[currentGlobals->currentTrack]<<8)|(currentPattern->trackSampleLSB[currentGlobals->currentTrack]);
			numPrinter(menuScreen->screen2[1],10,4,(trackSample));
			outputS(menuScreen->screen2[1],1);
			break;
	
			case TrackMenuArrow2:
			menuScreen->screen2[1][19]= ' ';
			menuScreen->screen2[2][19] = 8;
			menuScreen->screen2[3][19] = ' ';
			outputS(menuScreen->screen2[1], 1);
			outputS(menuScreen->screen2[2], 2);
			outputS(menuScreen->screen2[3], 3);
			break;
		
			case TrackMenuArrow2Select:
			//we need some serious button code in these two cases. 
			switch (currentPattern->trackPlayMode[currentGlobals->currentTrack])
			{
				case 0:
				menuScreen->screen2[2][10] = 'S';
				menuScreen->screen2[2][11] = 'o';
				menuScreen->screen2[2][12] = 'l';
				menuScreen->screen2[2][13] = 'o';
				break;

				case 1:
				menuScreen->screen2[2][10] = 'P';
				menuScreen->screen2[2][11] = 'o';
				menuScreen->screen2[2][12] = 'l';
				menuScreen->screen2[2][13] = 'y';
				break;

				//these additional cases will be for loops and other stuff. have not decided on how to deal with them yet.
				case 2:
				break;
	
				case 3:
				break;
			}
			
			outputS(menuScreen->screen2[2], 2);
			break;

			case TrackMenuArrow3:
			menuScreen->screen2[1][19]= ' ';
			menuScreen->screen2[2][19] = ' ';
			menuScreen->screen2[3][19] = 8;
			outputS(menuScreen->screen2[1], 1);
			outputS(menuScreen->screen2[2], 2);
			outputS(menuScreen->screen2[3], 3);
			break;
		
			case TrackMenuArrow3Select:
			numPrinter(menuScreen->screen2[3],10,2,(currentPattern->trackOutputRoute[currentGlobals->currentTrack])+1);
			outputS(menuScreen->screen2[3],3);
			break;
			
			case TrackMenuArrow4:
			menuScreen->screen2[2][19]= ' ';
			menuScreen->screen2[3][19] = ' ';
			menuScreen->screen2[4][19] = 8;
			outputS(menuScreen->screen2[2], 1);
			outputS(menuScreen->screen2[3], 2);
			outputS(menuScreen->screen2[4], 3);
			break;
			
			case TrackMenuArrow4Select:
			switch(currentPattern->envelopeType[currentGlobals->currentTrack])
			{
				case 0: //A/R
				menuScreen->screen2[4][14] = 'A';
				menuScreen->screen2[4][15] = '-';
				menuScreen->screen2[4][16] = 'R';
				menuScreen->screen2[4][17] = ' ';
				break;
				
				case 1: //only release
				menuScreen->screen2[4][14] = 'R';
				menuScreen->screen2[4][15] = ' ';
				menuScreen->screen2[4][16] = ' ';
				menuScreen->screen2[4][17] = ' ';
				break;
				
				case 2: //only attack
				menuScreen->screen2[4][14] = 'A';
				menuScreen->screen2[4][15] = ' ';
				menuScreen->screen2[4][16] = ' ';
				menuScreen->screen2[4][17] = ' ';
				break;
				
				case 3: //No envelope
				menuScreen->screen2[4][14] = 'N';
				menuScreen->screen2[4][15] = 'o';
				menuScreen->screen2[4][16] = 'n';
				menuScreen->screen2[4][17] = 'e';
				break;
			}
			outputS(menuScreen->screen2[4],3);
			break;
			
			case TrackMenuArrow5:
			menuScreen->screen2[3][19]= ' ';
			menuScreen->screen2[4][19] = ' ';
			menuScreen->screen2[5][19] = 8;
			outputS(menuScreen->screen2[3], 1);
			outputS(menuScreen->screen2[4], 2);
			outputS(menuScreen->screen2[5], 3);
			break;

			case TrackMenuArrow5Select:;
			uint16_t totalSustainTime = currentPattern->trackSustainTimeLSB[currentGlobals->currentTrack]|((currentPattern->trackSustainTimeMSB[currentGlobals->currentTrack])<<8);
			numPrinter(menuScreen->screen2[5],13, 5, totalSustainTime);
			menuScreen->screen2[5][12] = menuScreen->screen2[5][13];
			menuScreen->screen2[5][13] = menuScreen->screen2[5][14];
			menuScreen->screen2[5][14] = '.';
			outputS(menuScreen->screen2[5],3);
			break;

			case GlobalMenuInit:
			outputS(menuScreen->screen3[0], 0);
			outputS(menuScreen->screen3[1], 1);
			outputS(menuScreen->screen3[2], 2);
			outputS(menuScreen->screen3[3], 3);
			currentGlobals->menuState = GlobalMenuArrow1;

			case GlobalMenuArrow1:
			menuScreen->screen3[1][19]= 8;
			menuScreen->screen3[2][19] = ' ';
			menuScreen->screen3[3][19] = ' ';
			outputS(menuScreen->screen3[1], 1);
			outputS(menuScreen->screen3[2], 2);
			outputS(menuScreen->screen3[3], 3);
			break;
			
			case GlobalMenuArrow1Select:
			numPrinter(menuScreen->screen3[1],14,2,(currentGlobals->midiChannel)+1);
			outputS(menuScreen->screen3[1],1);
			break;

			case GlobalMenuArrow2:
			menuScreen->screen3[1][19]= ' ';
			menuScreen->screen3[2][19] = 8;
			menuScreen->screen3[3][19] = ' ';
			outputS(menuScreen->screen3[1], 1);
			outputS(menuScreen->screen3[2], 2);
			outputS(menuScreen->screen3[3], 3);
			break;
			
			case GlobalMenuArrow2Select:
			midiNotePrinter(menuScreen->screen3[2],14,currentGlobals->midiTrackNote[currentGlobals->currentTrack]); 
			outputS(menuScreen->screen3[2],2);
			break;

			case GlobalMenuArrow3:
			menuScreen->screen3[1][19]= ' ';
			menuScreen->screen3[2][19] = ' ';
			menuScreen->screen3[3][19] = 8;
			outputS(menuScreen->screen3[1], 1);
			outputS(menuScreen->screen3[2], 2);
			outputS(menuScreen->screen3[3], 3);
			break;
		}
		prevMenuState = currentGlobals->menuState;
	}
	//We should only reach this in track selection and global settings for setting midi notes. 
	if(currentGlobals->valueChangeFlag&(1<<triggerChange))
	{
		currentGlobals->valueChangeFlag = currentGlobals->valueChangeFlag&(0<<triggerChange); //this is wrong. Will erase all of valueChange Flag. 
		switch((currentGlobals->menuState)>>4) //we don't need to worry about what the bottom encoder is doing.  
		{
			case 2:;
			uint16_t trackSample = (currentPattern->trackSampleMSB[currentGlobals->currentTrack]<<8)|(currentPattern->trackSampleLSB[currentGlobals->currentTrack]);
			numPrinter(menuScreen->screen2[1], 7, 2, (currentGlobals->currentTrack)+1);
			numPrinter(menuScreen->screen2[1], 10, 4, trackSample);
			//this feels dumb having it in two places, but It should take care of both cases. Maybe this should be a function?
					switch (currentPattern->trackPlayMode[currentGlobals->currentTrack])
					{
						case 0:
						menuScreen->screen2[2][10] = 'S';
						menuScreen->screen2[2][11] = 'o';
						menuScreen->screen2[2][12] = 'l';
						menuScreen->screen2[2][13] = 'o';
						break;

						case 1:
						menuScreen->screen2[2][10] = 'P';
						menuScreen->screen2[2][11] = 'o';
						menuScreen->screen2[2][12] = 'l';
						menuScreen->screen2[2][13] = 'y';
						break;

						//these additional cases will be for loops and other stuff. have not decided on how to deal with them yet.
						case 2:
						break;

						case 3:
						break;
					}
					
			numPrinter(menuScreen->screen2[3], 10, 2, (currentPattern->trackOutputRoute[currentGlobals->currentTrack]+1));
			switch(currentPattern->envelopeType[currentGlobals->currentTrack])
			{
				case 0: //A/R
				menuScreen->screen2[4][14] = 'A';
				menuScreen->screen2[4][15] = '-';
				menuScreen->screen2[4][16] = 'R';
				menuScreen->screen2[4][17] = ' ';
				break;
				
				case 1: //only release
				menuScreen->screen2[4][14] = 'R';
				menuScreen->screen2[4][15] = ' ';
				menuScreen->screen2[4][16] = ' ';
				menuScreen->screen2[4][17] = ' ';
				break;
				
				case 2: //only attack
				menuScreen->screen2[4][14] = 'A';
				menuScreen->screen2[4][15] = ' ';
				menuScreen->screen2[4][16] = ' ';
				menuScreen->screen2[4][17] = ' ';
				break;
				
				case 3: //No envelope
				menuScreen->screen2[4][14] = 'N';
				menuScreen->screen2[4][15] = 'o';
				menuScreen->screen2[4][16] = 'n';
				menuScreen->screen2[4][17] = 'e';
				break;
			}
		uint16_t totalSustainTime = currentPattern->trackSustainTimeLSB[currentGlobals->currentTrack]|((currentPattern->trackSustainTimeMSB[currentGlobals->currentTrack])<<8);
		numPrinter(menuScreen->screen2[5],13, 5, totalSustainTime);
		menuScreen->screen2[5][12] = menuScreen->screen2[5][13];
		menuScreen->screen2[5][13] = menuScreen->screen2[5][14];
		menuScreen->screen2[5][14] = '.';
			//the track settings screens should now be populated
			
			
			//this is a bit messy, but seems to fix bugs on this portion of the menu for now. 
			uint8_t triggerChangeScreen = 1;
			
			if(((currentGlobals->menuState)&selectBit)>35) //this accounts for menu stats 36,37,44, and 45
			{
				triggerChangeScreen = ((currentGlobals->menuState)&selectBit) - 34; //mask to get rid of encoder B pushed state. 
			}
			
			outputS(menuScreen->screen2[triggerChangeScreen], 1); 
			outputS(menuScreen->screen2[triggerChangeScreen+1], 2); 
			outputS(menuScreen->screen2[triggerChangeScreen+2], 3);
			
			break;
			
			case 3:;
			//do we need this variable?
			numPrinter(menuScreen->screen3[2],10,2,(currentGlobals->currentTrack)+1);
			midiNotePrinter(menuScreen->screen3[2],14,currentGlobals->midiTrackNote[currentGlobals->currentTrack]); 
			outputS(menuScreen->screen3[2],2);
			break;
			
		}
	}
	if(currentGlobals->valueChangeFlag&(1<<knobChange))
	{
		uint8_t positionSelect = currentGlobals->knobStatus&0x0F; //this is the bottom 4 bits, for the track location
		uint8_t positionSelectUpper = 0;
		if((currentGlobals->buttonSwitchFlag)&0x01)
		{
			positionSelectUpper = 8;
		}
		

		switch((currentGlobals->knobStatus)>>4)
		{
			case 0: //output volume
			if((currentPattern->outputLevelMSB[positionSelect])==0)
			{ //value is positive
				numPrinter(menuScreen->knobScreen[0],14,3,currentPattern->outputLevelLSB[positionSelect]); //should be a value between 0 and 8
			}else
			{
				menuScreen->knobScreen[0][14] = '-';
				menuScreen->knobScreen[0][15] = ((((currentPattern->outputLevelLSB[positionSelect]^255)+1)%100)/10)+48; //negative 8 bit numbers: flip every bit and add 1.
				menuScreen->knobScreen[0][16] = (((currentPattern->outputLevelLSB[positionSelect]^255)+1)%10)+48;
			}
				
			menuScreen->knobScreen[0][10] = positionSelect + 49;
			outputS(menuScreen->knobScreen[0], 3);
			break;
				
			case 1: //pitch
			menuScreen->knobScreen[1][5] = positionSelect+49; 
			if(currentPattern->outputPitch[positionSelect]>>7)
			{
				menuScreen->knobScreen[1][7] = '-';
				numPrinter(menuScreen->knobScreen[1], 8, 3, (currentPattern->outputPitch[positionSelect])^255);
			}else
			{
				menuScreen->knobScreen[1][7] = '+';
				numPrinter(menuScreen->knobScreen[1],8,3,currentPattern->outputPitch[positionSelect]);
			}
			outputS(menuScreen->knobScreen[1],3);
			break;
				
			case 2:; //attack envelope 
			uint16_t totalAttackTime = currentPattern->trackAttackTimeLSB[positionSelect+positionSelectUpper]|((currentPattern->trackAttackTimeMSB[positionSelect+positionSelectUpper])<<8);				
			numPrinter(menuScreen->knobScreen[2],14, 5, totalAttackTime);
			menuScreen->knobScreen[2][13] = menuScreen->knobScreen[2][14];
			menuScreen->knobScreen[2][14] = menuScreen->knobScreen[2][15];
			menuScreen->knobScreen[2][15] = '.';	
			numPrinter(menuScreen->knobScreen[2],10,2,(positionSelect+1+positionSelectUpper));
			outputS(menuScreen->knobScreen[2], 3); //This is not MS, but ideal for testing it Attack really works. 
			break;
				
			case 3:; //release envelope
			uint16_t totalReleaseTime = currentPattern->trackReleaseTimeLSB[positionSelect+positionSelectUpper]|((currentPattern->trackReleaseTimeMSB[positionSelect+positionSelectUpper])<<8);
			numPrinter(menuScreen->knobScreen[3],15, 5, totalReleaseTime);
			menuScreen->knobScreen[3][14] = menuScreen->knobScreen[3][15];
			menuScreen->knobScreen[3][15] = menuScreen->knobScreen[3][16];
			menuScreen->knobScreen[3][16] = '.';
			numPrinter(menuScreen->knobScreen[3],11,2,(positionSelect+1+positionSelectUpper));
			outputS(menuScreen->knobScreen[3], 3);
			break;
				
			case 4: //track volume
			if(currentPattern->trackMainVolumeMSB[(positionSelect+positionSelectUpper)]==0)
			{
				numPrinter(menuScreen->knobScreen[4],15, 2, currentPattern->trackMainVolumeLSB[(positionSelect+positionSelectUpper)]);
			}else
			{
				menuScreen->knobScreen[4][15] = '-';
				menuScreen->knobScreen[4][16] = ((((currentPattern->trackMainVolumeLSB[(positionSelect+positionSelectUpper)]^255)+1)%100)/10)+48; //negative 8 bit numbers: flip every bit and add 1.
				menuScreen->knobScreen[4][17] = (((currentPattern->trackMainVolumeLSB[(positionSelect+positionSelectUpper)]^255)+1)%10)+48;
			}
				 numPrinter(menuScreen->knobScreen[4],12,2,(positionSelect+positionSelectUpper+1));
				 outputS(menuScreen->knobScreen[4], 3);
			break;
			
			case 5:
			numPrinter(menuScreen->screen0[2],5,3,currentPattern->patternBPM);
			if(currentGlobals->menuState==PreformanceModeInit)
			{
				outputS(menuScreen->screen0[2],2);
			}
			break;
		}
		currentGlobals->valueChangeFlag = currentGlobals->valueChangeFlag&(0xFF&(0<<knobChange));			
	}
}