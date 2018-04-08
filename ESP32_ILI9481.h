
//========================================================================//
//                                                                        //
//  ## ESP32-ILI9481-LCD-Library ##                                       //
//  ILI9481 320 x 480 LCD driver and graphics library for ESP32 boards    //
//                                                                        //
//  Filename : ESP32_ILI9481.h                                          //
//  Author : Vishnu M Aiea                                                //
//  Source : https://github.com/vishnumaiea/ESP32-ILI9481-LCD-Library     //
//  Author's website : www.vishnumaiea.in                                 //
//  Initial release : IST 11:51 PM 07-04-2018, Saturday                   //
//  License : GNU GPL version 3.0                                         //
//  Additional source license/s :                                         //
//    1. BSD license @ Adafruit Industries                                //
//       https://github.com/adafruit/Adafruit-GFX-Library                 //
//                                                                        //
//  File last modified : IST 11:34 AM 08-04-2018, Sunday                  //
//                                                                        //
//========================================================================//

#include <Arduino.h>
#include <stdint.h>

//========================================================================//
//class declarations

class fontClass;
class fontAwesome;
class buttonClass;
class boxClass;

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
//LCD class

static const int Width = 320; //constants
static const int Height = 480;
static const uint32_t PORT_MASK = 0x1U;

class LCD_ILI9481 {
  private:
    uint8_t _CS_PIN_LCD, _RST_PIN_LCD, _DC_PIN_LCD, _WR_PIN_LCD;
    uint8_t _PD0, _PD1, _PD2, _PD3, _PD4, _PD5, _PD6, _PD7;
    uint8_t _PD8, _PD9, _PD10, _PD11, _PD12, _PD13, _PD14, _PD15;
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
    void drawChar (char, int16_t, int16_t, uint16_t, uint16_t, fontClass*); //draws a single char
    void printText (String, int16_t, int16_t, uint16_t, uint16_t, fontClass*); //prints/draws a string
};

//========================================================================//
//font awesome icons

class fontAwesome {
  public:
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

    fontAwesome (const char*, int, int, int, int, uint32_t, String);
    void getSize(); //calculates the absolute width and height of an icon
};

//========================================================================//
//font class for ASCII text

class fontClass {
  public:
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

    fontClass (const char*, int, int, int, int, uint32_t, uint32_t, int);
    void getSize (char); //calculates the absolute width and height of a char
    void getSize (String); //calculates the absolute width and height of a string
};

//========================================================================//
//UI button class

class buttonClass {
  public:
    int x;
    int y;
    int width;
    int height;
    int radius;
    uint16_t borderColor;
    uint16_t borderHoverColor;
    uint16_t fillColor;
    uint16_t fillHoverColor;
    String label;
    fontClass* labelFont;
    fontAwesome* icon;
    uint16_t labelColor;
    uint16_t labelHoverColor;
    uint16_t iconColor;
    uint16_t iconHoverColor;
    bool buttonEnabled;
    bool borderEnabled;
    bool fillEnabled;
    bool labelEnabled;
    bool iconEnabled;
    bool buttonHoverEnabled;
    bool borderHoverEnabled;
    bool fillHoverEnabled;
    bool labelHoverEnabled;
    bool iconHoverEnabled;

    //------------------------------------------------------------------------//

    buttonClass (int, int, int, int, int, uint16_t, uint16_t, uint16_t, uint16_t,
                 String, fontClass*, fontAwesome*, uint16_t, uint16_t, uint16_t, uint16_t,
                 bool, bool, bool, bool, bool, bool, bool, bool, bool, bool);
    void draw();
    void show();
    void hide();
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
};

//========================================================================//

class boxClass {
  public:
    int x;
    int y;
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
