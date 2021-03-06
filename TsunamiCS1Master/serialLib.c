/*
 * serialLib.c
 *
 * Created: 8/9/2019 10:59:42 AM
 *  Author: Hal
 */
#define F_CPU 16000000UL
#define TX_BUFFER_SIZE 384 //this serial buffer is big, yes, but Every load is 272 bytes, so if a pattern is playing, it is necessary. 
#define BUADTsunami 57600
#define BRCTsunami ((F_CPU/16/BUADTsunami)-1)

//#define RX_BUFFER_SIZE 128

//buad rate: 57.6 kbps // 57600 buad - for Tsunami


#include <avr/interrupt.h>
#include <avr/io.h>

char serial0Buffer[TX_BUFFER_SIZE];
int serialReadPos = 0;
int serialWritePos = 0;

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

void appendSerial0(unsigned char c)
{
	serial0Buffer[serialWritePos] = c;
	serialWritePos++;
	if(serialWritePos >= TX_BUFFER_SIZE)
	{
		serialWritePos = 0;
	}
}

void serialWrite0(unsigned char c[], uint8_t messageLength)
{
	for (uint8_t i = 0; i<messageLength; i++) //this may need to be 11
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


UCSR0B = (1 << TXEN0)  | (1 << TXCIE0);
UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); //8 bit chars will be sent

}

/*
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
*/
