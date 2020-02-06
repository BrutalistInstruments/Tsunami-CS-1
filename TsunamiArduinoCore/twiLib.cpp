

//based on the avr twi library from Peter Fleury
#include <avr/io.h>
#include "globalVariables.h"
#include <util/delay.h>
//#include <util/twi.h> this has most of the definitions in it I have been making up


#define SCL_CLOCK 4000L // I2C clock in Hz
#define TW_STATUS TWSR //define for Two wire status Register
#define TW_START 0x08
#define TW_REP_START 0x10
#define TW_MT_SLA_ACK 0x18 //master trasnmit SLaddress Acknowledge
#define TW_MT_SLA_NACK 0x20 //master transmit SLAdress Not Acknowledge
#define TW_MR_SLA_ACK 0x40//master recieve SLAddress Acknowledge
#define TW_MR_DATA_NACK 0x58//master recieve Data Not Acknowledge
#define TW_MT_DATA_ACK 0x28

#define EEPROMADDRESSWRITE 0b10100000 //this is only for the first 512K block. if trying to access the other block, the number will be 0b10101000
#define EEPROMADDRESSREAD 0b10100001 //this is only for the first 512K block. if trying to access the other block, the number will be 0b10101001

void twi_init()
{
TWSR = 0;
TWBR = ((F_CPU/SCL_CLOCK-16)/2);
}

uint8_t twi_start(uint8_t address)
{

  uint8_t   twst;

  	// send START condition
  	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);

  	// wait until transmission completed
  	while(!(TWCR & (1<<TWINT)));

  	// check value of TWI Status Register. Mask prescaler bits.
  	twst = TW_STATUS & 0xF8; //this needs to be fixed, tw_status refers to the status register, which we can reffer to as not a macro in real avr
  	if ( (twst != TW_START) && (twst != TW_REP_START)) return 1; //same story with TW_REP_START. check the library header for deffines also.

  	// send device address
  	TWDR = address;
  	TWCR = (1<<TWINT) | (1<<TWEN);

  	// wail until transmission completed and ACK/NACK has been received
  	while(!(TWCR & (1<<TWINT)));

  	// check value of TWI Status Register. Mask prescaler bits.
  	twst = TW_STATUS & 0xF8;
  	if ( (twst != TW_MT_SLA_ACK) && (twst != TW_MR_SLA_ACK) ) return 1; //I think this might be wrong. why are we anding both macros, and not checkking them individulally?

  	return 0;
  //
}

void twi_start_wait(uint8_t address)
{
    uint8_t   twst;


    while ( 1 )
    {
	    // send START condition
	    TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);

    	// wait until transmission completed
    	while(!(TWCR & (1<<TWINT)));

    	// check value of TWI Status Register. Mask prescaler bits.
    	twst = TW_STATUS & 0xF8;
    	if ( (twst != TW_START) && (twst != TW_REP_START)) continue;

    	// send device address
    	TWDR = address;
    	TWCR = (1<<TWINT) | (1<<TWEN);

    	// wail until transmission completed
    	while(!(TWCR & (1<<TWINT)));

    	// check value of TWI Status Register. Mask prescaler bits.
    	twst = TW_STATUS & 0xF8;
    	if ( (twst == TW_MT_SLA_NACK )||(twst ==TW_MR_DATA_NACK) )
    	{
    	    /* device busy, send stop condition to terminate write operation */
	        TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);

	        // wait until stop condition is executed and bus released
	        while(TWCR & (1<<TWSTO));

    	    continue;
    	}
    	//if( twst != TW_MT_SLA_ACK) return 1;
    	break;
     }

}/* twi_start_wait */

uint8_t twi_rep_start(uint8_t address)
{
    return twi_start( address );

}/* twi_rep_start */

void twi_stop()
{
    /* send stop condition */
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);

	// wait until stop condition is executed and bus released
	while(TWCR & (1<<TWSTO));

}/* twi_stop */

uint8_t twi_write(uint8_t data )
{
    uint8_t   twst;

	// send data to the previously addressed device
	TWDR = data;
	TWCR = (1<<TWINT) | (1<<TWEN);

	// wait until transmission completed
	while(!(TWCR & (1<<TWINT)));

	// check value of TWI Status Register. Mask prescaler bits
	twst = TW_STATUS & 0xF8;
	if( twst != TW_MT_DATA_ACK) return 1;
	return 0;

}/* twi_write */

uint8_t twi_readAck(void)
{
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
	while(!(TWCR & (1<<TWINT)));

    return TWDR;

}/* twi_readAck */

uint8_t twi_readNak(void)
{
	TWCR = (1<<TWINT) | (1<<TWEN);
	while(!(TWCR & (1<<TWINT)));

    return TWDR;

}/* twi_readNak */



