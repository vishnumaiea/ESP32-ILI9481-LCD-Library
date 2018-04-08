
//========================================================================//
//                                                                        //
//  ## ESP32-ILI9481-LCD-Library ##                                       //
//  ILI9481 320 x 480 LCD driver and graphics library for ESP32 boards    //
//                                                                        //
//  Filename : ESP32_ILI9481.cpp                                          //
//  Author : Vishnu M Aiea                                                //
//  Source : https://github.com/vishnumaiea/ESP32-ILI9481-LCD-Library     //
//  Author's website : www.vishnumaiea.in                                 //
//  Initial release : IST 11:51 PM 07-04-2018, Saturday                   //
//  License : GNU GPL version 3.0                                         //
//  Additional source license/s :                                         //
//    1. BSD license @ Adafruit Industries                                //
//       https://github.com/adafruit/Adafruit-GFX-Library                 //
//                                                                        //
//  File last modified : IST 11:17 AM 08-04-2018, Sunday                  //
//                                                                        //
//========================================================================//

#include "ESP32_ILI9481.h"

//========================================================================//
//parallel data pins for ILI9481 display (recommended)
//below pins are  for DOIT DevKit board version 1
//pin numbers are of digital pins
//note : pins for ESP32 Devkit and Node32S are different

// #define CS_PIN_LCD  26
// #define RST_PIN_LCD 12
// #define DC_PIN_LCD  14
// #define WR_PIN_LCD  27
//
// #define PD0   15    //DB8  - D15
// #define PD1   2     //DB9  - D2
// #define PD2   0     //DB10 - D0
// #define PD3   4     //DB11 - D4
// #define PD4   16    //DB12 - D16
// #define PD5   17    //DB13 - D17
// #define PD6   21    //DB14 - D21
// #define PD7   22    //DB15 - D22

//========================================================================//
//LCD functions

LCD_ILI9481::LCD_ILI9481 (uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e, uint8_t f, uint8_t g, uint8_t h,
                          uint8_t i, uint8_t j, uint8_t k, uint8_t l) {
  _PD0 = a;
  _PD1 = b;
  _PD2 = c;
  _PD3 = d;
  _PD4 = e;
  _PD5 = f;
  _PD6 = g;
  _PD7 = h;
  _CS_PIN_LCD = i;
  _RST_PIN_LCD = j;
  _DC_PIN_LCD = k;
  _WR_PIN_LCD = l;
  _bus_width = 8;
}

//------------------------------------------------------------------------//
//initializes the display with proper reset sequence

void LCD_ILI9481::initializeDisplay() {
  //the following statement won't work on the rev 0 of ESP32 chip
  GPIO.out |= (PORT_MASK << _DC_PIN_LCD); //DC high
  GPIO.out |= (PORT_MASK << _CS_PIN_LCD); //CS high
  GPIO.out |= (PORT_MASK << _WR_PIN_LCD); //WR high
  GPIO.out |= (PORT_MASK << _RST_PIN_LCD); //RST high
  delay(100);
  GPIO.out &= (~(PORT_MASK << _DC_PIN_LCD)); //RST low
  delay(100);
  GPIO.out |= (PORT_MASK << _RST_PIN_LCD); //RST high
  delay(100);
  GPIO.out &= (~(PORT_MASK << _CS_PIN_LCD)); //CS low

  startDisplay();
  // Serial.println("LCD Initialized.");
}

//------------------------------------------------------------------------//
//starts the display with required configurations

