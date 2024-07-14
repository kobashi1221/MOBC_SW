################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/src_core/Drivers/Protocol/common_tlm_cmd_packet_for_driver_super.c \
../src/src_core/Drivers/Protocol/eb90_frame_for_driver_super.c \
../src/src_core/Drivers/Protocol/eb90_packet_for_driver_super.c 

C_DEPS += \
./src/src_core/Drivers/Protocol/common_tlm_cmd_packet_for_driver_super.d \
./src/src_core/Drivers/Protocol/eb90_frame_for_driver_super.d \
./src/src_core/Drivers/Protocol/eb90_packet_for_driver_super.d 

OBJS += \
./src/src_core/Drivers/Protocol/common_tlm_cmd_packet_for_driver_super.o \
./src/src_core/Drivers/Protocol/eb90_frame_for_driver_super.o \
./src/src_core/Drivers/Protocol/eb90_packet_for_driver_super.o 


# Each subdirectory must supply rules for building sources it contributes
src/src_core/Drivers/Protocol/%.o src/src_core/Drivers/Protocol/%.su src/src_core/Drivers/Protocol/%.cyclo: ../src/src_core/Drivers/Protocol/%.c src/src_core/Drivers/Protocol/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F469xx -DTWO_U -c -I../2U_config/Inc -I../2U_config/Drivers/STM32F4xx_HAL_Driver/Inc -I../2U_config/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../2U_config/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../2U_config/Drivers/CMSIS/Include -I../src -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-src-2f-src_core-2f-Drivers-2f-Protocol

clean-src-2f-src_core-2f-Drivers-2f-Protocol:
	-$(RM) ./src/src_core/Drivers/Protocol/common_tlm_cmd_packet_for_driver_super.cyclo ./src/src_core/Drivers/Protocol/common_tlm_cmd_packet_for_driver_super.d ./src/src_core/Drivers/Protocol/common_tlm_cmd_packet_for_driver_super.o ./src/src_core/Drivers/Protocol/common_tlm_cmd_packet_for_driver_super.su ./src/src_core/Drivers/Protocol/eb90_frame_for_driver_super.cyclo ./src/src_core/Drivers/Protocol/eb90_frame_for_driver_super.d ./src/src_core/Drivers/Protocol/eb90_frame_for_driver_super.o ./src/src_core/Drivers/Protocol/eb90_frame_for_driver_super.su ./src/src_core/Drivers/Protocol/eb90_packet_for_driver_super.cyclo ./src/src_core/Drivers/Protocol/eb90_packet_for_driver_super.d ./src/src_core/Drivers/Protocol/eb90_packet_for_driver_super.o ./src/src_core/Drivers/Protocol/eb90_packet_for_driver_super.su

.PHONY: clean-src-2f-src_core-2f-Drivers-2f-Protocol

