/*
 * serialLib.c
 *
 * Created: 8/9/2019 10:59:42 AM
 *  Author: Hal
 */ 
#define F_CPU 16000000UL
#define TX_BUFFER_SIZE 128
#define BUADTsunami 57600
#define BAUDMidi 31250 
#define BRCTsunami ((F_CPU/16/BUADTsunami)-1)
#define BRCMidi ((F_CPU/16/BAUDMidi)-1)

#define RX_BUFFER_SIZE 128

//we'll need to chage all of these define names for when we send Midi CCs and notes, but we'll worry about that later.

//buad rate: 57.6 kbps // 57600 buad - for Tsunami


#include <avr/interrupt.h>
#include <avr/io.h>

char serial0Buffer[TX_BUFFER_SIZE];
char serial1Buffer[RX_BUFFER_SIZE];
int serialReadPos = 0;////this is a necessary global for the serial Library
int serialWritePos = 0;//this is a necessary global for the serial Library

uint8_t rxReadPosition = 0;
uint8_t rxWritePosition = 0;

ISR (USART0_TX_vect)
{
	if(serialReadPos != serialWritePos)
	{
		UDR0 = serial0Buffer[serialReadPos];
		serialReadPos++;
		
		if(serialReadPos >= TX_BUFFER_SIZE)
		{
			serialReadPos=0; //this seems wrong, I think we should be setting this to 0.
		}
	}
}

ISR (USART1_RX_vect)
{
		serial1Buffer[rxWritePosition] = UDR1;
		rxWritePosition++;
	
		if(rxWritePosition>= RX_BUFFER_SIZE)
		{
			rxWritePosition = 0; //this could cause some issues.
		}


}

void appendSerial0(char c)
{
	serial0Buffer[serialWritePos] = c;
	serialWritePos++;
	if(serialWritePos >= TX_BUFFER_SIZE)
	{
		serialWritePos = 0;
	}
}

void serialWrite0(char c[])
{
	for (uint8_t i = 0; i<10; i++) //this may need to be 11
	{
		appendSerial0(c[i]);
		
	}
	
	if(UCSR0A & (1 << UDRE0))
	{
		UDR0 = 0;
	}
}

void serialInit0()
{
UBRR0H = (BRCTsunami >> 8);
UBRR0L = BRCTsunami;

UBRR1H = (BRCMidi >> 8);
UBRR1L = BRCMidi;

UCSR0B = (1 << TXEN0)  | (1 << TXCIE0);
UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); //8 bit chars will be sent


UCSR1B = (1 << RXEN1)  | (1 << RXCIE0);
UCSR1C = (1 << UCSZ11) | (1 << UCSZ10); //8 bit chars will be sent

}

char getChar()
{
	char returnMe = '\0';
	
	if(rxReadPosition != rxWritePosition)
	{
		returnMe = serial1Buffer[rxReadPosition];
		
		rxReadPosition++;
		
		if(rxReadPosition >= RX_BUFFER_SIZE)
		{
			rxReadPosition = 0;
		}
	
	}
	return returnMe;
	
}