void LCD_ILI9481::startDisplay() {
  writeCommand(EXIT_SLEEP_MODE); //exit sleep mode
    delay(80);

  writeCommand(POWER_SETTING); //power setting
    writeData8(0x07U);
    writeData8(0x42U);
    writeData8(0x18U);

  writeCommand(VCOM_CONTROL); //VCOM control
    writeData8(0x00U);
    writeData8(0x07U);
    writeData8(0x10U);

  writeCommand(POWER_SETTING_NORMAL); //power setting for normal mode
    writeData8(0x01U);
    writeData8(0x02U); //Fosc setting

  writeCommand(PANEL_DRIVE_SETTING); //panel driving setting - 5 parameters
    writeData8(0x10U); //REV=1, SM=0, GS=0 - grayscale inversion enabled : will invert colors
    //writeData8(0x00); //REV=0, SM=0, GS=0 - no color inversion

    writeData8(0x3BU); //NL[5] - max lines
    writeData8(0x00U); //SCN - scanning start position
    writeData8(0x0U); //PTV=1 (halts vcom)
    writeData8(0x02U); //NDL (non-display area o/p level), PTS[3]

    writeData8(0x1U); //PTG=0 (interval scan), ISC[3]=0001 (3 frames)
    // writeData8(0x11); //PTG=1 (interval scan), ISC[3]=0001 (3 frames)
    // writeData8(0x1F); //PTG=1 (interval scan), ISC[3]=1111 (31 frames)
    // writeData8(0x01); //PTG=0 (normal scan), ISC[3]=0002 (3 frames)

  writeCommand(FRAME_RATE_CONTROL); //frame rate and inversion control - 1 parameter
    //writeData8(0x03); //72FPS (default) - this had backlight flickering
    writeData8(0x00U); //125 FPS (max) - this has no flickering

  writeCommand(FRAME_MEMORY_ACCESS_SETTING); //frame memeory access and interface setting
    writeData8(0x0U); //extra data is ignored
    writeData8(0x0U); //all defaults
    writeData8(0x0U);
    writeData8(0x0U);

  writeCommand(SET_TEAR_OFF); //tearing compensation
    // writeData8(0x0);

  writeCommand(TIMING_SETTING_NORMAL); //display timing for normal mode
    writeData8(0x10U); //BC=0 (frame inversion waveform)
    writeData8(0x10U); //line period is 16 clocks
    writeData8(0x22U); //front and back porch lines = 2

  writeCommand(GAMMA_SETTING); //gamma setting
    writeData8(0x00U);
    writeData8(0x32U);
    writeData8(0x36U);
    writeData8(0x45U);
    writeData8(0x06U);
    writeData8(0x16U);
    writeData8(0x37U);
    writeData8(0x75U);
    writeData8(0x77U);
    writeData8(0x54U);
    writeData8(0x0CU);
    writeData8(0x00U);

  writeCommand(SET_ADDRESS_MODE); //set address mode - 1 parameter
    writeData8(0x0AU); //BGR, horizontal flip

  writeCommand(ENTER_NORMAL_MODE); //eneter normal mode - no param

  writeCommand(SET_PIXEL_FORMAT); //set pixel format - 1 parameter
    writeData8(0x55U); //16-bit per pixel
    // writeData(0x66); //18-bit per pixel

  setAddrWindow(0, 0, _width, _height);
  //writeCommand(0x2A); //set column address
    //writeData8(0x00);
    //writeData8(0x00);
    //writeData8(0x01);
    //writeData8(0x3F);

  //writeCommand(0x2B); //set page address
    //writeData8(0x00);
    //writeData8(0x00);
    //writeData8(0x01);
    //writeData8(0xDF);
    delay(120);

  writeCommand(SET_DISPLAY_ON); //set display on
}

//------------------------------------------------------------------------//
//writes 8 bit data

void LCD_ILI9481::writeData (uint8_t inputData) {
  GPIO.out &= (~(PORT_MASK << _CS_PIN_LCD)); //CS low
  GPIO.out |= (PORT_MASK << _RST_PIN_LCD); //RST high
  GPIO.out |= (PORT_MASK << _DC_PIN_LCD); //DC high
  GPIO.out |= (PORT_MASK << _WR_PIN_LCD); //WR high

  writeToPins(inputData);

  // Serial.print("B : ");
  // Serial.println(millis());
}

//------------------------------------------------------------------------//
//writes 8 bit command

void LCD_ILI9481::writeCommand (uint8_t inputCommand) {
  GPIO.out &= (~(PORT_MASK << _CS_PIN_LCD)); //CS low
  GPIO.out |= (PORT_MASK << _RST_PIN_LCD); //RST high
  GPIO.out &= (~(PORT_MASK << _DC_PIN_LCD)); //DC low
  GPIO.out |= (PORT_MASK << _WR_PIN_LCD); //WR high

  writeToPins(inputCommand);

  GPIO.out |= (PORT_MASK << _DC_PIN_LCD); //DC high

  // Serial.print("C : ");
  // Serial.println(millis());
}

//------------------------------------------------------------------------//
//writes 16 bit data to the 8-bit bus

void LCD_ILI9481::writeData16 (uint16_t inputData) {
  GPIO.out |= (PORT_MASK << _DC_PIN_LCD); //DC high
  writeToPins(((inputData >> 8) & 0x00FF));
  writeToPins(inputData & 0x00FF);

  // Serial.print("D : ");
  // Serial.println(millis());
}

//------------------------------------------------------------------------//
//writes 8-bit data to 8-bit bus

void LCD_ILI9481::writeData8 (uint8_t inputData) {
  GPIO.out |= (PORT_MASK << _DC_PIN_LCD); //DC high
  writeToPins(inputData);

  // Serial.print("E : ");
  // Serial.println(millis());
}

//------------------------------------------------------------------------//
//writes 8-bit data to individual pins
//LSB is ANDed with 1, and writes HIGH is the ouput is 1, LOW otherwise
//then the data is shifted right to do this for all 8 bits

