################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/src_user/Applications/DriverInstances/block_command_handler.c \
../src/src_user/Applications/DriverInstances/command_response_handler.c \
../src/src_user/Applications/DriverInstances/dbg_packet_handler.c \
../src/src_user/Applications/DriverInstances/di_LoRa.c \
../src/src_user/Applications/DriverInstances/di_aobc.c \
../src/src_user/Applications/DriverInstances/di_cobc.c \
../src/src_user/Applications/DriverInstances/di_gs.c \
../src/src_user/Applications/DriverInstances/di_intersat.c \
../src/src_user/Applications/DriverInstances/di_rxpic.c \
../src/src_user/Applications/DriverInstances/di_sobc.c \
../src/src_user/Applications/DriverInstances/di_txpic.c \
../src/src_user/Applications/DriverInstances/di_uart_test.c \
../src/src_user/Applications/DriverInstances/mode_auto_transition.c \
../src/src_user/Applications/DriverInstances/tlm_packet_handler.c 

C_DEPS += \
./src/src_user/Applications/DriverInstances/block_command_handler.d \
./src/src_user/Applications/DriverInstances/command_response_handler.d \
./src/src_user/Applications/DriverInstances/dbg_packet_handler.d \
./src/src_user/Applications/DriverInstances/di_LoRa.d \
./src/src_user/Applications/DriverInstances/di_aobc.d \
./src/src_user/Applications/DriverInstances/di_cobc.d \
./src/src_user/Applications/DriverInstances/di_gs.d \
./src/src_user/Applications/DriverInstances/di_intersat.d \
./src/src_user/Applications/DriverInstances/di_rxpic.d \
./src/src_user/Applications/DriverInstances/di_sobc.d \
./src/src_user/Applications/DriverInstances/di_txpic.d \
./src/src_user/Applications/DriverInstances/di_uart_test.d \
./src/src_user/Applications/DriverInstances/mode_auto_transition.d \
./src/src_user/Applications/DriverInstances/tlm_packet_handler.d 

OBJS += \
./src/src_user/Applications/DriverInstances/block_command_handler.o \
./src/src_user/Applications/DriverInstances/command_response_handler.o \
./src/src_user/Applications/DriverInstances/dbg_packet_handler.o \
./src/src_user/Applications/DriverInstances/di_LoRa.o \
./src/src_user/Applications/DriverInstances/di_aobc.o \
./src/src_user/Applications/DriverInstances/di_cobc.o \
./src/src_user/Applications/DriverInstances/di_gs.o \
./src/src_user/Applications/DriverInstances/di_intersat.o \
./src/src_user/Applications/DriverInstances/di_rxpic.o \
./src/src_user/Applications/DriverInstances/di_sobc.o \
./src/src_user/Applications/DriverInstances/di_txpic.o \
./src/src_user/Applications/DriverInstances/di_uart_test.o \
./src/src_user/Applications/DriverInstances/mode_auto_transition.o \
./src/src_user/Applications/DriverInstances/tlm_packet_handler.o 


# Each subdirectory must supply rules for building sources it contributes
src/src_user/Applications/DriverInstances/%.o src/src_user/Applications/DriverInstances/%.su src/src_user/Applications/DriverInstances/%.cyclo: ../src/src_user/Applications/DriverInstances/%.c src/src_user/Applications/DriverInstances/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F469xx -DTWO_U -c -I../2U_config/Inc -I../2U_config/Drivers/STM32F4xx_HAL_Driver/Inc -I../2U_config/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../2U_config/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../2U_config/Drivers/CMSIS/Include -I../src -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-src-2f-src_user-2f-Applications-2f-DriverInstances

