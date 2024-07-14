################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/src_user/Applications/UserDefined/Memory/MEM_cmd_EEPROM.c \
../src/src_user/Applications/UserDefined/Memory/MEM_cmd_FLASH.c \
../src/src_user/Applications/UserDefined/Memory/MEM_cmd_MRAM.c \
../src/src_user/Applications/UserDefined/Memory/MEM_cmd_SD.c \
../src/src_user/Applications/UserDefined/Memory/MEM_cmd_SPIPROM.c \
../src/src_user/Applications/UserDefined/Memory/MEM_cmd_SRAM.c 

C_DEPS += \
./src/src_user/Applications/UserDefined/Memory/MEM_cmd_EEPROM.d \
./src/src_user/Applications/UserDefined/Memory/MEM_cmd_FLASH.d \
./src/src_user/Applications/UserDefined/Memory/MEM_cmd_MRAM.d \
./src/src_user/Applications/UserDefined/Memory/MEM_cmd_SD.d \
./src/src_user/Applications/UserDefined/Memory/MEM_cmd_SPIPROM.d \
./src/src_user/Applications/UserDefined/Memory/MEM_cmd_SRAM.d 

OBJS += \
./src/src_user/Applications/UserDefined/Memory/MEM_cmd_EEPROM.o \
./src/src_user/Applications/UserDefined/Memory/MEM_cmd_FLASH.o \
./src/src_user/Applications/UserDefined/Memory/MEM_cmd_MRAM.o \
./src/src_user/Applications/UserDefined/Memory/MEM_cmd_SD.o \
./src/src_user/Applications/UserDefined/Memory/MEM_cmd_SPIPROM.o \
./src/src_user/Applications/UserDefined/Memory/MEM_cmd_SRAM.o 


# Each subdirectory must supply rules for building sources it contributes
src/src_user/Applications/UserDefined/Memory/%.o src/src_user/Applications/UserDefined/Memory/%.su src/src_user/Applications/UserDefined/Memory/%.cyclo: ../src/src_user/Applications/UserDefined/Memory/%.c src/src_user/Applications/UserDefined/Memory/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F469xx -DONE_U -c -I../src -I../1U_config/Inc -I../1U_config/Drivers/STM32F4xx_HAL_Driver/Inc -I../1U_config/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../1U_config/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../1U_config/Drivers/CMSIS/Include -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-src-2f-src_user-2f-Applications-2f-UserDefined-2f-Memory

clean-src-2f-src_user-2f-Applications-2f-UserDefined-2f-Memory:
	-$(RM) ./src/src_user/Applications/UserDefined/Memory/MEM_cmd_EEPROM.cyclo ./src/src_user/Applications/UserDefined/Memory/MEM_cmd_EEPROM.d ./src/src_user/Applications/UserDefined/Memory/MEM_cmd_EEPROM.o ./src/src_user/Applications/UserDefined/Memory/MEM_cmd_EEPROM.su ./src/src_user/Applications/UserDefined/Memory/MEM_cmd_FLASH.cyclo ./src/src_user/Applications/UserDefined/Memory/MEM_cmd_FLASH.d ./src/src_user/Applications/UserDefined/Memory/MEM_cmd_FLASH.o ./src/src_user/Applications/UserDefined/Memory/MEM_cmd_FLASH.su ./src/src_user/Applications/UserDefined/Memory/MEM_cmd_MRAM.cyclo ./src/src_user/Applications/UserDefined/Memory/MEM_cmd_MRAM.d ./src/src_user/Applications/UserDefined/Memory/MEM_cmd_MRAM.o ./src/src_user/Applications/UserDefined/Memory/MEM_cmd_MRAM.su ./src/src_user/Applications/UserDefined/Memory/MEM_cmd_SD.cyclo ./src/src_user/Applications/UserDefined/Memory/MEM_cmd_SD.d ./src/src_user/Applications/UserDefined/Memory/MEM_cmd_SD.o ./src/src_user/Applications/UserDefined/Memory/MEM_cmd_SD.su ./src/src_user/Applications/UserDefined/Memory/MEM_cmd_SPIPROM.cyclo ./src/src_user/Applications/UserDefined/Memory/MEM_cmd_SPIPROM.d ./src/src_user/Applications/UserDefined/Memory/MEM_cmd_SPIPROM.o ./src/src_user/Applications/UserDefined/Memory/MEM_cmd_SPIPROM.su ./src/src_user/Applications/UserDefined/Memory/MEM_cmd_SRAM.cyclo ./src/src_user/Applications/UserDefined/Memory/MEM_cmd_SRAM.d ./src/src_user/Applications/UserDefined/Memory/MEM_cmd_SRAM.o ./src/src_user/Applications/UserDefined/Memory/MEM_cmd_SRAM.su

.PHONY: clean-src-2f-src_user-2f-Applications-2f-UserDefined-2f-Memory

