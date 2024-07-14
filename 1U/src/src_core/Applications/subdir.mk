################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/src_core/Applications/divided_cmd_utility.c \
../src/src_core/Applications/event_utility.c \
../src/src_core/Applications/gs_command_dispatcher.c \
../src/src_core/Applications/memory_dump.c \
../src/src_core/Applications/nop.c \
../src/src_core/Applications/realtime_command_dispatcher.c \
../src/src_core/Applications/telemetry_manager.c \
../src/src_core/Applications/timeline_command_dispatcher.c \
../src/src_core/Applications/utility_command.c \
../src/src_core/Applications/utility_counter.c 

C_DEPS += \
./src/src_core/Applications/divided_cmd_utility.d \
./src/src_core/Applications/event_utility.d \
./src/src_core/Applications/gs_command_dispatcher.d \
./src/src_core/Applications/memory_dump.d \
./src/src_core/Applications/nop.d \
./src/src_core/Applications/realtime_command_dispatcher.d \
./src/src_core/Applications/telemetry_manager.d \
./src/src_core/Applications/timeline_command_dispatcher.d \
./src/src_core/Applications/utility_command.d \
./src/src_core/Applications/utility_counter.d 

OBJS += \
./src/src_core/Applications/divided_cmd_utility.o \
./src/src_core/Applications/event_utility.o \
./src/src_core/Applications/gs_command_dispatcher.o \
./src/src_core/Applications/memory_dump.o \
./src/src_core/Applications/nop.o \
./src/src_core/Applications/realtime_command_dispatcher.o \
./src/src_core/Applications/telemetry_manager.o \
./src/src_core/Applications/timeline_command_dispatcher.o \
./src/src_core/Applications/utility_command.o \
./src/src_core/Applications/utility_counter.o 


# Each subdirectory must supply rules for building sources it contributes
src/src_core/Applications/%.o src/src_core/Applications/%.su src/src_core/Applications/%.cyclo: ../src/src_core/Applications/%.c src/src_core/Applications/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F469xx -DONE_U -c -I../src -I../1U_config/Inc -I../1U_config/Drivers/STM32F4xx_HAL_Driver/Inc -I../1U_config/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../1U_config/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../1U_config/Drivers/CMSIS/Include -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-src-2f-src_core-2f-Applications

clean-src-2f-src_core-2f-Applications:
	-$(RM) ./src/src_core/Applications/divided_cmd_utility.cyclo ./src/src_core/Applications/divided_cmd_utility.d ./src/src_core/Applications/divided_cmd_utility.o ./src/src_core/Applications/divided_cmd_utility.su ./src/src_core/Applications/event_utility.cyclo ./src/src_core/Applications/event_utility.d ./src/src_core/Applications/event_utility.o ./src/src_core/Applications/event_utility.su ./src/src_core/Applications/gs_command_dispatcher.cyclo ./src/src_core/Applications/gs_command_dispatcher.d ./src/src_core/Applications/gs_command_dispatcher.o ./src/src_core/Applications/gs_command_dispatcher.su ./src/src_core/Applications/memory_dump.cyclo ./src/src_core/Applications/memory_dump.d ./src/src_core/Applications/memory_dump.o ./src/src_core/Applications/memory_dump.su ./src/src_core/Applications/nop.cyclo ./src/src_core/Applications/nop.d ./src/src_core/Applications/nop.o ./src/src_core/Applications/nop.su ./src/src_core/Applications/realtime_command_dispatcher.cyclo ./src/src_core/Applications/realtime_command_dispatcher.d ./src/src_core/Applications/realtime_command_dispatcher.o ./src/src_core/Applications/realtime_command_dispatcher.su ./src/src_core/Applications/telemetry_manager.cyclo ./src/src_core/Applications/telemetry_manager.d ./src/src_core/Applications/telemetry_manager.o ./src/src_core/Applications/telemetry_manager.su ./src/src_core/Applications/timeline_command_dispatcher.cyclo ./src/src_core/Applications/timeline_command_dispatcher.d ./src/src_core/Applications/timeline_command_dispatcher.o ./src/src_core/Applications/timeline_command_dispatcher.su ./src/src_core/Applications/utility_command.cyclo ./src/src_core/Applications/utility_command.d ./src/src_core/Applications/utility_command.o ./src/src_core/Applications/utility_command.su ./src/src_core/Applications/utility_counter.cyclo ./src/src_core/Applications/utility_counter.d ./src/src_core/Applications/utility_counter.o ./src/src_core/Applications/utility_counter.su

.PHONY: clean-src-2f-src_core-2f-Applications

