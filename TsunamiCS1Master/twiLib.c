

//based on the avr twi library from Peter Fleury
#include <avr/io.h>
#include "globalVariables.h"
#include <util/delay.h>
#include "OLEDLib.h"
#include "twiLib.h"
#include "tsunamiLib.h"
#include <avr/eeprom.h>

#define F_CPU 16000000UL

/* 
A Note about the weird delay times in this library:
For some reason, the F_CPU define above is throwing some weird errors. 
On my current compilation, compiler is complaining about it being re-defined, but also that it's undefined. (Not sure how that works...)

So, this library results to the original F_CPU definition in the delay.h library, which is 1000000, or 1Mhz. 
Since we are running at 16 times the speed, our delay times are 16x larger. 
So, instead of delaying the 5ms recommended in the data sheet, we are delaying 16*5 = 80ms. 
Which, in our terms, is really 5ms. 

This quick fix will no longer be required after we set the save and load commands to run off of timer interrupts instead of delays. 
Should happen before larger release, or soon after. 
*/



void twi_init()
{
	TWSR = 0;
	TWBR = ((F_CPU/SCL_CLOCK-16)/2); //is this line the issue?
}

uint8_t twi_start(uint8_t address)
{

	uint8_t twst;

	// send START condition
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);

	// wait until transmission completed
	while(!(TWCR & (1<<TWINT)));

	// check value of TWI Status Register. Mask pre-scaler bits.
	twst = TW_STATUS & 0xF8; 
	if ( (twst != TW_START) && (twst != TW_REP_START))
	{
		return 1;
	}  

	// send device address
	TWDR = address;
	TWCR = (1<<TWINT) | (1<<TWEN);

	// wail until transmission completed and ACK/NACK has been received
	while(!(TWCR & (1<<TWINT)));

	// check value of TWI Status Register. Mask pre-scaler bits.
	twst = TW_STATUS & 0xF8;
	if ( (twst != TW_MT_SLA_ACK) && (twst != TW_MR_SLA_ACK) )
	{
		return 1;
	}  
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

		// check value of TWI Status Register. Mask pre-scaler bits.
		twst = TW_STATUS & 0xF8;
		if ( (twst != TW_START) && (twst != TW_REP_START)) continue;

		// send device address
		TWDR = address;
		TWCR = (1<<TWINT) | (1<<TWEN);

		// wail until transmission completed
		while(!(TWCR & (1<<TWINT)));

		// check value of TWI Status Register. Mask pre-scaler bits.
		twst = TW_STATUS & 0xF8;
		if ( (twst == TW_MT_SLA_NACK )||(twst ==TW_MR_DATA_NACK) )
		{
			/* device busy, send stop condition to terminate write operation */
			TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);

			// wait until stop condition is executed and bus released
			while(TWCR & (1<<TWSTO));

			continue;
		}
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
	//write buffer will have to be the size of one page, so we will need to do 3 page writes.
	//every bank will take up 3 pages of memory, or 384 bytes
	//for 256 patterns of 384 bytes, we'll have 98,304 bytes.
	//each page on the EEprom is only 65,536 Bytes, so we need to go to the other block of the chip to have this many patterns. 
	//we'll split the blocks for now, since that should be simpler with the algorytm. 


	uint8_t eepromWriteBlock = EEPROMADDRESSWRITEA;
	uint16_t eepromWriteAddress = ((patternNumber-1)*384);
	if(patternNumber>127)
	{
		eepromWriteBlock = EEPROMADDRESSWRITEB;
		eepromWriteAddress = ((patternNumber-128)*384); //128 is our 0 index for BlockB. 
	}
	
	uint8_t eepromWriteBuffer[128];
	//first write (first 128 bytes)============================/////////
	for(int i = 0; i<8; i++)//0-7 bytes
	{
		eepromWriteBuffer[i]=inPattern.outputLevelMSB[i];
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
		eepromWriteBuffer[i+72]=inPattern.trackAttackTimeMSB[i];
	}
	for(int i=0; i<16; i++)//88-103 bytes
	{
		eepromWriteBuffer[i+88]=inPattern.trackAttackTimeLSB[i];
	}
	for(int i=0; i<16; i++)//104-120 bytes
	{
		eepromWriteBuffer[i+104]=inPattern.trackReleaseTimeMSB[i];
	}//first buffer filled
	
	eepromWriteBuffer[121] = (inPattern.patternBPM>>8); //MSB
	eepromWriteBuffer[122] = (inPattern.patternBPM & 0xFF);  //LSB

	eepromWriteBuffer[123] = (inPattern.numSteps);

	
	
	//page write here.
	if(twi_start(eepromWriteBlock)==1)
	{
		char error1[20] = "Address Write Error1";
		outputS(error1,3);
	}
	
	twi_write((eepromWriteAddress>>8)); //MSB
	twi_write((eepromWriteAddress & 0xFF));  //LSB
	
	for (int i = 0; i<123; i++)
	{
		twi_write(eepromWriteBuffer[i]);
	}
	twi_stop();

	_delay_ms(80); 
	
	for(int i=0; i<16; i++)//0-15 bytes
	{
		eepromWriteBuffer[i]=inPattern.trackReleaseTimeLSB[i];
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
	for(int i=0; i<16; i++)//80-95
	{
		eepromWriteBuffer[i+80]=inPattern.trackSustainTimeMSB[i];
	}
	for(int i=0; i<16; i++)//96-111
	{
		eepromWriteBuffer[i+96]=inPattern.trackSustainTimeLSB[i];
	}
	for(int i=0; i<16; i++)//112-127
	{
		eepromWriteBuffer[i+112]=inPattern.envelopeType[i];
	}
	//we need to move these to the previous eeprom write. 

		//page write here, but with 128 bytes of offset from the first write

		if(twi_start(eepromWriteBlock)==1)
		{
			char error1[20] = "Address Write Error2";
			outputS(error1,3);
		}
		
		twi_write(((eepromWriteAddress+128)>>8)); //MSB
		twi_write(((eepromWriteAddress+128) & 0xFF));  //LSB
		
		for (int i = 0; i<128; i++)
		{
			twi_write(eepromWriteBuffer[i]);
		}
		twi_stop();

		_delay_ms(80);


		if(twi_start(eepromWriteBlock)==1)
		{
			char error1[20] = "Address Write Error3";
			outputS(error1,3);
		}
		twi_write(((eepromWriteAddress+256)>>8)); //MSB
		twi_write(((eepromWriteAddress+256) & 0xFF));  //LSB
		
		for (int i = 0; i<64; i++)
		{
			twi_write((inPattern.trackSequence[i]>>8));
			twi_write((inPattern.trackSequence[i] & 0xFF));
		}
		twi_stop();
		_delay_ms(80);

}



