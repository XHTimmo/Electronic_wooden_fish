#ifndef __I2C_H__
#define __I2C_H__

#define I2C_SCL (GPIO_NUM_13)
#define I2C_SDA (GPIO_NUM_12)
#define I2C_MASTER_NUM (I2C_NUM_0)  /*!< I2C port number for master dev */
#define I2C_MASTER_FREQ_HZ (1* 1000 * 1000) /*!< I2C master clock frequency */

void I2C_init(void);
// void LED_on(void);
// void LED_off(void);
#endif