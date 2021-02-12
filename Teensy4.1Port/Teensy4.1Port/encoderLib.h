// encoderLib.h

#ifndef _ENCODERLIB_h
#define _ENCODERLIB_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "globalVariables.h"

void initEncoders();
void listenEncoders(volatile Globals* encoderGlobals);

#endif
