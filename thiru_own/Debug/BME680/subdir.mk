################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../BME680/bme680.c \
../BME680/bmemain.c 

OBJS += \
./BME680/bme680.o \
./BME680/bmemain.o 

C_DEPS += \
./BME680/bme680.d \
./BME680/bmemain.d 


# Each subdirectory must supply rules for building sources it contributes
BME680/%.o BME680/%.su BME680/%.cyclo: ../BME680/%.c BME680/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F091xC -c -I../Core/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/thiru/STM32CubeIDE/workspace_1.18.0/thiru/thiru_own/Accelerometer&Gyroscope&temp/inc" -I"C:/Users/thiru/STM32CubeIDE/workspace_1.18.0/thiru/thiru_own/BME680" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-BME680

clean-BME680:
	-$(RM) ./BME680/bme680.cyclo ./BME680/bme680.d ./BME680/bme680.o ./BME680/bme680.su ./BME680/bmemain.cyclo ./BME680/bmemain.d ./BME680/bmemain.o ./BME680/bmemain.su

.PHONY: clean-BME680

