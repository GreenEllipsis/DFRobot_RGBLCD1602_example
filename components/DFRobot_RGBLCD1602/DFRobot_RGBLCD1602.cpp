/*!
 * @file DFRobot_RGBLCD1602.cpp
 * @brief DFRobot_RGBLCD1602 class infrastructure, the implementation of basic methods
 * @copyright	Copyright (c) 2023 Brian Alano (http://www.greenellipsis.org)
 * @licence     The MIT License (MIT)
 * @maintainer [GreenEllipsis](brian@greenellipsis.org)
 * @note ported from [DFRobot_RGBLCD1602](https://github.com/DFRobot/DFRobot_RGBLCD1602)
 * @note maintained by [yangfeng](feng.yang@dfrobot.com)
 * @version  V0.0.0
 * @date  2023-03-02
 * @url https://github.com/GreenEllipsis/DFRobot_RGBLCD1602
 */

// Standard C libraries
#include <inttypes.h>
#include <stdio.h>
#include <string.h>

// ESP-IDF libraries
#include "esp_err.h"
#include "esp_check.h"
#include "esp_log.h"

// component libraries
#include "DFRobot_RGBLCD1602.h"

#define LIB_TAG "DFRobot_RGBLCD1602"

const uint8_t color_define[4][3] =
{
    {255, 255, 255}, // white
    {255, 0, 0},     // red
    {0, 255, 0},     // green
    {0, 0, 255},     // blue, also monochrome
};

// /*******************************public*******************************/
    DFRobot_RGBLCD1602::DFRobot_RGBLCD1602(uint8_t lcdCols, uint8_t lcdRows, i2c_port_t i2c_num, uint8_t lcdAddr, uint8_t RGBAddr)
    {
        _lcdAddr = lcdAddr;
        _RGBAddr = RGBAddr;
        _cols = lcdCols;
        _rows = lcdRows;
        _i2c_num = i2c_num;
}
/**
 * @brief scan an I2C address
 * 
 */
esp_err_t DFRobot_RGBLCD1602::i2c_scan(const uint8_t addr)
{
    esp_err_t ret;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (addr << 1) | I2C_MASTER_WRITE, 1);
    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(I2C_NUM_0, cmd, 10 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
    ESP_LOGD(_TAG, "i2c_scan(0x%2x) returning:%i", addr, (int)ret);
    return ret;
}

/**
 * @brief  test communication to device.
 * @note i2c_driver_install must be done before init() by the calling program
 * @return esp_err_t returns the return value is i2c_master_cmd_begin()
 **/
// TODO change LOGI to LOGD
esp_err_t DFRobot_RGBLCD1602::init()
{
    esp_err_t ret;
    // find the LCD
    ESP_RETURN_ON_ERROR(i2c_scan(RGBLCD1602_LCD_ADDRESS), _TAG, "lcd not found");
    // find the RGB device and set the color PWM registers
    ret = i2c_scan(RGBLCD1602_RGB_ADDRESS);
    if (ret == ESP_OK) // found it
    {
        _RGBAddr = RGBLCD1602_RGB_ADDRESS;
        REG_RED = 0x04;
        REG_GREEN = 0x03;
        REG_BLUE = 0x02; // 0x02 in the original, but had to set it to 0x01 to work on my ESP32-WROOM and monochrome 1602Module BLWBBA. I dunno.
    }
    else
    {
        ret = i2c_scan(RGBLCD1602_RGB_ADDRESS_ALT);
        if (ret == ESP_OK) // found it
        {
            _RGBAddr = RGBLCD1602_RGB_ADDRESS_ALT;
            REG_RED = 0x06;   // pwm2
            REG_GREEN = 0x07; // pwm1
            REG_BLUE = 0x08;  // pwm0
        }
    }


	_showFunction = RGBLCD1602_4BITMODE | RGBLCD1602_1LINE | RGBLCD1602_5x8DOTS;
	begin(_rows);
    return ret;
}

esp_err_t DFRobot_RGBLCD1602::clear()
{
    esp_err_t  ret = command(RGBLCD1602_CLEARDISPLAY); // clear display, set cursor position to zero
    vTaskDelay(pdMS_TO_TICKS(2));     // this command takes a long time!
    return ret;
}

    esp_err_t DFRobot_RGBLCD1602::home()
    {
        esp_err_t ret = command(RGBLCD1602_RETURNHOME);        // set cursor position to zero
        vTaskDelay(pdMS_TO_TICKS(2));          // this command takes a long time!
        return ret;
    }

    esp_err_t DFRobot_RGBLCD1602::noDisplay()
    {
        _showControl &= ~RGBLCD1602_DISPLAYON;
        return command(RGBLCD1602_DISPLAYCONTROL | _showControl);
    }

