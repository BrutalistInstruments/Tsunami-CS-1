#ifndef EEPROMLIB_H_
#define EEPROMLIB_H_

void initEeprom();
void writeEeprom(int deviceaddress, unsigned int eeaddress, uint8_t data );
uint8_t readEeprom(int deviceaddress, unsigned int eeaddress);
void pageWriteEeprom(int deviceaddress, unsigned int eeaddress, char* data);
void writePattern(Pattern sendPattern, uint8_t toWritePattern);
void readPattern(Pattern internalPattern, uint8_t eepromPattern);



#endif
