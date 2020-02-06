#ifndef ENCODERLIB_H_
#define ENCODERLIB_H_

#include <Arduino.h>
#include <Encoder.h>
void initEncoders();
void listenEncoders(uint8_t *encoderMenustate, uint8_t *currentStep);


#endif /* ENCODERLIB_H_ */
