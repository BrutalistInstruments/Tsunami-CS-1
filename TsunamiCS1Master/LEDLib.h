/*
 * LEDLib.h
 *
 * Created: 8/22/2019 8:58:27 AM
 *  Author: Hal
 */


#ifndef LEDLIB_H_
#define LEDLIB_H_

void initLEDs();
void parseLEDs(uint16_t LEDInput);
void updateLEDs(Pattern ledCurrentPattern,Globals currentGlobals);



#endif /* LEDLIB_H_ */