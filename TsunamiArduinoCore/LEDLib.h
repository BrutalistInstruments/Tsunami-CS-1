/*
 * LEDLib.h
 *
 * Created: 8/22/2019 8:58:27 AM
 *  Author: Hal
 */


#ifndef LEDLIB_H_
#define LEDLIB_H_

void parseLEDs(uint16_t LEDInput);
void initLEDs();
void updateLEDs(uint8_t ledMenuState, Pattern ledCurrentPattern, uint16_t ledCurrenttrigButtons, uint8_t curentStep);



#endif /* LEDLIB_H_ */
