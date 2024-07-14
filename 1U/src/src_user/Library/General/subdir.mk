################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/src_user/Library/General/DBG_printf.c \
../src/src_user/Library/General/NU1_printf.c 

C_DEPS += \
./src/src_user/Library/General/DBG_printf.d \
./src/src_user/Library/General/NU1_printf.d 

OBJS += \
./src/src_user/Library/General/DBG_printf.o \
./src/src_user/Library/General/NU1_printf.o 


# Each subdirectory must supply rules for building sources it contributes
src/src_user/Library/General/%.o src/src_user/Library/General/%.su src/src_user/Library/General/%.cyclo: ../src/src_user/Library/General/%.c src/src_user/Library/General/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F469xx -DONE_U -c -I../src -I../1U_config/Inc -I../1U_config/Drivers/STM32F4xx_HAL_Driver/Inc -I../1U_config/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../1U_config/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../1U_config/Drivers/CMSIS/Include -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-src-2f-src_user-2f-Library-2f-General

clean-src-2f-src_user-2f-Library-2f-General:
	-$(RM) ./src/src_user/Library/General/DBG_printf.cyclo ./src/src_user/Library/General/DBG_printf.d ./src/src_user/Library/General/DBG_printf.o ./src/src_user/Library/General/DBG_printf.su ./src/src_user/Library/General/NU1_printf.cyclo ./src/src_user/Library/General/NU1_printf.d ./src/src_user/Library/General/NU1_printf.o ./src/src_user/Library/General/NU1_printf.su

.PHONY: clean-src-2f-src_user-2f-Library-2f-General

