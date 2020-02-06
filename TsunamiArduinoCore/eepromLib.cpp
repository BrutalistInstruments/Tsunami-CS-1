#include <Arduino.h>
#include <Wire.h>

#include "globalVariables.h"

#define EEPROMADDRESS 0x50

void initEeprom()
{
  Wire.begin();
}

void writeEeprom(int deviceaddress, unsigned int eeaddress, uint8_t data )
{
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8));   // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.write(data);
  Wire.endTransmission();
}

uint8_t readEeprom(int deviceaddress, unsigned int eeaddress)
{
  byte rdata = 0xFF;

  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8));   // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.endTransmission();

  Wire.requestFrom(deviceaddress,1);

  if (Wire.available()) rdata = Wire.read();

  return rdata;

}

//these functions are mainly here as templates. we don't really need them if space becomes an issue.
void pageWriteEeprom(int deviceaddress, unsigned int eeaddress, char* data)
{
  // Uses Page Write for 24LC256
 // Allows for 64 byte page boundary - OURs has a 128 Byte page Boundry
 // Splits string into max 16 byte writes
 unsigned char i=0, counter=0;
 unsigned int  address;
 unsigned int  page_space;
 unsigned int  page=0;
 unsigned int  num_writes;
 unsigned int  data_len=0;
 unsigned char first_write_size;
 unsigned char last_write_size;
 unsigned char write_size;

 // Calculate length of data
 do{ data_len++; } while(data[data_len]); // we know the length of our data

 // Calculate space available in first page
 page_space = int(((eeaddress/64) + 1)*64)-eeaddress; // we should know this too

 // Calculate first write size
 if (page_space>16){
    first_write_size=page_space-((page_space/16)*16);
    if (first_write_size==0) first_write_size=16; // we know this, since we know our struct
 }
 else
    first_write_size=page_space;

 // calculate size of last write
 if (data_len>first_write_size)
    last_write_size = (data_len-first_write_size)%16; //what is this for?

 // Calculate how many writes we need
 if (data_len>first_write_size)
    num_writes = ((data_len-first_write_size)/16)+2;
 else
    num_writes = 1;

 i=0;
 address=eeaddress;
 for(page=0;page<num_writes;page++)
 {
    if(page==0) write_size=first_write_size;
    else if(page==(num_writes-1)) write_size=last_write_size;
    else write_size=16;

    Wire.beginTransmission(deviceaddress);
    Wire.write((int)((address) >> 8));   // MSB
    Wire.write((int)((address) & 0xFF)); // LSB
    counter=0; //so it's litterally just writing bytes and not stoping. ok.
    do{
       Wire.write((byte) data[i]);
       i++;
       counter++;
    } while((data[i]) && (counter<write_size));
    Wire.endTransmission();
    address+=write_size;   // Increment address for next write

//this should be a timer interrupt, or a millis counter. litterally anything but this.
    delay(3);  // needs 3ms for page write
 }
}

