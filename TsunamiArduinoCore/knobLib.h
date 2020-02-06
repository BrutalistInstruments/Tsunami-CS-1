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
void interperetKnob(uint8_t select, Pattern currentKnobPattern, uint8_t knobMenuState, Screen knobScreen, uint8_t buttonSwitchFlag);
void startADCConversion();
void initADC();
void listenKnobs(Pattern currentKnobPattern, uint8_t knobMenuState, Screen knobScreen, uint8_t buttonSwitchFlag);

#endif /* KNOBLIB_H_ */
