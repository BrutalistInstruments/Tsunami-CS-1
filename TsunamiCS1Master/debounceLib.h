/*
 * DebounceLib.h
 *
 * Created: 9/6/2019 8:15:54 PM
 *  Author: user
 */ 


#ifndef DEBOUNCELIB_H_
#define DEBOUNCELIB_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

#define BUTTON_PIN PINB
#define BUTTON_PORT PORTB
#define BUTTON_DDR DDRB
#define BUTTON_MASK 0B01111111


extern volatile uint8_t buttons_down;
uint8_t button_down(uint8_t button_mask);
void debounce_init();

#define VC_DEC_OR_SET(high, low, mask) \
low = ~(low & mask); \
high = low ^ (high & mask)

static inline void debounce()
{
	static uint8_t vcount_low = 0xFF, vcount_high = 0xFF;
	static uint8_t button_state = 0;
	uint8_t state_changed = ~BUTTON_PIN ^ button_state;
	VC_DEC_OR_SET(vcount_high, vcount_low, state_changed);
	state_changed &= vcount_low & vcount_high;
	button_state ^= state_changed;
	buttons_down |= button_state&state_changed;
}
#endif /* DEBOUNCELIB_H_ */