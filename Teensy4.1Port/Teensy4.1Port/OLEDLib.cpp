// 
// 
// 

#include "OLEDLib.h"
#include "globalVariables.h"

uint8_t new_line[4] = { 0x80, 0xA0, 0xC0, 0xE0 };
uint8_t OLEDPinArray[9] = {OLEDData0,OLEDData1,OLEDData2,OLEDData3,OLEDData4,OLEDData5, OLEDData6, OLEDData7, OLEDDataCommand};

void enableCycle(volatile Globals *OLEDGlobals) //called on by interval timer.
{
	if (OLEDGlobals->OLEDBuffer[OLEDGlobals->oledReadIndex]) //if this is a non-0 value, continue with interupt. 
	{
		//we need to bring the enable pin high, then wait one micro second, then low. 
		//this interupt will be happenign every 5 to 10 microseconds. We'll keep the delay in for now, and if it negativley impacts performance, we can get rid of it. 
		uint16_t toParse = OLEDGlobals->OLEDBuffer[OLEDGlobals->oledReadIndex]; //create a pare variable, so we don't destroy the variable in the buffer. Might be unnessisary. 
		for (int i= 0; i<9;i++ ) 
		{
			digitalWriteFast(OLEDPinArray[i],(toParse&1)); //mask toParse with 1, so we just get the first bit. 
			toParse = toParse >> 1; //shift toParse down one, so we can get the next bit to write.
		}
		OLEDGlobals->OLEDBuffer[OLEDGlobals->oledReadIndex] = 0; //reset buffer to 0, now that we have shifted out the data.
		OLEDGlobals->oledReadIndex = OLEDGlobals->oledReadIndex + 1; //increment read Index

		digitalWriteFast(OLEDEnable, HIGH);
		delayMicroseconds(1);
		digitalWriteFast(OLEDEnable, LOW); //This pulse sends our data to the screen
	}

}

void command(uint8_t c, Globals *OLEDGlobals)
{
	uint16_t toBuffer; //this number will be inserted into the buffer, at current buffer index. 
	toBuffer = c; // since D/C pin is 0, we don't need to shift anything in. bit #8 is just a 0.
	OLEDGlobals->OLEDBuffer[OLEDGlobals->oledWriteIndex] = toBuffer;
	OLEDGlobals->oledWriteIndex = OLEDGlobals->oledWriteIndex + 1; //incriment write index. 
	//no need to worry about overflows, we overflow to the next part of the buffer.

}

void data(uint8_t d, Globals *OLEDGlobals)
{
	uint16_t toBuffer; //this number will be inserted into the buffer, at current buffer index. 
	toBuffer = d;
	toBuffer = toBuffer | (1 << 8); //this will be our "HIGH" message to the D/C pin
	OLEDGlobals->OLEDBuffer[OLEDGlobals->oledWriteIndex] = toBuffer;
	OLEDGlobals->oledWriteIndex = OLEDGlobals->oledWriteIndex + 1; //incriment write index. 
}

void initPins() 
{
	for (int i = 0; i < 9; i++)
	{
		pinMode(OLEDPinArray[i], OUTPUT);
	}
	pinMode(OLEDEnable, OUTPUT);

	for (int j = 0; j < 9; j++) 
	{
		digitalWriteFast(OLEDPinArray[j], LOW);
	}
	digitalWriteFast(OLEDEnable, LOW);
	delayMicroseconds(200);

}

