# Cross Development for TinyJoypad
 A sample project for developing games for the TinyJoypad using a more connective and resourceful Arduino UNO or Mega 2560 (and many more microcontrollers)

## The Idea
When I was developing for the TinyJoypad most code didn't work as it was expected on the first try.
That's normal for development - and even more when developing on a microcontroller without any debugging support.
Even if there would have been a possibility to use software serial, there is no free pin on the TinyJoypad!
So I spent a lot of time thinking about possible causes, that's good brain training - but mostly frustrating (especially when the problem was something like wrong parameter order). Thus I thought it would be great to test the code on a more powerful "machine".

At first I wanted to test the code on the PC, but the Arduino code is not pure C++ and it would have been difficult to check the creation of the video output.

So I thought that maybe I could get the <ssd1306xled> to work on an Arduino. A closer look into the code showed me, that this was above my abilites (because of the direct programming of the ATtiny registers).
After some reflection I decided to try using the <Adafruit_SSD1306> library, which is available for many microcontrollers (including Arduino UNO and Mega 2560).