esp_err_t DFRobot_RGBLCD1602::display() 
{
    _showControl |= RGBLCD1602_DISPLAYON;
    return command(RGBLCD1602_DISPLAYCONTROL | _showControl);
}

// void DFRobot_RGBLCD1602::stopBlink()
// {
//     _showControl &= ~RGBLCD1602_BLINKON;
//     command(RGBLCD1602_DISPLAYCONTROL | _showControl);
// }
// void DFRobot_RGBLCD1602::blink()
// {
//     _showControl |= RGBLCD1602_BLINKON;
//     command(RGBLCD1602_DISPLAYCONTROL | _showControl);
// }

esp_err_t DFRobot_RGBLCD1602::noCursor()
{
    _showControl &= ~RGBLCD1602_CURSORON;
    return command(RGBLCD1602_DISPLAYCONTROL | _showControl);
}

esp_err_t DFRobot_RGBLCD1602::cursor()
{
    _showControl |= RGBLCD1602_CURSORON;
    return command(RGBLCD1602_DISPLAYCONTROL | _showControl);
}

// void DFRobot_RGBLCD1602::scrollDisplayLeft(void)
// {
//     command(RGBLCD1602_CURSORSHIFT | RGBLCD1602_DISPLAYMOVE | RGBLCD1602_MOVELEFT);
// }

// void DFRobot_RGBLCD1602::scrollDisplayRight(void)
// {
//     command(RGBLCD1602_CURSORSHIFT | RGBLCD1602_DISPLAYMOVE | RGBLCD1602_MOVERIGHT);
// }

// void DFRobot_RGBLCD1602::leftToRight(void)
// {
//     _showMode |= RGBLCD1602_ENTRYLEFT;
//     command(RGBLCD1602_ENTRYMODESET | _showMode);
// }

// void DFRobot_RGBLCD1602::rightToLeft(void)
// {
//     _showMode &= ~RGBLCD1602_ENTRYLEFT;
//     command(RGBLCD1602_ENTRYMODESET | _showMode);
// }

esp_err_t DFRobot_RGBLCD1602::noAutoscroll(void)
{
    _showMode &= ~RGBLCD1602_ENTRYSHIFTINCREMENT;
    return command(RGBLCD1602_ENTRYMODESET | _showMode);
}

esp_err_t DFRobot_RGBLCD1602::autoscroll(void)
{
    _showMode |= RGBLCD1602_ENTRYSHIFTINCREMENT;
    return command(RGBLCD1602_ENTRYMODESET | _showMode);
}

// void DFRobot_RGBLCD1602::customSymbol(uint8_t location, uint8_t charmap[])
// {

//     location &= 0x7; // we only have 8 locations 0-7
//     command(RGBLCD1602_SETCGRAMADDR | (location << 3));
    
    
//     uint8_t data[9];
//     data[0] = 0x40;
//     for(int i=0; i<8; i++)
//     {
//         data[i+1] = charmap[i];
//     }
//     send(data, 9);
// }

esp_err_t DFRobot_RGBLCD1602::setCursor(uint8_t col, uint8_t row)
{

    col = (row == 0 ? col|0x80 : col|0xc0);
    const uint8_t data[] = {0x80, col};
    return i2c_master_write_to_device(I2C_NUM_0, _lcdAddr,
                               data, 2,
                               10 / portTICK_PERIOD_MS);
}

esp_err_t DFRobot_RGBLCD1602::setRGB(uint8_t r, uint8_t g, uint8_t b)
{
    ESP_RETURN_ON_ERROR(setReg(REG_RED, r), _TAG, "setReg(0x%2x, 0x%2x)", REG_RED, r);
    ESP_RETURN_ON_ERROR(setReg(REG_GREEN, g), _TAG, "setReg(0x%2x, 0x%2x)", REG_GREEN, g);
    ESP_RETURN_ON_ERROR(setReg(REG_BLUE, b), _TAG, "setReg(0x%2x, 0x%2x)", REG_BLUE, b);
    return ESP_OK;
}

// void DFRobot_RGBLCD1602::setColor(uint8_t color)
// {
//     if(color > 3)return ;
//     setRGB(color_define[color][0], color_define[color][1], color_define[color][2]);
// }

esp_err_t DFRobot_RGBLCD1602::blinkLED(void)
{
    ///< blink period in seconds = (<reg 7> + 1) / 24
    ///< on/off ratio = <reg 6> / 256
    setReg(0x07, 0x17);  // blink every second
    return setReg(0x06, 0x7f);  // half on, half off
}

esp_err_t DFRobot_RGBLCD1602::noBlinkLED(void)
{
    setReg(0x07, 0x00);
    return setReg(0x06, 0xff);
}

