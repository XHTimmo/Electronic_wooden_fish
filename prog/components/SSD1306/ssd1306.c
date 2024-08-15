#include "include/ssd1306.h"
#include "../I2C/include/i2c.h"
#include "driver/i2c.h"
#include "unity.h"
#include <sys/time.h>
#include <time.h>

#include "esp_log.h"

static void *ssd1306 = NULL;
static char *TAG = "SSD1306";
typedef struct {
  i2c_port_t bus;
  gpio_num_t int_pin;
  uint16_t dev_addr;
  uint32_t counter;
  float dt; /*!< delay time between two measurements, dt should be small (ms
               level) */
  struct timeval *timer;
} ssd1306_dev_t;

static esp_err_t ssd1306_write_cmd(void *sensor, const uint8_t command) {
  ssd1306_dev_t *sens = (ssd1306_dev_t *)sensor;
  esp_err_t ret;

  i2c_cmd_handle_t cmd = i2c_cmd_link_create();
  ret = i2c_master_start(cmd);
  assert(ESP_OK == ret);
  ret = i2c_master_write_byte(cmd, sens->dev_addr | I2C_MASTER_WRITE, true);
  assert(ESP_OK == ret);
  ret = i2c_master_write_byte(cmd, 0x00, true);
  assert(ESP_OK == ret);
  ret = i2c_master_write_byte(cmd, command, true);
  assert(ESP_OK == ret);
  ret = i2c_master_stop(cmd);
  assert(ESP_OK == ret);
  ret = i2c_master_cmd_begin(sens->bus, cmd, 1000 / portTICK_PERIOD_MS);
  i2c_cmd_link_delete(cmd);

  return ret;
}

static esp_err_t ssd1306_write_data(void *sensor, const uint8_t *const data_buf,
                                    const uint8_t data_len) {
  ssd1306_dev_t *sens = (ssd1306_dev_t *)sensor;
  esp_err_t ret;

  i2c_cmd_handle_t cmd = i2c_cmd_link_create();
  ret = i2c_master_start(cmd);
  assert(ESP_OK == ret);
  ret = i2c_master_write_byte(cmd, sens->dev_addr | I2C_MASTER_WRITE, true);
  assert(ESP_OK == ret);
  ret = i2c_master_write_byte(cmd, 0x40, true);
  assert(ESP_OK == ret);
  ret = i2c_master_write(cmd, data_buf, data_len, true);
  assert(ESP_OK == ret);
  ret = i2c_master_stop(cmd);
  assert(ESP_OK == ret);
  ret = i2c_master_cmd_begin(sens->bus, cmd, 1000 / portTICK_PERIOD_MS);
  i2c_cmd_link_delete(cmd);

  return ret;
}

static void *ssd1306_create(i2c_port_t port, const uint16_t dev_addr) {
  ssd1306_dev_t *sensor = (ssd1306_dev_t *)calloc(1, sizeof(ssd1306_dev_t));
  sensor->bus = port;
  sensor->dev_addr = dev_addr << 1;
  sensor->counter = 0;
  sensor->dt = 0;
  sensor->timer = (struct timeval *)calloc(1, sizeof(struct timeval));
  return (void *)sensor;
}

static void ssd1306_clean() {
  uint8_t data_buf[128] = {1};
  for (uint8_t i = 0; i < 128; i++) {
    data_buf[i] = 0xFF;
  }
  for (uint8_t i = 0; i < 8; i++) {
    ssd1306_write_cmd(ssd1306, 0xb0 + i);
    ssd1306_write_cmd(ssd1306, 0x00);
    ssd1306_write_cmd(ssd1306, 0x10);

    ssd1306_write_data(ssd1306, data_buf, 128);
  }
}

void SSD1306_init(void) {

  esp_err_t ret;

  ssd1306 = ssd1306_create(I2C_MASTER_NUM, SSD1306_ADDR);
  TEST_ASSERT_NOT_NULL_MESSAGE(ssd1306, "SSD1306 create returned NULL");
  ssd1306_write_cmd(ssd1306, 0xAE);
  ssd1306_write_cmd(ssd1306, 0x00);
  ssd1306_write_cmd(ssd1306, 0x10);
  ssd1306_write_cmd(ssd1306, 0x40);
  ssd1306_write_cmd(ssd1306, 0xB0);
  ssd1306_write_cmd(ssd1306, 0x81);
  ssd1306_write_cmd(ssd1306, 0xFF);
  ssd1306_write_cmd(ssd1306, 0xA1);
  ssd1306_write_cmd(ssd1306, 0xA6);
  ssd1306_write_cmd(ssd1306, 0xA8);
  ssd1306_write_cmd(ssd1306, 0x3F);
  ssd1306_write_cmd(ssd1306, 0xC8);
  ssd1306_write_cmd(ssd1306, 0xD3);
  ssd1306_write_cmd(ssd1306, 0x00);
  ssd1306_write_cmd(ssd1306, 0xD5);
  ssd1306_write_cmd(ssd1306, 0x80);
  ssd1306_write_cmd(ssd1306, 0xD8);
  ssd1306_write_cmd(ssd1306, 0x05);
  ssd1306_write_cmd(ssd1306, 0xD9);
  ssd1306_write_cmd(ssd1306, 0xF1);
  ssd1306_write_cmd(ssd1306, 0xDA);
  ssd1306_write_cmd(ssd1306, 0x12);
  ssd1306_write_cmd(ssd1306, 0xDB);
  ssd1306_write_cmd(ssd1306, 0x30);
  ssd1306_write_cmd(ssd1306, 0x8D);
  ssd1306_write_cmd(ssd1306, 0x14);
  ssd1306_write_cmd(ssd1306, 0xAF);

  ssd1306_clean();
  ESP_LOGI(TAG, "Finish init %s", TAG);
}


void SSD1306_fill(uint8_t *data_buf) {
  for (uint8_t i = 0; i < 8; i++) {
    ssd1306_write_cmd(ssd1306, 0xb0 + i);
    ssd1306_write_cmd(ssd1306, 0x00);
    ssd1306_write_cmd(ssd1306, 0x10);

    ssd1306_write_data(ssd1306, data_buf + i*128, 128);
  }
}