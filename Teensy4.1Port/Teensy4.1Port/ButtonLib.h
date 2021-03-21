// ButtonLib.h

#ifndef _BUTTONLIB_h
#define _BUTTONLIB_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "globalVariables.h"

#define buttonLatch 33
#define buttonData 34
#define buttonClock 35

void initButtons();
void listenButton(volatile Globals* buttonGlobals);


#endif

