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
#define I2C_MASTER_FREQ_HZ 400000               /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE 0             /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE 0             /*!< I2C master doesn't need buffer */
#define I2C_MASTER_TIMEOUT_MS 1000

const int colorR = 255;
const int colorG = 0;
const int colorB = 255;

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
    esp_log_level_set("*", ESP_LOG_DEBUG); // set all components to DEBUG level
    ESP_ERROR_CHECK(i2c_master_init());
    ESP_LOGI(TAG, "I2C initialized successfully");

    /* Read see if the device is there */
    ESP_ERROR_CHECK(lcd.init());
    
    /* Demonstrate writing settings the background color/intensity */
    ESP_ERROR_CHECK(lcd.setRGB(colorR, colorG, colorB));

    vTaskDelay(pdMS_TO_TICKS(1000));

    int ms = 1000;
    {
        ESP_LOGI(TAG, "noDisplay");
        ESP_ERROR_CHECK(lcd.noDisplay());
        vTaskDelay(pdMS_TO_TICKS(ms));
        ESP_LOGI(TAG, "display");
        ESP_ERROR_CHECK(lcd.display());
        vTaskDelay(pdMS_TO_TICKS(ms));
        ESP_LOGI(TAG, "noCursor");
        ESP_ERROR_CHECK(lcd.noCursor());
        vTaskDelay(pdMS_TO_TICKS(ms));
        ESP_LOGI(TAG, "cursor");
        ESP_ERROR_CHECK(lcd.cursor());
        vTaskDelay(pdMS_TO_TICKS(ms));
    }
    

    // finally, let's write to the display!
    lcd.write('%');
    
    lcd.setCursor(3, 0);
    lcd.print("Hello, world!");
    lcd.setCursor(2, 1);

    ESP_ERROR_CHECK(i2c_driver_delete(I2C_MASTER_NUM));
    ESP_LOGI(TAG, "I2C de-initialized successfully");
}
