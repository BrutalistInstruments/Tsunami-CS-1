/*
 * EncoderLib.h
 *
 * Created: 7/31/2019 4:57:43 PM
 *  Author: Hal
 */ 


#ifndef ENCODERLIB_H_
#define ENCODERLIB_H_

void initEncoders();
void listenEncoders(Pattern *currentPattern, Globals *currentGlobals);
uint8_t listenEnoderReset();



#endif /* ENCODERLIB_H_ */