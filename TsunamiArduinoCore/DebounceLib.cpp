/*
 * DebounceLib.c
this library is based on the AVR freaks debounce tutorial:
 https://www.avrfreaks.net/sites/default/files/forum_attachments/debounce.pdf
 
 
*/ 

#include "debounceLib.h"

volatile uint8_t buttons_down;
uint8_t button_down(uint8_t button_mask)
{
	//I should probably check out what this is.
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		button_mask &= buttons_down;
		buttons_down ^= button_mask;
	}
	
	return button_mask;
}