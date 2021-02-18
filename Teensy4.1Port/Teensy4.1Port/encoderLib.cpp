// 
// 
// 

#include "encoderLib.h"
#include "globalVariables.h"

volatile uint8_t prevNextCodeTop = 0;
volatile uint8_t storeTop = 0;
volatile uint8_t prevNextCodeBottom = 0;
volatile uint8_t storeBottom = 0;
static int8_t rotEncTable[] = {0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0};
//volatile uint8_t topEncoderValue;
//volatile uint8_t bottomEncoderValue;
uint8_t topEncoderLastValue = 0;
uint8_t bottomEncoderLastValue = 0;

void initEncoders() 
{
	pinMode(2, INPUT_PULLUP);
	pinMode(3, INPUT_PULLUP);
	pinMode(4, INPUT_PULLUP);
	pinMode(5, INPUT_PULLUP);

}

void listenEncoders(volatile Globals *encoderGlobals) 
{
	//we'll be calling this every millisecond. 
	//still need to implement bottom encoder. 
	prevNextCodeTop = prevNextCodeTop << 1;
	prevNextCodeTop |= digitalReadFast(3);
	prevNextCodeTop = prevNextCodeTop << 1;
	prevNextCodeTop |= digitalReadFast(2);
	prevNextCodeTop &= 0x0F;

	prevNextCodeBottom = prevNextCodeBottom << 1;
	prevNextCodeBottom |= digitalReadFast(5);
	prevNextCodeBottom = prevNextCodeBottom << 1;
	prevNextCodeBottom |= digitalReadFast(4);
	prevNextCodeBottom &= 0x0F;



	if (rotEncTable[prevNextCodeTop])
	{
		storeTop = storeTop << 4;
		storeTop |= prevNextCodeTop;
		if ((storeTop&0xFF)==0x2B) 
		{
			uint8_t currentTopEncoderMenu = (encoderGlobals->menuState)&0b01110000;
			currentTopEncoderMenu = currentTopEncoderMenu >> 4; 
			currentTopEncoderMenu++;
			if (currentTopEncoderMenu > 3) 
			{
				currentTopEncoderMenu = 0;
			}
			currentTopEncoderMenu = currentTopEncoderMenu << 4;
			currentTopEncoderMenu = currentTopEncoderMenu & 0b01110000; //stay in range, we don't want to trigger a click
			encoderGlobals->menuState = currentTopEncoderMenu; //assign the new menustate
			encoderGlobals->valueChangeFlag |= (1 << encoderChange);
		}
		if ((storeTop & 0xFF) == 0x17) 
		{
			uint8_t currentTopEncoderMenu = (encoderGlobals->menuState) & 0b01110000;
			currentTopEncoderMenu = currentTopEncoderMenu >> 4;
			if (currentTopEncoderMenu == 0) 
			{
				currentTopEncoderMenu = 4; //set this to 1 higher than the last menu option. 
			}
			currentTopEncoderMenu--;
			currentTopEncoderMenu = currentTopEncoderMenu << 4;
			currentTopEncoderMenu = currentTopEncoderMenu & 0b01110000; //stay in range, we don't want to trigger a click
			encoderGlobals->menuState = currentTopEncoderMenu; //assign the new menustate
			encoderGlobals->valueChangeFlag |= (1 << encoderChange);
		}
	}

	if (rotEncTable[prevNextCodeBottom])
	{
		storeBottom = storeBottom << 4;
		storeBottom |= prevNextCodeBottom; //This whole thing should probably be handled differently
		if ((storeTop & 0xFF) == 0x2B)
		{
			uint8_t currentBottomEncoderMenu = (encoderGlobals->menuState) & 0b00000111; //we only use the bottom 3 bits to make menu changes;
			currentBottomEncoderMenu++;
			currentBottomEncoderMenu = currentBottomEncoderMenu & 0b00000111; 
			encoderGlobals->menuState = (encoderGlobals->menuState & 0b11111000) | currentBottomEncoderMenu;
			encoderGlobals->valueChangeFlag |= (1 << encoderChange); 
		}
		if ((storeTop & 0xFF) == 0x17)
		{
			uint8_t currentBottomEncoderMenu = (encoderGlobals->menuState) & 0b00000111; //we only use the bottom 3 bits to make menu changes;
			currentBottomEncoderMenu--;
			currentBottomEncoderMenu = currentBottomEncoderMenu & 0b00000111;
			encoderGlobals->menuState = (encoderGlobals->menuState & 0b11111000) | currentBottomEncoderMenu;
			encoderGlobals->valueChangeFlag |= (1 << encoderChange);
		}
	}
}