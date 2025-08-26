/*
 * bmemain.h
 *
 *  Created on: Aug 26, 2025
 *      Author: thiru
 */

#ifndef BMEMAIN_H_
#define BMEMAIN_H_
#include"main.h"
#include"bme680.h"
#include"bme680_defs.h"
#include"string.h"
#include"stdio.h"
int8_t user_i2c_read(uint8_t dev_id, uint8_t reg_addr, uint8_t *data, uint16_t len);
int8_t user_i2c_write(uint8_t dev_id, uint8_t reg_addr, uint8_t *data, uint16_t len);
void user_delay_ms(uint32_t period);
void I2C_Scan(void);
int8_t BME680_Initialize(struct bme680_dev *gas_sensor);
int8_t BME680_Configure(struct bme680_dev *gas_sensor);
#endif /* BMEMAIN_H_ */