void LCD_ILI9481::writeToPins (uint32_t inputData) {
  ((inputData >> 0) & 0x01) ? (GPIO.out_w1ts |= ((uint32_t)1 << _PD0)) : (GPIO.out_w1tc |= ((uint32_t)1 << _PD0));
  ((inputData >> 1) & 0x01) ? (GPIO.out_w1ts |= ((uint32_t)1 << _PD1)) : (GPIO.out_w1tc |= ((uint32_t)1 << _PD1));
  ((inputData >> 2) & 0x01) ? (GPIO.out_w1ts |= ((uint32_t)1 << _PD2)) : (GPIO.out_w1tc |= ((uint32_t)1 << _PD2));
  ((inputData >> 3) & 0x01) ? (GPIO.out_w1ts |= ((uint32_t)1 << _PD3)) : (GPIO.out_w1tc |= ((uint32_t)1 << _PD3));
  ((inputData >> 4) & 0x01) ? (GPIO.out_w1ts |= ((uint32_t)1 << _PD4)) : (GPIO.out_w1tc |= ((uint32_t)1 << _PD4));
  ((inputData >> 5) & 0x01) ? (GPIO.out_w1ts |= ((uint32_t)1 << _PD5)) : (GPIO.out_w1tc |= ((uint32_t)1 << _PD5));
  ((inputData >> 6) & 0x01) ? (GPIO.out_w1ts |= ((uint32_t)1 << _PD6)) : (GPIO.out_w1tc |= ((uint32_t)1 << _PD6));
  ((inputData >> 7) & 0x01) ? (GPIO.out_w1ts |= ((uint32_t)1 << _PD7)) : (GPIO.out_w1tc |= ((uint32_t)1 << _PD7));

  GPIO.out_w1tc |= (PORT_MASK << _WR_PIN_LCD); //WR low
  // delayMicroseconds(100);
  GPIO.out_w1ts |= (PORT_MASK << _WR_PIN_LCD); //WR high

  // Serial.print("F : ");
  // Serial.println(millis());
}

//------------------------------------------------------------------------//

void LCD_ILI9481::displayOn () {
    writeCommand (SET_DISPLAY_ON); //set display on
}

//------------------------------------------------------------------------//

void LCD_ILI9481::displayOff () {
    writeCommand (SET_DISPLAY_OFF); //set display off
}

//------------------------------------------------------------------------//
//sets the number of pixels (or a rectangular window) data to be written to

void LCD_ILI9481::setAddrWindow (int x0, int y0, int x1, int y1) {
  x1 = (x1 >= _width) ? _width -1 : x1;
  y1 = (y1 >= _height) ? _height -1 : y1;
    writeCommand(SET_COL_ADDRESS); //set col address - 4 param
      writeData16(x0);
      writeData16(x1);

    writeCommand(SET_PAGE_ADDRESS); //set page address - 4 param
      writeData16(y0);
      writeData16(y1);

    writeCommand(WRITE_MEMORY_START); //write memory start
    //writeCommand(WRITE_MEMORY_CONTINUE); //write memory continue
}

//------------------------------------------------------------------------//

void LCD_ILI9481::setRotation (int m) {
  writeCommand (SET_ADDRESS_MODE); //set address mode
  int rotation = m % 4; // can't be higher than 3
  switch (rotation) {
    case 0:
      writeData8(0x0A);
      _width  = Width;
      _height = Height;
      break;
    case 1:
      writeData8(0x28);
      _width  = Height;
      _height = Width;
      break;
    case 2:
      writeData8(0x09);
      _width  = Width;
      _height = Height;
      break;
    case 3:
      writeData8(0x2B);
      _width  = Height;
      _height = Width;
      break;
  }
}

//------------------------------------------------------------------------//

void LCD_ILI9481::invertDisplay (boolean i) {
  writeCommand(i ? ENTER_INVERT_MODE : EXIT_INVERT_MODE);
}

//------------------------------------------------------------------------//

void LCD_ILI9481::openWindow (int x0, int y0, int x1, int y1) {
  setAddrWindow(x0, y0, x0 + x1 - 1, y0 + y1 - 1);
}

//------------------------------------------------------------------------//
//writes a single 16b data to the data lines
void LCD_ILI9481::windowData (uint16_t c) {
    writeData16(c);
}

//------------------------------------------------------------------------//
//writes an array of data to the data lines

void LCD_ILI9481::windowData (uint16_t *c, int len) {
    for (uint32_t i = 0; i < len; i++) {
        writeData16(c[i]);
    }
}

//------------------------------------------------------------------------//
//sets a single pixel to a color

void LCD_ILI9481::drawPixel (int x, int y, uint16_t color) {
  if((x < 0) ||(x >= _width) || (y < 0) || (y >= _height)) return;
  setAddrWindow(x,y,x,y);
  writeData16(color);
}

//------------------------------------------------------------------------//
//fills the entire screen with a color

void LCD_ILI9481::fillScreen (uint16_t color) {
  //fillRectangle(0, 0,  _width, _height, color);
  setAddrWindow(0, 0, _width, _height);
  for(int i=0; i<_height; i++) {
    for(int j=0; j<_width; j++) {
      writeData16(color);
    }
  }
}

//------------------------------------------------------------------------//
//fast drawing of horizontal line

void LCD_ILI9481::drawHorizontalLine (int x, int y, int w, uint16_t color) {
  // Rudimentary clipping
  if((x >= _width) || (y >= _height))
    return;
  if((x+w-1) >= _width)
    w = _width-x;
  setAddrWindow(x, y, x+w-1, y);

  while (w--) {
    writeData16(color);
  }
}

//------------------------------------------------------------------------//
//fast drawing of vertical line

