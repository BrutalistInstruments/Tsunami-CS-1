/*
 * serialLib.c
 *
 * Created: 8/9/2019 10:59:42 AM
 *  Author: Hal
 */ 
#define F_CPU 16000000UL
#define TX_BUFFER_SIZE 128
#define BUAD 57600
#define BRC ((F_CPU/16/BUAD)-1)

//buad rate: 57.6 kbps // 57600 buad - for Tsunami


#include <avr/interrupt.h>
#include <avr/io.h>

char serial0Buffer[TX_BUFFER_SIZE];

int serialReadPos = 0;////this is a necessary global for the serial Library
int serialWritePos = 0;//this is a necessary global for the serial Library

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
UBRR0H = (BRC >> 8);
UBRR0L = BRC;

UCSR0B = (1 << TXEN0)  | (1 << TXCIE0);
UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);


}