inline size_t DFRobot_RGBLCD1602::write(uint8_t value)
{

    const uint8_t data[] = {0x40, value};
    esp_err_t ret = i2c_master_write_to_device(I2C_NUM_0, _lcdAddr,
                               data, 2,
                               10 / portTICK_PERIOD_MS);
    if (ret == ESP_OK)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

inline esp_err_t DFRobot_RGBLCD1602::command(uint8_t value)
{
    const uint8_t data[] = {0x80, value};
    return i2c_master_write_to_device(_i2c_num, _lcdAddr,
                                             data, 2,
                                             10 / portTICK_PERIOD_MS);
}

void DFRobot_RGBLCD1602::print(const char *str)
{
    while (*str)
    {
        write(*str++);
    }
}
void DFRobot_RGBLCD1602::print(const int i)
{
    char str[7];
    sprintf(str, "%i", i);
    print(str);
}

// void DFRobot_RGBLCD1602::setBacklight(bool mode){
// 	if(mode){
// 		setColorWhite();		// turn backlight on
// 	}else{
// 		closeBacklight();		// turn backlight off
// 	}
// }

// /*******************************private*******************************/
esp_err_t DFRobot_RGBLCD1602::begin( uint8_t rows, uint8_t charSize) 
{
    if (rows > 1) {
        _showFunction |= RGBLCD1602_2LINE;
    }
    _numLines = rows;
    _currLine = 0;
    ///< for some 1 line displays you can select a 10 pixel high font
    if ((charSize != 0) && (rows == 1)) {
        _showFunction |= RGBLCD1602_5x10DOTS;
    }

    ///< SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
    ///< according to datasheet, we need at least 40ms after power rises above 2.7V
    ///< before sending commands. Arduino can turn on way befer 4.5V so we'll wait 50
    vTaskDelay(pdMS_TO_TICKS(50));

    ///< this is according to the hitachi HD44780 datasheet
    ///< page 45 figure 23

    ///< Send function set command sequence
    command(RGBLCD1602_FUNCTIONSET | _showFunction);
    vTaskDelay(pdMS_TO_TICKS(5));  // wait more than 4.1ms
	
	///< second try
    command(RGBLCD1602_FUNCTIONSET | _showFunction);
    vTaskDelay(pdMS_TO_TICKS(5));

    ///< third go
    ESP_ERROR_CHECK(command(RGBLCD1602_FUNCTIONSET | _showFunction));

    ///< turn the display on with no cursor or blinking default
    _showControl = RGBLCD1602_DISPLAYON | RGBLCD1602_CURSOROFF | RGBLCD1602_BLINKOFF;
    display();

    ///< clear it off
    clear();

    ///< Initialize to default text direction (for romance languages)
    _showMode = RGBLCD1602_ENTRYLEFT | RGBLCD1602_ENTRYSHIFTDECREMENT;
    ///< set the entry mode
    command(RGBLCD1602_ENTRYMODESET | _showMode);
    
    if(_RGBAddr == RGBLCD1602_RGB_ADDRESS){
      ///< backlight init
        setReg(RGBLCD1602_REG_MODE1, 0);
        ///< set LEDs controllable by both PWM and GRPPWM registers
        setReg(RGBLCD1602_REG_OUTPUT, 0xFF);
        ///< set MODE2 values
        ///< 0010 0000 -> 0x20  (DMBLNK to 1, ie blinky mode)
        setReg(RGBLCD1602_REG_MODE2, 0x20);
    }else{
      setReg(0x04, 0x15);
    }
    setColorWhite();
    return ESP_OK;
}

// esp_err_t DFRobot_RGBLCD1602::send(const uint8_t *data, uint8_t len)
// {
//     // _pWire->beginTransmission(_lcdAddr);        // transmit to device #4
//     // for(int i=0; i<len; i++) {
//     //     _pWire->write(data[i]);
// 	// 	vTaskDelay(pdMS_TO_TICKS(5));
//     // }
//     // _pWire->endTransmission();                     // stop transmitting
//     return i2c_master_write_to_device(_i2c_num, _lcdAddr,
//                                       data, len,
//                                       10 / portTICK_PERIOD_MS);
// }

esp_err_t DFRobot_RGBLCD1602::setReg(uint8_t addr, uint8_t data)
{
    ESP_LOGD(_TAG, "i2c_master_write_to_device(port:%i, addr:0x%02x, reg:0x%02x, data:0x%02x)", _i2c_num, _RGBAddr, addr, data);
    const uint8_t write_buffer[] = {addr, data};
    return i2c_master_write_to_device(I2C_NUM_0, _RGBAddr,
                                        write_buffer, 2,
                                        10 / portTICK_PERIOD_MS);
}
