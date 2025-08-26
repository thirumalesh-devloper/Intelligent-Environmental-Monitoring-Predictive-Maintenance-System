/*
 * mpu.h
 *
 *  Created on: Aug 26, 2025
 *      Author: thiru
 */

#ifndef INC_MPU_H_
#define INC_MPU_H_
#include"main.h"
#include"string.h"
#include"stdio.h"
#include"math.h"
#include <stdbool.h>
void MPU6050_Init(void);
void MPU6050_Read_Accel(void);
void MPU6050_Read_Gyro(void);
void MPU6050_Read_Temp(void);
void MPU6050_Read_All(void);
bool MPU6050_Validate_Data(void);
bool MPU6050_Read_All_Safe(void);

// MPU6050 I2C Address
#define MPU6050_ADDR 0xD0

// MPU6050 Register Addresses
#define SMPLRT_DIV_REG 0x19
#define GYRO_CONFIG_REG 0x1B
#define ACCEL_CONFIG_REG 0x1C
#define ACCEL_XOUT_H_REG 0x3B
#define TEMP_OUT_H_REG 0x41
#define GYRO_XOUT_H_REG 0x43
#define PWR_MGMT_1_REG 0x6B
#define WHO_AM_I_REG 0x75

typedef struct {
    int16_t Accel_X_RAW;
    int16_t Accel_Y_RAW;
    int16_t Accel_Z_RAW;
    double Ax;
    double Ay;
    double Az;

    int16_t Gyro_X_RAW;
    int16_t Gyro_Y_RAW;
    int16_t Gyro_Z_RAW;
    double Gx;
    double Gy;
    double Gz;

    float Temperature;
} MPU6050_t;
#endif /* INC_MPU_H_ */