clean-src-2f-src_user-2f-Applications-2f-DriverInstances:
	-$(RM) ./src/src_user/Applications/DriverInstances/block_command_handler.cyclo ./src/src_user/Applications/DriverInstances/block_command_handler.d ./src/src_user/Applications/DriverInstances/block_command_handler.o ./src/src_user/Applications/DriverInstances/block_command_handler.su ./src/src_user/Applications/DriverInstances/command_response_handler.cyclo ./src/src_user/Applications/DriverInstances/command_response_handler.d ./src/src_user/Applications/DriverInstances/command_response_handler.o ./src/src_user/Applications/DriverInstances/command_response_handler.su ./src/src_user/Applications/DriverInstances/dbg_packet_handler.cyclo ./src/src_user/Applications/DriverInstances/dbg_packet_handler.d ./src/src_user/Applications/DriverInstances/dbg_packet_handler.o ./src/src_user/Applications/DriverInstances/dbg_packet_handler.su ./src/src_user/Applications/DriverInstances/di_LoRa.cyclo ./src/src_user/Applications/DriverInstances/di_LoRa.d ./src/src_user/Applications/DriverInstances/di_LoRa.o ./src/src_user/Applications/DriverInstances/di_LoRa.su ./src/src_user/Applications/DriverInstances/di_aobc.cyclo ./src/src_user/Applications/DriverInstances/di_aobc.d ./src/src_user/Applications/DriverInstances/di_aobc.o ./src/src_user/Applications/DriverInstances/di_aobc.su ./src/src_user/Applications/DriverInstances/di_cobc.cyclo ./src/src_user/Applications/DriverInstances/di_cobc.d ./src/src_user/Applications/DriverInstances/di_cobc.o ./src/src_user/Applications/DriverInstances/di_cobc.su ./src/src_user/Applications/DriverInstances/di_gs.cyclo ./src/src_user/Applications/DriverInstances/di_gs.d ./src/src_user/Applications/DriverInstances/di_gs.o ./src/src_user/Applications/DriverInstances/di_gs.su ./src/src_user/Applications/DriverInstances/di_intersat.cyclo ./src/src_user/Applications/DriverInstances/di_intersat.d ./src/src_user/Applications/DriverInstances/di_intersat.o ./src/src_user/Applications/DriverInstances/di_intersat.su ./src/src_user/Applications/DriverInstances/di_rxpic.cyclo ./src/src_user/Applications/DriverInstances/di_rxpic.d ./src/src_user/Applications/DriverInstances/di_rxpic.o ./src/src_user/Applications/DriverInstances/di_rxpic.su ./src/src_user/Applications/DriverInstances/di_sobc.cyclo ./src/src_user/Applications/DriverInstances/di_sobc.d ./src/src_user/Applications/DriverInstances/di_sobc.o ./src/src_user/Applications/DriverInstances/di_sobc.su ./src/src_user/Applications/DriverInstances/di_txpic.cyclo ./src/src_user/Applications/DriverInstances/di_txpic.d ./src/src_user/Applications/DriverInstances/di_txpic.o ./src/src_user/Applications/DriverInstances/di_txpic.su ./src/src_user/Applications/DriverInstances/di_uart_test.cyclo ./src/src_user/Applications/DriverInstances/di_uart_test.d ./src/src_user/Applications/DriverInstances/di_uart_test.o ./src/src_user/Applications/DriverInstances/di_uart_test.su ./src/src_user/Applications/DriverInstances/mode_auto_transition.cyclo ./src/src_user/Applications/DriverInstances/mode_auto_transition.d ./src/src_user/Applications/DriverInstances/mode_auto_transition.o ./src/src_user/Applications/DriverInstances/mode_auto_transition.su ./src/src_user/Applications/DriverInstances/tlm_packet_handler.cyclo ./src/src_user/Applications/DriverInstances/tlm_packet_handler.d ./src/src_user/Applications/DriverInstances/tlm_packet_handler.o ./src/src_user/Applications/DriverInstances/tlm_packet_handler.su

.PHONY: clean-src-2f-src_user-2f-Applications-2f-DriverInstances

