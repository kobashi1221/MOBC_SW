################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/src_core/Library/ascii_conv.c \
../src/src_core/Library/c2a_round.c \
../src/src_core/Library/crc.c \
../src/src_core/Library/endian.c \
../src/src_core/Library/majority_vote_for3.c 

C_DEPS += \
./src/src_core/Library/ascii_conv.d \
./src/src_core/Library/c2a_round.d \
./src/src_core/Library/crc.d \
./src/src_core/Library/endian.d \
./src/src_core/Library/majority_vote_for3.d 

OBJS += \
./src/src_core/Library/ascii_conv.o \
./src/src_core/Library/c2a_round.o \
./src/src_core/Library/crc.o \
./src/src_core/Library/endian.o \
./src/src_core/Library/majority_vote_for3.o 


# Each subdirectory must supply rules for building sources it contributes
src/src_core/Library/%.o src/src_core/Library/%.su src/src_core/Library/%.cyclo: ../src/src_core/Library/%.c src/src_core/Library/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F469xx -DONE_U -c -I../src -I../1U_config/Inc -I../1U_config/Drivers/STM32F4xx_HAL_Driver/Inc -I../1U_config/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../1U_config/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../1U_config/Drivers/CMSIS/Include -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-src-2f-src_core-2f-Library

clean-src-2f-src_core-2f-Library:
	-$(RM) ./src/src_core/Library/ascii_conv.cyclo ./src/src_core/Library/ascii_conv.d ./src/src_core/Library/ascii_conv.o ./src/src_core/Library/ascii_conv.su ./src/src_core/Library/c2a_round.cyclo ./src/src_core/Library/c2a_round.d ./src/src_core/Library/c2a_round.o ./src/src_core/Library/c2a_round.su ./src/src_core/Library/crc.cyclo ./src/src_core/Library/crc.d ./src/src_core/Library/crc.o ./src/src_core/Library/crc.su ./src/src_core/Library/endian.cyclo ./src/src_core/Library/endian.d ./src/src_core/Library/endian.o ./src/src_core/Library/endian.su ./src/src_core/Library/majority_vote_for3.cyclo ./src/src_core/Library/majority_vote_for3.d ./src/src_core/Library/majority_vote_for3.o ./src/src_core/Library/majority_vote_for3.su

.PHONY: clean-src-2f-src_core-2f-Library

