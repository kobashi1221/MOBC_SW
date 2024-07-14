################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/src_user/TlmCmd/Ccsds/TCPacket.c \
../src/src_user/TlmCmd/Ccsds/m_pdu.c \
../src/src_user/TlmCmd/Ccsds/tc_segment.c \
../src/src_user/TlmCmd/Ccsds/tc_transfer_frame.c \
../src/src_user/TlmCmd/Ccsds/tcp_to_m_pdu.c \
../src/src_user/TlmCmd/Ccsds/vcdu.c 

C_DEPS += \
./src/src_user/TlmCmd/Ccsds/TCPacket.d \
./src/src_user/TlmCmd/Ccsds/m_pdu.d \
./src/src_user/TlmCmd/Ccsds/tc_segment.d \
./src/src_user/TlmCmd/Ccsds/tc_transfer_frame.d \
./src/src_user/TlmCmd/Ccsds/tcp_to_m_pdu.d \
./src/src_user/TlmCmd/Ccsds/vcdu.d 

OBJS += \
./src/src_user/TlmCmd/Ccsds/TCPacket.o \
./src/src_user/TlmCmd/Ccsds/m_pdu.o \
./src/src_user/TlmCmd/Ccsds/tc_segment.o \
./src/src_user/TlmCmd/Ccsds/tc_transfer_frame.o \
./src/src_user/TlmCmd/Ccsds/tcp_to_m_pdu.o \
./src/src_user/TlmCmd/Ccsds/vcdu.o 


# Each subdirectory must supply rules for building sources it contributes
src/src_user/TlmCmd/Ccsds/%.o src/src_user/TlmCmd/Ccsds/%.su src/src_user/TlmCmd/Ccsds/%.cyclo: ../src/src_user/TlmCmd/Ccsds/%.c src/src_user/TlmCmd/Ccsds/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F469xx -DONE_U -c -I../src -I../1U_config/Inc -I../1U_config/Drivers/STM32F4xx_HAL_Driver/Inc -I../1U_config/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../1U_config/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../1U_config/Drivers/CMSIS/Include -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-src-2f-src_user-2f-TlmCmd-2f-Ccsds

clean-src-2f-src_user-2f-TlmCmd-2f-Ccsds:
	-$(RM) ./src/src_user/TlmCmd/Ccsds/TCPacket.cyclo ./src/src_user/TlmCmd/Ccsds/TCPacket.d ./src/src_user/TlmCmd/Ccsds/TCPacket.o ./src/src_user/TlmCmd/Ccsds/TCPacket.su ./src/src_user/TlmCmd/Ccsds/m_pdu.cyclo ./src/src_user/TlmCmd/Ccsds/m_pdu.d ./src/src_user/TlmCmd/Ccsds/m_pdu.o ./src/src_user/TlmCmd/Ccsds/m_pdu.su ./src/src_user/TlmCmd/Ccsds/tc_segment.cyclo ./src/src_user/TlmCmd/Ccsds/tc_segment.d ./src/src_user/TlmCmd/Ccsds/tc_segment.o ./src/src_user/TlmCmd/Ccsds/tc_segment.su ./src/src_user/TlmCmd/Ccsds/tc_transfer_frame.cyclo ./src/src_user/TlmCmd/Ccsds/tc_transfer_frame.d ./src/src_user/TlmCmd/Ccsds/tc_transfer_frame.o ./src/src_user/TlmCmd/Ccsds/tc_transfer_frame.su ./src/src_user/TlmCmd/Ccsds/tcp_to_m_pdu.cyclo ./src/src_user/TlmCmd/Ccsds/tcp_to_m_pdu.d ./src/src_user/TlmCmd/Ccsds/tcp_to_m_pdu.o ./src/src_user/TlmCmd/Ccsds/tcp_to_m_pdu.su ./src/src_user/TlmCmd/Ccsds/vcdu.cyclo ./src/src_user/TlmCmd/Ccsds/vcdu.d ./src/src_user/TlmCmd/Ccsds/vcdu.o ./src/src_user/TlmCmd/Ccsds/vcdu.su

.PHONY: clean-src-2f-src_user-2f-TlmCmd-2f-Ccsds

