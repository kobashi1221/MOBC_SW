################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/src_core/TlmCmd/Ccsds/cmd_space_packet.c \
../src/src_core/TlmCmd/Ccsds/space_packet.c \
../src/src_core/TlmCmd/Ccsds/tlm_space_packet.c 

C_DEPS += \
./src/src_core/TlmCmd/Ccsds/cmd_space_packet.d \
./src/src_core/TlmCmd/Ccsds/space_packet.d \
./src/src_core/TlmCmd/Ccsds/tlm_space_packet.d 

OBJS += \
./src/src_core/TlmCmd/Ccsds/cmd_space_packet.o \
./src/src_core/TlmCmd/Ccsds/space_packet.o \
./src/src_core/TlmCmd/Ccsds/tlm_space_packet.o 


# Each subdirectory must supply rules for building sources it contributes
src/src_core/TlmCmd/Ccsds/%.o src/src_core/TlmCmd/Ccsds/%.su src/src_core/TlmCmd/Ccsds/%.cyclo: ../src/src_core/TlmCmd/Ccsds/%.c src/src_core/TlmCmd/Ccsds/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F469xx -DTWO_U -c -I../2U_config/Inc -I../2U_config/Drivers/STM32F4xx_HAL_Driver/Inc -I../2U_config/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../2U_config/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../2U_config/Drivers/CMSIS/Include -I../src -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-src-2f-src_core-2f-TlmCmd-2f-Ccsds

clean-src-2f-src_core-2f-TlmCmd-2f-Ccsds:
	-$(RM) ./src/src_core/TlmCmd/Ccsds/cmd_space_packet.cyclo ./src/src_core/TlmCmd/Ccsds/cmd_space_packet.d ./src/src_core/TlmCmd/Ccsds/cmd_space_packet.o ./src/src_core/TlmCmd/Ccsds/cmd_space_packet.su ./src/src_core/TlmCmd/Ccsds/space_packet.cyclo ./src/src_core/TlmCmd/Ccsds/space_packet.d ./src/src_core/TlmCmd/Ccsds/space_packet.o ./src/src_core/TlmCmd/Ccsds/space_packet.su ./src/src_core/TlmCmd/Ccsds/tlm_space_packet.cyclo ./src/src_core/TlmCmd/Ccsds/tlm_space_packet.d ./src/src_core/TlmCmd/Ccsds/tlm_space_packet.o ./src/src_core/TlmCmd/Ccsds/tlm_space_packet.su

.PHONY: clean-src-2f-src_core-2f-TlmCmd-2f-Ccsds

