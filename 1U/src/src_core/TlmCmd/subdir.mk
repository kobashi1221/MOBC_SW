################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/src_core/TlmCmd/block_command_executor.c \
../src/src_core/TlmCmd/block_command_loader.c \
../src/src_core/TlmCmd/block_command_table.c \
../src/src_core/TlmCmd/command_analyze.c \
../src/src_core/TlmCmd/command_dispatcher.c \
../src/src_core/TlmCmd/common_cmd_packet_util.c \
../src/src_core/TlmCmd/packet_handler.c \
../src/src_core/TlmCmd/packet_list.c \
../src/src_core/TlmCmd/packet_list_util.c \
../src/src_core/TlmCmd/telemetry_frame.c \
../src/src_core/TlmCmd/telemetry_generator.c 

C_DEPS += \
./src/src_core/TlmCmd/block_command_executor.d \
./src/src_core/TlmCmd/block_command_loader.d \
./src/src_core/TlmCmd/block_command_table.d \
./src/src_core/TlmCmd/command_analyze.d \
./src/src_core/TlmCmd/command_dispatcher.d \
./src/src_core/TlmCmd/common_cmd_packet_util.d \
./src/src_core/TlmCmd/packet_handler.d \
./src/src_core/TlmCmd/packet_list.d \
./src/src_core/TlmCmd/packet_list_util.d \
./src/src_core/TlmCmd/telemetry_frame.d \
./src/src_core/TlmCmd/telemetry_generator.d 

OBJS += \
./src/src_core/TlmCmd/block_command_executor.o \
./src/src_core/TlmCmd/block_command_loader.o \
./src/src_core/TlmCmd/block_command_table.o \
./src/src_core/TlmCmd/command_analyze.o \
./src/src_core/TlmCmd/command_dispatcher.o \
./src/src_core/TlmCmd/common_cmd_packet_util.o \
./src/src_core/TlmCmd/packet_handler.o \
./src/src_core/TlmCmd/packet_list.o \
./src/src_core/TlmCmd/packet_list_util.o \
./src/src_core/TlmCmd/telemetry_frame.o \
./src/src_core/TlmCmd/telemetry_generator.o 


# Each subdirectory must supply rules for building sources it contributes
src/src_core/TlmCmd/%.o src/src_core/TlmCmd/%.su src/src_core/TlmCmd/%.cyclo: ../src/src_core/TlmCmd/%.c src/src_core/TlmCmd/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F469xx -DONE_U -c -I../src -I../1U_config/Inc -I../1U_config/Drivers/STM32F4xx_HAL_Driver/Inc -I../1U_config/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../1U_config/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../1U_config/Drivers/CMSIS/Include -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-src-2f-src_core-2f-TlmCmd

clean-src-2f-src_core-2f-TlmCmd:
	-$(RM) ./src/src_core/TlmCmd/block_command_executor.cyclo ./src/src_core/TlmCmd/block_command_executor.d ./src/src_core/TlmCmd/block_command_executor.o ./src/src_core/TlmCmd/block_command_executor.su ./src/src_core/TlmCmd/block_command_loader.cyclo ./src/src_core/TlmCmd/block_command_loader.d ./src/src_core/TlmCmd/block_command_loader.o ./src/src_core/TlmCmd/block_command_loader.su ./src/src_core/TlmCmd/block_command_table.cyclo ./src/src_core/TlmCmd/block_command_table.d ./src/src_core/TlmCmd/block_command_table.o ./src/src_core/TlmCmd/block_command_table.su ./src/src_core/TlmCmd/command_analyze.cyclo ./src/src_core/TlmCmd/command_analyze.d ./src/src_core/TlmCmd/command_analyze.o ./src/src_core/TlmCmd/command_analyze.su ./src/src_core/TlmCmd/command_dispatcher.cyclo ./src/src_core/TlmCmd/command_dispatcher.d ./src/src_core/TlmCmd/command_dispatcher.o ./src/src_core/TlmCmd/command_dispatcher.su ./src/src_core/TlmCmd/common_cmd_packet_util.cyclo ./src/src_core/TlmCmd/common_cmd_packet_util.d ./src/src_core/TlmCmd/common_cmd_packet_util.o ./src/src_core/TlmCmd/common_cmd_packet_util.su ./src/src_core/TlmCmd/packet_handler.cyclo ./src/src_core/TlmCmd/packet_handler.d ./src/src_core/TlmCmd/packet_handler.o ./src/src_core/TlmCmd/packet_handler.su ./src/src_core/TlmCmd/packet_list.cyclo ./src/src_core/TlmCmd/packet_list.d ./src/src_core/TlmCmd/packet_list.o ./src/src_core/TlmCmd/packet_list.su ./src/src_core/TlmCmd/packet_list_util.cyclo ./src/src_core/TlmCmd/packet_list_util.d ./src/src_core/TlmCmd/packet_list_util.o ./src/src_core/TlmCmd/packet_list_util.su ./src/src_core/TlmCmd/telemetry_frame.cyclo ./src/src_core/TlmCmd/telemetry_frame.d ./src/src_core/TlmCmd/telemetry_frame.o ./src/src_core/TlmCmd/telemetry_frame.su ./src/src_core/TlmCmd/telemetry_generator.cyclo ./src/src_core/TlmCmd/telemetry_generator.d ./src/src_core/TlmCmd/telemetry_generator.o ./src/src_core/TlmCmd/telemetry_generator.su

.PHONY: clean-src-2f-src_core-2f-TlmCmd

