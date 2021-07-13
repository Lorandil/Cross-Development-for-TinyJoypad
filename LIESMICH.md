# Cross Development Framework für TinyJoypad
![English Version](https://github.com/Lorandil/Cross-Development-for-TinyJoypad/blob/main/README.md)

Dieses Repository enthält ein kleines Framework zur Entwicklung von Spielen für das TinyJoypad unter Verwendung eines anschlussfreudigeren und leistungsfähigeren Mikorcontrollers 
(wie z.B. dem Arduino UNO/Leonardo/Mega 2560 oder auch vielen anderen).


![TinyJoypad and LeonardoJoypad](https://github.com/Lorandil/Cross-Development-for-TinyJoypad/blob/main/pic/TinyJoypad-and-LeonardoJoypad-800x600.jpg)
 
## Was ist TinyJoypad?
TinyJoypad ist ein großartiges Projekt des kanadischen Entwicklers Daniel C, welches eine standardisierte ATtiny85-Spieleplatform definiert.
On the TinyJoypad homepage https://www.tinyjoypad.com/ you will find the schematics and a lot of cool games.

Ein TinyJoypad besteht aus einem ATtiny85, einem I2C OLED mit dem SSD1306-Chipsatz, einem Joystyck, einem Button, einem Beeper und einigen Widerständen.
Die Verdrahtung ist simpel und kann auch auf einem Steckbrett oder einer Prototyp-Platine erfolgen.
Eine wirklich coole Idee ist die Verwendung eines Modulports, welche einen schnellen Wechsel zwischen Spielen durch den Austausch des ATtiny85 ermöglicht.


## Warum sollte ich Cross Development nutzen wollen?
Als ich für das TinyJoypad entwickelt habe, funktionierte ein guter Teil meines Codes nicht auf Anhieb so, wie ich es mir vorgestellt hatte.
Das ist normal beim Programmieren - umso mehr, wenn man für einen Mikrocontroller mit begrenzten Resourcen und ohne Debugging-Unterstützung entwickelt.
Erschwerend kommt hinzu, dass beim TinyJoypad wirklich *alle* Pins bereits belegt sind, eine Verwendung von Bibliotheken wie SoftwareSerial ist somit nicht möglich.
Also habe ich über die Ursachen meine Bugs gegrübelt - das trainiert das logische Denken - ist aber oft frustrierend (besonders, wenn das Problem durch einen 
dämlichen Fehler wie eine falsche Anordnung der Parameter verursacht wird...).

Am Anfang wollte ich den Code auf dem PC testen, aber zum einen ist Arduino-Code kein reiner C++-Code und zum Anderen wäre eine Prüfung der Videoausgabe aufwändig gewesen.

Meine nächste Idee war, ob ich vielleicht den <ssd1306xled> auf einem Arduino zum Laufen bringen könnten.
Bei genauerer Betrachtung habe ich aber einsehen müssen, dass dies meine Fähigkeiten übersteigt (Programmierung der für I2C benutzten USI-Register auf dem ATtiny85).
Nach einigem Überlegen entschied ich mich die  <Adafruit_SSD1306>-Bibliothek (welche für nahezu alle Mikrocontroller verfügbar ist) zu verwenden.
Und es funktioniert: Die Adafruit-Bibliothek erlaubt einen direkten Zugriff auf ihren internen Bildspeicher.
Anstatt die berechneten Pixel direkt über I2C an das Display zu senden (wie beim ATtiny85 notwendig), werden die Pixel in den Bildspeicher der Bibliothek übertragen 
und nach dem "Bildaufbau" in einem Transfer an das Display gesendet. Dieses Display müsste theoretisch nicht einmal per I2C angebunden sein, sondern könnte auch an SPI hängen.

Sogar die Soundausgabe ist möglich. Auf einem Arduino UNO/Leonardo/Mega 2560 funktioniert das sogar ohne Code-Änderungen. Bei anderen Mikrocontrollern muss ggf. ein anderes
Portbit gewählt werden).


## Advantages of using a more resourceful micro controller
* Serial debugging (by 'Serial.print()') is possible
* On newer micro controllers and with Arduino IDE 2.0 even live debugging using breakpoints should be possible
* Faster turn-around-cycles, because no cables need to be unplugged/plugged and no fiddling with modules or even chips
is necessary
* More memory to get things working - before optimizing for flash and RAM size (reducing readability)
* Additional cool features as dumping a screenshot to the serial interface as a hexdump


## How Does It Work?
Without an installed ATTiny85, the TinyJoypad is a passive device which offers all signals on the module connector J1.
That means, that we can use the display, the buzzer, the joystick and the fire buttons from the outside!
I just had to figure out the correct wiring and voilà!
 
## Wiring for Arduino UNO, Leonardo and Mega 2560 with 5V Tolerant OLED Display
If your TinyJoypad display is 5V tolerant, you can directly connect the TinyJoypad's module port to the Arduino.

### PLEASE REMOVE THE BATTERY FROM THE TINYJOYPAD BEFORE CONNECTING !!!

 
| TinyJoypad J1    | Function      | UNO R3        | MEGA2560      | Leonardo      |
| ---------------- |:-------------:|:-------------:|:-------------:|:-------------:|
| Pin 1 (leftmost) | left/right    | A0            | A0            | A0            |
| Pin 2            | up/down       | A3            | A3            | A3            |
| Pin 3            | buzzer        | `D12`         | `D10`         | `D8`          |
| Pin 4            | GND           | GND           | GND           | GND           |
| Pin 5            | SDA (i2c)     | SDA           | SDA           | SDA           |
| Pin 6            | fire          | A1            | A1            | A1            |
| Pin 7            | SCL (i2c)     | SCL           | SCL           | SCL           |
| Pin 8 (rightmost)| VCC           | VCC           | VCC           | VCC           |
 
Please note that the connection of pin 3 differs between the controllers due to the location of PB4!


## Wiring for Other Micro Controllers and non 5V Tolerant Displays
My first intuition was to simply use a level shifter. That works great for the display, the beeper and the fire button, but sadly *NOT for the direction buttons because these are analog inputs*.
It's probably the easiest solution to use a level shifter where possible and voltage dividers for the two analog inputs.


## How Do I Get the Same Software Running on TinyJoypad and Other Micro Controllers?
I encapsulated all hardware related code into special functions, so that in your application code there is no need 
for any special treatment on which controller the program is running!
The overhead of these functions is zero or nearly zero because the compiler will inline most of the code.
If your flash memory gets really low at the end of your project, you are free to try inlining the code by yourself.

 
## Sample Project
The sample project  will display an image of the iconic Rubjerg Knude Fyr. The image is 192x256 pixels (6kB) and can be scrolled with the joystick. For every movement a short blip sound is played.
 
The code works on TinyJoypad and on Arduino UNO/Leonardo/Mega 2560 (and probably many other controllers) without changing a single line of code!


### Here is a simplified version of the code without the bitmap and joystick stuff:
For the full version please check out the sketch in the repository!

## Definitions of the Required Functions
```javascript
#include "tinyJoypadUtils.h"
```

## Initialization of the TinyJoypad I/O-Pins and the Display
```javascript
/*--------------------------------------------------------*/
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
/*--------------------------------------------------------*/
void loop()
{
  // render the image
  Tiny_Flip();  
}
```

## Render and Display the Image
 ```javascript
/*--------------------------------------------------------*/
void Tiny_Flip()
{
  // there are 8 rows with a height of 8 pixels
  for ( uint8_t y = 0; y < 8; y++)
  {
    // prepare display of row <y>
    TinyFlip_PrepareDisplayRow( y );

    // there are 128 columns of 8 vertical aligned pixels
    for ( uint8_t x = 0; x < 128; x++ )
    {
      // get the pixels from somewhere
      uint8_t pixels = ...
      // send 8 vertical pixels to the display
      TinyFlip_SendPixels( pixels );
    }
    
    // this row has been finished
    TinyFlip_FinishDisplayRow();
  }

  // display the whole screen
  TinyFlip_DisplayBuffer();
}
```


## What are the Remaining Differences?
If the micro controllers are of comparable types like ATMEGA328 (Arduino Uno) or MEGA2560 (Arduino Mega 2560) or AVR32U4 (Arduino Leonardo), the only differences you might notice will be:

* The drawing of the image: Because the TinyJoypad has not enough RAM to render the output image in memory, the image is rendered while it is displayed. Depending on the speed of the calculations, this can be visible. The Adafruit library uses a RAM buffer for storing the rendered image. After rendering is complete, the image will be transfered in one piece, looking snappier.

* The brightness of the OLED and the loudness of the beeper might be higher when using a 5V micro controller (due to the increased voltage).

If your controller is much faster, you might see increased speed to the point, that your project (especially the controls) will begin to behave differently.
You might counter that effect with `#ifdef` and `_delay_ms()`, but it's probably easier to stay with the above mentioned AVR chips.



