################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/src_user/Settings/TlmCmd/common_cmd_packet_define.c 

C_DEPS += \
./src/src_user/Settings/TlmCmd/common_cmd_packet_define.d 

OBJS += \
./src/src_user/Settings/TlmCmd/common_cmd_packet_define.o 


# Each subdirectory must supply rules for building sources it contributes
src/src_user/Settings/TlmCmd/%.o src/src_user/Settings/TlmCmd/%.su src/src_user/Settings/TlmCmd/%.cyclo: ../src/src_user/Settings/TlmCmd/%.c src/src_user/Settings/TlmCmd/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F469xx -DONE_U -c -I../src -I../1U_config/Inc -I../1U_config/Drivers/STM32F4xx_HAL_Driver/Inc -I../1U_config/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../1U_config/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../1U_config/Drivers/CMSIS/Include -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-src-2f-src_user-2f-Settings-2f-TlmCmd

clean-src-2f-src_user-2f-Settings-2f-TlmCmd:
	-$(RM) ./src/src_user/Settings/TlmCmd/common_cmd_packet_define.cyclo ./src/src_user/Settings/TlmCmd/common_cmd_packet_define.d ./src/src_user/Settings/TlmCmd/common_cmd_packet_define.o ./src/src_user/Settings/TlmCmd/common_cmd_packet_define.su

.PHONY: clean-src-2f-src_user-2f-Settings-2f-TlmCmd

