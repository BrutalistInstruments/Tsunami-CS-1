/*
 * OLEDLib.c
 *
 * Created: 6/26/2019 5:47:50 PM
 *  Author: Hal
 */
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/io.h>

uint8_t new_line[4] = {0x80, 0xA0, 0xC0, 0xE0};

//enable cycle is functioning properly.
void enableCycle()
{

	//required for timing on LCD Screen
	//the only delay in this library.


	PORTH |= 0B00100000;
	_delay_us(1);
	PORTH &= 0B11011111;

}

void send8bit(uint8_t value)
{
	//set all pins on port C to output pins.
	//using unsigned int 8 bit values should protect this
	//function from overflow.

	//also,this should just work.
	//since we want to send an 8 bit value over an entire port.
	PORTC = value;
}

void command(uint8_t c)
{
	//digitalWrite(DC, 0);
	PORTH &= 0B10111111; //set our DC pin low, to get ready to write data.
	//we need to figure out where our DC pin is.
	send8bit(c);
	enableCycle();

}

void data(uint8_t d)
{
	//digitalWrite(DC, 1);
	PORTH |=0B01000000; //set out DC Pin high, so it's ready to write data.
	send8bit(d);
	enableCycle();
}

void initScreen()
{
	//this is where we will do all of the screen
	//initialization.
	DDRH = 0x60; //pins 14 and 15, 14 is Enable (PortJ1), 15 is data/command (PortJ0)
	DDRC =0xFF; //all pins on the LCD Data Bus.


	PORTH &= 0B10011111;		//set both the DC line and E line of the display to 0. leave all other bits on the ports alone

	PORTC = 0x00;				// Initializes all Arduino pins for the data bus
	_delay_us(200);				// Waits 200 us for stabilization purpose


	uint8_t rows = 0x08;                    // Display mode: 2/4 lines

	command(0x22 | rows); // Function set: extended command set (RE=1), lines #
	command(0x71);        // Function selection A:
	data(0x5C);           //  enable internal Vdd regulator at 5V I/O mode (def. value) (0x00 for disable, 2.8V I/O)
	command(0x20 | rows); // Function set: fundamental command set (RE=0) (exit from extended command set), lines #
	command(0x08);        // Display ON/OFF control: display off, cursor off, blink off (default values)
	command(0x22 | rows); // Function set: extended command set (RE=1), lines #
	command(0x79);        // OLED characterization: OLED command set enabled (SD=1)
	command(0xD5);        // Set display clock divide ratio/oscillator frequency:
	command(0x70);        //  divide ratio=1, frequency=7 (default values)
	command(0x78);        // OLED characterization: OLED command set disabled (SD=0) (exit from OLED command set)


	command(0x09);     // Extended function set (RE=1): 5-dot font, B/W inverting disabled (def. val.), 3/4 lines

	command(0x06);        // Entry Mode set - COM/SEG direction: COM0->COM31, SEG99->SEG0 (BDC=1, BDS=0)
	command(0x72);        // Function selection B:
	data(0x0A);           //  ROM/CGRAM selection: ROM C, CGROM=250, CGRAM=6 (ROM=10, OPR=10)
	command(0x79);        // OLED characterization: OLED command set enabled (SD=1)
	command(0xDA);        // Set SEG pins hardware configuration:
	command(0x10);        //  alternative odd/even SEG pin, disable SEG left/right remap (default values)
	command(0xDC);        // Function selection C:
	command(0x00);        //  internal VSL, GPIO input disable
	command(0x81);        // Set contrast control:
	command(0x7F);        //  contrast=127 (default value)
	command(0xD9);        // Set phase length:
	command(0xF1);        //  phase2=15, phase1=1 (default: 0x78)
	command(0xDB);        // Set VCOMH deselect level:
	command(0x40);        //  VCOMH deselect level=1 x Vcc (default: 0x20=0,77 x Vcc)
	command(0x78);        // OLED characterization: OLED command set disabled (SD=0) (exit from OLED command set)
	command(0x20 | rows); // Function set: fundamental command set (RE=0) (exit from extended command set), lines #
	command(0x01);        // Clear display
	_delay_ms(2);             // After a clear display, a minimum pause of 1-2 ms is required

	command(0x80);        // Set DDRAM address 0x00 in address counter (cursor home) (default value)
	command(0x0C);        // Display ON/OFF control: display ON, cursor off, blink off
	_delay_ms(250);           // Waits 250 ms for stabilization purpose after display on

}

void outputS(char* lineIn, int row)
{
	uint8_t r = row;
	uint8_t c = 0;

	command(new_line[r]);
	//20, because our display is 20x4.
	for(c=0; c<20; c++)
	{
		data(lineIn[c]);
	}
}

void numPrinter(char* charArray,uint8_t startingPos, uint8_t numCharacters, uint16_t inputNumber)
{
	//this needs to go in the OLED Library.
	uint8_t onesPlace = 0;
	uint8_t tensPlace = 0;
	uint8_t hunderedsPlace = 0;
	uint8_t thousandsPlace = 0;
	uint8_t tenThousandsPlace = 0;

	switch(numCharacters)
	{
		case 0:
		break;

		case 1:
		onesPlace = (inputNumber%10)+48; //this should be a value between 1 and 10.
		charArray[startingPos] = onesPlace;
		break;

		case 2:
		onesPlace = (inputNumber%10)+48; //this should be a value between 1 and 10.
		tensPlace = (inputNumber/10)+48;
		charArray[(startingPos+1)] = onesPlace;
		charArray[startingPos] = tensPlace;
		break;

		case 3:
		onesPlace = (inputNumber%10)+48; //this should be a value between 1 and 10.
		tensPlace = ((inputNumber%100)/10)+48;
		hunderedsPlace = (inputNumber/100)+48;
		charArray[(startingPos+2)] = onesPlace;
		charArray[(startingPos+1)] = tensPlace;
		charArray[startingPos] = hunderedsPlace;
		break;

		case 4:
		onesPlace = (inputNumber%10)+48; //this should be a value between 1 and 10.
		tensPlace = ((inputNumber%100)/10)+48;
		hunderedsPlace = ((inputNumber%1000)/100)+48;
		thousandsPlace = (inputNumber/1000)+48;
		charArray[(startingPos+3)] = onesPlace;
		charArray[(startingPos+2)] = tensPlace;
		charArray[(startingPos+1)] = hunderedsPlace;
		charArray[startingPos] = thousandsPlace;
		break;

		case 5:
		onesPlace = (inputNumber%10)+48; //this should be a value between 1 and 10.
		tensPlace = ((inputNumber%100)/10)+48;
		hunderedsPlace = ((inputNumber%1000)/100)+48;
		thousandsPlace = ((inputNumber%10000)/1000)+48;
		tenThousandsPlace = (inputNumber/10000)+48;
		charArray[(startingPos+4)] = onesPlace;
		charArray[(startingPos+3)] = tensPlace;
		charArray[(startingPos+2)] = hunderedsPlace;
		charArray[(startingPos+1)] = thousandsPlace;
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
	printNumber = (noteNumber/12)+47; 
	
	//theres a weird wrap around with note numbers here. Since there isn't really an easy math patern we can take advantage of. 
	valueSwitch = noteNumber%12; //this should give us a value between 0 and 11. 
	switch(valueSwitch)
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
	charArray[startingPosition+1]= printSharp;
	charArray[startingPosition+2]= printNumber;
	
}