void LCD_ILI9481::drawVerticalLine (int x, int y, int h, uint16_t color) {
  if((x >= _width) || (y >= _height))
    return;
  if((y+h-1) >= _height)
    h = _height-y;
  setAddrWindow(x, y, x, y+h-1);

  while (h--) {
    writeData16(color);
  }
}


//------------------------------------------------------------------------//
//bressenham's line algorithm

void LCD_ILI9481::drawLine (int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
    int16_t steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep) {
        _swap_int16_t(x0, y0);
        _swap_int16_t(x1, y1);
    }

    if (x0 > x1) {
        _swap_int16_t(x0, x1);
        _swap_int16_t(y0, y1);
    }

    int16_t dx, dy;
    dx = x1 - x0;
    dy = abs(y1 - y0);

    int16_t err = dx / 2;
    int16_t ystep;

    if (y0 < y1) {
        ystep = 1;
    } else {
        ystep = -1;
    }

    for (; x0<=x1; x0++) {
        if (steep) {
            drawPixel(y0, x0, color);
        } else {
            drawPixel(x0, y0, color);
        }
        err -= dy;
        if (err < 0) {
            y0 += ystep;
            err += dx;
        }
    }
}

//------------------------------------------------------------------------//
// Draw a rectangle

void LCD_ILI9481::drawRectangle (int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
  drawHorizontalLine(x, y, w, color);
  drawHorizontalLine(x, y+h-1, w, color);
  drawVerticalLine(x, y, h, color);
  drawVerticalLine(x+w-1, y, h, color);
}

//------------------------------------------------------------------------//
//fills a rectanglar area os display with a color

void LCD_ILI9481::fillRectangle (int x, int y, int w, int h, uint16_t color) { //fillRectangle(0, 0, 480, 320, 6500);
  if((x >= _width) || (y >= _height))
    return;
  if((x + w - 1) >= _width)
    w = _width  - x;
  if((y + h - 1) >= _height)
    h = _height - y;
  setAddrWindow(x, y, x+w-1, y+h-1);

  for(y=h*2; y>0; y--) {
    for(x=w; x>0; x--) {
      writeData16(color);
    }
  }
}

//------------------------------------------------------------------------//
// Draw a circle outline

void LCD_ILI9481::drawCircle (int16_t x0, int16_t y0, int16_t r, uint16_t color) {
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;

  drawPixel(x0  , y0+r, color);
  drawPixel(x0  , y0-r, color);
  drawPixel(x0+r, y0  , color);
  drawPixel(x0-r, y0  , color);

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;

    drawPixel(x0 + x, y0 + y, color);
    drawPixel(x0 - x, y0 + y, color);
    drawPixel(x0 + x, y0 - y, color);
    drawPixel(x0 - x, y0 - y, color);
    drawPixel(x0 + y, y0 + x, color);
    drawPixel(x0 - y, y0 + x, color);
    drawPixel(x0 + y, y0 - x, color);
    drawPixel(x0 - y, y0 - x, color);
  }
}

//------------------------------------------------------------------------//

void LCD_ILI9481::drawCircleHelper (int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color) {
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    while (x<y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;
        if (cornername & 0x4) {
            drawPixel(x0 + x, y0 + y, color);
            drawPixel(x0 + y, y0 + x, color);
        }
        if (cornername & 0x2) {
            drawPixel(x0 + x, y0 - y, color);
            drawPixel(x0 + y, y0 - x, color);
        }
        if (cornername & 0x8) {
            drawPixel(x0 - y, y0 + x, color);
            drawPixel(x0 - x, y0 + y, color);
        }
        if (cornername & 0x1) {
            drawPixel(x0 - y, y0 - x, color);
            drawPixel(x0 - x, y0 - y, color);
        }
    }
}

//------------------------------------------------------------------------//

void LCD_ILI9481::fillCircle (int16_t x0, int16_t y0, int16_t r, uint16_t color) {
  drawVerticalLine(x0, y0-r, 2*r+1, color);
  fillCircleHelper(x0, y0, r, 3, 0, color);
}

//------------------------------------------------------------------------//
// Used to do circles and roundrects

void LCD_ILI9481::fillCircleHelper (int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color) {
    int16_t f  = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    while (x<y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        if (cornername & 0x1) {
            drawVerticalLine(x0+x, y0-y, 2*y+1+delta, color);
            drawVerticalLine(x0+y, y0-x, 2*x+1+delta, color);
        }
        if (cornername & 0x2) {
            drawVerticalLine(x0-x, y0-y, 2*y+1+delta, color);
            drawVerticalLine(x0-y, y0-x, 2*x+1+delta, color);
        }
    }
}

//------------------------------------------------------------------------//
// Draw a rounded rectangle

