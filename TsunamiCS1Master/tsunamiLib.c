/*
 * tsunamiLib1.c
 *
 * Created: 8/9/2019 12:18:54 PM
 *  Author: Hal
 */

#include "serialLib.h"
#include <avr/io.h>
#include "globalVariables.h"
#include <avr/interrupt.h>

//               0     1     2     3     4     5     6     7     8     9
//message format:0xf0, 0xaa, 0x0a, 0x03, 0x01, 0x11, 0x00, 0x00, 0x00, 0x55
//|StartOfMessage|StartOfMessage|BytesInMessage|MessageCode|MessageData|EndOfData|
//|      0       |        1     |      2       |     3     |     4-8   |    9    |
uint32_t releaseCounter = 0;
extern currentGlobals;

void initEnvelopes()
{
	//here we need to set up a timer to tell when the release stage needs to happen. 
	//should be in milliseconds, and should hold a 32 bit integer. We could probably store it in Globals. 
	
	//we also need an update envelopes function that checks if the timer has been reached, if the 16 bit flag has been set, and do the release stage. 
	//We'll be using timer3 for this. 
	TCCR3A = (1<<WGM32); //set timer to CTC mode (clear timer on correct value)
	//this is CTC mode 4 on data sheet page 145. We may want to use mode 12, since OCRnA may be in use from timer 0
	TCCR3B = (1<<CS31)|(1<<CS30); //64 prescaller. 
	OCR3AH = 0;
	OCR3AL = 250; //250*64 = 1,600 ->1Ms worth of clock cycles. 
	TIMSK3 = (1<<OCIE3A);
	
}

void getVersion()
{//gets the version from tsunami. may be useful in later versions,
	//to print the version on the OLED Screen.
	unsigned char version[5] = {0xf0, 0xaa, 0x05, 0x01, 0x55};
	serialWrite0(version, 5);
}

void getSysInfo()
{//requests total number of voices in firmware, and total tracks on SD Card.
	unsigned char info[5] = {0xf0, 0xaa, 0x05, 0x02, 0x55};
	serialWrite0(info, 5);
}

void setReporting(char reportBool)
{//if true, set reporting to on.
	unsigned char report[6] = {0xf0, 0xaa, 0x06, 0x0d, reportBool, 0x55};
		serialWrite0(report, 6);
}

void trackControl(char trackNumberLSB, char trackNumberMSB, char outputNumber, char trackCommand)
{
	unsigned char sendtrackCommand[10] = {0xf0, 0xaa, 0x0a, 0x03, trackCommand, trackNumberLSB, trackNumberMSB, outputNumber, 0x00, 0x55};
	serialWrite0(sendtrackCommand, 10);
}

void stopAll()
{
	unsigned char stop[5] =  {0xf0, 0xaa, 0x05, 0x04, 0x55};
	serialWrite0(stop, 5);
}

void setOutputVolume(uint8_t gainLSB, uint8_t gainMSB, uint8_t outputNumber )
{
	unsigned char outVolume[8] = {0xf0, 0xaa, 0x08, 0x05, outputNumber, gainLSB, gainMSB, 0x55};
	serialWrite0(outVolume, 8);
}


void setTrackVolume(uint8_t trackLSB, uint8_t trackMSB, uint8_t gainLSB, uint8_t gainMSB)
{//we may want to make a conversion function to convert 1 negative int into a 2 uint8_t number.
	unsigned char trackVolume[9] = {0xf0, 0xaa, 0x09, 0x08, trackLSB, trackMSB, gainLSB, gainMSB, 0x55};
	serialWrite0(trackVolume, 9);
}

void setTrackFade(uint8_t trackLSB, uint8_t trackMSB, uint8_t gainLSB, uint8_t gainMSB, uint8_t millisecondsLSB, uint8_t millisecondsMSB, uint8_t stopFlag)
{//sets envelope time, must be called directly after a play message.
//envelopes can range from 0 to 2000ms

	unsigned char trackEnvelope[12] = {0xf0, 0xaa, 0x0c, 0x0a, trackLSB, trackMSB, gainLSB, gainMSB, millisecondsLSB, millisecondsMSB, stopFlag, 0x55};
	serialWrite0(trackEnvelope, 12);

}

void resumeAll()
{
	unsigned char resumePlay[5] = {0xf0, 0xaa, 0x05, 0x0b, 0x55};
	serialWrite0(resumePlay, 5);

}

void outputSampleRate(uint8_t outputSelect, uint8_t offsetLSB, uint8_t offsetMSB)
{
	unsigned char pitchChange[8] = {0xf0, 0xaa, 0x08, 0x0c, outputSelect, offsetLSB, offsetMSB, 0x55};
	serialWrite0(pitchChange, 8);
}

