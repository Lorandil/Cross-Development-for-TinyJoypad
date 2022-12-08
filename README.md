# Cross Development Framework for TinyJoypad
![<Deutsche Version>](https://github.com/Lorandil/Cross-Development-for-TinyJoypad/blob/main/LIESMICH.md)

This repository contains a small framework for developing games for the **TinyJoypad** using a more connective and resourceful micro controller like **Arduino UNO**, **Leonardo**, **Mega 2560** or even the **Arduino Zero** (and many other micro controllers).

![TinyJoypad and LeonardoJoypad](https://github.com/Lorandil/Cross-Development-for-TinyJoypad/blob/main/pic/TinyJoypad-and-MakerUnoJoypad-reduced.jpg)


# Disclaimer
Although I took great care creating this framework and this tutorial, the software framework and the tutorial come without any warranties.
You use them at your own risk.
I may not be held responsible for any kind of damages that might occur.

 
## What is TinyJoypad?
TinyJoypad is a great project by the Canadian developer Daniel C which defines a standardized ATtiny85 gaming platform.
On the TinyJoypad homepage https://www.tinyjoypad.com/ you will find the schematics and a lot of cool games.


## Why Should I Want to use Cross Development?
When I was developing for the TinyJoypad, most code didn't work as it was expected on the first try.
That's normal for development - and even more when developing on a micro controller with limited resources and without any debugging support.
Even if there would have been a possibility to use a library like SoftwareSerial, there is no free pin on the TinyJoypad!
So I spent a lot of time thinking about possible causes for my bugs. That's good brain training - but mostly frustrating (especially when the problem is something stupid like wrong parameter order). So I thought it would be great to test the code on a more powerful "machine".

At first I wanted to test the code on the PC, but the Arduino code is not pure C++ and it would have been difficult to check the rendered video output.

I thought that maybe I could get the `<ssd1306xled>` library to work on an Arduino. A closer look into the code showed me, that this was above my abilities (because of the direct programming of the ATtiny registers for the I2C communication).
After some reflection I decided to try using the `<Adafruit_SSD1306>` library, which is available for many micro controllers (including several Arduinos).
And it worked - the Adafruit library supports direct access to the video buffer. Instead of writing a rendered byte directly to the display (as it is done on the ATtiny), the rendered data is stored in the video buffer and displayed when the buffer is complete.

Even sound output is possible!


## Advantages of Using a More Resourceful Micro Controller
* Serial debugging (by 'Serial.print()') is possible
* On newer micro controllers like the Arduino Zero and with Arduino IDE 2.0 even live debugging using breakpoints is possible!
* Faster turn-around-cycles, because no cables need to be unplugged/plugged and no fiddling with modules or even chips
is necessary
* More memory to get things working - before optimizing for flash and RAM size (reducing readability)
* Additional cool features as dumping a screenshot to the serial interface as a hexdump


## How Does It Work?
Without an installed ATtiny85 (and without the battery!) the TinyJoypad is a passive device which offers all signals on the ATtiny DIL8 socket or even handier on the module connector J1.
That means, that we can use the display, the buzzer, the joystick and the fire buttons from the outside!
I just had to figure out the correct wiring and voilà!
 
## Wiring for Arduino UNO, Leonardo and Mega 2560 with 5V Tolerant OLED Display
If your TinyJoypad display is 5V tolerant, you can directly connect the TinyJoypad's module port **J1** (or the DIL8 socket if you have a homemade TinyJoypad) to the Arduino.

### !!! REMOVE THE BATTERY FROM THE TINYJOYPAD BEFORE CONNECTING !!!

 
| TinyJoypad J1 / ATtiny85 Pin | Function | UNO R3 / MEGA2560 / Leonardo | Color |
| ---------------- |:-------------:|:-------------:|:------:|
| Pin 1 (leftmost) | left/right    | A0            | orange |
| Pin 2            | up/down       | A3            | yellow |
| Pin 3            | buzzer        | D12           | brown  |
| Pin 4            | GND           | **GND**       | black  |
| Pin 5            | SDA (I2C)     | SDA           | green  |
| Pin 6            | fire          | A1            | purple |
| Pin 7            | SCL (I2C)     | SCL           | blue   |
| Pin 8 (rightmost)| VCC           | **5V**        | red    |


## Wiring for Arduino Zero
Because the Arduino Zero runs on the same 3.3V as the TinyJoypad, you can directly connect the TinyJoypad's module port **J1** (or the DIL8 socket if you have a homemade TinyJoypad) to the Arduino Zero.

**Please don't connect the 5V to the TinyJoypad, this might damage your Arduino board!**

### !!! REMOVE THE BATTERY FROM THE TINYJOYPAD BEFORE CONNECTING !!!

 
| TinyJoypad J1 / ATtiny85 Pin | Function | Arduino Zero | Color |
| ---------------- |:-------------:|:-------------:|:------:|
| Pin 1 (leftmost) | left/right    | A0            | orange |
| Pin 2            | up/down       | A3            | yellow |
| Pin 3            | buzzer        | D12           | brown  |
| Pin 4            | GND           | **GND**       | black  |
| Pin 5            | SDA (I2C)     | SDA           | green  |
| Pin 6            | fire          | A1            | purple |
| Pin 7            | SCL (I2C)     | SCL           | blue   |
| Pin 8 (rightmost)| VCC           | **3.3V**      | red    |


## Wiring for Other Micro Controllers and non 5V Tolerant Displays
My first intuition was to simply use a level shifter. That works great for the display, the beeper and the fire button, but sadly *NOT for the direction buttons because these are analog inputs*.
It's probably the easiest solution to use a level shifter where possible and voltage dividers for the two analog inputs.


## How Do I Get the Same Software Running on TinyJoypad and Other Micro Controllers?
That's what this framework was made for! I encapsulated all hardware related code into special functions, so that in your application
code there is no need for any special treatment on which controller the program is running!
In my experience the overhead of these functions is zero, because the compiler will inline the functions (if not empty for the target platform) or discard them (if empty).
I verified this claim on ![TinyInvaders v4.2](https://github.com/Lorandil/Tiny-invaders-v4.2).

 
## Sample Project "Lighthouse"
The sample project  will display a part of an image of the iconic Rubjerg Knude Fyr. The image is 192x256 pixels (6kB) and can be scrolled with the joystick.
With every movement a short blip sound is played.
 
![TinyJoypad and LeonardoJoypad](https://github.com/Lorandil/Cross-Development-for-TinyJoypad/blob/main/pic/TinyJoypad-and-MakerUnoJoypad-reduced.jpg)

The code works on TinyJoypad and on Arduino UNO/Leonardo/Mega 2560 (and probably many other controllers) without changing a single line of code!

### Here is a simplified version of the code without the bitmap and joystick stuff:
For the full version please check out the sketch in the repository!

## Definitions of the Required Functions
```javascript
#include "tinyJoypadUtils.h"
```

## Initialization of the TinyJoypad I/O-Pins and the Display
```javascript
void setup()
{
  // initialize the pins (and serial port if present)
  InitTinyJoypad();
  // perform display initialization
  InitDisplay();
}
```

## Cyclic Image Rendering Call
```javascript
void loop()
{
  // render the image
  RenderImage();  
}
```

## Render and Display the Image
 ```javascript
void RenderImage()
{
  // there are 8 rows with a height of 8 pixels
  for ( uint8_t y = 0; y < 8; y++)
  {
    // prepare display of row <y>
    PrepareDisplayRow( y );

    // there are 128 columns of 8 vertical aligned pixels
    for ( uint8_t x = 0; x < 128; x++ )
    {
      // get the pixels from somewhere
      uint8_t pixels = ...
      // send 8 vertical pixels to the display
      SendPixels( pixels );
    }
    
    // this row has been finished
    FinishDisplayRow();
  }

  // display the whole screen
  DisplayBuffer();
}
```


## What are the Remaining Differences?
If the micro controllers are of comparable types like ATMEGA328 (Arduino UNO) or MEGA2560 (Arduino Mega 2560) or AVR32U4 (Arduino Leonardo), the only differences you might notice will be:

* The drawing of the image: Because the TinyJoypad has not enough RAM to render the output image in memory, the image is rendered while it is displayed. Depending on the speed of the calculations, this can be visible. The Adafruit library uses a RAM buffer for storing the rendered image. After rendering is complete, the image will be transferred in one piece, looking snappier.

* The brightness of the OLED and the loudness of the beeper might be higher when using a 5V micro controller (due to the increased voltage).

If your controller is much faster, you might see increased speed to the point, that your project (especially the controls) will begin to behave differently.
You might counter that effect with `#ifdef` and `_delay_ms()`, but it's probably easier to stay with the above mentioned AVR chips.
The Cortex M0+ of the Arduino Zero is considerably faster than the AVR chips on the other Arduinos, but still worked ok for me. You should just test regularly on the real ATtiny to avoid frustration :)


## You Said Something About Screenshots?
To get a screenshot you only have to do two things in `"tinyJoypadUtils.h"`:

* enable the screenshot function 
```javascript
////////////////////////////////////////////////////////////
// enable serial screenshot
#define _ENABLE_SERIAL_SCREENSHOT_
```
* and define a trigger condition
```javascript
// perform a serial screenshot if this condition is true:
#define _SERIAL_SCREENSHOT_TRIGGER_CONDITION_ ( isFirePressed() )
```
     
That's all. Every time your trigger condition is met during `DisplayBuffer()`, the screenshot will be printed to the first serial port. There will even some instructions how to convert the hexdump back to a proper image using free tools:
     
```
This is a TinyJoypad screenshot. Output is one hex byte per pixel. To get the actual image perform the following steps:
(1) The output can be converted online to binary with 'https://tomeko.net/online_tools/hex_to_file.php?lang=en'.
(2) Then import the file with IrfanView (https://www.irfanview.com/): Open as -> RAW file...
(3) Set Image width to 64 and Image height to 128, 8 BPP -> OK
(4) Rotate and mirror the result as needed :)
Hint: If you only get partial screenshots, try using a terminal program to capture the serial output.
     
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,....
```
 

## License
GNU General Public License v3.0
