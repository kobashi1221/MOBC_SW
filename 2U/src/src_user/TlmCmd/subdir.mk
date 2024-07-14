################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/src_user/TlmCmd/block_command_definitions.c \
../src/src_user/TlmCmd/block_command_user_settings.c \
../src/src_user/TlmCmd/command_definitions.c \
../src/src_user/TlmCmd/common_cmd_packet.c \
../src/src_user/TlmCmd/common_tlm_cmd_packet.c \
../src/src_user/TlmCmd/common_tlm_packet.c \
../src/src_user/TlmCmd/telemetry_definitions.c \
../src/src_user/TlmCmd/user_packet_handler.c 

C_DEPS += \
./src/src_user/TlmCmd/block_command_definitions.d \
./src/src_user/TlmCmd/block_command_user_settings.d \
./src/src_user/TlmCmd/command_definitions.d \
./src/src_user/TlmCmd/common_cmd_packet.d \
./src/src_user/TlmCmd/common_tlm_cmd_packet.d \
./src/src_user/TlmCmd/common_tlm_packet.d \
./src/src_user/TlmCmd/telemetry_definitions.d \
./src/src_user/TlmCmd/user_packet_handler.d 

OBJS += \
./src/src_user/TlmCmd/block_command_definitions.o \
./src/src_user/TlmCmd/block_command_user_settings.o \
./src/src_user/TlmCmd/command_definitions.o \
./src/src_user/TlmCmd/common_cmd_packet.o \
./src/src_user/TlmCmd/common_tlm_cmd_packet.o \
./src/src_user/TlmCmd/common_tlm_packet.o \
./src/src_user/TlmCmd/telemetry_definitions.o \
./src/src_user/TlmCmd/user_packet_handler.o 


# Each subdirectory must supply rules for building sources it contributes
src/src_user/TlmCmd/%.o src/src_user/TlmCmd/%.su src/src_user/TlmCmd/%.cyclo: ../src/src_user/TlmCmd/%.c src/src_user/TlmCmd/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F469xx -DTWO_U -c -I../2U_config/Inc -I../2U_config/Drivers/STM32F4xx_HAL_Driver/Inc -I../2U_config/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../2U_config/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../2U_config/Drivers/CMSIS/Include -I../src -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-src-2f-src_user-2f-TlmCmd

clean-src-2f-src_user-2f-TlmCmd:
	-$(RM) ./src/src_user/TlmCmd/block_command_definitions.cyclo ./src/src_user/TlmCmd/block_command_definitions.d ./src/src_user/TlmCmd/block_command_definitions.o ./src/src_user/TlmCmd/block_command_definitions.su ./src/src_user/TlmCmd/block_command_user_settings.cyclo ./src/src_user/TlmCmd/block_command_user_settings.d ./src/src_user/TlmCmd/block_command_user_settings.o ./src/src_user/TlmCmd/block_command_user_settings.su ./src/src_user/TlmCmd/command_definitions.cyclo ./src/src_user/TlmCmd/command_definitions.d ./src/src_user/TlmCmd/command_definitions.o ./src/src_user/TlmCmd/command_definitions.su ./src/src_user/TlmCmd/common_cmd_packet.cyclo ./src/src_user/TlmCmd/common_cmd_packet.d ./src/src_user/TlmCmd/common_cmd_packet.o ./src/src_user/TlmCmd/common_cmd_packet.su ./src/src_user/TlmCmd/common_tlm_cmd_packet.cyclo ./src/src_user/TlmCmd/common_tlm_cmd_packet.d ./src/src_user/TlmCmd/common_tlm_cmd_packet.o ./src/src_user/TlmCmd/common_tlm_cmd_packet.su ./src/src_user/TlmCmd/common_tlm_packet.cyclo ./src/src_user/TlmCmd/common_tlm_packet.d ./src/src_user/TlmCmd/common_tlm_packet.o ./src/src_user/TlmCmd/common_tlm_packet.su ./src/src_user/TlmCmd/telemetry_definitions.cyclo ./src/src_user/TlmCmd/telemetry_definitions.d ./src/src_user/TlmCmd/telemetry_definitions.o ./src/src_user/TlmCmd/telemetry_definitions.su ./src/src_user/TlmCmd/user_packet_handler.cyclo ./src/src_user/TlmCmd/user_packet_handler.d ./src/src_user/TlmCmd/user_packet_handler.o ./src/src_user/TlmCmd/user_packet_handler.su

.PHONY: clean-src-2f-src_user-2f-TlmCmd

