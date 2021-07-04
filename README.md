# Cross Development for TinyJoypad
 A sample project for developing games for the TinyJoypad using a more connective and resourceful Arduino UNO or Mega 2560 (and many more micro controllers)
 

## What is TinyJoypad?
TinyJoypad is a great project under GPL v3 by Daniel C which defines a standard ATtiny85 gaming platform.
On https://www.tinyjoypad.com/ you will find the schematics and a lot of cool games.
The wiring is simple and can be done on a breadboard or single sided prototype board.
(If you want a more professional appearance, you can even order professional PCBs at a commercial
PCB service.)


## The Idea
When I was developing for the TinyJoypad most code didn't work as it was expected on the first try.
That's normal for development - and even more when developing on a micro controller with limited resources and without any debugging support.
Even if there would have been a possibility to use software serial, there is no free pin on the TinyJoypad!
So I spent a lot of time thinking about possible causes, that's good brain training - but mostly frustrating (especially when the problem was something like wrong parameter order). Thus I thought it would be great to test the code on a more powerful "machine".

At first I wanted to test the code on the PC, but the Arduino code is not pure C++ and it would have been difficult to check the rendered video output.

I thought that maybe I could get the <ssd1306xled> to work on an Arduino. A closer look into the code showed me, that this was above my abilities (because of the direct programming of the ATtiny registers for the I2C communication).
After some reflection I decided to try using the <Adafruit_SSD1306> library, which is available for many micro controllers (including Arduino UNO and Mega 2560).
And it worked - the Adafruit library supports direct access to the video buffer. Instead of writing a rendered byte directly to the display (as it is done on the ATtiny), the rendered data is stored in the video buffer and displayed when the buffer is complete.

Even sound output is possible (at least on Arduino UNO and Mega 2560, for other micro controllers a different port might be required).


## Advantages of using a more resourceful micro controller
* Serial debugging (by 'Serial.print()') is possible
* on newer micro controllers and with Arduino IDE 2.0 even live debugging using breakpoints should be possible!
* faster turn-around-cycles because no cables need to be unplugged/plugged and no fiddling with modules or even chips
* additional features as dumping a screenshot to the serial interface as a hexdump
* more memory to just getting things working before optimizing for flash and RAM size
* it would be even possible to use the TinyJoypad as a gamepad for the Arduino ;) 


## Wiring for Arduino UNO and Mega 2560 with 5V tolerant OLED display
If your TinyJoypad display is 5V tolerant, you can directly connect the TinyJoypad's module port to the Arduino.

PLEASE REMEMBER TO REMOVE THE BATTERIE AND THE ATTINY85 FROM THE TINYJOYPAD BEFORE CONNECTING !!!

I used the following connections:

<insert fritzing! image here...>

![Picture](https://github.com/Lorandil/Cross-Development-for-TinyJoypad/blob/main/pic/TinyJoypad-wiring-to-Arduino.png)


## Wiring for other micro controllers and non 5V tolerant displays
My first intuition was to simply use a level shifter. That works great for the display, the beeper and the fire button, but sadly *NOT for the direction buttons because these are analog inputs*.
It's probably the easiest solution to use a level shifter where possible and voltage dividers for the two analog inputs.


## How do I get the software running on TinyJoypad and other micro controllers?
I encapsulated all hardware related code into special functions, so that in your application code there is no need 
for any special treatment on which controller the program is running!
The overhead of these functions is zero or nearly zero because the compiler will inline most of the code.
If your flash memory gets really low at the end of your project, you are free to try inlining the code by yourself.

Have a look at this sample project:

It will display an image and play a short sound, on TinyJoypad and on Arduino UNO/Mega 2560 (and probably many other controllers) without changing anything in the code!

The only differences you should notice could be:
* the drawing of the image: Because the TinyJoypad has not enough RAM to render the output image in memory it is rendered while it is displayed. Depending on the speed of the calculations this can be visible. The Adafruit library uses a RAM buffer for storing the rendered image which will be transfered in one piece, looking snappier.
* the brightness of the OLED and the loudness of the beeper might be higher when using a 5V micro controller (due to the increased voltage)



