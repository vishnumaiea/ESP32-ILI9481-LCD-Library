
//========================================================================//
//                                                                        //
//  ## ESP32-ILI9481-LCD-Library ##                                       //
//  ILI9481 320 x 480 LCD driver and graphics library for ESP32 boards    //
//                                                                        //
//  Filename : ESP32_ILI9481.h                                            //
//  Description : Part of ILI9481 TFT LCD library.                        //
//  Library version : 2.7                                                 //
//  Author : Vishnu M Aiea                                                //
//  Source : https://github.com/vishnumaiea/ESP32-ILI9481-LCD-Library     //
//  Author's website : www.vishnumaiea.in                                 //
//  Initial release : IST 11:51 PM 07-04-2018, Saturday                   //
//  License : MIT                                                         //
//  Additional source license/s :                                         //
//    1. BSD license @ Adafruit Industries                                //
//       https://github.com/adafruit/Adafruit-GFX-Library                 //
//                                                                        //
//  File last modified : +05:30 12:43:40 PM, 12-05-2018, Saturday         //
//                                                                        //
//========================================================================//

#include <Arduino.h>
#include <stdint.h>
#include "XPT2046_Touchscreen.h"

//========================================================================//
//LCD commands

#define   SET_COL_ADDRESS                 0x2AU
#define   SET_PAGE_ADDRESS                0x2BU
#define   WRITE_MEMORY_START              0x2CU
#define   WRITE_MEMORY_CONTINUE           0x3CU
#define   SET_DISPLAY_OFF                 0x28U
#define   SET_DISPLAY_ON                  0x29U
#define   SET_ADDRESS_MODE                0x36U
#define   EXIT_INVERT_MODE                0x20U
#define   ENTER_INVERT_MODE               0x21U
#define   ENTER_NORMAL_MODE               0x13U //0 param
#define   EXIT_SLEEP_MODE                 0x11U
#define   SET_TEAR_OFF                    0x34U //0 param
#define   SET_TEAR_ON                     0x35U //1 param
#define   SET_PIXEL_FORMAT                0x3AU
#define   SET_TEAR_SCANLINE               0x44U //2 param
#define   FRAME_MEMORY_ACCESS_SETTING     0xB3U //4 param
#define   SET_DISPLAY_MODE                0xB4U //1 param
#define   PANEL_DRIVE_SETTING             0xC0U //6 param
#define   TIMING_SETTING_NORMAL           0xC1U //3 param
#define   TIMING_SETTING_PARTIAL          0xC2U //3 param
#define   FRAME_RATE_CONTROL              0xC5U //1 param
#define   INTERFACE_CONTROL               0xC6U //1 param
#define   POWER_SETTING                   0xD0U //3 param
#define   VCOM_CONTROL                    0xD1U //3 param
#define   POWER_SETTING_NORMAL            0xD2U //2 param
#define   POWER_SETTING_PARTIAL           0xD3U //2 param
#define   GAMMA_SETTING                   0xC8U //12 param

#define _swap_int16_t(a, b) { int16_t t = a; a = b; b = t; } //swaps two 16 bit values

//========================================================================//
//RGB565 color values

#define DISABLED_FG 0xEF3C
#define DISABLED_BG 0xCE59

//========================================================================//
//class declarations

class fontClass;
class fontAwesome;
class buttonClass;
class boxClass;

//========================================================================//
//LCD class

static const int Width = 320; //constants
static const int Height = 480;
static const uint32_t PORT_MASK = 0x1U;

class LCD_ILI9481 {
  private:
    uint8_t _CS_PIN_LCD, _RST_PIN_LCD, _DC_PIN_LCD, _WR_PIN_LCD;
    uint8_t _PD0, _PD1, _PD2, _PD3, _PD4, _PD5, _PD6, _PD7;
    // uint8_t _PD8, _PD9, _PD10, _PD11, _PD12, _PD13, _PD14, _PD15;
    // uint8_t dataPins [] = {PD0, PD1, PD2, PD3, PD4, PD5, PD6, PD7};
    uint8_t _bus_width;
    uint8_t _rotation;

