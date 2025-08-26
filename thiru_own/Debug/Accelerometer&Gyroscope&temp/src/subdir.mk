################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Accelerometer&Gyroscope&temp/src/mpu.c 

OBJS += \
./Accelerometer&Gyroscope&temp/src/mpu.o 

C_DEPS += \
./Accelerometer&Gyroscope&temp/src/mpu.d 


# Each subdirectory must supply rules for building sources it contributes
Accelerometer&Gyroscope&temp/src/%.o Accelerometer&Gyroscope&temp/src/%.su Accelerometer&Gyroscope&temp/src/%.cyclo: ../Accelerometer&Gyroscope&temp/src/%.c Accelerometer&Gyroscope&temp/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F091xC -c -I../Core/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/thiru/STM32CubeIDE/workspace_1.18.0/thiru/thiru_own/Accelerometer&Gyroscope&temp/inc" -I"C:/Users/thiru/STM32CubeIDE/workspace_1.18.0/thiru/thiru_own/BME680/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Accelerometer-26-Gyroscope-26-temp-2f-src

clean-Accelerometer-26-Gyroscope-26-temp-2f-src:
	-$(RM) ./Accelerometer&Gyroscope&temp/src/mpu.cyclo ./Accelerometer&Gyroscope&temp/src/mpu.d ./Accelerometer&Gyroscope&temp/src/mpu.o ./Accelerometer&Gyroscope&temp/src/mpu.su

.PHONY: clean-Accelerometer-26-Gyroscope-26-temp-2f-src