void LCD_ILI9481::drawRoundRectangle (int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color) {
  // smarter version
  drawHorizontalLine(x+r, y, w-2*r, color); // Top
  drawHorizontalLine(x+r, y+h-1, w-2*r, color); // Bottom
  drawVerticalLine(x, y+r, h-2*r, color); // Left
  drawVerticalLine(x+w-1, y+r, h-2*r, color); // Right
  // draw four corners
  drawCircleHelper(x+r, y+r, r, 1, color);
  drawCircleHelper(x+w-r-1, y+r, r, 2, color);
  drawCircleHelper(x+w-r-1, y+h-r-1, r, 4, color);
  drawCircleHelper(x+r, y+h-r-1, r, 8, color);
}

//------------------------------------------------------------------------//
// Fill a rounded rectangle

void LCD_ILI9481::fillRoundRectangle (int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color) {
  // smarter version
  fillRectangle(x+r, y, w-2*r, h, color);
  // draw four corners
  fillCircleHelper(x+w-r-1, y+r, r, 1, h-2*r-1, color);
  fillCircleHelper(x+r    , y+r, r, 2, h-2*r-1, color);
}

//------------------------------------------------------------------------//
// Draw a triangle

void LCD_ILI9481::drawTriangle (int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) {
    drawLine(x0, y0, x1, y1, color);
    drawLine(x1, y1, x2, y2, color);
    drawLine(x2, y2, x0, y0, color);
}

//------------------------------------------------------------------------//
// Fill a triangle

void LCD_ILI9481::fillTriangle (int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) {
    int16_t a, b, y, last;

    // Sort coordinates by Y order (y2 >= y1 >= y0)
    if (y0 > y1) {
        _swap_int16_t(y0, y1); _swap_int16_t(x0, x1);
    }
    if (y1 > y2) {
        _swap_int16_t(y2, y1); _swap_int16_t(x2, x1);
    }
    if (y0 > y1) {
        _swap_int16_t(y0, y1); _swap_int16_t(x0, x1);
    }

    if(y0 == y2) { // Handle awkward all-on-same-line case as its own thing
        a = b = x0;
        if(x1 < a)      a = x1;
        else if(x1 > b) b = x1;
        if(x2 < a)      a = x2;
        else if(x2 > b) b = x2;
        drawHorizontalLine(a, y0, b-a+1, color);
        return;
    }

    int16_t
    dx01 = x1 - x0,
    dy01 = y1 - y0,
    dx02 = x2 - x0,
    dy02 = y2 - y0,
    dx12 = x2 - x1,
    dy12 = y2 - y1;
    int32_t
    sa   = 0,
    sb   = 0;

    // For upper part of triangle, find scanline crossings for segments
    // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
    // is included here (and second loop will be skipped, avoiding a /0
    // error there), otherwise scanline y1 is skipped here and handled
    // in the second loop...which also avoids a /0 error here if y0=y1
    // (flat-topped triangle).
    if(y1 == y2) last = y1;   // Include y1 scanline
    else         last = y1-1; // Skip it

    for(y=y0; y<=last; y++) {
        a   = x0 + sa / dy01;
        b   = x0 + sb / dy02;
        sa += dx01;
        sb += dx02;
        /* longhand:
        a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
        b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
        */
        if(a > b) _swap_int16_t(a,b);
        drawHorizontalLine(a, y, b-a+1, color);
    }

    // For lower part of triangle, find scanline crossings for segments
    // 0-2 and 1-2.  This loop is skipped if y1=y2.
    sa = dx12 * (y - y1);
    sb = dx02 * (y - y0);
    for(; y<=y2; y++) {
        a   = x1 + sa / dy12;
        b   = x0 + sb / dy02;
        sa += dx12;
        sb += dx02;
        /* longhand:
        a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
        b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
        */
        if(a > b) _swap_int16_t(a,b);
        drawHorizontalLine(a, y, b-a+1, color);
    }
}

//------------------------------------------------------------------------//
//draws a single icon to the display

void LCD_ILI9481::drawIcon (fontAwesome* icon, int16_t x, int16_t y, uint16_t color, uint16_t bg) {

  char verticalByte = 0; //for vertical bytes

  //iterate through the _width of the glyph
  for(int i=0; i < (icon->fontArray[0]); i++) { //finds start of each set of vertical bytes
    //get a single set of vertical bytes
    for(int j=0; j < icon->verticalByteCount; j++) {
      verticalByte = icon->fontArray[(j+1) + (i * icon->verticalByteCount)]; //copy the set of vertical bytes
      if(verticalByte != 0) { //don't display if a byte is zero
        for(int m=0; m < 8; m++) { //iterates through each pixel of a byte
          if((unsigned(verticalByte >> m)) & 0x1) { //check if a bit is 1
            drawPixel(x+i, y + m + (8*j), color); //if a bit is 1, then draw pixel with color
          }
          else {
            drawPixel(x+i, y + m + (8*j), bg); //draw pixel with bg if otherwise
          }
        }
      }
    }
  }
}

//------------------------------------------------------------------------//
//draws a single char to the display