void setInPutMix(uint8_t outputMask)
{
	unsigned char inMix[6] = {0xf0, 0xaa, 0x06, 0x0f, outputMask, 0x55};
	serialWrite0(inMix,6);

}
void playTrack(Pattern *currentPattern, Globals *currentGlobals, uint8_t trigInput) //most of these params are just getting passed through.
{ //4 cases:
	
	uint16_t sustainTime = (currentPattern->trackSustainTimeLSB[trigInput])|((currentPattern->trackSustainTimeMSB[trigInput])<<8);
	
	switch(currentPattern->envelopeType[trigInput])
	{
		case 0: //A-R //not currently implemented
			setTrackVolume(currentPattern->trackSampleLSB[trigInput], currentPattern->trackSampleMSB[trigInput],255,186); //we might want to do this on load, and on option change.
			trackControl(currentPattern->trackSampleLSB[trigInput], currentPattern->trackSampleMSB[trigInput],
			currentPattern->trackOutputRoute[trigInput], currentPattern->trackPlayMode[trigInput]);
			setTrackFade(currentPattern->trackSampleLSB[trigInput], currentPattern->trackSampleMSB[trigInput],
			currentPattern->trackMainVolumeLSB[trigInput], currentPattern->trackMainVolumeMSB[trigInput],
			currentPattern->trackAttackTimeLSB[trigInput], currentPattern->trackAttackTimeMSB[trigInput], 0);
			currentGlobals->releaseTracker|=(1<<trigInput); //set tracking 
			currentGlobals->sustainCounterArray[trigInput] = currentGlobals->releaseCounter+sustainTime;
		break;
		
		case 1: //R //not currently implemented. 
		trackControl(currentPattern->trackSampleLSB[trigInput], currentPattern->trackSampleMSB[trigInput],
		currentPattern->trackOutputRoute[trigInput], currentPattern->trackPlayMode[trigInput]);
		currentGlobals->releaseTracker|=(1<<trigInput);
		currentGlobals->sustainCounterArray[trigInput] = currentGlobals->releaseCounter+sustainTime;
		break;
		
		case 2: //A 
		setTrackVolume(currentPattern->trackSampleLSB[trigInput], currentPattern->trackSampleMSB[trigInput],255,186); //we might want to do this on load, and on option change. 
		trackControl(currentPattern->trackSampleLSB[trigInput], currentPattern->trackSampleMSB[trigInput],
		currentPattern->trackOutputRoute[trigInput], currentPattern->trackPlayMode[trigInput]);
		setTrackFade(currentPattern->trackSampleLSB[trigInput], currentPattern->trackSampleMSB[trigInput],
		currentPattern->trackMainVolumeLSB[trigInput], currentPattern->trackMainVolumeMSB[trigInput],
		currentPattern->trackAttackTimeLSB[trigInput], currentPattern->trackAttackTimeMSB[trigInput], 0);
		break;
		
		case 3: //none
		trackControl(currentPattern->trackSampleLSB[trigInput], currentPattern->trackSampleMSB[trigInput],
		currentPattern->trackOutputRoute[trigInput], currentPattern->trackPlayMode[trigInput]);
		break;
	}
}

void sendPatternOnLoad(Pattern *currentPattern, Pattern oldPattern)
{
	for(int i=0; i<8; i++)
	{
		//if(currentPattern->outputLevelLSB[i]!=oldPattern.outputLevelLSB[i]) //if the volume does not change from pattern to pattern, no need to send serial data. 
		//{//this should speed up the physical time this method takes significantly.
		//set output volume here
		setOutputVolume(currentPattern->outputLevelLSB[i], currentPattern->outputLevelMSB[i], i);
		//}
	}
	for(int i=0; i<8; i++)
	{
		//if(currentPattern->outputPitch[i]!=oldPattern.outputPitch[i])
		//{
			//set pitch volume
			outputSampleRate(i,0,currentPattern->outputPitch[i]);
		//}
	}
	for(int i=0; i<16; i++)
	{	//not really a good way to do a redundancy check here, since tsunami indexes is track volumes per sample 
		//i.e., it stores 4096 sample volumes per session. 
		//we actually could maybe send all of those on startup? and then never have to send them unless there is a change?
		//this should be fine for now though. 
			//set track volume
			setTrackVolume(currentPattern->trackSampleLSB[i], currentPattern->trackSampleMSB[i], currentPattern->trackMainVolumeLSB[i], currentPattern->trackMainVolumeMSB[i]);
	}
	
}

void releaseUpdate(Pattern *currentPattern, Globals *currentGlobals)
{
	
	
}