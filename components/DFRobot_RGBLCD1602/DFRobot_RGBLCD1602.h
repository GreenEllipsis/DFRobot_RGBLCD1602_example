/*!
 * @file DFRobot_RGBLCD1602.h
 * @brief DFRobot_RGBLCD1602 class infrastructure
 * @copyright	Copyright (c) 2023 Brian Alano (http://www.greenellipsis.org)
 * @license     The MIT License (MIT)
 * @maintainer [GreenEllipsis](brian@greenellipsis.org)
 * @version  V0.0.0
 * @date  2023-03-02
 * @url https://github.com/GreenEllipsis/DFRobot_RGBLCD1602
 */

#pragma once

// /*
// #include <inttypes.h>
// #include "Print.h"
// #include <stdio.h>
// #include <string.h>
// ESP-IDF component libraries
#include "driver/i2c.h"

/*!
 *  @brief default Device I2C Addresses
 */
#define RGBLCD1602_LCD_ADDRESS (0x7c >> 1)
#define RGBLCD1602_RGB_ADDRESS_ALT (0x60 >> 1)
#define RGBLCD1602_RGB_ADDRESS (0xc0 >> 1)

/*!
 *  @brief color define
 */
#define RGBLCD1602_WHITE 0
#define RGBLCD1602_RED 1
#define RGBLCD1602_GREEN 2
#define RGBLCD1602_BLUE 3

#define RGBLCD1602_REG_MODE1 0x00
#define RGBLCD1602_REG_MODE2 0x01
#define RGBLCD1602_REG_OUTPUT 0x08

/*!
 *  @brief commands
 */
#define RGBLCD1602_CLEARDISPLAY 0x01
#define RGBLCD1602_RETURNHOME 0x02
#define RGBLCD1602_ENTRYMODESET 0x04
#define RGBLCD1602_DISPLAYCONTROL 0x08
#define RGBLCD1602_CURSORSHIFT 0x10
#define RGBLCD1602_FUNCTIONSET 0x20
#define RGBLCD1602_SETCGRAMADDR 0x40
#define RGBLCD1602_SETDDRAMADDR 0x80

/*!
 *  @brief flags for display entry mode
 */
#define RGBLCD1602_ENTRYRIGHT 0x00
#define RGBLCD1602_ENTRYLEFT 0x02
#define RGBLCD1602_ENTRYSHIFTINCREMENT 0x01
#define RGBLCD1602_ENTRYSHIFTDECREMENT 0x00

/*!
 *  @brief flags for display on/off control
 */
#define RGBLCD1602_DISPLAYON 0x04
#define RGBLCD1602_DISPLAYOFF 0x00
#define RGBLCD1602_CURSORON 0x02
#define RGBLCD1602_CURSOROFF 0x00
#define RGBLCD1602_BLINKON 0x01
#define RGBLCD1602_BLINKOFF 0x00

/*!
 *  @brief flags for display/cursor shift
 */
#define RGBLCD1602_DISPLAYMOVE 0x08
#define RGBLCD1602_CURSORMOVE 0x00
#define RGBLCD1602_MOVERIGHT 0x04
#define RGBLCD1602_MOVELEFT 0x00

/*!
 *  @brief flags for function set
 */
#define RGBLCD1602_8BITMODE 0x10
#define RGBLCD1602_4BITMODE 0x00
#define RGBLCD1602_2LINE 0x08
#define RGBLCD1602_1LINE 0x00
#define RGBLCD1602_5x10DOTS 0x04
#define RGBLCD1602_5x8DOTS 0x00

class DFRobot_RGBLCD1602
{
private:
    static constexpr const char *_TAG = "DFRobot_RGBLCD1602";
    i2c_config_t conf;
    uint8_t _lcdAddr;
    uint8_t _RGBAddr;
    uint8_t _cols;
    uint8_t _rows;
    i2c_port_t _i2c_num;

    // Read Functions:
    //	uint16_t readWord(uint8_t registerAddress);