void LCD_ILI9481::drawChar (char letter, int16_t x, int16_t y, uint16_t color, uint16_t bg, fontClass* selectedFont) {

  char verticalByte = 0; //for vertical bytes
  int iterateStep = ((letter - selectedFont->startChar) * selectedFont->charByteLength); //bytes need to be jumped to acquire the starting of the next char

  //iterate through the width of the glyph
  for(int i=0; i < (selectedFont->fontArray[iterateStep]); i++) { //finds start of each set of vertical bytes
    //get a single set of vertical bytes
    for(int j=0; j < selectedFont->verticalByteCount; j++) {
      verticalByte = selectedFont->fontArray[iterateStep + (j+1) + (i * selectedFont->verticalByteCount)]; //copy the set of vertical bytes
      if(verticalByte != 0) { //don't display if a byte is zero
        for(int m=0; m < 8; m++) { //iterates though each pixel of a byte
          if((unsigned(verticalByte >> m)) & 0x1) { //check if a bit is 1
            drawPixel(x+i, y + m + (8*j), color); //if a bit is 1, then draw pixel with color
          }
          else {
            drawPixel(x+i, y + m + (8*j), bg); //draw pixel with bg if otherwise
          }
        }
      }
    }
  }
}

//------------------------------------------------------------------------//

void LCD_ILI9481::printText (String S, int16_t x, int16_t y, uint16_t color, uint16_t bg, fontClass* selectedFont) {
  char stringBuffer[S.length()+1];
  S.toCharArray(stringBuffer, S.length()+1); //create a char array

  for(int i=0; i < S.length(); i++) {
    if(((stringBuffer[i] < selectedFont->startChar) || (stringBuffer[i] > selectedFont->endChar)) && (stringBuffer[i] != 32)) { //check for char validity
      Serial.print("Illegal character found : ");
      Serial.print((int)stringBuffer[i]);
      Serial.print(" at ");
      Serial.println(i);
      Serial.println("String Length : " + S.length());
      return;
    }
  }

  int cumulatedLength = 0;

  for(int i=0; i < S.length(); i++) {
    if(i==0) {
      if(stringBuffer[i] == 32) { //if the char is a space
        cumulatedLength += 10; //width for space char
        // Serial.println("Writing space..");
      }
      else {
        // Serial.print("Writing first char : ");
        // Serial.println(stringBuffer[i]);
        drawChar(stringBuffer[i], x, y, color, bg, selectedFont);
        cumulatedLength += selectedFont->fontArray[((stringBuffer[i] - selectedFont->startChar) * selectedFont->charByteLength)] + 2;
      }
    }
    else {
      if(stringBuffer[i] == 32) { //if the char is a space
        // Serial.println("Writing space..");
        cumulatedLength += 10; //width for space char
      }
      else {
        // Serial.print("Writing char : ");
        // Serial.println(stringBuffer[i]);
        drawChar(stringBuffer[i], (x + cumulatedLength), y, color, bg, selectedFont);
        cumulatedLength += selectedFont->fontArray[((stringBuffer[i] - selectedFont->startChar) * selectedFont->charByteLength)] + 2;
      }
    }
  }
}

//========================================================================//

fontAwesome::fontAwesome (const char* a, int w, int h, int c, int v, uint32_t u, String n) {
  fontArray = a;
  fontWidth = w;
  fontHeight = h;
  charByteLength = c;
  verticalByteCount = v;
  unicodeId = u;
  name = n;
  glyphWidth = 0;
  glyphHeight = 0;
  glyphX = 0;
  glyphY = 0;
}

//------------------------------------------------------------------------//
//gets the width, height and offsets of a icon, char or string in pixels
//an offset is simply how many lines of empty pixels are before a valid black pixel

void fontAwesome::getSize() {
  char verticalByte = 0; //for vertical bytes

  bool areAllBytesZero = true; //whether all bytes in a verticl set are zero
  bool nonZeroByteFound = false; //whether any non-zero bytes found in any vertical set

  int glyphOffsetLeft = 0;
  int glyphOffsetTop = -1; //-1 so that we can have a start condition without conflicting with actual 0 offset value
  int glyphOffsetBottom = 0;
  int glyphOffsetRight = 0;

  //iterate through the width of the glyph
  for(int i=0; i < (fontArray[0]); i++) { //finds start of each set of vertical bytes
    //get a single set of vertical bytes
    for(int j=0; j < verticalByteCount; j++) {
      verticalByte = fontArray[(j+1) + (i * verticalByteCount)]; //copy the set of vertical bytes
      if(verticalByte != 0) { //don't display if a byte is zero
        areAllBytesZero = false;
        nonZeroByteFound = true;
        glyphOffsetRight = i; //if there's a non-zero vertical byte in a cloumn, save it. This will eventually find the right-most pixel
        for(int m=0; m < 8; m++) { //iterates through each pixel of a byte
          if((unsigned(verticalByte >> m)) & 0x1) { //check if a bit is 1
            if(glyphOffsetTop < 0) glyphOffsetTop = (j*8)+m; //assign the location of the first pixel found
            else if(glyphOffsetTop > ((j*8)+m)) glyphOffsetTop = (j*8)+m; //now check if any pixel higher than previous found
            if(glyphOffsetBottom < ((j*8)+m)) glyphOffsetBottom = (j*8)+m;
          }
        }
      }
    }

    if(areAllBytesZero && (!nonZeroByteFound)) {
      glyphOffsetLeft++;
    }
    areAllBytesZero = true;
  }

  if(glyphOffsetTop < 0) glyphOffsetTop = 0;

  glyphX = glyphOffsetLeft;
  glyphY = glyphOffsetTop;

  // glyphOffsetRight = fontWidth - (glyphOffsetRight + 1); //find the Right offset from right-most pixel location
  // glyphOffsetBottom = fontHeight - (glyphOffsetBottom + 1);

  glyphWidth = fontWidth - (glyphOffsetLeft + (fontWidth - (glyphOffsetRight + 1)));
  glyphHeight = fontHeight - (glyphOffsetTop + (fontHeight - (glyphOffsetBottom + 1)));
}

