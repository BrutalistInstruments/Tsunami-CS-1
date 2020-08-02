## Electronics Components:
The current BOM is held at this mangaged DigiKey shopping cart.
If a part becomes obselete, we'll be updating it here. 
https://www.digikey.com/short/z5mmrv

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
Screws: 92125A128
Bolts: 90592A085
Standoffs: 98952A106
Screws are hex driven, so any standard allen wrench set should suit you well. 
This project requires 18 standoffs. 
The other packages of 100 screws and nuts should get you though the rest of enclosure, and only run around $4. 


# Notes
-Currently, it looks like digikey and mouser are both out of the required audio jacks. 
It looks like this is a suitable drop in replacement: https://www.mouser.com/ProductDetail/Amphenol-Audio/ACJS-MHDRM?qs=t8%2F5FiDdxGZPnxKNuhurhQ%3D%3D
Though the in-stock numbers do not seem to be very high at the moment. 
I would guess that Covid has slowed down manufacturing at the plant. 
Grabbing some different audio connectors and wiring them up by hand should work fine.
I would recomend holding off on soldering up the IO PCB until the correct parts come back in stock,
because for some reason, the vast majority of 1/4" connectors have weird proriotary nubs on the bottom,
and there seems to be no standard for pin placement. 
Sorry for the inconvince.
