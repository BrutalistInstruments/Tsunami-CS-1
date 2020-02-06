#ifndef SEQUENVCERLIB_H_
#define SEQUENCERLIB_H_

#include <Arduino.h>
#include <globalVariables.h>

void initSequencer();
void updateSequencer(Pattern sequencerPattern, uint8_t playState);

#endif /* SEQUENCERLIB_H_ */
