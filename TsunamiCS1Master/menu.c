/*
 * menu.c
 *
 * Created: 8/13/2019 5:54:51 PM
 *  Author: Hal
 */ 

#include <avr/io.h>
#include "globalVariables.h"
#include "OLEDLib.h"

//screen 0:
char s0line0[] = "Performance Mode    ";
char s0line1[] = "Pattern:            ";
char s0line2[] = "BPM:                ";
char s0line3[] = "current value change";

//screen 1:
char s1line0[] = "Sequence            ";
char s1line1[] = "Steps  :            ";
char s1line2[] = "Step number:        ";
char s1line3[] = "current value change";

//screen 2:
char s2line0[] = "Sample Select       ";
char s2line1[] = "Name goes here      ";
char s2line2[] = "New Name?           ";
char s2line3[] = "                    ";

//screen 3:
char s3line0[] = "Global Settings     ";
char s3line1[] = "Midi Channel: xx    ";
char s3line2[] = "Midi trigger Notes  ";
char s3line3[] = "(put triggered note)";

void initMenu()
{
	
outputS(s0line0, 0);
outputS(s0line1, 1);
outputS(s0line2, 2);
outputS(s0line3, 3);

}

void updateScreen()
{
	if(prevEncoderAValue!=encoderAValue){
	encoderAValue = encoderAValue%4;
	switch (encoderAValue)
	{
		case 0:
		outputS(s0line0, 0);
		outputS(s0line1, 1);
		outputS(s0line2, 2);
		outputS(s0line3, 3);
		break;
		
		case 1:
 		outputS(s1line0, 0);
 		outputS(s1line1, 1);
 		outputS(s1line2, 2);
 		outputS(s1line3, 3);
 		break;
		 
		case 2:
		outputS(s2line0, 0);
		outputS(s2line1, 1);
		outputS(s2line2, 2);
		outputS(s2line3, 3);
		break;
		 
		case 3:
		outputS(s3line0, 0);
		outputS(s3line1, 1);
		outputS(s3line2, 2);
		outputS(s3line3, 3);
		break;
		 
		 
	}
	prevEncoderAValue = encoderAValue;
	
	}


}