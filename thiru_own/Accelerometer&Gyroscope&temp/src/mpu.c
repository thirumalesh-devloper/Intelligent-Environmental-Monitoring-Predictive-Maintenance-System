/*
 * mpu.c
 *
 *  Created on: Aug 26, 2025
 *      Author: thiru
 */
#include"mpu.h"
I2C_HandleTypeDef hi2c1;
UART_HandleTypeDef huart2;
MPU6050_t MPU6050;
char uart_buf[2000];
void MPU6050_Init(void) {
    uint8_t check;
    uint8_t Data;

    // Check device ID WHO_AM_I
    HAL_I2C_Mem_Read(&hi2c1, MPU6050_ADDR, WHO_AM_I_REG, 1, &check, 1, 1000);

    if (check == 104) { // 0x68 will be returned by the sensor if everything goes well
        // Power management register 0X6B we should write all 0's to wake the sensor up
        Data = 0;
        HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, PWR_MGMT_1_REG, 1, &Data, 1, 1000);

        // Set DATA RATE of 1KHz by writing SMPLRT_DIV register
        Data = 0x07;
        HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, SMPLRT_DIV_REG, 1, &Data, 1, 1000);

        // Set accelerometer configuration in ACCEL_CONFIG Register
        // XA_ST=0,YA_ST=0,ZA_ST=0, FS_SEL=0 -> ± 2g
        Data = 0x00;
        HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, ACCEL_CONFIG_REG, 1, &Data, 1, 1000);

        // Set Gyroscopic configuration in GYRO_CONFIG Register
        // XG_ST=0,YG_ST=0,ZG_ST=0, FS_SEL=0 -> ± 250 °/s
        Data = 0x00;
        HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, GYRO_CONFIG_REG, 1, &Data, 1, 1000);

        sprintf(uart_buf, "MPU6050 Initialization Successful!\r\n");
        HAL_UART_Transmit(&huart2, (uint8_t*)uart_buf, strlen(uart_buf), HAL_MAX_DELAY);
    } else {
        sprintf(uart_buf, "MPU6050 Initialization Failed! WHO_AM_I = %d\r\n", check);
        HAL_UART_Transmit(&huart2, (uint8_t*)uart_buf, strlen(uart_buf), HAL_MAX_DELAY);
    }
}

void MPU6050_Read_Accel(void) {
    uint8_t Rec_Data[6];

    // Read 6 BYTES of data starting from ACCEL_XOUT_H register
    HAL_I2C_Mem_Read(&hi2c1, MPU6050_ADDR, ACCEL_XOUT_H_REG, 1, Rec_Data, 6, 1000);

    MPU6050.Accel_X_RAW = (int16_t)(Rec_Data[0] << 8 | Rec_Data[1]);
    MPU6050.Accel_Y_RAW = (int16_t)(Rec_Data[2] << 8 | Rec_Data[3]);
    MPU6050.Accel_Z_RAW = (int16_t)(Rec_Data[4] << 8 | Rec_Data[5]);

    /*** convert the RAW values into acceleration in 'g'
         we have to divide according to the Full scale value set in FS_SEL
         I have configured FS_SEL = 0. So I am dividing by 16384.0
         for more details check ACCEL_CONFIG Register              ****/

    MPU6050.Ax = MPU6050.Accel_X_RAW / 16384.0;
    MPU6050.Ay = MPU6050.Accel_Y_RAW / 16384.0;
    MPU6050.Az = MPU6050.Accel_Z_RAW / 16384.0;
}

void MPU6050_Read_Gyro(void) {
    uint8_t Rec_Data[6];

    // Read 6 BYTES of data starting from GYRO_XOUT_H register
    HAL_I2C_Mem_Read(&hi2c1, MPU6050_ADDR, GYRO_XOUT_H_REG, 1, Rec_Data, 6, 1000);

    MPU6050.Gyro_X_RAW = (int16_t)(Rec_Data[0] << 8 | Rec_Data[1]);
    MPU6050.Gyro_Y_RAW = (int16_t)(Rec_Data[2] << 8 | Rec_Data[3]);
    MPU6050.Gyro_Z_RAW = (int16_t)(Rec_Data[4] << 8 | Rec_Data[5]);

    /*** convert the RAW values into dps (°/s)
         we have to divide according to the Full scale value set in FS_SEL
         I have configured FS_SEL = 0. So I am dividing by 131.0
         for more details check GYRO_CONFIG Register                ****/

    MPU6050.Gx = MPU6050.Gyro_X_RAW / 131.0;
    MPU6050.Gy = MPU6050.Gyro_Y_RAW / 131.0;
    MPU6050.Gz = MPU6050.Gyro_Z_RAW / 131.0;
}

