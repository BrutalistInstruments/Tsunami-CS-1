/*
 * MidiLib.c
 *
 * Created: 8/22/2019 1:26:07 PM
 *  Author: Hal
 */ 

#include <avr/io.h>
#include "globalVariables.h"
#include "serialLib.h"
#include "OLEDLib.h"
#include "tsunamiLib.h"

char currentMidiMessage[3] = {0, 0, 0};
char printMidiNote[20]= "Note On Message     ";
char printMidiCC[20] = "ControlChangeMessage";
char printMidiPC[20] = "ProgramChangeMessage";
char blank[20] = "--------------------";
uint8_t midiNoteCheck = 0;
uint8_t midiCCCheck = 0;
uint8_t midiPCCheck = 0;
uint8_t midiOffCheck = 0;


void listenMidi()
{
	midiNoteCheck = (0B10010000|midiChannel);
	midiCCCheck = (0B10110000|midiChannel);
	midiPCCheck = (0B11000000|midiChannel);
	midiOffCheck = (0B10000000|midiChannel);
	
	//right now, this is just for testing.
	currentMidiMessage[0] = getChar();
	//while(currentMidiMessage[0]==254)
	//{
	//	currentMidiMessage[0]=getChar(); //for now, get rid of midi clock signals
	//}
	if(currentMidiMessage[0]==midiNoteCheck)
	{
		currentMidiMessage[1] = getChar(); //this should be byte1 (note)
		currentMidiMessage[2] = getChar(); //this should be byte2 (velocity)
		//outputS(printMidiNote, 3);
		
		for(int i=0; i<16; i++)
		{
			//I don't like this forloop, but It seems like the best way to deal with this struct at the moment.
			if((currentPattern.midiTrackNote[i])==(currentMidiMessage[1]))
			trackControl(currentPattern.trackSampleLSB[i], currentPattern.trackSampleMSB[i], currentPattern.trackOutputRoute[i], currentPattern.trackPlayMode[i]);
		}
		
	}
	if(currentMidiMessage[0]==midiCCCheck)
	{
		currentMidiMessage[1] = getChar(); //this should be byte1 (control)
		currentMidiMessage[2] = getChar(); //this should be byte2 (velocity)
		outputS(printMidiCC, 3);
	
	}
	if(currentMidiMessage[0]==midiPCCheck)
	{
		currentMidiMessage[1] = getChar(); //this should be a byte1 (progChageNumber)
		outputS(printMidiPC, 3);
		
	}
	if(currentMidiMessage[0]==midiOffCheck)
	{
		currentMidiMessage[1] = getChar(); //this should be byte1 (note)
		currentMidiMessage[2] = getChar(); //this should be byte2 (velocity)
		outputS(blank, 3);
	}



}