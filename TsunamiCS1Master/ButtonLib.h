/*
 * ButtonLib.h
 *
 * Created: 7/31/2019 4:32:17 PM
 *  Author: Hal
 */


#ifndef BUTTONLIB_H_
#define BUTTONLIB_H_

void listenTrigButtons(Pattern *buttonCurrentPattern, Globals *currentGlobals);
void initButtons();
void listenGPButtons(Pattern currentPattern, Globals *currentGlobals);



#endif /* BUTTONLIB_H_ */