void MPU6050_Read_Temp(void) {
    uint8_t Rec_Data[2];
    int16_t temp;

    // Read 2 BYTES of data starting from TEMP_OUT_H_REG
    HAL_I2C_Mem_Read(&hi2c1, MPU6050_ADDR, TEMP_OUT_H_REG, 1, Rec_Data, 2, 1000);

    temp = (int16_t)(Rec_Data[0] << 8 | Rec_Data[1]);
    MPU6050.Temperature = ((float)temp / 340.0) + 36.53;
}
void MPU6050_Read_All(void) {
    uint8_t Rec_Data[14];
    HAL_StatusTypeDef status;
    static uint32_t error_count = 0;

    // Clear buffer before reading
    memset(Rec_Data, 0, sizeof(Rec_Data));

    // Read 14 BYTES of data starting from ACCEL_XOUT_H register with error checking
    status = HAL_I2C_Mem_Read(&hi2c1, MPU6050_ADDR, ACCEL_XOUT_H_REG, 1, Rec_Data, 14, 2000);

    if (status != HAL_OK) {
        error_count++;
        sprintf(uart_buf, "I2C Error! Status: %d, Error Count: %lu\r\n", status, error_count);
        HAL_UART_Transmit(&huart2, (uint8_t*)uart_buf, strlen(uart_buf), HAL_MAX_DELAY);

        // Try to recover I2C
        if (error_count % 5 == 0) { // Every 5 errors, try to reinitialize I2C
            sprintf(uart_buf, "Attempting I2C recovery...\r\n");
            HAL_UART_Transmit(&huart2, (uint8_t*)uart_buf, strlen(uart_buf), HAL_MAX_DELAY);

            HAL_I2C_DeInit(&hi2c1);
            HAL_Delay(100);
            HAL_I2C_Init(&hi2c1);
            HAL_Delay(100);
        }
        return; // Don't update values on error
    }

    // Check if we got valid data (not all zeros which might indicate communication issue)
    bool valid_data = false;
    for (int i = 0; i < 14; i++) {
        if (Rec_Data[i] != 0) {
            valid_data = true;
            break;
        }
    }

    if (!valid_data) {
        sprintf(uart_buf, "Warning: All zero data received!\r\n");
        HAL_UART_Transmit(&huart2, (uint8_t*)uart_buf, strlen(uart_buf), HAL_MAX_DELAY);
        return;
    }

    // Parse the data only if read was successful
    MPU6050.Accel_X_RAW = (int16_t)(Rec_Data[0] << 8 | Rec_Data[1]);
    MPU6050.Accel_Y_RAW = (int16_t)(Rec_Data[2] << 8 | Rec_Data[3]);
    MPU6050.Accel_Z_RAW = (int16_t)(Rec_Data[4] << 8 | Rec_Data[5]);
    int16_t temp = (int16_t)(Rec_Data[6] << 8 | Rec_Data[7]);
    MPU6050.Gyro_X_RAW = (int16_t)(Rec_Data[8] << 8 | Rec_Data[9]);
    MPU6050.Gyro_Y_RAW = (int16_t)(Rec_Data[10] << 8 | Rec_Data[11]);
    MPU6050.Gyro_Z_RAW = (int16_t)(Rec_Data[12] << 8 | Rec_Data[13]);

    // Convert to engineering units
    MPU6050.Ax = MPU6050.Accel_X_RAW / 16384.0;
    MPU6050.Ay = MPU6050.Accel_Y_RAW / 16384.0;
    MPU6050.Az = MPU6050.Accel_Z_RAW / 16384.0;

    MPU6050.Temperature = ((float)temp / 340.0) + 36.53;

    MPU6050.Gx = MPU6050.Gyro_X_RAW / 131.0;
    MPU6050.Gy = MPU6050.Gyro_Y_RAW / 131.0;
    MPU6050.Gz = MPU6050.Gyro_Z_RAW / 131.0;

    // Reset error count on successful read
    error_count = 0;
}