//========================================================================//

fontClass::fontClass (const char* a, int w, int h, int c, int v, uint32_t s, uint32_t e, int n) {
  fontArray = a;
  fontWidth = w;
  fontHeight = h;
  charByteLength = c;
  verticalByteCount = v;
  startChar = s;
  endChar = e;
  charCount = n;
}

//------------------------------------------------------------------------//
//gets the width, height and offsets of a icon, char or string in pixels

void fontClass::getSize (char letter) {
  char verticalByte = 0; //for vertical bytes
  int iterateStep = ((letter - startChar) * charByteLength); //bytes need to be jumped to acquire the starting of the next char

  bool areAllBytesZero = true; //whether all bytes in a verticl set are zero
  bool nonZeroByteFound = false; //whether any non-zero bytes found in any vertical set

  int glyphOffsetLeft = 0;
  int glyphOffsetTop = -1; //-1 so that we can have a start condition without conflicting with actual 0 offset value
  int glyphOffsetBottom = 0;
  int glyphOffsetRight = 0;

  //iterate through the width of the glyph
  for(int i=0; i < (fontArray[iterateStep]); i++) { //finds start of each set of vertical bytes

    //get a single set of vertical bytes
    for(int j=0; j < verticalByteCount; j++) {
      verticalByte = fontArray[iterateStep + (j+1) + (i * verticalByteCount)]; //copy the set of vertical bytes
      if(verticalByte != 0) { //don't display if a byte is zero
        areAllBytesZero = false;
        nonZeroByteFound = true; //because there can be empty columns inside a char
        glyphOffsetRight = i; //if there's a non-zero vertical byte in a cloumn, save it. This will eventually find the right-most pixel
        for(int m=0; m < 8; m++) { //iterates through each pixel of a byte
          if((unsigned(verticalByte >> m)) & 0x1) { //check if a bit is 1
            if(glyphOffsetTop < 0) glyphOffsetTop = (j*8)+m; //assign the location of the first pixel found
            else if(glyphOffsetTop > ((j*8)+m)) glyphOffsetTop = (j*8)+m; //now check if any pixel higher than previous found
            if(glyphOffsetBottom < ((j*8)+m)) glyphOffsetBottom = (j*8)+m;
          }
        }
      }
    }

    if(areAllBytesZero && (!nonZeroByteFound)) { //only count the empty columns before any valid pixels
      glyphOffsetLeft++;
    }
    areAllBytesZero = true;
  }

  if(glyphOffsetTop < 0) glyphOffsetTop = 0; //in case if a char has max height, offset will be zero

  glyphX = glyphOffsetLeft; //the relative XY of the min rectangle that encloses the char
  glyphY = glyphOffsetTop;

  glyphOffsetRight = fontWidth - (glyphOffsetRight + 1); //find the Right offset from right-most pixel location
  glyphOffsetBottom = fontHeight - (glyphOffsetBottom + 1);

  glyphWidth = fontWidth - (glyphOffsetLeft + glyphOffsetRight);
  glyphHeight = fontHeight - (glyphOffsetTop + glyphOffsetBottom);
}

//------------------------------------------------------------------------//
//finds the width, height and XY in terms of pixels of a string printed to the display

