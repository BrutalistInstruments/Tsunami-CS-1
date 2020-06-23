/*
 * twiLib.h
 *
 * Created: 12/23/2019 1:01:17 PM
 *  Author: Hal
 * this library servers mainly to interact with with eeprom.
 */
#include <avr/io.h>
#include "globalVariables.h"
#define SCL_CLOCK 100000 // I2C clock in Hz - 100KHz
#define TW_STATUS TWSR //define for Two wire status Register
#define TW_START 0x08
#define TW_REP_START 0x10
#define TW_MT_SLA_ACK 0x18 //master transmit SLaddress Acknowledge
#define TW_MT_SLA_NACK 0x20 //master transmit SLAdress Not Acknowledge
#define TW_MR_SLA_ACK 0x40//master receive SLAddress Acknowledge
#define TW_MR_DATA_NACK 0x58//master receive Data Not Acknowledge
#define TW_MT_DATA_ACK 0x28

#define EEPROMADDRESSWRITEA 0b10100000 //512K bit block A 
#define EEPROMADDRESSWRITEB	0b10101000 //512K bit block B
#define EEPROMADDRESSREADA 0b10100001 
#define EEPROMADDRESSREADB 0b10101001
#define F_CPU 16000000UL


#ifndef TWILIB_H_
#define TWILIB_H_


void twi_init();
uint8_t twi_start(uint8_t address);
void twi_start_wait(uint8_t address);
uint8_t twi_rep_start(uint8_t address);
void twi_stop();
uint8_t twi_write(uint8_t data );
uint8_t twi_readAck();
uint8_t twi_readNak();
void eepromSavePattern(Pattern inPattern, uint8_t patternNumber);
void eepromLoadPattern(Pattern *currentPattern, uint8_t patternNumber);
void factoryResetEeprom(Pattern inPattern);
uint8_t readEEpromChar(uint16_t readAddress);
void globalWrite(Globals *currentGlobals);
void globalLoad(Globals *currentGlobals, uint8_t factoryReset);



#endif /* TWILIB_H_ */