void writePattern(Pattern sendPattern, uint8_t toWritePattern)
{//we'll calculate the address needed based on pattern chosen.
//we need 3 writes, and probably to 4 pages
//yeah, it's a lot of for loops, what are you gonna do about it?
uint16_t currentAddress = (toWritePattern-1)*512; //pattern 1 = 0*512 = 0,1 will be 512
//having a page in between each pattern will leave room for expansion.
//still need to test the bounds of these numbers to make sure we're still in the same memory block.
//the defined bit might have to turn into an int, because of  block select bit

char toWrite[128];
uint8_t toWriteIndex = 0;
//write 1
for(int a=0; a<8; a++) //8
{
  toWrite[toWriteIndex] = sendPattern.outputLevelMSB[a];
  toWriteIndex++;
}
for(int b=0; b<8; b++) //16
{
  toWrite[toWriteIndex] = sendPattern.outputLevelLSB[b];
  toWriteIndex++;
}
for(int c=0; c<8; c++) //24
{
  toWrite[toWriteIndex] = sendPattern.outputPitch[c];
  toWriteIndex++;
}
for(int d=0; d<16; d++) //40
{
  toWrite[toWriteIndex] = sendPattern.trackOutputRoute[d];
  toWriteIndex++;
}
for(int e=0; e<16; e++) //56
{
  toWrite[toWriteIndex] = sendPattern.trackMainVolumeMSB[e];
  toWriteIndex++;
}
for(int f=0; f<16; f++) //72
{
  toWrite[toWriteIndex] = sendPattern.trackMainVolumeLSB[f];
  toWriteIndex++;
}
for(int g=0; g<16; g++) //88
{
  toWrite[toWriteIndex] = sendPattern.trackFadeGainMSB[g];
  toWriteIndex++;
}
for(int h=0; h<16; h++) //104
{
  toWrite[toWriteIndex] = sendPattern.trackFadeGainLSB[h];
  toWriteIndex++;
}
for(int i=0; i<16; i++) //120
{
  toWrite[toWriteIndex] = sendPattern.trackFadeTimeMSB[i];
  toWriteIndex++;
}

Wire.beginTransmission(EEPROMADDRESS);
Wire.write((int)((currentAddress) >> 8));   // MSB
Wire.write((int)((currentAddress) & 0xFF)); // LSB
for(int j=0; j<128; j++)
{
  Wire.write(toWrite[j]);
}
Wire.endTransmission();
delay(3);
currentAddress = currentAddress+128;
toWriteIndex = 0;


//write 2
//we're going to do the same thing again.
for(int a=0; a<16; a++) //16
{
  toWrite[toWriteIndex] = sendPattern.trackFadeTimeLSB[a];
  toWriteIndex++;
}
for(int b=16; b<16; b++) //32
{
  toWrite[toWriteIndex] = sendPattern.trackPlayMode[b];
  toWriteIndex++;
}
for(int c=0; c<16; c++) //48
{
  toWrite[toWriteIndex] = sendPattern.trackSampleMSB[c];
  toWriteIndex++;
}
for(int d=0; d<16; d++) //64
{
  toWrite[toWriteIndex] = sendPattern.trackSampleLSB[d];
  toWriteIndex++;
}
for(int e=0; e<16; e++) //80
{
  toWrite[toWriteIndex] = sendPattern.voiceLockFlag[e];
  toWriteIndex++;
}
for(int f=0; f<16; f++) //96
{
  toWrite[toWriteIndex] = sendPattern.midiTrackNote[f];
  toWriteIndex++;
}
toWrite[toWriteIndex] = (sendPattern.patternBPM)>>8;
toWriteIndex++;

toWrite[toWriteIndex] = (sendPattern.patternBPM)&0xFF;
toWriteIndex++;

toWrite[toWriteIndex] = sendPattern.numSteps;

Wire.beginTransmission(EEPROMADDRESS);
Wire.write((int)((currentAddress) >> 8));   // MSB
Wire.write((int)((currentAddress) & 0xFF)); // LSB
for(int j=0; j<128; j++)
{
  Wire.write(toWrite[j]);
}
Wire.endTransmission();
delay(3);
currentAddress = currentAddress+128;
toWriteIndex = 0;

//increment address
//write 3
Wire.beginTransmission(EEPROMADDRESS);
Wire.write((int)((currentAddress) >> 8));   // MSB
Wire.write((int)((currentAddress) & 0xFF)); // LSB
for(int i=0; i<128; i++)
{
  //this should divide the 16 bit number into two 8 bit numbers.
  if(i%1==0){ //we don't ~~reeeeeaally need a write buffer for this.
    Wire.write(sendPattern.trackSequence[(i/2)]>>8); //MSB
  }else
  {
    Wire.write(sendPattern.trackSequence[(i/2)] & 0xFF); //LSB
  }
  //no need to increment address, we'll do that on the next write when we calculate the address.
}
Wire.endTransmission();
delay(3); //delay 3ms for
//now we need to write the array.
}

