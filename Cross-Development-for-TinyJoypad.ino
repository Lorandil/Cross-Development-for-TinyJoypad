//   >>>>>  Cross Development for TinyJoypad 1.0 GPLv3 <<<<
//						Tinyjoypad rev2 compatible
//                   Programmer: Sven B 2021
//              Contact EMAIL: Lorandil@gmx.de 

// This is a sample program to demonstrate cross development for the Tiny Joypad platform using
// another micro controller (e.g. Arduino UNO, Arduino Mega 2560 or Arduino Leonardo).
// It is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// The code works at 16MHZ internal
// and uses ssd1306xled Library for SSD1306 oled display 128x64.
// It works also with SSD1309 and with a modification also with SH1106.
//
// To stuff all code and data into the 8192 bytes of the ATtiny85
// the ATTinyCore (v1.5.2) by Spence Konde is recommended.
// The core is available at github: [https://github.com/SpenceKonde/ATTinyCore], just add the
// following board manager to the Arduino IDE: [http://drazzy.com/package_drazzy.com_index.json]
// Please enable LTO (link time optimization) and disable 'millis()' and
// 'micros()'.

#include <Arduino.h>
#include "tinyJoypadUtils.h"
#include "sampleBitmap.h"

const uint16_t SCREEN_COLS = 128;
const uint16_t SCREEN_ROWS = 8;
uint8_t posX = ( IMAGE_COLS - SCREEN_COLS ) / 2;
uint8_t posY = 8;

/*--------------------------------------------------------*/
void setup()
{
  // initialize the pins (and serial port if present)
  InitTinyJoypad();
  // perform display initialization
  InitDisplay();
}

/*--------------------------------------------------------*/
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
      uint8_t pixels = pgm_read_byte( RubjergKnudeFyr + posX + x + ( y + posY ) * IMAGE_COLS );
      // send 8 vertical pixels
      SendPixels( pixels );
    } // for x
    
    // this row is finished
    FinishDisplayRow();
  } // for y

  // display the whole screen at once
  DisplayBuffer();
}

/*--------------------------------------------------------*/
void blip()
{
  Sound( 60, 20 );
  Sound( 150, 20 );
}

/*--------------------------------------------------------*/
void loop()
{
  bool userAction = false;

  // check buttons
  if ( isLeftPressed() && ( posX >= 8 ) ) { posX -= 8; userAction = true; }
  if ( isRightPressed() && ( posX < IMAGE_COLS - SCREEN_COLS - 8 ) ) { posX += 8; userAction = true; }
  if ( isUpPressed() && ( posY > 0 ) ) { posY -= 1; userAction = true; }
  if ( isDownPressed() && ( posY < IMAGE_ROWS - SCREEN_ROWS - 1 ) ) { posY += 1; userAction = true; }
  if ( isFirePressed() ) { posX = ( IMAGE_COLS - SCREEN_COLS ) / 2; posY = 8; userAction = true; }

  // user action?
  if ( userAction )
  {
    // play a sound
    blip();
  }

  // render the image
  RenderImage();  
}