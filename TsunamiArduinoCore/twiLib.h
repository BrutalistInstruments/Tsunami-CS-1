/*
 * twiLib.h
 *
 * Created: 12/23/2019 1:01:17 PM
 *  Author: Hal
 * this library servers mainly to interact with with eeprom.
 */
#include <avr/io.h>
#include "globalVariables.h"

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
Pattern eepromLoadPattern(uint8_t patternNumber);
void factoryResetEeprom(Pattern inPattern);
uint8_t readEEpromChar(uint16_t readAddress);



#endif /* TWILIB_H_ */
