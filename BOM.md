## Electronics Components:
The current BOM is held at this mangaged DigiKey shopping cart.
If a part becomes obselete, we'll be updating it here. 
https://www.digikey.com/short/zrz401

In addition to the Digikey cart, You'll also need to source your own Tsunami Wav Trigger, and Arduio mega boards. 
The wav trigger can be had from here, or any sparkfun distributer:
https://www.sparkfun.com/products/13810

Arduino Mega development boards can be had from a number of sources at a few different price points. (ebay, amazon, official sources, robotics webstores, ect)
This project was developed on Elagoo clones. 
Any clone should get the job done, as long as it has a 16Mhz Crystal Oscillator.
I have seen a few mega clones that have 12Mhz Oscillators, and some with ceramic Oscillators. 
Since timing is extremely important to this code, having the correct clock source is vital.

## Mounting Hardware:
We're currently sourcing all of our mounting hardware from McMaster Carr.
https://www.mcmaster.com/
Parts numbers are as follows:
Screws (short, for frontpannel mounting): 92125A128
Screws (longer, for sidepannels): 92125A132
Bolts: 90592A085
Standoffsx18: 98952A106
Standoffs for OLED Screenx4: 98952A103 
Screws are hex driven, so any standard allen wrench set should suit you well. 
This project requires 18 standoffs. 
The other packages of 100 screws and nuts should get you though the rest of enclosure, and only run around $4. 


# Notes
-Currently, it looks like digikey and mouser are both out of the required audio jacks. 
It looks like this is a suitable drop in replacement:https://www.mouser.com/ProductDetail/Amphenol-Audio/ACJM-HHDR?qs=t8%2F5FiDdxGZR4gBVKrXjqA%3D%3D

## Practical BOM
For buidling purposes, here are the part names and values for this project:

## Tsunami-CS1 Board:
|Qty|Value|Part Number|Name|Description|
| ------- |-------| ------- |-------| ------- |
|1|CONN_02|0705530036‎|TSUNAMIPOWER|Power Connector for Tsunami Board|
|1|CONN_03|N/C|EXTERNALSERIALHEADER|Header is not included, availible for possible expansion|
|1|CONN_06|0705530040‎|MIDIHEADER|Molex connector for midi portion of IO Board|
|1|CONN_07|‎2190‎|BUCKCONVERTER|Adafruit Buck Converter|
|2|FE05-1|FE05-1|JTAGLEFT, JTAGRIGHT|Unnecessary for non-debug / development builds|
|26|LED3MM|WP710A10SRD/J4‎|LED1, LED2, LED3, LED4, LED5, LED6, LED7, LED8, LED9, LED10, LED11, LED12, LED13, LED14, LED15, LED16, LED17, LED18, LED19, LED20, LED21, LED22, LED23, LED24, LED25, LED26|LEDs|
|1|PINHD-1X20-BIG|‎NHD-0420CW-AY3‎|OLED|OLED display and  1x20 pinheader |
|5|0.1uF|AR205F104K4R‎|C1, C2, C3, C4, C5|ceramic capacitor|
|26|1.5K|‎LR1F1K5|R1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11, R12, R13, R14, R15, R16, R17, R18, R19, R20, R21, R36, R38, R39, R40, R41|Axial Resistor|
|8|10K|	‎LR1F10K‎ |R22, R23, R24, R25, R26, R27, R28, R29|Axial Resistor|
|4|10nF|FA18C0G2A103JRU06‎|C6, C7, C8, C9|ceramic capacitor|
|3|1K|LR1F1K0‎ |R30, R31, R43|Generic Resistor Package|
|1|1N4448|‎1N4448TR‎|D1|Diode|
|1|2.2K|‎CF18JT2K20‎|R44|Axial Resistor|
|1|240|‎LR1F240R‎|R32|Axial Resistor|
|2|24LC1025-I/P|24LC1025-I/P|U9, U10|Eeprom chips, storage|
|2|4.02K|‎MFR-25FBF52-4K02‎|R37, R42|Axial Reistor|
|1|6N138|‎6N138-000E‎|U11|Optocoupler/Optoisolator|
|1|6_PIN_SERIAL_CABLEPTH|0705530040|J1|Serial Connection to Tsunami Board|
|1|7414N|‎SN74HC14N‎|IC1|Hex schmitt trigger INVERTER|
|1|ARDUINO_MEGA_R3FULL|ARDUINO_MEGA_R3FULL|B2|Arduino Mega R3|
|5|CD74HC4051E|CD74HC4051E|U1, U2, U3, U4, U5|analog Multiplexer|
|2|ENCODER|	‎EN11-HSM1BF20|ENCODERBOTTOM, TOPENCODER|Rotary Encoders|
|8|10K|PTA4543-2015DPB103‎ |TRACKVOLUME0, TRACKVOLUME1, TRACKVOLUME2, TRACKVOLUME3, TRACKVOLUME4, TRACKVOLUME5, TRACKVOLUME6, TRACKVOLUME7|Slide Potentiometers|
|22|MOMENTARY-SWITCH-SPST-PTH-12MM|‎TL1100F160Q‎|GPBUTTON0, GPBUTTON1, GPBUTTON2, GPBUTTON3, GPBUTTON4, GPBUTTON5, TRIG1, TRIG2, TRIG3, TRIG4, TRIG5, TRIG6, TRIG7, TRIG8, TRIG9, TRIG10, TRIG11, TRIG12, TRIG13, TRIG14, TRIG15, TRIG16|Momentary Switch (Pushbutton) - SPST|
|26|10K|‎PTV09A-4025F-B103‎|ATTACK0, ATTACK1, ATTACK2, ATTACK3, ATTACK4, ATTACK5, ATTACK6, ATTACK7, KNOB0, KNOB1,  OUTVOL0, OUTVOL1, OUTVOL2, OUTVOL3, OUTVOL4, OUTVOL5, OUTVOL6, OUTVOL7, RELEASE0, RELEASE1, RELEASE2, RELEASE3, RELEASE4, RELEASE5, RELEASE6, RELEASE7|Pannel Mount Potentiometers|
|10|10K|‎PTV09A-4225F-B103|KNOB2, KNOB3,PITCH0, PITCH1, PITCH2, PITCH3, PITCH4, PITCH5, PITCH6, PITCH7|Pannel mount Potentiometers, Center Dentent|
|3|SN74HC595N|‎SN74HC595N‎|U6, U7, U8|8 bit shift registers|

## Tsunami IO Board
|Qty|Value|Part Number|Name|Description|
| ------- |-------| ------- |-------| ------- |
|2|CONN_10X2|3020-20-0200-00|J14|IDC Connector to Tsunami Audio headers|
|1|PINHD-1X6|0705530040|JP1|Molex connector for Midi to CS Board|
|4|240|‎LR1F240R‎|R1, R2, R3, R4|Axial Resistor|
|10|ACJS-MHDR|ACJS-MHDR‎| J4, J5, J6, J7, J8, J9, J10, J11, J12, J13|1/4" audio connection jacks|
|1|MIDI-IN|SDS-50J|J1|Midi Din socket|
|1|MIDI-OUT|SDS-50J|J2|Midi Din socket|
|1|MIDI-THRU|SDS-50J|J3|Midi Din socket|