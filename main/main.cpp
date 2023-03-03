// standard C libraries
#include <stdio.h>

// esp-idf libraries
#include "esp_log.h"

// esp-idf drivers
#include "driver/i2c.h"

#include "DFRobot_RGBLCD1602.h"

static const char *TAG = "DFRobot_RGBLCD1602";

#define I2C_MASTER_SCL_IO  GPIO_NUM_16 /*!< GPIO number used for I2C master clock */
#define I2C_MASTER_SDA_IO  GPIO_NUM_13 /*!< GPIO number used for I2C master data  */
#define I2C_MASTER_NUM 0                        /*!< I2C master i2c port number, the number of i2c peripheral interfaces available will depend on the chip */
// TEST #define I2C_MASTER_FREQ_HZ 400000               /*!< I2C master clock frequency */
#define I2C_MASTER_FREQ_HZ 300000               /*!< I2C master clock frequency */ //320000 too fast for the AIP display
#define I2C_MASTER_TX_BUF_DISABLE 0             /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE 0             /*!< I2C master doesn't need buffer */
#define I2C_MASTER_TIMEOUT_MS 1000

const int colorR = 255;
const int colorG = 0;
const int colorB = 255;

uint8_t heart[8] = {
    0b00000,
    0b01010,
    0b11111,
    0b11111,
    0b11111,
    0b01110,
    0b00100,
    0b00000};

uint8_t smiley[8] = {
    0b00000,
    0b00000,
    0b01010,
    0b00000,
    0b00000,
    0b10001,
    0b01110,
    0b00000};

uint8_t frownie[8] = {
    0b00000,
    0b00000,
    0b01010,
    0b00000,
    0b00000,
    0b00000,
    0b01110,
    0b10001};

uint8_t armsDown[8] = {
    0b00100,
    0b01010,
    0b00100,
    0b00100,
    0b01110,
    0b10101,
    0b00100,
    0b01010};

uint8_t armsUp[8] = {
    0b00100,
    0b01010,
    0b00100,
    0b10101,
    0b01110,
    0b00100,
    0b00100,
    0b01010};

DFRobot_RGBLCD1602 lcd(/*lcdCols*/ 16, /*lcdRows*/ 2, I2C_NUM_0); // 16 characters and 2 lines of show

/**
 * @brief i2c master initialization
 */
static esp_err_t i2c_master_init(void)
{
    int i2c_master_port = I2C_MASTER_NUM;

    i2c_config_t conf = {
        I2C_MODE_MASTER,
        I2C_MASTER_SDA_IO,
        I2C_MASTER_SCL_IO,
        GPIO_PULLUP_ENABLE, //.sda_pullup_en
        GPIO_PULLUP_ENABLE, //.scl_pullup_en
        I2C_MASTER_FREQ_HZ, //.master.clk_speed
        (uint32_t)0 //.clk_flags =  default clock
    };

    i2c_param_config(i2c_master_port, &conf);

    return i2c_driver_install(i2c_master_port, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
}

extern "C" void app_main(void)
{
    /**
     *  @brief custom character
     */

    esp_log_level_set("*", ESP_LOG_DEBUG); // set all components to DEBUG level
    uint16_t delay = 300;
    ESP_ERROR_CHECK(i2c_master_init());

    ESP_LOGI(TAG, "I2C initialized successfully");

    /* Read see if the device is there */
    ESP_ERROR_CHECK(lcd.init());
    lcd.customSymbol(0, heart);
    lcd.customSymbol(1, smiley);
    lcd.customSymbol(2, frownie);
    lcd.customSymbol(3, armsDown);
    lcd.customSymbol(4, armsUp);
    
    lcd.setCursor(0, 0);
    // Print a message to the lcd.
    lcd.print("I ");
    lcd.write((unsigned char)0);
    lcd.print(" Arduino! ");
    lcd.write(1);

    while(true)
    {
        // read the potentiometer on A0:
        // int sensorReading = delay;
        // map the result to 200 - 1000:
        // int delayTime = map(sensorReading, 0, 1023, 200, 1000);
        // set the cursor to the bottom row, 5th position:
        lcd.setCursor(4, 1);
        // draw the little man, arms down:
        lcd.write(3);
        vTaskDelay(pdMS_TO_TICKS(delay));
        lcd.setCursor(4, 1);
        // draw him arms up:
        lcd.write(4);
        vTaskDelay(pdMS_TO_TICKS(delay));
    }

    lcd.setCursor(2, 0);
    // lcd.print("Hello, world!");
    vTaskDelay(pdMS_TO_TICKS(delay));
    //lcd.clear();

    // lcd.setCursor(0, 0);
    lcd.write(65);
    lcd.write(0);
    lcd.write(1);
    lcd.write(2);
    lcd.write(3);
    lcd.write(4);
    lcd.write(66);
    // //     vTaskDelay(pdMS_TO_TICKS(delay));
    // lcd.clear(); // clear screen for the next loop

    lcd.setCursor(1, 1);
    lcd.write(67);
    lcd.write(0);
    lcd.write(1);
    lcd.write(2);
    lcd.write(3);
    lcd.write(4);
    lcd.write(68);
    vTaskDelay(pdMS_TO_TICKS(delay * 100));

    // for (int i = 0; i < 16; i += 2)
    // {
    //     lcd.write(0);
    //     vTaskDelay(pdMS_TO_TICKS(delay));
    //     lcd.write(1);
    //     vTaskDelay(pdMS_TO_TICKS(delay));
    // }
    // for (int i = 15; i > 0; i -= 2)
    // {
    //     lcd.setCursor(i, 1);
    //     lcd.write(2);
    //     vTaskDelay(pdMS_TO_TICKS(delay));
    //     lcd.setCursor(i - 1, 1);
    //     lcd.write(3);
    //     vTaskDelay(pdMS_TO_TICKS(delay));
    // }

    /* Demonstrate writing settings the background color/intensity */
    for (int i = 255; i >= 0; i-=2)
    {
        ESP_ERROR_CHECK(lcd.setRGB(colorR, colorG, i));
    }
    for (int i = 0; i <= 255; i+=2)
    {
        ESP_ERROR_CHECK(lcd.setRGB(colorR, colorG, i));    
    }

        vTaskDelay(pdMS_TO_TICKS(1000));

        // now do an autoroll, whatever that is
        // and a blinking cursor
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.cursor(); // on
        lcd.blink();
        vTaskDelay(pdMS_TO_TICKS(delay));
        // print from 0 to 9:
        for (int thisChar = 0; thisChar < 10; thisChar++)
        {
            lcd.print(thisChar);
            vTaskDelay(pdMS_TO_TICKS(delay));
        }

        lcd.setCursor(16, 1); // set the cursor to (16,1):
        lcd.stopBlink();
        /**
         *  @brief This will 'right justify' text from the showCursor
         */
        lcd.autoscroll(); // set the show to automatically scroll
        // print from 0 to 9:
        for (int thisChar = 0; thisChar < 10; thisChar++)
        {
            lcd.print(thisChar);
            vTaskDelay(pdMS_TO_TICKS(delay));
        }

        /**
         *  @brief This will 'left justify' text from the showCursor
         */
        lcd.noAutoscroll(); // turn off automatic scrolling

        lcd.clear(); // clear screen for the next loop

        // the fun is over
            ESP_ERROR_CHECK(i2c_driver_delete(I2C_MASTER_NUM));
            ESP_LOGI(TAG, "I2C de-initialized successfully");
}