    friend class fontAwesome;
    friend class fontClass;
    friend class buttonClass;
    friend class boxClass;

  public:
    int _width = Width; //becasue we can change these later if needed
    int _height = Height; //eg. in setRotation()

    //------------------------------------------------------------------------//

    LCD_ILI9481 (uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t,
                 uint8_t, uint8_t, uint8_t, uint8_t, uint8_t); //for 8-bit interface (total 12 pins)
    void initializeDisplay (void); //resets the display
    void startDisplay (void); //update config registers with default values
    void setRotation (int); //set rotation of the screen
    void invertDisplay (bool); //inverts colors
    void displayOn (void); //turns on the display
    void displayOff (void); //turns of the display
    void openWindow (int, int, int, int); //opens a custom pixel window with X, Y, width and height
    void windowData (uint16_t); //data values for custom window
    void windowData (uint16_t*, int); //data values as array, and count for custom window
    void writeData (uint8_t); //writes 8-bit data to bus
    void writeCommand (uint8_t); //writes 8-bit command to bus
    void writeData16 (uint16_t); //writes 16-bit data to 8-bit bus
    void writeData8 (uint8_t); //writes any 8-bit data to 8-bit bus
    void writeToPins (uint32_t);
    void setAddrWindow (int, int, int, int); //opens a custom pixel window with X1, Y1, X2, Y2
    void fillScreen (uint16_t); //fills the entire screen with a 16-bit color
    void drawPixel (int, int, uint16_t); //updates the color of a single pixel
    void drawHorizontalLine (int, int, int, uint16_t); //draws horizontal straight line
    void drawVerticalLine (int, int, int, uint16_t); //draws vertical straight line
    void drawLine (int16_t, int16_t, int16_t, int16_t, uint16_t); //draws line between two points
    void drawRectangle (int16_t, int16_t, int16_t,int16_t, uint16_t); //draws a rectangle outline with single pixel thickness
    void fillRectangle (int, int, int, int, uint16_t); //fills a rectangle with a 16-bit color
    void drawRoundRectangle (int16_t, int16_t, int16_t, int16_t, int16_t, uint16_t); //draws a rounded corner rectangle outline
    void fillRoundRectangle (int16_t, int16_t, int16_t, int16_t, int16_t, uint16_t); //fills a rounded corner rectangle with a color
    void drawTriangle (int16_t, int16_t, int16_t, int16_t, int16_t, int16_t, uint16_t); //draws a triangle outline
    void fillTriangle (int16_t, int16_t, int16_t, int16_t, int16_t, int16_t, uint16_t); //fills a triangle with a color
    void drawCircle (int16_t, int16_t, int16_t, uint16_t); //draws a circle outline
    void drawCircleHelper (int16_t, int16_t, int16_t, uint8_t, uint16_t);
    void fillCircle (int16_t, int16_t, int16_t, uint16_t); //fills a circle with a color
    void fillCircleHelper (int16_t, int16_t, int16_t, uint8_t, int16_t, uint16_t);
    void drawIcon (fontAwesome*, int16_t, int16_t, uint16_t, uint16_t); //draws a single Font Awesome icon
    void drawIcon (fontAwesome*, int16_t, int16_t, uint16_t); //draws a single Font Awesome icon
    void drawChar (char, int16_t, int16_t, uint16_t, uint16_t, fontClass*); //draws a single char
    void drawChar (char, int16_t, int16_t, uint16_t, fontClass*); //draws a single char
    void printText (String, int16_t, int16_t, uint16_t, uint16_t, fontClass*); //prints/draws a string
    void printText (String, int16_t, int16_t, uint16_t, fontClass*); //prints/draws a string
    void clearText (String, int16_t, int16_t, uint16_t, fontClass*);
};

//========================================================================//
//font awesome icons

class fontAwesome {
  private:
    LCD_ILI9481* lcdParent;

