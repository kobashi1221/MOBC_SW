################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../1U_config/Src/main.c \
../1U_config/Src/stm32f4xx_hal_msp.c \
../1U_config/Src/stm32f4xx_hal_timebase_tim.c \
../1U_config/Src/stm32f4xx_it.c \
../1U_config/Src/syscalls.c \
../1U_config/Src/sysmem.c \
../1U_config/Src/system_stm32f4xx.c 

C_DEPS += \
./1U_config/Src/main.d \
./1U_config/Src/stm32f4xx_hal_msp.d \
./1U_config/Src/stm32f4xx_hal_timebase_tim.d \
./1U_config/Src/stm32f4xx_it.d \
./1U_config/Src/syscalls.d \
./1U_config/Src/sysmem.d \
./1U_config/Src/system_stm32f4xx.d 

OBJS += \
./1U_config/Src/main.o \
./1U_config/Src/stm32f4xx_hal_msp.o \
./1U_config/Src/stm32f4xx_hal_timebase_tim.o \
./1U_config/Src/stm32f4xx_it.o \
./1U_config/Src/syscalls.o \
./1U_config/Src/sysmem.o \
./1U_config/Src/system_stm32f4xx.o 


# Each subdirectory must supply rules for building sources it contributes
1U_config/Src/%.o 1U_config/Src/%.su 1U_config/Src/%.cyclo: ../1U_config/Src/%.c 1U_config/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F469xx -DONE_U -c -I../src -I../1U_config/Inc -I../1U_config/Drivers/STM32F4xx_HAL_Driver/Inc -I../1U_config/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../1U_config/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../1U_config/Drivers/CMSIS/Include -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-1U_config-2f-Src

clean-1U_config-2f-Src:
	-$(RM) ./1U_config/Src/main.cyclo ./1U_config/Src/main.d ./1U_config/Src/main.o ./1U_config/Src/main.su ./1U_config/Src/stm32f4xx_hal_msp.cyclo ./1U_config/Src/stm32f4xx_hal_msp.d ./1U_config/Src/stm32f4xx_hal_msp.o ./1U_config/Src/stm32f4xx_hal_msp.su ./1U_config/Src/stm32f4xx_hal_timebase_tim.cyclo ./1U_config/Src/stm32f4xx_hal_timebase_tim.d ./1U_config/Src/stm32f4xx_hal_timebase_tim.o ./1U_config/Src/stm32f4xx_hal_timebase_tim.su ./1U_config/Src/stm32f4xx_it.cyclo ./1U_config/Src/stm32f4xx_it.d ./1U_config/Src/stm32f4xx_it.o ./1U_config/Src/stm32f4xx_it.su ./1U_config/Src/syscalls.cyclo ./1U_config/Src/syscalls.d ./1U_config/Src/syscalls.o ./1U_config/Src/syscalls.su ./1U_config/Src/sysmem.cyclo ./1U_config/Src/sysmem.d ./1U_config/Src/sysmem.o ./1U_config/Src/sysmem.su ./1U_config/Src/system_stm32f4xx.cyclo ./1U_config/Src/system_stm32f4xx.d ./1U_config/Src/system_stm32f4xx.o ./1U_config/Src/system_stm32f4xx.su

.PHONY: clean-1U_config-2f-Src

