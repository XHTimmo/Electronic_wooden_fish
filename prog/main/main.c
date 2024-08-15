#include "../components/I2C/include/i2c.h"
#include "../components/SSD1306/include/ssd1306.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_rom_gpio.h"
#include "freertos/idf_additions.h"
#include "hal/gpio_types.h"
#include "portmacro.h"
#include "soc/gpio_num.h"
#include <stdint.h>

#define KEY GPIO_NUM_9
const char *TAG = "main";
uint8_t ssd1306_buf[128 * 8] = {0};
void app_main(void) {
  I2C_init();
  SSD1306_init();
  esp_rom_gpio_pad_select_gpio(KEY);
  gpio_reset_pin(KEY);
  gpio_set_direction(KEY, GPIO_MODE_INPUT);
  while (1) {
    SSD1306_fill(ssd1306_buf);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    for (uint16_t i = 0; i < 128 * 8; i++) {
      ssd1306_buf[i] = (ssd1306_buf[i] == 0 ? 0xff : 0);
    }
  }
}