  public:
    friend class fontClass;
    friend class buttonClass;
    friend class boxClass;

    const char* fontArray;
    int fontWidth; //width of each char (don't worry; char widths are still variable)
    int fontHeight; //font height
    int charByteLength; //count of bytes for each char
    int verticalByteCount; //no. of vertical bytes needed for a char
    int glyphWidth;
    int glyphHeight;
    int glyphX;
    int glyphY;
    uint32_t unicodeId;
    String name;

    //------------------------------------------------------------------------//

    fontAwesome (const char*, int, int, int, int, uint32_t, String, LCD_ILI9481*);
    fontAwesome (const char*, int, int, int, int, uint32_t, String);
    void getSize(); //calculates the absolute width and height of an icon
};

//========================================================================//
//font class for ASCII text

class fontClass {
  private:
    LCD_ILI9481* lcdParent;

  public:
    friend class fontAwesome;
    friend class buttonClass;
    friend class boxClass;

    const char* fontArray;
    int fontWidth; //width of each char (don't worry; char widths are still variable)
    int fontHeight; //font height
    int charByteLength; //count of bytes for each char
    int verticalByteCount; //no. of vertical bytes needed for a char
    uint32_t startChar; //first char in font
    uint32_t endChar; //last char in font
    int charCount; //total char count in font
    int glyphWidth;
    int glyphHeight;
    int glyphX;
    int glyphY;

    //------------------------------------------------------------------------//

    fontClass (const char*, int, int, int, int, uint32_t, uint32_t, int, LCD_ILI9481*);
    fontClass (const char*, int, int, int, int, uint32_t, uint32_t, int);
    void getSize (char); //calculates the absolute width and height of a char
    void getSize (String); //calculates the absolute width and height of a string
};

//========================================================================//
//UI button class

class buttonClass {
  private:
    LCD_ILI9481* lcdParent;
    XPT2046_Touchscreen* touchParent;

  public:
    friend class fontAwesome;
    friend class fontClass;
    friend class boxClass;

    int16_t buttonX;
    int16_t buttonY;
    int16_t buttonWidth;
    int16_t buttonHeight;
    int16_t radius;
    uint16_t borderColor;
    uint16_t borderHoverColor;
    uint16_t fillColor;
    uint16_t fillHoverColor;
    String labelString;
    fontClass* labelFont;
    int labelAlign;
    int labelOffsetX;
    int labelOffsetY;
    fontAwesome* icon;
    int iconAlign;
    int iconOffsetX;
    int iconOffsetY;
    uint16_t labelColor;
    uint16_t labelHoverColor;
    uint16_t iconColor;
    uint16_t iconHoverColor;
    bool buttonEnabled;
    bool buttonFunctional;
    bool borderEnabled;
    bool fillEnabled;
    bool labelEnabled;
    bool iconEnabled;
    bool buttonHoverEnabled;
    bool borderHoverEnabled;
    bool fillHoverEnabled;
    bool labelHoverEnabled;
    bool iconHoverEnabled;
    bool prevTouchState;
    bool currentTouchState;
    bool stateChange;
    bool activeState;

    //------------------------------------------------------------------------//
    //aavashyam ullathu aethaanennu vechaa eduthu kaachikko..!

    buttonClass (int16_t, int16_t, int16_t, int16_t, int16_t, uint16_t, uint16_t, uint16_t, uint16_t,
                 String, fontClass*, fontAwesome*, uint16_t, uint16_t, uint16_t, uint16_t,
                 bool, bool, bool, bool, bool, bool, bool, bool, bool, bool, LCD_ILI9481*, XPT2046_Touchscreen*);

    buttonClass (int16_t a, int16_t b, int16_t c, int16_t d, int16_t e, uint16_t f, uint16_t g, uint16_t h, uint16_t i,
                 String j, fontClass* k, int l, int m, int n, fontAwesome* o, uint16_t p, uint16_t q, uint16_t r, uint16_t s,
                 bool t, bool u, bool v, bool w, bool x, bool y, bool z, bool aa, bool ab, bool ac,
                 LCD_ILI9481* ad, XPT2046_Touchscreen* ae);

