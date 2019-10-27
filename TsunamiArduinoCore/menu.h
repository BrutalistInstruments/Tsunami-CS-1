/*
 * menu.h
 *
 * Created: 8/13/2019 5:54:40 PM
 *  Author: Hal
 */


#ifndef MENU_H_
#define MENU_H_

void updateScreen(uint8_t *menuStateChange, Screen menuScreen);
void initMenu(Screen initTheScreen, Pattern initPattern, uint8_t initPatternNumber);
//void numToMidiNote(uint8_t inputNum, char midiNote[3]);



#endif /* MENU_H_ */
