# Cross Development for TinyJoypad
 A sample project for developing games for the TinyJoypad using a more connective and resourceful Arduino UNO or Mega 2560 (and many more micro controllers)

## The Idea
When I was developing for the TinyJoypad most code didn't work as it was expected on the first try.
That's normal for development - and even more when developing on a micro controller without any debugging support.
Even if there would have been a possibility to use software serial, there is no free pin on the TinyJoypad!
So I spent a lot of time thinking about possible causes, that's good brain training - but mostly frustrating (especially when the problem was something like wrong parameter order). Thus I thought it would be great to test the code on a more powerful "machine".

At first I wanted to test the code on the PC, but the Arduino code is not pure C++ and it would have been difficult to check the creation of the video output.

So I thought that maybe I could get the <ssd1306xled> to work on an Arduino. A closer look into the code showed me, that this was above my abilities (because of the direct programming of the ATtiny registers).
After some reflection I decided to try using the <Adafruit_SSD1306> library, which is available for many micro controllers (including Arduino UNO and Mega 2560).

Even sound output is possible (at least on Arduino UNO and Mega 2560, for other micro controllers a different port might be required).

## Wiring for Arduino UNO and Mega 2560 with 5V tolerant OLED display
If your TinyJoypad display is 5V tolerant, you can directly connect the TinyJoypad's module port to the Arduino.

!PLEASE REMEMBER TO REMOVE THE BATTERIE AND THE ATTINY85 FROM THE TINYJOYPAD BEFORE CONNECTING!

I used the following connections:

TinyJoypad J1        Function                   Arduino UNO           Mega 2560
Pin 1 (leftmost)	 left/right button          A0				      A0
Pin 2                up/down button             A3                    A3
Pin 3                buzzer                     D12                   D10
Pin 4                GND                        GND                   GND
Pin 5                SDA (i2c)                  SDA                   SDA
Pin 6                fire button                A1                    A1
Pin 7                SCL (i2c)                  SCL/SCK               SCL/SCK
Pin 8                VCC                        VCC                   VCC

## Wiring for other micro controllers and non 5V tolerant displays
My first intuition was to simply use a level shifter. That works for the display, the beeper and the fire button, but sadly *NOT for the direction buttons because these are analog inputs*.
It's probably the easiest solution to use a level shifter where possible and voltage dividers for the two analog inputs.



