/*
 * tsunamiLib1.c
 *
 * Created: 8/9/2019 12:18:54 PM
 *  Author: Hal
 */ 

#include "serialLib.h"
#include <avr/io.h>

//               0     1     2     3     4     5     6     7     8     9    
//message format:0xf0, 0xaa, 0x0a, 0x03, 0x01, 0x11, 0x00, 0x00, 0x00, 0x55
//|StartOfMessage|StartOfMessage|BytesInMessage|MessageCode|MessageData|EndOfData| 
//|      0       |        1     |      2       |     3     |     4-8   |    9    |



void getVersion()
{//gets the version from tsunami. may be useful in later versions, 
	//to print the version on the OLED Screen.
	char version[5] = {0xf0, 0xaa, 0x05, 0x01, 0x55};
	serialWrite0(version);
}

void getSysInfo()
{//requests total number of voices in firmware, and total tracks on SD Card.
	char info[5] = {0xf0, 0xaa, 0x05, 0x02, 0x55};
	serialWrite0(info);
}

void setReporting(char reportBool)
{//if true, set reporting to on.
	char report[6] = {0xf0, 0xaa, 0x06, 0x0d, reportBool, 0x55};
		serialWrite0(report);
}

void trackControl(char trackNumberLSB, char trackNumberMSB, char outputNumber, char trackCommand)
{
	char sendtrackCommand[10] = {0xf0, 0xaa, 0x0a, 0x03, trackCommand, trackNumberLSB, trackNumberMSB, outputNumber, 0x00, 0x55};
	serialWrite0(sendtrackCommand);
}

void stopAll()
{
	char stop[5] =  {0xf0, 0xaa, 0x05, 0x04, 0x55};
	serialWrite0(stop);
}

void setOutputVolume(uint8_t gainLSB, uint8_t gainMSB, uint8_t outputNumber )
{
	char outVolume[8] = {0xf0, 0xaa, 0x08, 0x05, outputNumber, gainLSB, gainMSB, 0x55};
	serialWrite0(outVolume);
}


void setTrackVolume(uint8_t trackLSB, uint8_t trackMSB, uint8_t gainLSB, uint8_t gainMSB)
{//we may want to make a conversion function to convert 1 negative int into a 2 uint8_t number.
	char trackVolume[9] = {0xf0, 0xaa, 0x09, 0x08, trackLSB, trackMSB, gainLSB, gainMSB, 0x55};
	serialWrite0(trackVolume);



}

void setTrackFade(uint8_t trackLSB, uint8_t trackMSB, uint8_t gainLSB, uint8_t gainMSB, uint8_t millisecondsLSB, uint8_t millisecondsMSB, uint8_t stopFlag) 
{//sets envelope time, must be called directly after a play message.
//envelopes can range from 0 to 2000ms

	char trackEnvelope[12] = {0xf0, 0xaa, 0x0c, 0x0a, trackLSB, trackMSB, gainLSB, gainMSB, millisecondsLSB, millisecondsMSB, stopFlag, 0x55};
	serialWrite0(trackEnvelope);

}

void resumeAll()
{
	char resumePlay[5] = {0xf0, 0xaa, 0x05, 0x0b, 0x55};
	serialWrite0(resumePlay);
	
}

void outputSampleRate(uint8_t outputSelect, uint8_t offsetLSB, uint8_t offsetMSB)
{
	char pitchChange[8] = {0xf0, 0xaa, 0x08, 0x0c, outputSelect, offsetLSB, offsetMSB, 0x55};
	serialWrite0(pitchChange);
}

void setInPutMix(uint8_t outputMask)
{
	char inMix[6] = {0xf0, 0xaa, 0x06, 0x0f, outputMask, 0x55};
	serialWrite0(inMix);

}