void eepromLoadPattern(Pattern *currentPattern, uint8_t patternNumber)
{
	// read buffer can be the size of a pattern, we can sequentially read more than just one page.
	//Pattern returnPattern;
	uint8_t eepromReadBlock = EEPROMADDRESSREADA;
	uint8_t eepromWriteBlock = EEPROMADDRESSWRITEA;
	uint16_t eepromReadAddress = ((patternNumber-1)*384); //this is the start.
	if(patternNumber>127)
	{
		eepromReadBlock = EEPROMADDRESSREADB;
		eepromWriteBlock = EEPROMADDRESSWRITEB;
		eepromReadAddress = ((patternNumber-128)*384);
	}
	
	//We'll be hijacking this function to send relevant pattern data to the tsunami as it loads. This should (I think) give the serial buffer enough time to catch up.
	//at least a few clock cycles from the other copying happening in the loop. 
	
	uint8_t patternBuffer[384]; //this will hold all of the info from the reads. We don't want to lose anything.
	//start page read
	if(twi_start(eepromWriteBlock)==1)
	{
		outputS("Address Read Error1 ",3);
	}
	twi_write((eepromReadAddress>>8)); //MSB
	twi_write((eepromReadAddress & 0xFF));  //LSB
	
	if(twi_start(eepromReadBlock)==1) //this may need to be a repeat start? or a start wait?
	{
		char error2[20] = "Address Read Error2 ";
		outputS(error2,3);
	}

	for(int j=0; j<383; j++)
	{
		patternBuffer[j] = twi_readAck();
	}
		patternBuffer[383] = twi_readNak();
		twi_stop();

		// now we get to assign all the numbers from this read into there respective locations in this pattern.
	for(int i = 0; i<8; i++)//0-7 bytes
	{
		currentPattern->outputLevelMSB[i] = patternBuffer[i];
	}
	for(int i=0; i<8; i++)//8-15 bytes
	{
		currentPattern->outputLevelLSB[i]=patternBuffer[i+8];
		setOutputVolume(currentPattern->outputLevelLSB[i], currentPattern->outputLevelMSB[i], i);
	}
	for(int i=0; i<8; i++)//16-23 bytes
	{
		currentPattern->outputPitch[i]=patternBuffer[i+16];
		outputSampleRate(i,0,currentPattern->outputPitch[i]);
	}
	for(int i=0; i<16; i++)//24-39 bytes
	{
		currentPattern->trackOutputRoute[i]=patternBuffer[i+24];
	}
	for(int i=0; i<16; i++)//40-55 bytes
	{
		currentPattern->trackMainVolumeMSB[i]=patternBuffer[i+40];
	}
	for(int i=0; i<16; i++)//56-71 bytes
	{
		currentPattern->trackMainVolumeLSB[i]=patternBuffer[i+56];
	}
	for(int i=0; i<16; i++)//72-87 bytes
	{
		currentPattern->trackAttackTimeMSB[i]=patternBuffer[i+72];
	}
	for(int i=0; i<16; i++)//88-103 bytes
	{
		currentPattern->trackAttackTimeLSB[i]=patternBuffer[i+88];
	}
	for(int i=0; i<16; i++)//104-120 bytes
	{
		currentPattern->trackReleaseTimeMSB[i]=patternBuffer[i+104];
	}
	
			currentPattern->patternBPM = ((patternBuffer[121]<<8)|patternBuffer[122]);
			currentPattern->numSteps=patternBuffer[123];
	
	for(int i=0; i<16; i++)//0-15 bytes //start of the read of the next block
	{
		currentPattern->trackReleaseTimeLSB[i]=patternBuffer[i+128];
	}
	for(int i=0; i<16; i++)//16-31 bytes
	{
		currentPattern->trackPlayMode[i]=patternBuffer[i+144];
	}
	for(int i=0; i<16; i++)//32-47 bytes
	{
		currentPattern->trackSampleMSB[i]=patternBuffer[i+160];
	}
	for(int i=0; i<16; i++)//48-63 bytes
	{
		currentPattern->trackSampleLSB[i]=patternBuffer[i+176];
		setTrackVolume(currentPattern->trackSampleLSB[i], currentPattern->trackSampleMSB[i], currentPattern->trackMainVolumeLSB[i], currentPattern->trackMainVolumeMSB[i]);
	}
	for(int i=0; i<16; i++)//64-79 bytes
	{
		currentPattern->voiceLockFlag[i]=patternBuffer[i+192];
	}
	for(int i=0; i<16; i++)//80-95
	{
		currentPattern->trackSustainTimeMSB[i]=patternBuffer[i+208];
	}
	for(int i=0; i<16; i++)//96-111
	{
		currentPattern->trackSustainTimeLSB[i]=patternBuffer[i+224];
	}
	for(int i=0; i<16; i++)//112-127
	{
		currentPattern->envelopeType[i]=patternBuffer[i+240];
	}
							// now we just need to read in the sequencer

	for (int i = 0; i<64; i++)
	{
		uint16_t patternBuffIndex = 256+(i*2); //start on 3rd page,
		uint16_t msb = patternBuffer[patternBuffIndex];
		uint16_t lsb = patternBuffer[patternBuffIndex+1];
		currentPattern->trackSequence[i]=((msb<<8)|lsb);
	}
	
	//after this, we need to set the volume on all of the knob settings that have changed. 
	//if we don't, loading will have no effect. 
	
	//sendPatternOnLoad(currentPattern, oldPattern);
}