void initScreen(volatile Globals* OLEDGlobals)
{
	//this is where we will do all of the screen
	//initialization.
	uint8_t rows = 0x08;                    // Display mode: 2/4 lines

	command((0x22 | rows),OLEDGlobals); // Function set: extended command set (RE=1), lines #
	command(0x71, OLEDGlobals);        // Function selection A:
	data(0x5C, OLEDGlobals);           //  enable internal Vdd regulator at 5V I/O mode (def. value) (0x00 for disable, 2.8V I/O)
	command((0x20 | rows), OLEDGlobals); // Function set: fundamental command set (RE=0) (exit from extended command set), lines #
	command(0x08, OLEDGlobals);        // Display ON/OFF control: display off, cursor off, blink off (default values)
	command((0x22 | rows), OLEDGlobals); // Function set: extended command set (RE=1), lines #
	command(0x79, OLEDGlobals);        // OLED characterization: OLED command set enabled (SD=1)
	command(0xD5, OLEDGlobals);        // Set display clock divide ratio/oscillator frequency:
	command(0x70, OLEDGlobals);        //  divide ratio=1, frequency=7 (default values)
	command(0x78, OLEDGlobals);        // OLED characterization: OLED command set disabled (SD=0) (exit from OLED command set)


	command(0x09, OLEDGlobals);     // Extended function set (RE=1): 5-dot font, B/W inverting disabled (def. val.), 3/4 lines

	command(0x06, OLEDGlobals);        // Entry Mode set - COM/SEG direction: COM0->COM31, SEG99->SEG0 (BDC=1, BDS=0)
	command(0x72, OLEDGlobals);        // Function selection B:
	data(0x0A, OLEDGlobals);           //  ROM/CGRAM selection: ROM C, CGROM=250, CGRAM=6 (ROM=10, OPR=10)
	command(0x79, OLEDGlobals);        // OLED characterization: OLED command set enabled (SD=1)
	command(0xDA, OLEDGlobals);        // Set SEG pins hardware configuration:
	command(0x10, OLEDGlobals);        //  alternative odd/even SEG pin, disable SEG left/right remap (default values)
	command(0xDC, OLEDGlobals);        // Function selection C:
	command(0x00, OLEDGlobals);        //  internal VSL, GPIO input disable
	command(0x81, OLEDGlobals);        // Set contrast control:
	command(0x7F, OLEDGlobals);        //  contrast=127 (default value)
	command(0xD9, OLEDGlobals);        // Set phase length:
	command(0xF1, OLEDGlobals);        //  phase2=15, phase1=1 (default: 0x78)
	command(0xDB, OLEDGlobals);        // Set VCOMH deselect level:
	command(0x40, OLEDGlobals);        //  VCOMH deselect level=1 x Vcc (default: 0x20=0,77 x Vcc)
	command(0x78, OLEDGlobals);        // OLED characterization: OLED command set disabled (SD=0) (exit from OLED command set)
	command((0x20 | rows), OLEDGlobals); // Function set: fundamental command set (RE=0) (exit from extended command set), lines #
	command(0x01, OLEDGlobals);        // Clear display
	//_delay_ms(2);             // After a clear display, a minimum pause of 1-2 ms is required
	delay(2);
	command(0x80, OLEDGlobals);        // Set DDRAM address 0x00 in address counter (cursor home) (default value)
	command(0x0C, OLEDGlobals);        // Display ON/OFF control: display ON, cursor off, blink off
	//_delay_ms(250);           // Waits 250 ms for stabilization purpose after display on
	delay(250);

}

void outputS(char* lineIn, int row,volatile Globals *OLEDGlobals)
{
	uint8_t r = row;
	uint8_t c = 0;

	command(new_line[r],OLEDGlobals);
	//20, because our display is 20x4.
	for (c = 0; c < 20; c++)
	{
		data(lineIn[c],OLEDGlobals);
	}
}

