# Cross Development Framework für TinyJoypad
![<English Version>](https://github.com/Lorandil/Cross-Development-for-TinyJoypad/blob/main/README.md)

Dieses Repository enthält ein kleines Framework zur Entwicklung von Spielen für das TinyJoypad unter Verwendung eines anschlussfreudigeren und leistungsfähigeren Mikorcontrollers 
(wie z.B. dem Arduino UNO/Leonardo/Mega 2560 oder auch vielen anderen).


![TinyJoypad and LeonardoJoypad](https://github.com/Lorandil/Cross-Development-for-TinyJoypad/blob/main/pic/TinyJoypad-and-LeonardoJoypad-800x600.jpg)
 
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

Meine nächste Idee war, ob ich vielleicht den <ssd1306xled> auf einem Arduino zum Laufen bringen könnten.
Bei genauerer Betrachtung habe ich aber einsehen müssen, dass dies meine Fähigkeiten übersteigt (Programmierung der für I2C benutzten USI-Register auf dem ATtiny85).
Nach einigem Überlegen entschied ich mich die  <Adafruit_SSD1306>-Bibliothek (welche für nahezu alle Mikrocontroller verfügbar ist) zu verwenden.
Und es funktioniert: Die Adafruit-Bibliothek erlaubt einen direkten Zugriff auf ihren internen Bildspeicher.
Anstatt die berechneten Pixel direkt über I2C an das Display zu senden (wie beim ATtiny85 notwendig), werden die Pixel in den Bildspeicher der Bibliothek übertragen 
und nach dem "Bildaufbau" in einem Transfer an das Display gesendet. Dieses Display müsste theoretisch nicht einmal per I2C angebunden sein, sondern könnte auch an SPI hängen.

Sogar die Soundausgabe ist möglich. Auf einem Arduino UNO/Leonardo/Mega 2560 funktioniert das sogar ohne Code-Änderungen. Bei anderen Mikrocontrollern muss ggf. ein anderes
Portbit gewählt werden).


## Vorteile bei der Verwendung eines leistungsstärkeren Mikrocontrolers
* Serielles Debugging (mit 'Serial.print()') ist möglich
* Auf neueren Mikrocontrollers und der Arduino IDE 2.0 sollte sogar Live-Debugging mit Breakpoints möglich sein
* Schnellere Entwicklungszyklen: Das umständliche Umstecken von Kabeln oder sogar ICs entfällt vollständig
* Mehr Speicher (Flash und RAM) um ein Projekt überhaupt erst zum Laufen zu bringen. Komplexe Optimierungen können so erst am Ende erfolgen!
* Weitere coole Möglichkeiten wie die Ausgabe eines Screenshots über den seriellen Port (als Hexdump)


## Wie funktioniert das?
Ohne einen gesteckten ATtiny85 ist das TinyJoypad ein passives Bauteil, welches alle Signale am Modulport J1 bereitstellt.
Das bedeutet, dass wir alle Funktionalitäten des TinyJoypads (Display, Lautsprecher, Joystick und Feuerknöpfe) von außen erreichen können.
Ich musste nur die Belegung des Ports herausfinden und voilà!
 
## Verdrahtung für Arduino UNO/Leonardo/Mega 2560 mit einem 5V-toleranten Display
Wenn Euer TinyJoypad-Display 5V-tolerant ist, kann der Modulport des TinyJoypads direkt mit dem Arduino verbunden werden:

### BITTE VOR DEM ANSCHLUSS DIE BATTERIE AUS DEM TINYJOYPAD ENTFERNEN !!!

 
| TinyJoypad J1    | Funktion      | UNO R3        | MEGA2560      | Leonardo      |
| ---------------- |:-------------:|:-------------:|:-------------:|:-------------:|
| Pin 1 (links)    | links/rechts  | A0            | A0            | A0            |
| Pin 2            | hoch/runter   | A3            | A3            | A3            |
| Pin 3            | Lautsprecher  | `D12`         | `D10`         | `D8`          |
| Pin 4            | GND           | GND           | GND           | GND           |
| Pin 5            | SDA (i2c)     | SDA           | SDA           | SDA           |
| Pin 6            | Feuer         | A1            | A1            | A1            |
| Pin 7            | SCL (i2c)     | SCL           | SCL           | SCL           |
| Pin 8 (rechts)   | VCC           | VCC           | VCC           | VCC           |
 
Bitte beachten, dass Pin 3 des Modulports wegen der Position von Port B4 je nach Mikrocontroller auf einem anderene Eingang liegt!


## Verdrahtung für andere Mikrocontroller und nicht 5V-tolerante Displays
Mein erster Gedanke war eine Level-Shifter zu verwenden. Das funktioniert hervorragend für Display, Lautsprecher und Feuerknopf,
leider aber *NICHT* für den Joystick, da dieser auf zwei Analogeingänge geht.
Es ist vermutlich die einfachste Lösung wo möglich einen Level-Shifter zu nehmen und die Analogsignale über Spannungsteiler anzupassen.


## Wie schaffe ich es dieselbe Software auf einem TinyJoypad und einem anderen Mikrocontroller laufen zu lassen?
Für diesen Zweck habe ich allen Hardware-bezogenen Code in spezielle Funktionen gekapselt.
In Eurem Anwendungscode müssen daher keine Controller-abhängigen Behandlungen eingebaut werden.
Der Overhead dieser Funktion ist Null oder nahe Null, da der Compiler leere Funktionen entfernen und den restlichen Code
direkt einbinden wird.
Nur falls der Flashspeicher in Eurem Projekt sehr knapp wird, könnte es sich lohnen hier genauer hinzuschauen.

 
## Beispielprojekt "Leuchtturm"
Das Beispielprojekt zeigt einen Ausschnitt aus einem Bild des ikonischen Leuchtturms "Rubjerg Knude Fyr" an.
Das Bild ist 129x256 Pixel groß (6kB) und kann mit dem Joystick gescrollt werden.
Bei jeder Bewegung wird ein kurzer "Blip" über den Lautsprecher ausgegeben.
 
![TinyJoypad and LeonardoJoypad](https://github.com/Lorandil/Cross-Development-for-TinyJoypad/blob/main/pic/TinyJoypad-and-LeonardoJoypad-800x600.jpg)

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
  Tiny_Flip();  
}
```

## Berechnung und Darstellung des Bildausschnitts
 ```javascript
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


## Was sind die verbleibenden Unterschiede?
Wenn Euer Mikrocontroller aus einer vergleichbaren Leistungsklasse wie dem  ATMEGA328 (Arduino Uno), AVR32U4 (Arduino Leonardo) oder MEGA2560 (Arduino Mega 2560) ist, sollten die einzigen Unterschiede sein:

* Das Zeichnen des Bildes: Weil das TinyJoypad nicht genug RAM hat um das Bild im Speicher vorzuberechnen, wird das Bild schon während der Berechnung angezeigt. Das kann sichtbar sein.
  Die Adafruit-Bibliothek hingegen verwendet einen RAM-Puffer. Dadurch kann das Bild nach der Berechnung in einem Transfer übertragen werden, die Anzeige wirkt "zackiger". 

* Die Helligkeit des Displays und die Lautstärke des Lautsprechers können bei einem 5V-Mikrocontroller durch die erhöhte Betriebsspannung höher sein.

Wenn Euer Mikrocontroller viel schneller ist, kann der Geschwindigkeitsunterschied so groß werden, dass Euer Projekt (insbesondere die Steuerung) sich anders verhält.
Diesem Effekt könntet Ihr mit `#ifdef` und `_delay_ms()` entgegenwirken, aber vermutlich ist es einfacher bei einem der genannten AVR-Chips zu bleiben.

## License
GNU General Public License v3.0
