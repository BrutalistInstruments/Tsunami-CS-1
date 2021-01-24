// menuLib.h

#ifndef _MENULIB_h
#define _MENULIB_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "globalVariables.h"
void initMenu(volatile Screen* initTheScreen, volatile Pattern* currentPattern, volatile Globals* currentGlobals);
void updateScreen(volatile Screen* menuScreen, volatile Pattern* currentPattern, volatile Globals* currentGlobals);


#endif

