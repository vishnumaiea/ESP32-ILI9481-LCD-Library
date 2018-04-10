
//========================================================================//
//                                                                        //
//  ILI9481 3.2" 320 x 480 TFT LCD Driver for ESP32                       //
//                                                                        //
//  File : Demo.ino                                                       //
//  Author : Vishnu M Aiea                                                //
//  GitHub : https://github.com/vishnumaiea/ESP32-ILI9481-LCD-Library     //
//  License : GNU GPL v3                                                  //
//  Date Created : IST 04:07:07 PM, 30-10-2017, Monday                    //
//  Last Modified : +05:30 11:05:18 PM, 10-04-2018, Tuesday               //
//                                                                        //
//========================================================================//

#include <pgmspace.h>
#include <XPT2046_Touchscreen.h>
#include <SPI.h>
#include "ESP32_ILI9481.h"

//pins for touch controller
//uses native SPI port
#define CS_PIN_TS    5   //chip selec pin
#define TIRQ_PIN_TS  34  //interrupt input pin

//pins for LCD
#define CS_PIN_LCD  26
#define RST_PIN_LCD 12
#define DC_PIN_LCD  14
#define WR_PIN_LCD  27

#define PD0   15    //DB8  - D15
#define PD1   2     //DB9  - D2
#define PD2   0     //DB10 - D0
#define PD3   4     //DB11 - D4
#define PD4   16    //DB12 - D16
#define PD5   17    //DB13 - D17
#define PD6   21    //DB14 - D21
#define PD7   22    //DB15 - D22

// XPT2046_Touchscreen ts(CS_PIN_TS, TIRQ_PIN_TS);  // Param 2 - Touch IRQ Pin - interrupt enabled polling
XPT2046_Touchscreen ts(CS_PIN_TS);  // Param 2 - Touch IRQ Pin - interrupt enabled polling
// LCD_ILI9481 lcd(15, 2, 0, 4, 16, 17, 21, 22, 26, 12, 14, 27);
LCD_ILI9481 lcd(PD0, PD1, PD2, PD3, PD4, PD5, PD6, PD7, CS_PIN_LCD, RST_PIN_LCD, DC_PIN_LCD, WR_PIN_LCD);

//========================================================================//

const char fa_cog_23x23_fontArray [] = {
          0x17, 0x00, 0x3E, 0x00, 0x00, 0x3E, 0x00, 0x30, 0x3E, 0x06, 0x78, 0x7F, 0x0F, 0xFC, 0xFF, 0x1F,
          0xFC, 0xFF, 0x1F, 0xF8, 0xFF, 0x0F, 0xF0, 0xFF, 0x07, 0xF8, 0xC1, 0x0F, 0xFF, 0x80, 0x7F, 0xFF,
          0x80, 0x7F, 0xFF, 0x80, 0x7F, 0xFF, 0x80, 0x7F, 0xFF, 0x80, 0x7F, 0xF8, 0xC1, 0x0F, 0xF0, 0xFF,
          0x07, 0xF8, 0xFF, 0x0F, 0xFC, 0xFF, 0x1F, 0xFC, 0xFF, 0x1F, 0x78, 0x7F, 0x0F, 0x30, 0x3E, 0x06,
          0x00, 0x3E, 0x00, 0x00, 0x3E, 0x00
        };


