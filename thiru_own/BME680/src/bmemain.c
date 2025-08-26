/*
 * bmemain.c
 *
 *  Created on: Aug 26, 2025
 *      Author: thiru
 */
#include"bmemain.h"
I2C_HandleTypeDef hi2c2;
extern UART_HandleTypeDef huart2;

int8_t rslt=BME680_OK;
struct bme680_dev gas_sensor;
struct bme680_field_data data;
// Implement user_i2c_read, user_i2c_write, and user_delay_ms functions
int8_t user_i2c_read(uint8_t dev_id, uint8_t reg_addr, uint8_t *data, uint16_t len)
{
    // Implement I2C read function
    return HAL_I2C_Mem_Read(&hi2c2, dev_id << 1, reg_addr, I2C_MEMADD_SIZE_8BIT, data, len, HAL_MAX_DELAY);
}

int8_t user_i2c_write(uint8_t dev_id, uint8_t reg_addr, uint8_t *data, uint16_t len)
{
    // Implement I2C write function
    return HAL_I2C_Mem_Write(&hi2c2, dev_id << 1, reg_addr, I2C_MEMADD_SIZE_8BIT, data, len, HAL_MAX_DELAY);
}

void user_delay_ms(uint32_t period)
{
    HAL_Delay(period);
}
void I2C_Scan(void)
{
    char buffer[25];
    int buffer_len;
    HAL_StatusTypeDef result;
    uint8_t i;
    buffer_len = sprintf(buffer, "Scanning I2C bus:\r\n");
    HAL_UART_Transmit(&huart2, (uint8_t*)buffer, buffer_len, 1000);

    for (i = 1; i < 128; i++)
    {
        result = HAL_I2C_IsDeviceReady(&hi2c2, (uint16_t)(i << 1), 2, 2);
        if (result == HAL_OK)
        {
            buffer_len = sprintf(buffer, "0x%02X ", i);
            HAL_UART_Transmit(&huart2, (uint8_t*)buffer, buffer_len, 1000);
        }
    }
    buffer_len = sprintf(buffer, "\r\n");
    HAL_UART_Transmit(&huart2, (uint8_t*)buffer, buffer_len, 1000);
}

// Function to initialize BME680 sensor
int8_t BME680_Initialize(struct bme680_dev *gas_sensor) {
	char init_msg[] = "Initializing BME680...\r\n";
	HAL_UART_Transmit(&huart2, (uint8_t*) init_msg, sizeof(init_msg) - 1,
			HAL_MAX_DELAY);

	// Set up sensor structure
	gas_sensor->dev_id = BME680_I2C_ADDR_PRIMARY; // 0x76
	gas_sensor->intf = BME680_I2C_INTF;
	gas_sensor->read = user_i2c_read;
	gas_sensor->write = user_i2c_write;
	gas_sensor->delay_ms = user_delay_ms;
	gas_sensor->amb_temp = 25;

	int8_t rslt = BME680_OK;
	int8_t rslt_secondary = BME680_OK;
	int attempt_count = 0;

	// Try initializing with primary I2C address
	while (attempt_count < 5) {
		rslt = bme680_init(gas_sensor);
		char buf[50];
		sprintf(buf, "Init attempt %d: result %d\r\n", attempt_count + 1, rslt);
		HAL_UART_Transmit(&huart2, (uint8_t*) buf, strlen(buf), HAL_MAX_DELAY);

		if (rslt == BME680_OK) {
			break;
		}
		attempt_count++;
		HAL_Delay(200);
	}

	// If primary address fails, try secondary I2C address
	if (rslt != BME680_OK) {
		gas_sensor->dev_id = BME680_I2C_ADDR_SECONDARY; // 0x77
		attempt_count = 0;
		while (attempt_count < 5) {
			rslt_secondary = bme680_init(gas_sensor);
			char buf[50];
			sprintf(buf, "Secondary Init attempt %d: result %d\r\n",
					attempt_count + 1, rslt_secondary);
			HAL_UART_Transmit(&huart2, (uint8_t*) buf, strlen(buf),
					HAL_MAX_DELAY);

			if (rslt_secondary == BME680_OK) {
				break;
			}
			attempt_count++;
			HAL_Delay(200);
		}
	}

	// Check final result
	if (rslt == BME680_OK || rslt_secondary == BME680_OK) {
		char success_msg[] = "BME680 initialized successfully!\r\n";
		HAL_UART_Transmit(&huart2, (uint8_t*) success_msg,
				sizeof(success_msg) - 1, HAL_MAX_DELAY);
		return BME680_OK;
	} else {
		char error_msg[] = "BME680 initialization failed!\r\n";
		HAL_UART_Transmit(&huart2, (uint8_t*) error_msg, sizeof(error_msg) - 1,
				HAL_MAX_DELAY);
		return BME680_E_COM_FAIL; // or appropriate error code
	}
}

// Function to configure BME680 sensor settings
int8_t BME680_Configure(struct bme680_dev *gas_sensor) {
	// Configure temperature, pressure, and humidity settings
	gas_sensor->tph_sett.os_hum = BME680_OS_2X;   // Humidity oversampling
	gas_sensor->tph_sett.os_pres = BME680_OS_4X;  // Pressure oversampling
	gas_sensor->tph_sett.os_temp = BME680_OS_8X;  // Temperature oversampling
	gas_sensor->tph_sett.filter = BME680_FILTER_SIZE_3; // IIR filter setting

	// Configure gas sensor settings
	gas_sensor->gas_sett.run_gas = BME680_ENABLE_GAS_MEAS; // Enable gas measurements
	gas_sensor->gas_sett.heatr_temp = 320; // Target temperature in degrees Celsius
	gas_sensor->gas_sett.heatr_dur = 150;  // Heating duration in milliseconds

	// Set power mode
	gas_sensor->power_mode = BME680_FORCED_MODE;

	// Define which settings to apply
	uint16_t desired_settings = BME680_OST_SEL | BME680_OSP_SEL | BME680_OSH_SEL
			| BME680_FILTER_SEL | BME680_GAS_SENSOR_SEL;

	// Apply the settings to the sensor
	int8_t rslt = bme680_set_sensor_settings(desired_settings, gas_sensor);
	if (rslt != BME680_OK) {
		char error_msg[] = "Failed to set sensor settings!\r\n";
		HAL_UART_Transmit(&huart2, (uint8_t*) error_msg, sizeof(error_msg) - 1,
				HAL_MAX_DELAY);
		return rslt;
	}

	// Get measurement period (optional, for information)
	uint16_t meas_period;
	bme680_get_profile_dur(&meas_period, gas_sensor);

	char config_msg[] = "BME680 configured successfully!\r\n";
	HAL_UART_Transmit(&huart2, (uint8_t*) config_msg, sizeof(config_msg) - 1,
			HAL_MAX_DELAY);

	return BME680_OK;
}