void readPattern(Pattern internalPattern, uint8_t eepromPattern)
{
  uint16_t toReadIndex = (eepromPattern-1)*512;

  for(int a=0; a<8; a++) //8
  {
    internalPattern.outputLevelMSB[a]= readEeprom(EEPROMADDRESS, toReadIndex);
    toReadIndex++;
  }
  for(int b=0; b<8; b++) //16
  {
    internalPattern.outputLevelLSB[b] = readEeprom(EEPROMADDRESS, toReadIndex);
    toReadIndex++;
  }
  for(int c=0; c<8; c++) //24
  {
    internalPattern.outputPitch[c] = readEeprom(EEPROMADDRESS, toReadIndex);
    toReadIndex++;
  }
  for(int d=0; d<16; d++) //40
  {
      internalPattern.trackOutputRoute[d] = readEeprom(EEPROMADDRESS, toReadIndex);
    toReadIndex++;
  }
  for(int e=0; e<16; e++) //56
  {
      internalPattern.trackMainVolumeMSB[e] = readEeprom(EEPROMADDRESS, toReadIndex);
    toReadIndex++;
  }
  for(int f=0; f<16; f++) //72
  {
      internalPattern.trackMainVolumeLSB[f] = readEeprom(EEPROMADDRESS, toReadIndex);
    toReadIndex++;
  }
  for(int g=0; g<16; g++) //88
  {
      internalPattern.trackFadeGainMSB[g] = readEeprom(EEPROMADDRESS, toReadIndex);
    toReadIndex++;
  }
  for(int h=0; h<16; h++) //104
  {
      internalPattern.trackFadeGainLSB[h] = readEeprom(EEPROMADDRESS, toReadIndex);
    toReadIndex++;
  }
  for(int i=0; i<16; i++) //120
  {
      internalPattern.trackFadeTimeMSB[i] = readEeprom(EEPROMADDRESS, toReadIndex);
    toReadIndex++;
  }

  toReadIndex = toReadIndex+9; //to Read index back to 0 of next page


  //read 2
  //we're going to do the same thing again.
  for(int a=0; a<16; a++) //16
  {
      internalPattern.trackFadeTimeLSB[a] = readEeprom(EEPROMADDRESS, toReadIndex);
    toReadIndex++;
  }
  for(int b=16; b<16; b++) //32
  {
      internalPattern.trackPlayMode[b] = readEeprom(EEPROMADDRESS, toReadIndex);
    toReadIndex++;
  }
  for(int c=0; c<16; c++) //48
  {
      internalPattern.trackSampleMSB[c] = readEeprom(EEPROMADDRESS, toReadIndex);
    toReadIndex++;
  }
  for(int d=0; d<16; d++) //64
  {
      internalPattern.trackSampleLSB[d] = readEeprom(EEPROMADDRESS, toReadIndex);
    toReadIndex++;
  }
  for(int e=0; e<16; e++) //80
  {
      internalPattern.voiceLockFlag[e] = readEeprom(EEPROMADDRESS, toReadIndex);
    toReadIndex++;
  }
  for(int f=0; f<16; f++) //96
  {
      internalPattern.midiTrackNote[f] = readEeprom(EEPROMADDRESS, toReadIndex);
    toReadIndex++;
  }

    (internalPattern.patternBPM) = readEeprom(EEPROMADDRESS, toReadIndex)<<8;
  toReadIndex++; //97

    (internalPattern.patternBPM) |= readEeprom(EEPROMADDRESS, toReadIndex);
  toReadIndex++; //98

    internalPattern.numSteps = readEeprom(EEPROMADDRESS, toReadIndex);
    toReadIndex++; //99

  toReadIndex = toReadIndex+30; //99+30 = 129, or 0 n next page.

  //last read, for sequencer
  for(int i=0; i<128; i++)
  {
    //this should divide the 16 bit number into two 8 bit numbers.
    if(i%1==0){ //we don't ~~reeeeeaally need a write buffer for this.
      internalPattern.trackSequence[(i/2)] = readEeprom(EEPROMADDRESS, toReadIndex)<<8; //MSB
    }else
    {
      internalPattern.trackSequence[(i/2)] |= readEeprom(EEPROMADDRESS, toReadIndex); //LSB
    }
    toReadIndex++;
    //no need to increment address, we'll do that on the next write when we calculate the address.
  }

  //that should be it!

}