// Alternative: Add data validation by checking for reasonable ranges
bool MPU6050_Validate_Data(void) {
    // Check if accelerometer values are reasonable (within ±4g for ±2g setting)
    if (fabs(MPU6050.Ax) > 4.0 || fabs(MPU6050.Ay) > 4.0 || fabs(MPU6050.Az) > 4.0) {
        return false;
    }

    // Check if gyro values are reasonable (within ±500°/s for ±250°/s setting)
    if (fabs(MPU6050.Gx) > 500.0 || fabs(MPU6050.Gy) > 500.0 || fabs(MPU6050.Gz) > 500.0) {
        return false;
    }

    // Check if temperature is reasonable (-40°C to +85°C)
    if (MPU6050.Temperature < -40.0 || MPU6050.Temperature > 85.0) {
        return false;
    }

    return true;
}

// Modified MPU6050_Read_All with better error handling
bool MPU6050_Read_All_Safe(void) {
    uint8_t Rec_Data[14];
    HAL_StatusTypeDef status;
    static uint32_t error_count = 0;

    // Clear buffer before reading
    memset(Rec_Data, 0, sizeof(Rec_Data));

    // Read 14 BYTES of data starting from ACCEL_XOUT_H register with error checking
    status = HAL_I2C_Mem_Read(&hi2c1, MPU6050_ADDR, ACCEL_XOUT_H_REG, 1, Rec_Data, 14, 2000);

    if (status != HAL_OK) {
        error_count++;
        sprintf(uart_buf, "I2C Error! Status: %d, Error Count: %lu\r\n", status, error_count);
        HAL_UART_Transmit(&huart2, (uint8_t*)uart_buf, strlen(uart_buf), HAL_MAX_DELAY);

        // Try to recover I2C every 3 errors
        if (error_count % 3 == 0) {
            sprintf(uart_buf, "Attempting I2C recovery...\r\n");
            HAL_UART_Transmit(&huart2, (uint8_t*)uart_buf, strlen(uart_buf), HAL_MAX_DELAY);

            HAL_I2C_DeInit(&hi2c1);
            HAL_Delay(50);
            HAL_I2C_Init(&hi2c1);
            HAL_Delay(50);
            MPU6050_Init(); // Re-initialize the sensor
        }
        return false; // Return false to indicate failure
    }

    // Check if we got valid data (not all zeros)
    bool valid_data = false;
    for (int i = 0; i < 14; i++) {
        if (Rec_Data[i] != 0) {
            valid_data = true;
            break;
        }
    }

    if (!valid_data) {
        error_count++;
        return false; // Return false for invalid data
    }

    // Parse the data only if read was successful and valid
    MPU6050.Accel_X_RAW = (int16_t)(Rec_Data[0] << 8 | Rec_Data[1]);
    MPU6050.Accel_Y_RAW = (int16_t)(Rec_Data[2] << 8 | Rec_Data[3]);
    MPU6050.Accel_Z_RAW = (int16_t)(Rec_Data[4] << 8 | Rec_Data[5]);
    int16_t temp = (int16_t)(Rec_Data[6] << 8 | Rec_Data[7]);
    MPU6050.Gyro_X_RAW = (int16_t)(Rec_Data[8] << 8 | Rec_Data[9]);
    MPU6050.Gyro_Y_RAW = (int16_t)(Rec_Data[10] << 8 | Rec_Data[11]);
    MPU6050.Gyro_Z_RAW = (int16_t)(Rec_Data[12] << 8 | Rec_Data[13]);

    // Convert to engineering units
    MPU6050.Ax = MPU6050.Accel_X_RAW / 16384.0;
    MPU6050.Ay = MPU6050.Accel_Y_RAW / 16384.0;
    MPU6050.Az = MPU6050.Accel_Z_RAW / 16384.0;

    MPU6050.Temperature = ((float)temp / 340.0) + 36.53;

    MPU6050.Gx = MPU6050.Gyro_X_RAW / 131.0;
    MPU6050.Gy = MPU6050.Gyro_Y_RAW / 131.0;
    MPU6050.Gz = MPU6050.Gyro_Z_RAW / 131.0;

    // Reset error count on successful read
    error_count = 0;
    return true; // Return true for successful read
}
