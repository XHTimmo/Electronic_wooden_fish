#include "include/led.h"
#include "driver/gpio.h"

void LED_init(void) {
  esp_rom_gpio_pad_select_gpio(LED);
  gpio_reset_pin(LED);
  gpio_set_direction(LED, GPIO_MODE_OUTPUT);
  gpio_set_level(LED, 0);
}

void LED_on(void) { gpio_set_level(LED, 1); }

void LED_off(void) { gpio_set_level(LED, 0); }