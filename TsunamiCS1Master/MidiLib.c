#include <avr/interrupt.h>
#include <avr/io.h>
#include "globalVariables.h"
#include "tsunamiLib.h"
#define F_CPU 16000000UL

#define MIDI_BAUD_RATE 31250
#define MIDI_NOTE_ON 0b1001
#define MIDI_NOTE_OFF 0b1000
#define MIDI_EVENT_BUFFER_SIZE 10 //we might want to make this bigger, since we will be receiving a lot of notes.
extern uint8_t midiChannel;

volatile typedef struct {
  uint8_t statusByte;
  uint8_t dataByte[2];
} midiEvent;

midiEvent midiEventBuffer[MIDI_EVENT_BUFFER_SIZE];
volatile uint8_t midiWriteIndex = 0;
volatile uint8_t midiWriteFlag = 255;
uint8_t midiReadIndex=0;

ISR (USART3_RX_vect) // interrupt service routine called each time USART data is received
{
  uint8_t data = UDR3;  // receives data from USART data register
  uint8_t midiMessageType = (data >> 4);
  if ( midiMessageType == MIDI_NOTE_ON || midiMessageType == MIDI_NOTE_OFF )  // if we receive a midi note on message or a midi note off message, begin writing to the midi buffer
  {
    midiEventBuffer[midiWriteIndex].statusByte = data;  // write the status byte to the current midi write index
    midiWriteFlag = 0;  // set the writing flag to begin writing data bytes
  } else if ( midiWriteFlag < 1 ) // we're only concerned with 2 data bytes, but if we were accepting messages of different lengths this value would have to be determined dynamically in the previous block
  {
    midiEventBuffer[midiWriteIndex].dataByte[midiWriteFlag] = data; // write data bytes to current midi write index
    midiWriteFlag++;  // increment the midi writing flag in order to write the next data byte
  } else if ( midiWriteFlag < 255 )
  {
    midiEventBuffer[midiWriteIndex].dataByte[midiWriteFlag] = data; // write data bytes to current midi write index

    uint8_t currentStatusByte = midiEventBuffer[midiWriteIndex].statusByte;
    midiWriteIndex = (midiWriteIndex + 1) % MIDI_EVENT_BUFFER_SIZE; // increment the midi write index since we've completed a midi event, use modulo operator for circular buffer

    midiEventBuffer[midiWriteIndex].statusByte = currentStatusByte;  // write the next midi event's status byte in case of running status
    midiWriteFlag = 0;  // set the writing flag to begin writing data bytes in case of running status
  }
}

void initMidi()
{
  uint16_t UBRR = (F_CPU / 16 / MIDI_BAUD_RATE) -1;
  UBRR3H = (uint8_t) (UBRR >> 8);
  UBRR3L = (uint8_t) (UBRR);

  UCSR3B |= 0b10011000; //(1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0); // configuring USART (RXEN = receiver enable, TXEN = transmitter enable, RXCIE = receiver interrupt enable

  UCSR3C |=  0b00000110; ////(1 << USCZ11) | (1 << USCZ10); // for some reasone USCZ11 and USCZ10 are not recognized
}

void midiTransmit(uint8_t data)
{
  while (!(UCSR3A & (1 << UDRE3)));
  UDR3 = data;
}

void midiRead(Pattern currentPattern, Globals currentGlobals)
{
  while (midiReadIndex != midiWriteIndex)
  {
    uint8_t midiMessageType = ((midiEventBuffer[midiReadIndex].statusByte)>>4);
    uint8_t midiVelocity = (midiEventBuffer[midiReadIndex].dataByte[1]);
    uint8_t midiChannelIn = ((midiEventBuffer[midiReadIndex].statusByte)&0b00001111);
    //uint8_t midiChannelRead = ((midiEventBuffer[midiReadIndex].statusByte)&00001111);
    if ((midiMessageType==MIDI_NOTE_ON)&&(midiVelocity!=0)&&currentGlobals.midiChannel==midiChannelIn)
    {
        for (int i=0; i<16; i++)
        {
          if(midiEventBuffer[midiReadIndex].dataByte[0]==currentGlobals.midiTrackNote[i])
          { //we don't care about velocity, at least not yet.
            //trackControl(char trackNumberLSB, char trackNumberMSB, char outputNumber, char trackCommand)
            trackControl(currentPattern.trackSampleLSB[i], currentPattern.trackSampleMSB[i], currentPattern.trackOutputRoute[i], currentPattern.trackPlayMode[i]);
          }
        }
    }
    midiReadIndex=(midiReadIndex+1)%MIDI_EVENT_BUFFER_SIZE; //we always want to increase the read index, even if our channel or message is not being used.
  }
}
