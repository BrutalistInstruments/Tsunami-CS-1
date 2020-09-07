## Programming the Arduino

Since this whole Project was developed using the atmel studio platform, Porting over to the Arduino IDE is not exactly the smoothest port. 
Mainly due to C to C++ conversions and the Arduino overhead. 
Instead, we provide a .hex file of the most stable build, which you can upload to your arduio via AVRDUDE. 
You can use this via command line, or use a tool we very much like, AVRDUDESS:
https://blog.zakkemble.net/avrdudess-a-gui-for-avrdude/

Steps:
- Connect your arduino via USB
- under the MCU dropdown, choose "Atmega2560"
- select your COM Port on the Port drop down (should be the only one that says "COM")
- use a baud rate of 115200
- for Programmer, use "Wiring"
- under the "Flash" section, locate the .hex file that you downloaded from thr repository by clicking the "..." button.
- check the "disable flash erase" check box, and the "Disable Verify" box.

Now you can click either "Go" under the flash section, or "Program". The firmware should be flashed to your Board!


If you experience some crazy values printng on your screen for knob movement, be sure your power supply is plugged in. This unit can not sucessfully be powered via the USB port. 
the last step is to do a factory reset to Format the external memory, and you've completed the programming process!