const char fa_cog_32x32_fontArray [] = {
        0x20, 0x00, 0xE0, 0x07, 0x00, 0x00, 0xE0, 0x07, 0x00, 0x00, 0xF0, 0x07, 0x00, 0xC0, 0xF0, 0x07,
        0x03, 0xE0, 0xF1, 0xCF, 0x07, 0xF0, 0xFF, 0xFF, 0x0F, 0xF8, 0xFF, 0xFF, 0x1F, 0xF8, 0xFF, 0xFF,
        0x1F, 0xF0, 0xFF, 0xFF, 0x0F, 0xF0, 0xFF, 0xFF, 0x07, 0xE0, 0xFF, 0xFF, 0x07, 0xE0, 0x1F, 0xF8,
        0x07, 0xFC, 0x0F, 0xF0, 0x1F, 0xFF, 0x07, 0xE0, 0xFF, 0xFF, 0x07, 0xE0, 0xFF, 0xFF, 0x07, 0xE0,
        0xFF, 0xFF, 0x07, 0xE0, 0xFF, 0xFF, 0x07, 0xE0, 0xFF, 0xFF, 0x07, 0xF0, 0xFF, 0xF0, 0x0F, 0xF0,
        0x07, 0xE0, 0x1F, 0xFC, 0x07, 0xE0, 0xFF, 0xFF, 0x07, 0xF0, 0xFF, 0xFF, 0x07, 0xF8, 0xFF, 0xFF,
        0x0F, 0xF8, 0xFF, 0xFF, 0x1F, 0xF8, 0xFF, 0xFF, 0x0F, 0xF0, 0xFF, 0xFF, 0x0F, 0xE0, 0xF1, 0x87,
        0x07, 0xC0, 0xF0, 0x07, 0x01, 0x00, 0xE0, 0x07, 0x00, 0x00, 0xE0, 0x07, 0x00, 0x00, 0xE0, 0x07,
        0x00
      };


fontAwesome* fa_cog_23 = new fontAwesome (fa_cog_23x23_fontArray, 23, 23, 70, 3, 0xF013, "fa-cog", &lcd);
fontAwesome* fa_cog_32 = new fontAwesome (fa_cog_32x32_fontArray, 32, 32, 129, 4, 0xF013, "fa-cog", &lcd);

//62172 - fa-snowflake-o - f2dc
//61549 - fa-fire - f06d
//61457 - fa-power-off - f011
//61751 - fa-chevron-circle-left - f137
//61776 - fa-caret-square-o-down - f150
//61777 - fa-caret-square-o-up - f151
//62056 - fa-chrome (compressor) - f268
//62173 - fa-superpowers (compressor) - f2dd
//61507 - fa-tint - f043
//61657 - fa-caret-left - f0d9
//61714 - fa-reply (back button) - f112

//========================================================================//
//font specific declarations

#include "Montserrat_Medium_21x20.h"
#include "Poppins_Medium_34x36.h"
#include "Poppins_Medium_23x24.h"
#include "Segoe_UI_24x25.h"
#include "Segoe_UI_21x22.h"
#include "Segoe_UI_20x21.h"
#include "Segoe_UI_Semibold_19x21.h"
#include "Segoe_UI_Bold_19x19.h"
#include "Segoe_UI_Bold_29x31.h"
#include "Segoe_UI_Bold_27x27.h"
#include "Segoe_UI_Bold_32x33.h"

//------------------------------------------------------------------------//

fontClass* Montserrat_Medium_21x20; //create a pointer so that can be initialized later
fontClass* Poppins_Medium_34x36;
fontClass* Poppins_Medium_23x24;
fontClass* Segoe_UI_24x25;
fontClass* Segoe_UI_21x22;
fontClass* Segoe_UI_20x21;
fontClass* Segoe_UI_Semibold_19x21;
fontClass* Segoe_UI_Bold_19x19;
fontClass* Segoe_UI_Bold_29x31;
fontClass* Segoe_UI_Bold_27x27;
fontClass* Segoe_UI_Bold_32x33;

//========================================================================//
//setup function runs once

