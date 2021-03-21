// 
// 
// 

#include "ButtonLib.h"
#include "globalVariables.h"

volatile uint8_t clockState = 0;
volatile uint8_t clockCounter = 0;
volatile uint32_t rawButtoninput = 0;
volatile uint32_t previousRawButtonRead = 0;

#define numButtons 24
#define readTime 10
#define triggerMask 0x0000FFFF
#define GPMask 0x00FF00000


void initButtons() 
{
	pinMode(buttonClock,OUTPUT);
	pinMode(buttonLatch,OUTPUT);
	pinMode(buttonData, OUTPUT);
	digitalWriteFast(buttonLatch, LOW);

	//do we need to do anything else?
}

void listenButton(volatile Globals* buttonGlobals) 
{
	if (clockState) 
	{
		digitalWriteFast(buttonClock, LOW);
		clockState = 0;
	}
	else 
	{
		digitalWriteFast(buttonClock, HIGH);
		clockState = 1;
		clockCounter++;
	}
	if (clockCounter = readTime) 
	{
		digitalWriteFast(buttonLatch, HIGH);
		
	}
	if (clockCounter > readTime) 
	{
		//read stuff here
		rawButtoninput |= digitalReadFast(buttonData); //returns 00000001 or 00000000
		rawButtoninput = rawButtoninput << 1; //this should end up being a 24 bit number, where bit 23-16 are GP buttons and two encoder buttons, and Bits 
		//0-15 are our 16 trigger inputs. this is probably backwards.
	}

	if (clockCounter > numButtons+readTime) 
	{
		clockCounter = 0;
		digitalWriteFast(buttonLatch, LOW);
		buttonGlobals->currentTrigButtons = rawButtoninput & triggerMask;
		buttonGlobals->currentGPButtons = rawButtoninput & GPMask;
		if (rawButtoninput!=previousRawButtonRead) 
		{
			buttonGlobals->valueChangeFlag |= (1 << triggerChange);
			previousRawButtonRead = rawButtoninput;	
		}

	}
}