void fontClass::getSize (String S) {
  char stringBuffer[S.length()+1];
  S.toCharArray(stringBuffer, S.length()+1); //create a char array

  for(int i=0; i < S.length(); i++) {
    if(((stringBuffer[i] < startChar) || (stringBuffer[i] > endChar)) && (stringBuffer[i] != 32)) { //check for char validity
      Serial.print("Illegal character found : ");
      Serial.print((int)stringBuffer[i]);
      Serial.print(" at ");
      Serial.println(i);
      Serial.println("String Length : " + S.length());
      return;
    }
  }

  int cumulatedLength = 0; //this will yield the string width inclusive of space
  int stringX = 0; //relative X and Y of the smallest rectangle that holds string
  int stringY = 0;
  int stringWidth = 0; //width and height of that smallest rectangle
  int stringHeight = 0;

  for(int i=0; i < S.length(); i++) {
    if(i==0) { //for the first char only
      if(stringBuffer[i] == 32) { //if the char is a space
        cumulatedLength += 10; //width for space char
        stringX = 0; //space char is considered for width calculation
      }
      else {
        getSize(stringBuffer[i]); //determine the parameters of a single char
        stringX = glyphX; //save first char's X and Y
        stringY = glyphY;
        stringHeight = glyphHeight;
        cumulatedLength += fontArray[((stringBuffer[i] - startChar) * charByteLength)] + 2;
      }
    }
    else {
      if(stringBuffer[i] == 32) { //if the char is a space
        cumulatedLength += 10; //width for space char
      }
      else {
        getSize(stringBuffer[i]); //determine the parameters of a single char
        if(stringY > glyphY) stringY = glyphY; //we need the smallest of all Ys of all chars
        if(stringHeight < glyphHeight) stringHeight = glyphHeight; //save the largest height from all chars
        cumulatedLength += fontArray[((stringBuffer[i] - startChar) * charByteLength)] + 2;
      }
    }
  }
  glyphX = stringX;
  glyphY = stringY;
  glyphWidth = stringWidth = cumulatedLength - 2; //cumulatedLength will be the effective width of the string
  glyphHeight = stringHeight;
}

//========================================================================//

buttonClass::buttonClass (int a, int b, int c, int d, int e, uint16_t f, uint16_t g, uint16_t h, uint16_t i,
                          String j, fontClass* k, fontAwesome* l, uint16_t m, uint16_t n, uint16_t o, uint16_t p,
                          bool q, bool r, bool s, bool t, bool u, bool v, bool w, bool x, bool y, bool z) {
  x = a;
  y = b;
  width = c;
  height = d;
  radius = e;
  borderColor = f;
  borderHoverColor = g;
  fillColor = h;
  fillHoverColor = i;
  label = j;
  labelFont = k;
  icon = l;
  labelColor = m;
  labelHoverColor = n;
  iconColor = o;
  iconHoverColor = p;
  buttonEnabled = q;
  borderEnabled = r;
  fillEnabled = s;
  labelEnabled = t;
  iconEnabled = u;
  buttonHoverEnabled = v;
  borderHoverEnabled = w;
  fillHoverEnabled = x;
  labelHoverEnabled = y;
  iconHoverEnabled = z;
}

//------------------------------------------------------------------------//

void buttonClass::draw () {
  if(buttonEnabled) {
    if(fillEnabled) {
      // fillRoundRectangle(x, y, width, height, radius, fillColor);
    }
    if(borderEnabled) {
      // drawRoundRectangle(x, y, width, height, radius, borderColor);
    }
    if(iconEnabled) {
      // icon->getSize();
      // drawIcon(icon, x+(int((width - icon->glyphWidth)/2)), y+(int((height - icon->glyphHeight)/2)), iconColor, fillColor);
    }
    if(labelEnabled) {

    }
  }
}

//------------------------------------------------------------------------//

void buttonClass::show () {
  buttonEnabled = true;
}

//------------------------------------------------------------------------//

void buttonClass::hide () {
  buttonEnabled = false;
}

//------------------------------------------------------------------------//

void buttonClass::hoverEnable () {
  buttonHoverEnabled = true;
}

//------------------------------------------------------------------------//

void buttonClass::hoverDisable () {
  buttonHoverEnabled = false;
}

//------------------------------------------------------------------------//

void buttonClass::showBorder () {
  borderEnabled = true;
}

//------------------------------------------------------------------------//

void buttonClass::hideBorder () {
  borderEnabled = false;
}

//------------------------------------------------------------------------//

void buttonClass::borderHoverDisable () {
  borderHoverEnabled = false;
}

//------------------------------------------------------------------------//

void buttonClass::borderHoverEnable () {
  borderHoverEnabled = true;
}

//------------------------------------------------------------------------//

void buttonClass::showFill () {
  fillEnabled = true;
}

//------------------------------------------------------------------------//

void buttonClass::hideFill () {
  fillEnabled = false;
}

//------------------------------------------------------------------------//

void buttonClass::fillHoverEnable () {
  fillHoverEnabled = true;
}

//------------------------------------------------------------------------//

void buttonClass::fillHoverDisable () {
  fillHoverEnabled = false;
}

//------------------------------------------------------------------------//

void buttonClass::showLabel () {
  labelEnabled = true;
}

//------------------------------------------------------------------------//

void buttonClass::hideLabel () {
  labelEnabled = false;
}

//------------------------------------------------------------------------//

void buttonClass::labelHoverEnable () {
  labelHoverEnabled = true;
}

//------------------------------------------------------------------------//

void buttonClass::labelHoverDisable () {
  labelHoverEnabled = false;
}

//------------------------------------------------------------------------//

void buttonClass::showIcon () {
  iconEnabled = true;
}

//------------------------------------------------------------------------//

void buttonClass::hideIcon () {
  iconEnabled = false;
}

//------------------------------------------------------------------------//

void buttonClass::iconHoverEnable () {
  iconHoverEnabled = true;
}

//------------------------------------------------------------------------//

void buttonClass::iconHoverDisable () {
  iconHoverEnabled = false;
}

//========================================================================//