void setup() {
  //font initializations

  Montserrat_Medium_21x20 = new fontClass (Montserrat_Medium_21x20_fontArray,
                                           Montserrat_Medium_21x20_fontWidth,
                                           Montserrat_Medium_21x20_fontHeight,
                                           Montserrat_Medium_21x20_charByteLength,
                                           Montserrat_Medium_21x20_verticalByteCount,
                                           Montserrat_Medium_21x20_startChar,
                                           Montserrat_Medium_21x20_endChar,
                                           Montserrat_Medium_21x20_charCount);

  Poppins_Medium_34x36 = new fontClass (Poppins_Medium_34x36_fontArray,
                                        Poppins_Medium_34x36_fontWidth,
                                        Poppins_Medium_34x36_fontHeight,
                                        Poppins_Medium_34x36_charByteLength,
                                        Poppins_Medium_34x36_verticalByteCount,
                                        Poppins_Medium_34x36_startChar,
                                        Poppins_Medium_34x36_endChar,
                                        Poppins_Medium_34x36_charCount);

  Poppins_Medium_23x24 = new fontClass (Poppins_Medium_23x24_fontArray,
                                        Poppins_Medium_23x24_fontWidth,
                                        Poppins_Medium_23x24_fontHeight,
                                        Poppins_Medium_23x24_charByteLength,
                                        Poppins_Medium_23x24_verticalByteCount,
                                        Poppins_Medium_23x24_startChar,
                                        Poppins_Medium_23x24_endChar,
                                        Poppins_Medium_23x24_charCount);

  Segoe_UI_24x25 = new fontClass (Segoe_UI_24x25_fontArray,
                                  Segoe_UI_24x25_fontWidth,
                                  Segoe_UI_24x25_fontHeight,
                                  Segoe_UI_24x25_charByteLength,
                                  Segoe_UI_24x25_verticalByteCount,
                                  Segoe_UI_24x25_startChar,
                                  Segoe_UI_24x25_endChar,
                                  Segoe_UI_24x25_charCount);

  Segoe_UI_21x22 = new fontClass (Segoe_UI_21x22_fontArray,
                                  Segoe_UI_21x22_fontWidth,
                                  Segoe_UI_21x22_fontHeight,
                                  Segoe_UI_21x22_charByteLength,
                                  Segoe_UI_21x22_verticalByteCount,
                                  Segoe_UI_21x22_startChar,
                                  Segoe_UI_21x22_endChar,
                                  Segoe_UI_21x22_charCount);

  Segoe_UI_20x21 = new fontClass (Segoe_UI_20x21_fontArray,
                                  Segoe_UI_20x21_fontWidth,
                                  Segoe_UI_20x21_fontHeight,
                                  Segoe_UI_20x21_charByteLength,
                                  Segoe_UI_20x21_verticalByteCount,
                                  Segoe_UI_20x21_startChar,
                                  Segoe_UI_20x21_endChar,
                                  Segoe_UI_20x21_charCount);

  Segoe_UI_Semibold_19x21 = new fontClass (Segoe_UI_Semibold_19x21_fontArray,
                                           Segoe_UI_Semibold_19x21_fontWidth,
                                           Segoe_UI_Semibold_19x21_fontHeight,
                                           Segoe_UI_Semibold_19x21_charByteLength,
                                           Segoe_UI_Semibold_19x21_verticalByteCount,
                                           Segoe_UI_Semibold_19x21_startChar,
                                           Segoe_UI_Semibold_19x21_endChar,
                                           Segoe_UI_Semibold_19x21_charCount);

  Segoe_UI_Bold_19x19 = new fontClass (Segoe_UI_Bold_19x19_fontArray,
                                       Segoe_UI_Bold_19x19_fontWidth,
                                       Segoe_UI_Bold_19x19_fontHeight,
                                       Segoe_UI_Bold_19x19_charByteLength,
                                       Segoe_UI_Bold_19x19_verticalByteCount,
                                       Segoe_UI_Bold_19x19_startChar,
                                       Segoe_UI_Bold_19x19_endChar,
                                       Segoe_UI_Bold_19x19_charCount);

 Segoe_UI_Bold_29x31 = new fontClass (Segoe_UI_Bold_29x31_fontArray,
                                      Segoe_UI_Bold_29x31_fontWidth,
                                      Segoe_UI_Bold_29x31_fontHeight,
                                      Segoe_UI_Bold_29x31_charByteLength,
                                      Segoe_UI_Bold_29x31_verticalByteCount,
                                      Segoe_UI_Bold_29x31_startChar,
                                      Segoe_UI_Bold_29x31_endChar,
                                      Segoe_UI_Bold_29x31_charCount);

 Segoe_UI_Bold_27x27 = new fontClass (Segoe_UI_Bold_27x27_fontArray,
                                      Segoe_UI_Bold_27x27_fontWidth,
                                      Segoe_UI_Bold_27x27_fontHeight,
                                      Segoe_UI_Bold_27x27_charByteLength,
                                      Segoe_UI_Bold_27x27_verticalByteCount,
                                      Segoe_UI_Bold_27x27_startChar,
                                      Segoe_UI_Bold_27x27_endChar,
                                      Segoe_UI_Bold_27x27_charCount);

  Segoe_UI_Bold_32x33 = new fontClass (Segoe_UI_Bold_32x33_fontArray,
                                       Segoe_UI_Bold_32x33_fontWidth,
                                       Segoe_UI_Bold_32x33_fontHeight,
                                       Segoe_UI_Bold_32x33_charByteLength,
                                       Segoe_UI_Bold_32x33_verticalByteCount,
                                       Segoe_UI_Bold_32x33_startChar,
                                       Segoe_UI_Bold_32x33_endChar,
                                       Segoe_UI_Bold_32x33_charCount);


  Serial.begin(115200);
  Serial.print("CPU Clock = ");
  Serial.println(ESP.getCpuFreqMHz());
  Serial.println("Initializing...");
  Serial.println();

  pinMode(TIRQ_PIN_TS, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  pinMode(CS_PIN_LCD, OUTPUT); //set all pins as outputs
  pinMode(RST_PIN_LCD, OUTPUT);
  pinMode(DC_PIN_LCD, OUTPUT);
  pinMode(WR_PIN_LCD, OUTPUT);
  pinMode(PD0, OUTPUT);
  pinMode(PD1, OUTPUT);
  pinMode(PD2, OUTPUT);
  pinMode(PD3, OUTPUT);
  pinMode(PD4, OUTPUT);
  pinMode(PD5, OUTPUT);
  pinMode(PD6, OUTPUT);
  pinMode(PD7, OUTPUT);

  lcd.initializeDisplay();
  lcd.setRotation(1);
  delay(1);
  lcd.setAddrWindow(0, 0, Width, Height);
  lcd.fillScreen(0x6F52);

  ts.begin();
  ts.setRotation(4);

  lcd.fillRoundRectangle(150, 225, 120, 60, 20, 0x54BB);
  lcd.fillRoundRectangle(340, 225, 120, 60, 20, 0x54BB);
  lcd.fillRectangle(0, 0, 100, 320, 0x54BB);
  lcd.fillRoundRectangle(19, 237, 60, 60, 15, 65535U);
  lcd.drawHorizontalLine(0, 35, 100, 65535U);
}

//========================================================================//
//infinite loop

int color = 0;
uint8_t inputData = 34;

void loop() {
  for(unsigned int i=0; i<65535U; i++) {
    lcd.printText("HOT", 187, 245, 65535U, 0x54BB, Segoe_UI_20x21);
    lcd.printText("NORMAL", 356, 245, 65535U, 0x54BB, Segoe_UI_20x21);
    lcd.printText("12:51:23 PM, Wed, 24-08-18", 180, 10, 0x54BB, 0x6F52, Segoe_UI_Bold_19x19);
    lcd.printText("SOME TEXT", 190, 100, 0x54BB, 0x6F52, Segoe_UI_Bold_32x33);
    lcd.printText("00:00:00", 240, 150, 0x54BB, 0x6F52, Segoe_UI_Bold_32x33);
    lcd.drawIcon(fa_cog_32, 34, 251, 0x54BB, 65535U);
    // fillRoundRectangle(100, 100, 200, 50, 20, i);
    // delayMicroseconds(10);
    // fillScreen(65535U);

    if (ts.tirqTouched()) {
      TS_Point p = ts.getPoint();
      if (p.z != 0) {
      Serial.print("Pressure = ");
      Serial.print(p.z);
      Serial.print(", x = ");
      Serial.print(p.x);
      Serial.print(", y = ");
      Serial.print(p.y);
    }
  }
}
