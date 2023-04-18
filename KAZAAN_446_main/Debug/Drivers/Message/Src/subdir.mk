################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/Message/Src/message.c \
../Drivers/Message/Src/xprintf.c 

OBJS += \
./Drivers/Message/Src/message.o \
./Drivers/Message/Src/xprintf.o 

C_DEPS += \
./Drivers/Message/Src/message.d \
./Drivers/Message/Src/xprintf.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Message/Src/%.o: ../Drivers/Message/Src/%.c Drivers/Message/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

