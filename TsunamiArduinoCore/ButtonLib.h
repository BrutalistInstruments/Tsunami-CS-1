/*
 * ButtonLib.h
 *
 * Created: 7/31/2019 4:32:17 PM
 *  Author: Hal
 */


#ifndef BUTTONLIB_H_
#define BUTTONLIB_H_

void listenTrigButtons(uint8_t buttonMenuState, Pattern buttonCurrentPattern, uint16_t *buttonsCurrentTrig, Screen buttonScreen, uint8_t *buttonCurrentTrack);
void initButtons();
void listenGPButtons(uint8_t *sgpMenuState);



#endif /* BUTTONLIB_H_ */
