# Cross Development Framework für TinyJoypad
![<English Version>](https://github.com/Lorandil/Cross-Development-for-TinyJoypad/blob/main/README.md)

Dieses Repository enthält ein kleines Framework zur Entwicklung von Spielen für das TinyJoypad unter Verwendung eines anschlussfreudigeren und leistungsfähigeren Mikrocontrollers 
(wie z.B. dem Arduino UNO/Leonardo/Mega 2560 oder auch vielen anderen).


![TinyJoypad and LeonardoJoypad](https://github.com/Lorandil/Cross-Development-for-TinyJoypad/blob/main/pic/TinyJoypad-and-MakerUnoJoypad-reduced.jpg)
 
## Was ist TinyJoypad?
TinyJoypad ist ein großartiges Projekt des kanadischen Entwicklers Daniel C, welches eine standardisierte ATtiny85-Spieleplatform definiert.

Auf der TinyJoypad Homepage https://www.tinyjoypad.com/ findet Ihr die Projektbeschreibung sowie viele coole Retro-Games.


## Warum sollte ich Cross Development nutzen wollen?
Als ich für das TinyJoypad entwickelt habe, funktionierte ein guter Teil meines Codes nicht auf Anhieb so, wie ich es mir vorgestellt hatte.
Das ist normal beim Programmieren - umso mehr, wenn man für einen Mikrocontroller mit begrenzten Resourcen und ohne Debugging-Unterstützung entwickelt.
Erschwerend kommt hinzu, dass beim TinyJoypad wirklich *alle* Pins bereits belegt sind, eine Verwendung von Bibliotheken wie SoftwareSerial ist somit nicht möglich.
Also habe ich über die Ursachen meine Bugs gegrübelt - das trainiert das logische Denken - ist aber oft frustrierend (besonders, wenn das Problem durch einen 
dämlichen Fehler wie eine falsche Anordnung der Parameter verursacht wird...).

Am Anfang wollte ich den Code auf dem PC testen, aber zum einen ist Arduino-Code kein reiner C++-Code und zum Anderen wäre eine Prüfung der Videoausgabe aufwändig gewesen.

Meine nächste Idee war, ob ich vielleicht den Code der `<ssd1306xled>`-Bibliothek auf einem Arduino zum Laufen bringen könnten.
Bei genauerer Betrachtung habe ich aber einsehen müssen, dass dies meine Fähigkeiten übersteigt (Programmierung der für I2C benutzten USI-Register auf dem ATtiny85).
Nach einigem Überlegen entschied ich mich die `<Adafruit_SSD1306>`-Bibliothek (welche für nahezu alle Mikrocontroller verfügbar ist) zu verwenden.
Und es funktioniert: Die Adafruit-Bibliothek erlaubt einen direkten Zugriff auf ihren internen Bildspeicher.
Anstatt die berechneten Pixel direkt über I2C an das Display zu senden (wie beim ATtiny85 notwendig), werden die Pixel in den Bildspeicher der Bibliothek übertragen 
und nach dem "Bildaufbau" in einem Transfer an das Display gesendet. Dieses Display müsste theoretisch nicht einmal per I2C angebunden sein, sondern könnte auch an SPI hängen.

Sogar die Soundausgabe ist möglich. Auf einem Arduino UNO/Leonardo/Mega 2560 funktioniert das sogar ohne Code-Änderungen. Bei anderen Mikrocontrollern muss ggf. ein anderes
Portbit gewählt werden).


## Vorteile bei der Verwendung eines leistungsstärkeren Mikrocontrollers
* Serielles Debugging (mit 'Serial.print()') ist möglich
* Auf neueren Mikrocontrollers und der Arduino IDE 2.0 sollte sogar Live-Debugging mit Breakpoints möglich sein
* Schnellere Entwicklungszyklen: Das umständliche Umstecken von Kabeln oder sogar ICs entfällt vollständig
* Mehr Speicher (Flash und RAM) um ein Projekt überhaupt zum Laufen zu bringen. Komplexe Optimierungen können so erst am Ende erfolgen
* Weitere coole Möglichkeiten wie die Ausgabe eines Screenshots über den seriellen Port (als Hexdump)


## Wie funktioniert das?
Ohne einen gesteckten ATtiny85 (und ohne Batterie!) ist das TinyJoypad ein passives Bauteil, welches alle Signale am Modulport J1 bereitstellt.
Das bedeutet, dass wir alle Funktionalitäten des TinyJoypads (Display, Lautsprecher, Joystick und Feuerknöpfe) von außen erreichen können.
Ich musste nur die Belegung des Ports herausfinden und voilà!
 
## Verdrahtung für Arduino UNO/Leonardo/Mega 2560 mit einem 5V-toleranten Display
Wenn Euer TinyJoypad-Display 5V-tolerant ist, kann der Modulport des TinyJoypads direkt mit dem Arduino verbunden werden:

### BITTE VOR DEM ANSCHLUSS DIE BATTERIE AUS DEM TINYJOYPAD ENTFERNEN !!!

 
| TinyJoypad J1 / ATtiny85 Pin | Funktion | UNO R3 / MEGA2560 / Leonardo | Farbe |
| ---------------- |:-------------:|:-------------:|:-------: |
| Pin 1 (links)    | links/rechts  | A0            | orange  |
| Pin 2            | hoch/runter   | A3            | gelb    |
| Pin 3            | Lautsprecher  | `D12`         | braun   |
| Pin 4            | GND           | GND           | schwarz |
| Pin 5            | SDA (I2C)     | SDA           | grün    |
| Pin 6            | Feuer         | A1            | lila    |
| Pin 7            | SCL (I2C)     | SCL           | blau    |
| Pin 8 (rechts)   | VCC           | VCC           | rot     | 


## Verdrahtung für andere Mikrocontroller und nicht 5V-tolerante Displays
Mein erster Gedanke war einen Level-Shifter zu verwenden. Das funktioniert hervorragend für Display, Lautsprecher und Feuerknopf,
leider aber *NICHT* für den Joystick, da die beiden Achsen auf je einen Analogeingang gehen.
Es ist vermutlich die einfachste Lösung wo möglich einen Level-Shifter zu nehmen und die Analogsignale über Spannungsteiler anzupassen.


## Wie schaffe ich es, dieselbe Software auf einem TinyJoypad und einem anderen Mikrocontroller laufen zu lassen?
Dafür gibt es dieses Framework! Der gesamte Hardware-bezogenen Code ist in speziellen Funktionen gekapselt.
In Eurem Anwendungscode müssen daher keine Controller-abhängigen Behandlungen eingebaut werden.
Nach meinen Erfahrungen ist der Overhead dieser Funktion Null, da der Compiler leere Funktionen entfernen und den restlichen Code direkt einbinden wird.
Diese Behauptung habe ich an ![TinyInvaders v4.2](https://github.com/Lorandil/Tiny-invaders-v4.2) bestätigen können.

 
## Beispielprojekt "Leuchtturm"
Das Beispielprojekt zeigt einen Ausschnitt aus einem Bild des ikonischen Leuchtturms "Rubjerg Knude Fyr" an.
Das Bild ist 192x256 Pixel groß (6kB) und kann mit dem Joystick gescrollt werden.
Bei jeder Bewegung wird ein kurzer "Blip" über den Lautsprecher ausgegeben.
 
![TinyJoypad and LeonardoJoypad](https://github.com/Lorandil/Cross-Development-for-TinyJoypad/blob/main/pic/TinyJoypad-and-MakerUnoJoypad-reduced.jpg)

Der Code läuft ohne jede Änderung sowohl auf einem TinyJoypad als auch auf einem Arduino UNO/Leonardo/Mega 2560 (und vielen weiteren Mikrocontrollern)!

### Hier eine gekürzte Version des Beispielprojekts ohne die Steuerung und Tonausgabe
Den vollständigen Sketch findet Ihr im Repository!

## Definitionen der verwendeten Funktionen
```javascript
#include "tinyJoypadUtils.h"
```

## Initialisierung der TinyJoypad I/O-Pins und des Displays
```javascript
void setup()
{
  // initialize the pins (and serial port if present)
  InitTinyJoypad();
  // perform display initialization
  InitDisplay();
}
```

## Zyklischer Bildaufbau
```javascript
void loop()
{
  // render the image
  RenderImage();  
}
```

## Berechnung und Darstellung des Bildausschnitts
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


## Was sind die verbleibenden Unterschiede?
Wenn Euer Mikrocontroller aus einer vergleichbaren Leistungsklasse wie dem  ATMEGA328 (Arduino Uno), AVR32U4 (Arduino Leonardo) oder MEGA2560 (Arduino Mega 2560) ist, sollten die einzigen Unterschiede sein:

* Das Zeichnen des Bildes: Weil das TinyJoypad nicht genug RAM hat um das Bild im Speicher vorzuberechnen, wird das Bild schon während der Berechnung angezeigt. Das kann sichtbar sein.
  Die Adafruit-Bibliothek hingegen verwendet einen RAM-Puffer. Dadurch kann das Bild nach der Berechnung in einem Transfer übertragen werden, die Anzeige wirkt "zackiger". 

* Die Helligkeit des Displays und die Lautstärke des Lautsprechers können bei einem 5V-Mikrocontroller durch die erhöhte Betriebsspannung höher sein.

Wenn Euer Mikrocontroller viel schneller ist, kann der Geschwindigkeitsunterschied so groß werden, dass Euer Projekt (insbesondere die Steuerung) sich anders verhält.
Diesem Effekt könntet Ihr mit `#ifdef` und `_delay_ms()` entgegenwirken, aber vermutlich ist es einfacher bei einem der genannten AVR-Chips zu bleiben.

## Wie war das mit Screenshots?
Um einen Screenshot zu erhalten müssen die folgenden zwei Dinge in `"tinyJoypadUtils.h"` angepasst werden:

* die Screenshot-Funktionalität muss aktiviert werden
```javascript
////////////////////////////////////////////////////////////
// enable serial screenshot
#define _ENABLE_SERIAL_SCREENSHOT_
```
* und eine Auslösebedingung muss festlegt sein
```javascript
// perform a serial screenshot if this condition is true:
#define _SERIAL_SCREENSHOT_TRIGGER_CONDITION_ ( isFirePressed() )
```
     
Das ist alles. Jedesmal, wenn die Auslösebedingung während `TinyFlip_DisplayBuffer()` erfüllt ist, wird ein Screenshot auf dem ersten seriellen Port ausgegeben.
Vor dem eigentlichen Screenshot werden Anweisungen ausgegeben, wie der Hexdump mit freien Tools wieder in ein Bild umgewandelt werden kann:
     
```
This is a TinyJoypad screenshot. Output is one hex byte per pixel. To get the actual image perform the following steps:
(1) The output can be converted to binary with 'https://tomeko.net/online_tools/hex_to_file.php?lang=en' online.
(2) Then import the file with IrfanView (https://www.irfanview.com/): Open as -> RAW file...
(3) Set Image width to 64 and Image height to 128, 8 BPP -> OK
(4) Rotate and mirror the result as needed :)
Hint: If you only get partial screenshots, try using a terminal program to capture the serial output.
     
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,....
```
 

## Lizenz
GNU General Public License v3.0
