//New encoder Library:

#include <Arduino.h>
#include <Encoder.h>
#include "globalVariables.h"
#include "OLEDLib.h"
//#include "eepromLib.h" //to implement

Encoder encoderA(9,8);
Encoder encoderB(7,6);
uint8_t prevEncoderA;
uint8_t prevEncoderB;
int32_t encoderAVal;
int32_t encoderBVal;
extern Pattern currentPattern;
extern uint8_t currentPatternNumber;
extern Screen screenBank;
extern uint8_t currentStep;
extern uint8_t currentTrack;
char patternNumPrint[21] = "Pattern:            ";


void initEncoders()
{
  prevEncoderA = 0;
  prevEncoderB = 0;
}

void listenEncoders(uint8_t *encoderMenuState)
{
  //so, we kindof only care about encoder B, if button is flagged.
encoderAVal = (((encoderA.read())/4)%4);

if(encoderAVal!=prevEncoderA){
  //if encoder A has changed, then we update the main menu
//  *encoderMenuState&=0b0000000; //clear out all previous bits in EncoderB.
  //this will make our arrow start at the top of the screen.
*encoderMenuState = (encoderAVal<<4);
encoderBVal = 0;
prevEncoderA = encoderAVal;
}else
{
  switch (*encoderMenuState){
    //possible cases:
    case 0:
    {
      encoderBVal = encoderB.read()/4;
      int patternChange = prevEncoderB-encoderBVal;
      if(patternChange>0)
      {
        currentPatternNumber=currentPatternNumber-1;
        numPrinter(screenBank.screen0[1], 9, 3, currentPatternNumber);
        numPrinter(screenBank.screen1[1], 9, 3, currentPatternNumber);
        outputS(screenBank.screen0[1], 1);
        //readPattern(currentPattern, currentPatternNumber); //to be implemented

      }else if (patternChange<0)
      {
        currentPatternNumber=currentPatternNumber+1;
        numPrinter(screenBank.screen0[1], 9, 3, currentPatternNumber);
        numPrinter(screenBank.screen1[1], 9, 3, currentPatternNumber);
        outputS(screenBank.screen0[1], 1);
      }

prevEncoderB = encoderBVal;
    }
    break;

    case 17:
    {
      encoderBVal = (encoderB.read()/4);
      int menuChange = prevEncoderB - encoderBVal;
      if(menuChange==1)
      {
        *encoderMenuState=18;
      }else if(menuChange==-1)
      {
        *encoderMenuState=19;
      }
      //if we didn't change the menu state, we didn't move.

      prevEncoderB = encoderBVal;
    }
    break;

    case 18:
    {
    encoderBVal = (encoderB.read()/4);
    int menuChange = prevEncoderB - encoderBVal;
    if(menuChange==1)
    {
      *encoderMenuState=19;
    }else if(menuChange==-1)
    {
      *encoderMenuState=17;
    }
    //if we didn't change the menu state, we didn't move.

    prevEncoderB = encoderBVal;
  }
    break;

    case 19:
    {
    encoderBVal = (encoderB.read()/4);
    int menuChange = prevEncoderB - encoderBVal;
    if(menuChange==1)
    {
      *encoderMenuState=17;
    }else if(menuChange==-1)
    {
      *encoderMenuState=18;
    }
    //if we didn't change the menu state, we didn't move.

    prevEncoderB = encoderBVal;
  }
    break;

    case 25:
    {
      encoderBVal = encoderB.read()/4;
      int patternChange = prevEncoderB-encoderBVal;
      if(patternChange>0)
      {
        currentPatternNumber=currentPatternNumber-1;
        numPrinter(screenBank.screen0[1], 9, 3, currentPatternNumber);
        numPrinter(screenBank.screen1[1], 9, 3, currentPatternNumber);
        outputS(screenBank.screen1[1], 1);
      }else if (patternChange<0)
      {
        currentPatternNumber=currentPatternNumber+1;
        numPrinter(screenBank.screen0[1], 9, 3, currentPatternNumber);
        numPrinter(screenBank.screen1[1], 9, 3, currentPatternNumber);
        outputS(screenBank.screen1[1], 1);
      }
      prevEncoderB = encoderBVal;
    }
    break;

    case 26:
    {
      encoderBVal = encoderB.read()/4;
      int maxStepChange = prevEncoderB-encoderBVal;
      if(maxStepChange>0)
      {
        if(currentPattern.numSteps>1){
        currentPattern.numSteps=currentPattern.numSteps-1;
        numPrinter(screenBank.screen1[2], 7, 2, currentPattern.numSteps);
        outputS(screenBank.screen1[2], 2);
      }
      }else if (maxStepChange<0)
      {
        if(currentPattern.numSteps<64){
        currentPattern.numSteps=currentPattern.numSteps+1;
        numPrinter(screenBank.screen1[2], 7, 2, currentPattern.numSteps);
        outputS(screenBank.screen1[2], 2);
      }
      }
      prevEncoderB = encoderBVal;
    }

    break;

    case 27:
    {
      encoderBVal = encoderB.read()/4;
      int stepChange = prevEncoderB-encoderBVal;
      if(stepChange>0)
      {
        if(currentStep>1){
        currentStep=currentStep-1;
        numPrinter(screenBank.screen1[3], 13, 2, currentStep);
        outputS(screenBank.screen1[3], 3);
      }
    }else if (stepChange<0)
      {
        if(currentStep<currentPattern.numSteps){
        currentStep=currentStep+1;
        numPrinter(screenBank.screen1[3], 13, 2, currentStep);
        outputS(screenBank.screen1[3], 3);
      }
      }
      prevEncoderB = encoderBVal;
    }
    break;

    case 33:
    {
      encoderBVal = (encoderB.read()/4);
      int menuChange = prevEncoderB - encoderBVal;
      if(menuChange==1)
      {
        *encoderMenuState=34;
      }else if(menuChange==-1)
      {
        *encoderMenuState=35;
      }
      //if we didn't change the menu state, we didn't move.

      prevEncoderB = encoderBVal;
    }
    break;

    case 34:
    {
      encoderBVal = (encoderB.read()/4);
      int menuChange = prevEncoderB - encoderBVal;
      if(menuChange==1)
      {
        *encoderMenuState=35;
      }else if(menuChange==-1)
      {
        *encoderMenuState=33;
      }
      //if we didn't change the menu state, we didn't move.

      prevEncoderB = encoderBVal;
    }
    break;

    case 35:
    {
      encoderBVal = (encoderB.read()/4);
      int menuChange = prevEncoderB - encoderBVal;
      if(menuChange==1)
      {
        *encoderMenuState=33;
      }else if(menuChange==-1)
      {
        *encoderMenuState=34;
      }
      //if we didn't change the menu state, we didn't move.

      prevEncoderB = encoderBVal;
    }
    break;


    case 41:
    {
      encoderBVal = encoderB.read()/4;
      int sampleChange = prevEncoderB-encoderBVal;
      uint16_t currentSample = currentPattern.trackSampleMSB[currentTrack]<<8|currentPattern.trackSampleLSB[currentTrack];
      if(sampleChange>0)
      {
        if(currentSample>1){
        currentSample=currentSample-1;
        numPrinter(screenBank.screen2[1], 10, 4, currentSample);
        outputS(screenBank.screen2[1], 1);
        currentPattern.trackSampleLSB[currentTrack] = currentSample;
        currentPattern.trackSampleMSB[currentTrack] = (currentSample>>8);
      }
    }else if (sampleChange<0)
      {
          if(currentSample<4096){
          currentSample=currentSample+1;
          numPrinter(screenBank.screen2[1], 10, 4, currentSample);
          outputS(screenBank.screen2[1], 1);
          currentPattern.trackSampleLSB[currentTrack] = currentSample;
          currentPattern.trackSampleMSB[currentTrack] = (currentSample>>8);
        }
      }
      prevEncoderB = encoderBVal;
    }
    break;

    case 42:
    //case 34, but with EncoderBFlag`
    break;

    case 43:
    {
      encoderBVal = encoderB.read()/4;
      int outputChange = prevEncoderB-encoderBVal;
      if(outputChange>0)
      {
        if(currentPattern.trackOutputRoute[currentTrack]>1){
        currentPattern.trackOutputRoute[currentTrack] = currentPattern.trackOutputRoute[currentTrack] - 1;
        numPrinter(screenBank.screen2[3], 10, 2, currentPattern.trackOutputRoute[currentTrack]);
        outputS(screenBank.screen2[3], 3);
      }
    }else if(outputChange<0)
      {
          if(currentPattern.trackOutputRoute[currentTrack]<8){
          currentPattern.trackOutputRoute[currentTrack] = currentPattern.trackOutputRoute[currentTrack] + 1;
          numPrinter(screenBank.screen2[3], 10, 2, currentPattern.trackOutputRoute[currentTrack]);
          outputS(screenBank.screen2[3], 3);
        }
      }
      prevEncoderB = encoderBVal;
    }

    break;

    case 49:
    {
      encoderBVal = (encoderB.read()/4);
      int menuChange = prevEncoderB - encoderBVal;
      if(menuChange==1)
      {
        *encoderMenuState=50;
      }else if(menuChange==-1)
      {
        *encoderMenuState=51;
      }
      //if we didn't change the menu state, we didn't move.

      prevEncoderB = encoderBVal;
    }
    break;

    case 50:
    {
      encoderBVal = (encoderB.read()/4);
      int menuChange = prevEncoderB - encoderBVal;
      if(menuChange==1)
      {
        *encoderMenuState=51;
      }else if(menuChange==-1)
      {
        *encoderMenuState=50;
      }
      //if we didn't change the menu state, we didn't move.

      prevEncoderB = encoderBVal;
    }
    break;

    case 51:
    {
      encoderBVal = (encoderB.read()/4);
      int menuChange = prevEncoderB - encoderBVal;
      if(menuChange==1)
      {
        *encoderMenuState=50;
      }else if(menuChange==-1)
      {
        *encoderMenuState=49;
      }
      //if we didn't change the menu state, we didn't move.

      prevEncoderB = encoderBVal;
    }
    break;

    case 57:
    //this is case49, but with EncoderBFlag
    break;

    case 58:
    //this is case50, but with EncoderBFlag
    break;

    case 59:
    //this is case51, but with EncoderBFlag
    break;

}
}


}