void numPrinter(char* charArray, uint8_t startingPos, uint8_t numCharacters, uint16_t inputNumber)
{
	//this needs to go in the OLED Library.
	uint8_t onesPlace = 0;
	uint8_t tensPlace = 0;
	uint8_t hunderedsPlace = 0;
	uint8_t thousandsPlace = 0;
	uint8_t tenThousandsPlace = 0;

	switch (numCharacters)
	{
	case 0:
		break;

	case 1:
		onesPlace = (inputNumber % 10) + 48; //this should be a value between 1 and 10.
		charArray[startingPos] = onesPlace;
		break;

	case 2:
		onesPlace = (inputNumber % 10) + 48; //this should be a value between 1 and 10.
		tensPlace = (inputNumber / 10) + 48;
		charArray[(startingPos + 1)] = onesPlace;
		charArray[startingPos] = tensPlace;
		break;

	case 3:
		onesPlace = (inputNumber % 10) + 48; //this should be a value between 1 and 10.
		tensPlace = ((inputNumber % 100) / 10) + 48;
		hunderedsPlace = (inputNumber / 100) + 48;
		charArray[(startingPos + 2)] = onesPlace;
		charArray[(startingPos + 1)] = tensPlace;
		charArray[startingPos] = hunderedsPlace;
		break;

	case 4:
		onesPlace = (inputNumber % 10) + 48; //this should be a value between 1 and 10.
		tensPlace = ((inputNumber % 100) / 10) + 48;
		hunderedsPlace = ((inputNumber % 1000) / 100) + 48;
		thousandsPlace = (inputNumber / 1000) + 48;
		charArray[(startingPos + 3)] = onesPlace;
		charArray[(startingPos + 2)] = tensPlace;
		charArray[(startingPos + 1)] = hunderedsPlace;
		charArray[startingPos] = thousandsPlace;
		break;

	case 5:
		onesPlace = (inputNumber % 10) + 48; //this should be a value between 1 and 10.
		tensPlace = ((inputNumber % 100) / 10) + 48;
		hunderedsPlace = ((inputNumber % 1000) / 100) + 48;
		thousandsPlace = ((inputNumber % 10000) / 1000) + 48;
		tenThousandsPlace = (inputNumber / 10000) + 48;
		charArray[(startingPos + 4)] = onesPlace;
		charArray[(startingPos + 3)] = tensPlace;
		charArray[(startingPos + 2)] = hunderedsPlace;
		charArray[(startingPos + 1)] = thousandsPlace;
		charArray[startingPos] = tenThousandsPlace;
		break;

		//if your number is higher than 16 bit, sorry, no can do.
	default:
		break;

	}


}

void midiNotePrinter(char* charArray, uint8_t startingPosition, uint8_t noteNumber)
{
	//will take up 3 character spaces. 
	char printLetter = 0;
	char printNumber = 0;
	char printSharp = 0;
	uint8_t valueSwitch = 0;
	//numbers will always go from B to C, and have 12 distinct values. 
	//we can get our number from this with division. 
	//midi note C0 starts at 12. So, we'll need to do some math there. 
	printNumber = (noteNumber / 12) + 47;

	//theres a weird wrap around with note numbers here. Since there isn't really an easy math patern we can take advantage of. 
	valueSwitch = noteNumber % 12; //this should give us a value between 0 and 11. 
	switch (valueSwitch)
	{

	case 0:
		printLetter = 'C';
		printSharp = ' ';
		break;

	case 1:
		printLetter = 'C';
		printSharp = '#';
		break;

	case 2:
		printLetter = 'D';
		printSharp = ' ';
		break;

	case 3:
		printLetter = 'D';
		printSharp = '#';
		break;

	case 4:
		printLetter = 'E';
		printSharp = ' ';
		break;

	case 5:
		printLetter = 'F';
		printSharp = ' ';
		break;

	case 6:
		printLetter = 'F';
		printSharp = '#';
		break;

	case 7:
		printLetter = 'G';
		printSharp = ' ';
		break;

	case 8:
		printLetter = 'G';
		printSharp = '#';
		break;

	case 9:
		printLetter = 'A';
		printSharp = ' ';
		break;

	case 10:
		printLetter = 'A';
		printSharp = '#';
		break;

	case 11:
		printLetter = 'B';
		printSharp = ' ';
		break;

	}
	charArray[startingPosition] = printLetter;
	charArray[startingPosition + 1] = printSharp;
	charArray[startingPosition + 2] = printNumber;

}


