################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../2U_config/Src/main.c \
../2U_config/Src/stm32f4xx_hal_msp.c \
../2U_config/Src/stm32f4xx_hal_timebase_tim.c \
../2U_config/Src/stm32f4xx_it.c \
../2U_config/Src/syscalls.c \
../2U_config/Src/sysmem.c \
../2U_config/Src/system_stm32f4xx.c 

C_DEPS += \
./2U_config/Src/main.d \
./2U_config/Src/stm32f4xx_hal_msp.d \
./2U_config/Src/stm32f4xx_hal_timebase_tim.d \
./2U_config/Src/stm32f4xx_it.d \
./2U_config/Src/syscalls.d \
./2U_config/Src/sysmem.d \
./2U_config/Src/system_stm32f4xx.d 

OBJS += \
./2U_config/Src/main.o \
./2U_config/Src/stm32f4xx_hal_msp.o \
./2U_config/Src/stm32f4xx_hal_timebase_tim.o \
./2U_config/Src/stm32f4xx_it.o \
./2U_config/Src/syscalls.o \
./2U_config/Src/sysmem.o \
./2U_config/Src/system_stm32f4xx.o 


# Each subdirectory must supply rules for building sources it contributes
2U_config/Src/%.o 2U_config/Src/%.su 2U_config/Src/%.cyclo: ../2U_config/Src/%.c 2U_config/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F469xx -DTWO_U -c -I../2U_config/Inc -I../2U_config/Drivers/STM32F4xx_HAL_Driver/Inc -I../2U_config/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../2U_config/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../2U_config/Drivers/CMSIS/Include -I../src -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-2U_config-2f-Src

clean-2U_config-2f-Src:
	-$(RM) ./2U_config/Src/main.cyclo ./2U_config/Src/main.d ./2U_config/Src/main.o ./2U_config/Src/main.su ./2U_config/Src/stm32f4xx_hal_msp.cyclo ./2U_config/Src/stm32f4xx_hal_msp.d ./2U_config/Src/stm32f4xx_hal_msp.o ./2U_config/Src/stm32f4xx_hal_msp.su ./2U_config/Src/stm32f4xx_hal_timebase_tim.cyclo ./2U_config/Src/stm32f4xx_hal_timebase_tim.d ./2U_config/Src/stm32f4xx_hal_timebase_tim.o ./2U_config/Src/stm32f4xx_hal_timebase_tim.su ./2U_config/Src/stm32f4xx_it.cyclo ./2U_config/Src/stm32f4xx_it.d ./2U_config/Src/stm32f4xx_it.o ./2U_config/Src/stm32f4xx_it.su ./2U_config/Src/syscalls.cyclo ./2U_config/Src/syscalls.d ./2U_config/Src/syscalls.o ./2U_config/Src/syscalls.su ./2U_config/Src/sysmem.cyclo ./2U_config/Src/sysmem.d ./2U_config/Src/sysmem.o ./2U_config/Src/sysmem.su ./2U_config/Src/system_stm32f4xx.cyclo ./2U_config/Src/system_stm32f4xx.d ./2U_config/Src/system_stm32f4xx.o ./2U_config/Src/system_stm32f4xx.su

.PHONY: clean-2U_config-2f-Src