void eepromSavePattern(Pattern inPattern, uint8_t patternNumber)
{
  //write buffer will have to be the size of one page, so we may need to do more than 1 page write,
  //probably 3.
  //every bank will take up 3 pages of memeory, or 384 bytes
  //for 128 of those, we should be at 49,152 bytes, and our addressable bank size is  65,535, so we should be good.
  uint16_t eepromWriteAddress = ((patternNumber-1)*384); //this is the start.


  uint8_t eepromWriteBuffer[128];
  //first write (first 128 bytes)============================/////////
  for(int i = 0; i<8; i++)//0-7 bytes
  {
    eepromWriteBuffer[i]=	inPattern.outputLevelMSB[i];
  }
  for(int i=0; i<8; i++)//8-15 bytes
  {
    eepromWriteBuffer[i+8]=inPattern.outputLevelLSB[i];
  }
  for(int i=0; i<8; i++)//16-23 bytes
  {
    eepromWriteBuffer[i+16]=inPattern.outputPitch[i];
  }
  for(int i=0; i<16; i++)//24-39 bytes
  {
    eepromWriteBuffer[i+24]=inPattern.trackOutputRoute[i];
  }
  for(int i=0; i<16; i++)//40-55 bytes
  {
    eepromWriteBuffer[i+40]=inPattern.trackMainVolumeMSB[i];
  }
  for(int i=0; i<16; i++)//56-71 bytes
  {
    eepromWriteBuffer[i+56]=inPattern.trackMainVolumeLSB[i];
  }
  for(int i=0; i<16; i++)//72-87 bytes
  {
    eepromWriteBuffer[i+72]=inPattern.trackFadeGainMSB[i];
  }
  for(int i=0; i<16; i++)//88-103 bytes
  {
    eepromWriteBuffer[i+88]=inPattern.trackFadeGainLSB[i];
  }
  for(int i=0; i<16; i++)//104-120 bytes
  {
    eepromWriteBuffer[i+104]=inPattern.trackFadeTimeMSB[i];
  }//first buffer filled

  //page write here.
  if(twi_start(EEPROMADDRESSWRITE)==1)
    {
      //Serial.print("EEprom write address error");
      //we might want to throw an error message on the LCD screen here
      }
    twi_write((eepromWriteAddress>>8)); //MSB
    twi_write((eepromWriteAddress & 0xFF));  //LSB
    for (int i = 0; i<128; i++)
    {
      twi_write(eepromWriteBuffer[i]);
    }
    twi_stop();

    _delay_ms(5);
    for(int i=0; i<16; i++)//0-15 bytes
    {
      eepromWriteBuffer[i]=inPattern.trackFadeTimeLSB[i];
    }
    for(int i=0; i<16; i++)//16-31 bytes
    {
      eepromWriteBuffer[i+16]=inPattern.trackPlayMode[i];
    }
    for(int i=0; i<16; i++)//32-47 bytes
    {
      eepromWriteBuffer[i+32]=inPattern.trackSampleMSB[i];
    }
    for(int i=0; i<16; i++)//48-63 bytes
    {
      eepromWriteBuffer[i+48]=inPattern.trackSampleLSB[i];
    }
    for(int i=0; i<16; i++)//64-79 bytes
    {
      eepromWriteBuffer[i+64]=inPattern.voiceLockFlag[i];
    }
    for(int i=0; i<16; i++)//80-96 bytes
    {
      eepromWriteBuffer[i+80]=inPattern.midiTrackNote[i];
    }
    eepromWriteBuffer[96] = (inPattern.patternBPM>>8); //MSB
    eepromWriteBuffer[97] = (inPattern.patternBPM & 0xFF);  //LSB

    eepromWriteBuffer[98] = inPattern.numSteps;

    //page write here, but with 128 bytes of offest from the first write

    if(twi_start(EEPROMADDRESSWRITE)==1)
      {
        //Serial.print("EEprom write address error");
        //we might want to throw an error message on the LCD screen here
        }
      twi_write(((eepromWriteAddress+128)>>8)); //MSB
      twi_write(((eepromWriteAddress+128) & 0xFF));  //LSB
      for (int i = 0; i<128; i++)
      {
        twi_write(eepromWriteBuffer[i]);
      }
      twi_stop();

      _delay_ms(5);

      if(twi_start(EEPROMADDRESSWRITE)==1)
        {
          //Serial.print("EEprom write address error");
          //we might want to throw an error message on the LCD screen here
          }
        twi_write(((eepromWriteAddress+256)>>8)); //MSB
        twi_write(((eepromWriteAddress+256) & 0xFF));  //LSB
        for (int i = 0; i<64; i++)
        {
          twi_write((inPattern.trackSequence[i]>>8));
          twi_write((inPattern.trackSequence[i] & 0xFF));
        }
        twi_stop();
        _delay_ms(5);

        //this should be the end of the writes.
}



