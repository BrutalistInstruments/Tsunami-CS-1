/*
 * knobLib.h
 *
 * Created: 8/11/2019 8:00:34 AM
 *  Author: Hal
 */ 


#ifndef KNOBLIB_H_
#define KNOBLIB_H_

void selectKnob(uint8_t select);
void updateKnob(uint8_t select);
void interperetKnob(uint8_t select);
void startADCConversion();
void initADC();
void listenKnobs();





#endif /* KNOBLIB_H_ */