    esp_err_t i2c_scan(const uint8_t addr);

    // Write functions, returning success or failure:
    esp_err_t writeBytes(uint8_t firstRegisterAddress, uint8_t *writeArray, uint8_t length);

public:
    uint8_t REG_RED = 0;   // pwm2
    uint8_t REG_GREEN = 0; // pwm1
    uint8_t REG_BLUE = 0;  // pwm0esp_err_t writeByte(uint8_t registerAddress, uint8_t writeValue);
    esp_err_t writeWord(uint8_t registerAddress, uint16_t writeValue);
    esp_err_t readWord(uint8_t registerAddress, uint16_t *value); // FIXME DEBUG Private
    esp_err_t readByte(uint8_t registerAddress, uint8_t *value);
    // Read Functions returning success or failure:
    esp_err_t readBytes(uint8_t firstRegisterAddress, uint8_t *destination, uint8_t length);
    
    /**
     * @brief set columns, rows, pass the i2C interface. Optionally set the display address and backlight address
     * 
     */
    // Constructor 
    // -----------------------------------------------------------------------------
    DFRobot_RGBLCD1602(uint8_t lcdCols, uint8_t lcdRows, i2c_port_t i2c_num = I2C_NUM_0, 
        uint8_t lcdAddr = RGBLCD1602_LCD_ADDRESS, uint8_t RGBAddr = RGBLCD1602_RGB_ADDRESS);

    // -----------------------------------------------------------------------------
    // begin(uint8_t address, uint8_t resetPin): This function initializes the instance.
    //  	It requires wire to already be begun (previous versions did not do this), resets the IC, and tries to read some
    //  	registers to prove it's connected.
    // Inputs:
    //		- slave_addr should be the 7-bit address of the device. This should be
    //		 one of four values - 0x3E, 0x3F, 0x70, 0x71 - all depending on what the
    //		 ADDR0 and ADDR1 pins ar se to. This variable is required.
    //		
    // Output: Returns a ESP_OK if communication is successful, ESP_ERR on error.
    // -----------------------------------------------------------------------------
    esp_err_t begin(uint16_t slave_addr, int sda_io_num, int scl_io_num);

    // -----------------------------------------------------------------------------
    // end(): This function unistalls the I2C driver for the device so a different
    //      driver can use the I2C bus
    // Output: Returns the result of i2c_driver_delete()
    // I2C_NUM_0 is hard coded
    // -----------------------------------------------------------------------------
    esp_err_t end();

    // -----------------------------------------------------------------------------
    // init()
    // test communications
    // -----------------------------------------------------------------------------
    esp_err_t init();

    



// class DFRobot_RGBLCD1602 : public Print 
// {

// public:
//   /**
//    *  @brief Constructor
//    */
//   DFRobot_RGBLCD1602(uint8_t lcdCols,uint8_t lcdRows,TwoWire *pWire=&Wire,uint8_t lcdAddr=LCD_ADDRESS,uint8_t RGBAddr=RGB_ADDRESS);

//   /**
//    *  @brief clear the display and return the cursor to the initial position (position 0)
//    */
//   void clear();

//   /**
//    *  @brief return the cursor to the initial position (0,0)
//    */
//   void home();

//     /**
//      *  @brief Turn off the display
//      */
//   esp_err_t noDisplay();

//   /**
//    *  @brief Turn on the display
//    */
//   esp_err_t display();

//   /**
//    *  @brief Turn  off the blinking showCursor
//    */
//   void stopBlink();

//   /**
//    *  @brief Turn on  the blinking showCursor
//    */
//   void blink();

//   /**
//    *  @brief Turn off the underline showCursor 
//    */
//   void noCursor();

//   /**
//    *  @brief Turn on the underline showCursor 
//    */
//   void cursor();

//   /**
//    *  @brief scroll left to display
//    */
//   void scrollDisplayLeft();

//   /**
//    *  @brief scroll right to display
//    */
//   void scrollDisplayRight();
 
//   /**
//    *  @brief This is for text that flows Left to Right
//    */
//   void leftToRight();
 
//   /**
//    *  @brief This is for text that flows Right to Left
//    */
//   void rightToLeft();

//   /**
//    *  @brief This will 'left justify' text from the showCursor
//    */
//   void noAutoscroll();
 
//   /**
//    *  @brief This will 'right justify' text from the showCursor
//    */
//   void autoscroll();
   
//   /**
//    *  @brief Allows us to fill the first 8 CGRAM locations with custom characters
//    *  @param location substitute character range (0-7)
//    *  @param charmap  character array the size is 8 bytes
//    */
//   void customSymbol(uint8_t location, uint8_t charmap[]);

//   /**
//    *  @brief set cursor position
//    *  @param col columns optional range 0-15
//    *  @param row rows optional range 0-1，0 is the first row, 1 is the second row
//    */
//   void setCursor(uint8_t col, uint8_t row);
  