Pattern eepromLoadPattern(uint8_t patternNumber)
{
  // read buffer can be the size of a pattern, we can sequentialy read more than just one page.
  Pattern returnPattern;
  uint16_t eepromReadAddress = ((patternNumber-1)*384); //this is the start.

  uint8_t patternBuffer[384]; //this will hold all of the info from the reads. We don't want to lose anything.
//start page read
  if(twi_start(EEPROMADDRESSWRITE)==1)
  {
    //Serial.print("EEprom read address error");
    }
  twi_write((eepromReadAddress>>8)); //MSB
  twi_write((eepromReadAddress & 0xFF));  //LSB
  if(twi_start(EEPROMADDRESSREAD)==1) //this may need to be a repeat start? or a start wait?
  {
    //Serial.print("EEprom read address error");
    //maybe print an error message on the LCD screen
    }

    for(int j=0; j<383; j++)
    {
      patternBuffer[j] = twi_readAck();
    }
    patternBuffer[383] = twi_readNak();
    twi_stop();

  // now we get to assign all teh numbers from this read into there respective locations in this pattern.
  for(int i = 0; i<8; i++)//0-7 bytes
  {
    returnPattern.outputLevelMSB[i] = patternBuffer[i];
  }
  for(int i=0; i<8; i++)//8-15 bytes
  {
    returnPattern.outputLevelLSB[i]=patternBuffer[i+8];
  }
  for(int i=0; i<8; i++)//16-23 bytes
  {
    returnPattern.outputPitch[i]=patternBuffer[i+16];
  }
  for(int i=0; i<16; i++)//24-39 bytes
  {
    returnPattern.trackOutputRoute[i]=patternBuffer[i+24];
  }
  for(int i=0; i<16; i++)//40-55 bytes
  {
    returnPattern.trackMainVolumeMSB[i]=patternBuffer[i+40];
  }
  for(int i=0; i<16; i++)//56-71 bytes
  {
    returnPattern.trackMainVolumeLSB[i]=patternBuffer[i+56];
  }
  for(int i=0; i<16; i++)//72-87 bytes
  {
    returnPattern.trackFadeGainMSB[i]=patternBuffer[i+72];
  }
  for(int i=0; i<16; i++)//88-103 bytes
  {
    returnPattern.trackFadeGainLSB[i]=patternBuffer[i+88];
  }
  for(int i=0; i<16; i++)//104-120 bytes
  {
    returnPattern.trackFadeTimeMSB[i]=patternBuffer[i+104];
  }


  for(int i=0; i<16; i++)//0-15 bytes //start of the read of the next block
  {
    returnPattern.trackFadeTimeLSB[i]=patternBuffer[i+128];
  }
  for(int i=0; i<16; i++)//16-31 bytes
  {
    returnPattern.trackPlayMode[i]=patternBuffer[i+144];
  }
  for(int i=0; i<16; i++)//32-47 bytes
  {
    returnPattern.trackSampleMSB[i]=patternBuffer[i+160];
  }
  for(int i=0; i<16; i++)//48-63 bytes
  {
    returnPattern.trackSampleLSB[i]=patternBuffer[i+176];
  }
  for(int i=0; i<16; i++)//64-79 bytes
  {
    returnPattern.voiceLockFlag[i]=patternBuffer[i+192];
  }
  for(int i=0; i<16; i++)//80-96 bytes
  {
    returnPattern.midiTrackNote[i]=patternBuffer[i+208];
  }
  returnPattern.patternBPM = ((patternBuffer[224]<<8)|patternBuffer[225]);

  returnPattern.numSteps=patternBuffer[226];
// now we just need to read in the sequencer

  for (int i = 0; i<64; i++)
  {
    uint16_t patternBuffIndex = 256+(i*2); //start on 3rd page,
    uint16_t msb = patternBuffer[patternBuffIndex];
    uint16_t lsb = patternBuffer[patternBuffIndex+1];
    returnPattern.trackSequence[i]=((msb<<8)|lsb);
  }
  return returnPattern;
}

void factoryResetEeprom(Pattern inPattern)
{
  for(uint8_t i = 0; i<128; i++)
  {
    eepromSavePattern(inPattern,i);
  }

}

uint8_t readEEpromChar(uint16_t readAddress)
{
  uint8_t returnNum;
  if(twi_start(EEPROMADDRESSWRITE)==1)
  {
    //Serial.print("EEprom read address error");
    }
  twi_write((readAddress>>8)); //MSB
  twi_write((readAddress & 0xFF));  //LSB
  if(twi_start(EEPROMADDRESSREAD)==1)
  {
    //Serial.print("EEprom read address error");
  }

  returnNum=twi_readNak();

  return returnNum;

}
