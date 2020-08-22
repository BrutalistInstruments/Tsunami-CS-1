## Programming the Arduino

Since this whole Project was developed using the atmel studio platform, Porting over to the Arduino IDE is not exactly the smoothest port. 
Mainly due to C to C++ conversions and the Arduino overhead. 
Instead, we provide a .hex file of the most stable build, which you can upload to your arduio via AVRDUDE. 
You can use this via command line, or use a tool we very much like, AVRDUDESS:
https://blog.zakkemble.net/avrdudess-a-gui-for-avrdude/

Connect your arduino via USB, and upload the .hex file useing the "arduino" programmer. 
After that, you should be good to go!