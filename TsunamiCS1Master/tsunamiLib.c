/*
 * tsunamiLib1.c
 *
 * Created: 8/9/2019 12:18:54 PM
 *  Author: Hal
 */

#include "serialLib.h"
#include <avr/io.h>
#include "globalVariables.h"

extern uint8_t muteVolMSB;
extern uint8_t muteVolLSB;
//maybe not best

//               0     1     2     3     4     5     6     7     8     9
//message format:0xf0, 0xaa, 0x0a, 0x03, 0x01, 0x11, 0x00, 0x00, 0x00, 0x55
//|StartOfMessage|StartOfMessage|BytesInMessage|MessageCode|MessageData|EndOfData|
//|      0       |        1     |      2       |     3     |     4-8   |    9    |



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
	//1:no envelopes/play sample as normal
	//2:only attack envelope
	//3:only release envelopes
	//4:both sides of the envelope
	//we could maybe streamline this by playing the sample in every case, before checking any if statements?
	//don't know if these would cause any more latency between the if statement.
	
	uint16_t totalAttack = currentPattern->trackAttackTimeLSB[trigInput]|((currentPattern->trackAttackTimeMSB[trigInput])<<8);
	uint16_t totalRelease = currentPattern->trackReleaseTimeLSB[trigInput]|((currentPattern->trackReleaseTimeMSB[trigInput])<<8);
	if((totalAttack==0)) //&&(totalRelease==0))
	{
		trackControl(currentPattern->trackSampleLSB[trigInput], currentPattern->trackSampleMSB[trigInput], 
			currentPattern->trackOutputRoute[trigInput], currentPattern->trackPlayMode[trigInput]);
	}
	else //if(totalRelease==0) //attack stage only
	{
		setTrackVolume(currentPattern->trackSampleLSB[trigInput], currentPattern->trackSampleMSB[trigInput],255,186);
				trackControl(currentPattern->trackSampleLSB[trigInput], currentPattern->trackSampleMSB[trigInput],
				currentPattern->trackOutputRoute[trigInput], currentPattern->trackPlayMode[trigInput]);
		setTrackFade(currentPattern->trackSampleLSB[trigInput], currentPattern->trackSampleMSB[trigInput], 
		currentPattern->trackMainVolumeLSB[trigInput], currentPattern->trackMainVolumeMSB[trigInput], 
		currentPattern->trackAttackTimeLSB[trigInput], currentPattern->trackAttackTimeMSB[trigInput], 0);
	}
	/*
	else if(totalAttack==0)//release stage only
	{
		trackControl(trackNumberLSB, trackNumberMSB, outputNumber, trackCommand);
		//do release section stuff
		//we need to set the release time here, and set the release bit in whatever is calling the trigger.
		
		
	}
	else
	{
		trackControl(trackNumberLSB, trackNumberMSB, outputNumber, trackCommand);
		setTrackFade(trackNumberLSB, trackNumberMSB, sustainLSB, sustainMSB, attackLSB, attackMSB, 0);
		//do release section stuff
		
		
	}
	*/
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