    buttonClass (int16_t a, int16_t b, int16_t c, int16_t d, int16_t e, uint16_t f, uint16_t g, uint16_t h, uint16_t i,
                 String j, fontClass* k, fontAwesome* l, int m, int n, int o, uint16_t p, uint16_t q, uint16_t r, uint16_t s,
                 bool t, bool u, bool v, bool w, bool x, bool y, bool z, bool aa, bool ab, bool ac,
                 LCD_ILI9481* ad, XPT2046_Touchscreen* ae);

    buttonClass (int16_t a, int16_t b, int16_t c, int16_t d, int16_t e, uint16_t f, uint16_t g, uint16_t h, uint16_t i,
                 String j, fontClass* k, int l, int m, int n, fontAwesome* o, int p, int q, int r, uint16_t s, uint16_t t, uint16_t u, uint16_t v,
                 bool w, bool x, bool y, bool z, bool aa, bool ab, bool ac, bool ad, bool ae, bool af,
                 LCD_ILI9481* ag, XPT2046_Touchscreen* ah);

    buttonClass (int16_t a, int16_t b, int16_t c, int16_t d, int16_t e, uint16_t f, uint16_t g, uint16_t h, uint16_t i,
                 String j, fontClass* k, uint16_t l, uint16_t m, bool n, bool o, bool p, bool q, bool r, bool s, bool t, bool u,
                 LCD_ILI9481* v, XPT2046_Touchscreen* w);

    buttonClass (int16_t a, int16_t b, int16_t c, int16_t d, int16_t e, uint16_t f, uint16_t g, uint16_t h, uint16_t i,
                 String j, fontClass* k, int l, int m, int n, uint16_t o, uint16_t p, bool q, bool r, bool s, bool t, bool u, bool v, bool w, bool x,
                 LCD_ILI9481* y, XPT2046_Touchscreen* z);

    buttonClass (int16_t a, int16_t b, int16_t c, int16_t d, int16_t e, uint16_t f, uint16_t g, uint16_t h, uint16_t i,
                 String j, fontAwesome* k, uint16_t l, uint16_t m, bool n, bool o, bool p, bool q, bool r, bool s, bool t, bool u,
                 LCD_ILI9481* v, XPT2046_Touchscreen* w);

    buttonClass (int16_t a, int16_t b, int16_t c, int16_t d, int16_t e, uint16_t f, uint16_t g, uint16_t h, uint16_t i,
                 String j, fontAwesome* k, int l, int m, int n, uint16_t o, uint16_t p, bool q, bool r, bool s, bool t, bool u, bool v, bool w, bool x,
                 LCD_ILI9481* y, XPT2046_Touchscreen* z);


    void draw();
    void show();
    void hide();
    void enable();
    void disable();
    void reset();
    void clear(uint32_t);
    void setXY(int, int);
    void hoverEnable();
    void hoverDisable();
    void showBorder();
    void hideBorder();
    void borderHoverDisable();
    void borderHoverEnable();
    void showFill();
    void hideFill();
    void fillHoverEnable();
    void fillHoverDisable();
    void showLabel();
    void hideLabel();
    void labelHoverEnable();
    void labelHoverDisable();
    void showIcon();
    void hideIcon();
    void iconHoverEnable();
    void iconHoverDisable();
    bool buttonTouched();
    bool buttonPressed();
    void updateState(bool);
    bool getState();
    void activate();
    void deactivate();
    void toggleState();
    void toggleState(bool);
};

//========================================================================//

class boxClass {
  private:
    LCD_ILI9481* lcdParent;
  public:
    int boxX;
    int boxY;
    int width;
    int height;
    int radius;
    uint16_t boxBorderColor;
    uint16_t boxFillColor;
    uint16_t boxHoverColor;
    bool boxVisible;
    bool borderVisible;
    bool hoverEnabled;
};

//========================================================================//
