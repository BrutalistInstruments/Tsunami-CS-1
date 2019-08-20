/*
 * EncoderLib.h
 *
 * Created: 7/31/2019 4:57:43 PM
 *  Author: Hal
 */ 


#ifndef ENCODERLIB_H_
#define ENCODERLIB_H_

void initEncoders();
void listenEncoders();
void pollEncoder0(uint8_t *encoderC0);
void pollEncoder1(uint8_t *encoderC1);



#endif /* ENCODERLIB_H_ */