/*
 * tsunamiLib.h
 *
 * Created: 8/9/2019 12:18:42 PM
 *  Author: Hal
 */

#include <avr/io.h>
#include "globalVariables.h"
#ifndef TSUNAMILIB_H_
#define TSUNAMILIB_H_

void initEnvelopes();
void getVersion();
void getSysInfo();
void setReporting(char reportBool);
void trackControl(char trackNumberLSB, char trackNumberMSB, char outputNumber, char trackCommand);
void stopAll();
void setOutputVolume(uint8_t gainLSB, uint8_t gainMSB, uint8_t outputNumber);
void setTrackVolume(uint8_t trackLSB, uint8_t trackMSB, uint8_t gainLSB, uint8_t gainMSB);
void setTrackFade(uint8_t trackLSB, uint8_t trackMSB, uint8_t gainLSB, uint8_t gainMSB, uint8_t millisecondsLSB, uint8_t millisecondsMSB, uint8_t stopFlag);
void resumeAll();
void outputSampleRate(uint8_t outputSelect, uint8_t offsetLSB, uint8_t offsetMSB);
void setInPutMix(uint8_t outputMask);
void playTrack(Pattern *currentPattern, Globals *currentGlobals, uint8_t trigInput);
void sendPatternOnLoad(Pattern *currentPattern, Pattern oldPattern); 
void releaseUpdate(Pattern *currentPattern, Globals *currentGlobals);
#endif /* TSUNAMILIB_H_ */
