################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/src_user/Drivers/downlink_printf.c \
../src/src_user/Drivers/global_interrupt.c \
../src/src_user/Drivers/large_packet_driver.c 

C_DEPS += \
./src/src_user/Drivers/downlink_printf.d \
./src/src_user/Drivers/global_interrupt.d \
./src/src_user/Drivers/large_packet_driver.d 

OBJS += \
./src/src_user/Drivers/downlink_printf.o \
./src/src_user/Drivers/global_interrupt.o \
./src/src_user/Drivers/large_packet_driver.o 


# Each subdirectory must supply rules for building sources it contributes
src/src_user/Drivers/%.o src/src_user/Drivers/%.su src/src_user/Drivers/%.cyclo: ../src/src_user/Drivers/%.c src/src_user/Drivers/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F469xx -DTWO_U -c -I../2U_config/Inc -I../2U_config/Drivers/STM32F4xx_HAL_Driver/Inc -I../2U_config/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../2U_config/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../2U_config/Drivers/CMSIS/Include -I../src -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-src-2f-src_user-2f-Drivers

clean-src-2f-src_user-2f-Drivers:
	-$(RM) ./src/src_user/Drivers/downlink_printf.cyclo ./src/src_user/Drivers/downlink_printf.d ./src/src_user/Drivers/downlink_printf.o ./src/src_user/Drivers/downlink_printf.su ./src/src_user/Drivers/global_interrupt.cyclo ./src/src_user/Drivers/global_interrupt.d ./src/src_user/Drivers/global_interrupt.o ./src/src_user/Drivers/global_interrupt.su ./src/src_user/Drivers/large_packet_driver.cyclo ./src/src_user/Drivers/large_packet_driver.d ./src/src_user/Drivers/large_packet_driver.o ./src/src_user/Drivers/large_packet_driver.su

.PHONY: clean-src-2f-src_user-2f-Drivers

