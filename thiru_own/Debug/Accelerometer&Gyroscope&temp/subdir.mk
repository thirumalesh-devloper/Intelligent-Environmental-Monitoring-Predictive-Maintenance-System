################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Accelerometer&Gyroscope&temp/mpu.c 

OBJS += \
./Accelerometer&Gyroscope&temp/mpu.o 

C_DEPS += \
./Accelerometer&Gyroscope&temp/mpu.d 


# Each subdirectory must supply rules for building sources it contributes
Accelerometer&Gyroscope&temp/%.o Accelerometer&Gyroscope&temp/%.su Accelerometer&Gyroscope&temp/%.cyclo: ../Accelerometer&Gyroscope&temp/%.c Accelerometer&Gyroscope&temp/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F091xC -c -I../Core/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/thiru/STM32CubeIDE/workspace_1.18.0/thiru/thiru_own/Accelerometer&Gyroscope&temp" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Accelerometer-26-Gyroscope-26-temp

clean-Accelerometer-26-Gyroscope-26-temp:
	-$(RM) ./Accelerometer&Gyroscope&temp/mpu.cyclo ./Accelerometer&Gyroscope&temp/mpu.d ./Accelerometer&Gyroscope&temp/mpu.o ./Accelerometer&Gyroscope&temp/mpu.su

.PHONY: clean-Accelerometer-26-Gyroscope-26-temp