void factoryResetEeprom(Pattern inPattern)
{
	for(uint8_t i = 0; i<255; i++)
	{
		eepromSavePattern(inPattern,i);
		_delay_ms(80);
	}

}

uint8_t readEEpromChar(uint16_t readAddress)
{
	uint8_t returnNum;
	if(twi_start(EEPROMADDRESSWRITEA)==1)
	{
		//Serial.print("EEprom read address error");
	}
		twi_write((readAddress>>8)); //MSB
		twi_write((readAddress & 0xFF));  //LSB
		if(twi_start(EEPROMADDRESSREADA)==1)
		{
			//Serial.print("EEprom read address error");
		}

		returnNum=twi_readNak();

	return returnNum;
}

//this happens when save button is pressed on global menu. 
void globalWrite(Globals *currentGlobals)
{
	eeprom_write_byte(0,currentGlobals->midiChannel); //this may be wrong, but it does get rid of the warning. 
	eeprom_write_block(currentGlobals->midiTrackNote,1,16);
}
//This happens at startup only, in function "initGlobals"

void globalLoad(Globals *currentGlobals, uint8_t factoryReset)
{
	//we need to load all of the global midi settings here. 
	//we can also check the factory reset bit here. 
	//if the program has not gone through the initial state, and had the internal eeprom formatted, then this will return garbage. 
	if(factoryReset==0)
	{
		currentGlobals->midiChannel = eeprom_read_byte(0);
		eeprom_read_block(currentGlobals->midiTrackNote,1,16);
	}
}