    /**
     *  @brief set RGB
     *  @param r  red   range(0-255)
     *  @param g  green range(0-255)
     *  @param b  blue  range(0-255)
     */
    esp_err_t setRGB(uint8_t r, uint8_t g, uint8_t b);

//   /**
//    *  @brief set backlight PWM output
//    *  @param color  backlight color  Preferences：REG_RED\REG_GREEN\REG_BLUE
//    *  @param pwm  color intensity   range(0-255)
//    */
//   void setPWM(uint8_t color, uint8_t pwm){setReg(color, pwm);}      // set pwm

//   /**
//    *  @brief backlight color
//    *  @param color  backlight color  Preferences： WHITE\RED\GREEN\BLUE
//    */
//   void setColor(uint8_t color);

//   /**
//    *  @brief close the backlight
//    */
//   void closeBacklight(){setRGB(0, 0, 0);}

//   /**
//    *  @brief set the backlight to white
//    */
//   void setColorWhite(){setRGB(255,255 , 255);}

//   /**
//    *  @brief blink the LED backlight
//    */
//   void blinkLED(void);

//   /**
//    *  @brief the LED backlight doesn't blink
//    */
//   void noBlinkLED(void);

//   /**
//    *  @brief write character
//    *  @param data the written data
//    */
//   virtual size_t write(uint8_t data);

  /**
   *  @brief send command
   *  @param data the sent command
   */
//   esp_err_t command(uint8_t data);

//   /**
//    *  @brief set the backlight
//    *  @param mode  true indicates the backlight is turned on and set to white, false indicates the backlight is turned off
//    */
//   void setBacklight(bool mode);

//   using Print::write;
  
// private:
//   /**
//    *  @brief the initialization function
//    *  @param row rows optional range 0-1，0 is the first row, 1 is the second row
//    *  @param charSize  character size LCD_5x8DOTS\LCD_5x10DOTS
//    */
//   void begin(uint8_t rows, uint8_t charSize = LCD_5x8DOTS);

  /**
   *  @brief send a data array
   *  @param data the data to send
   *  @param len length of the data
   */
//   esp_err_t send(const uint8_t *data, uint8_t len);

//   /**
//    *  @brief set the register
//    *  @param addr register address
//    *  @param data data
//    */
  esp_err_t setReg(uint8_t addr, uint8_t data);
//   uint8_t _showFunction;
//   uint8_t _showControl;
//   uint8_t _showMode;
//   uint8_t _initialized;
//   uint8_t _numLines,_currLine;
//   uint8_t _lcdAddr;
//   uint8_t _RGBAddr;
//   uint8_t _cols;
//   uint8_t _rows;
//   TwoWire *_pWire;
// public:
//   uint8_t REG_RED      =   0 ;       // pwm2
//   uint8_t REG_GREEN    =   0 ;       // pwm1
//   uint8_t REG_BLUE     =   0 ;       // pwm0
// };
};
