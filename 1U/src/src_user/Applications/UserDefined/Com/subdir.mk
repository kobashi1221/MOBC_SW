################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/src_user/Applications/UserDefined/Com/COM_cmd_COBC.c \
../src/src_user/Applications/UserDefined/Com/COM_cmd_INTERSAT.c \
../src/src_user/Applications/UserDefined/Com/COM_cmd_Large_packet.c \
../src/src_user/Applications/UserDefined/Com/COM_cmd_LoRa.c \
../src/src_user/Applications/UserDefined/Com/COM_cmd_RXPIC.c \
../src/src_user/Applications/UserDefined/Com/COM_cmd_Relay.c \
../src/src_user/Applications/UserDefined/Com/COM_cmd_SOBC.c \
../src/src_user/Applications/UserDefined/Com/COM_cmd_TXPIC.c \
../src/src_user/Applications/UserDefined/Com/COM_cmd_response.c 

C_DEPS += \
./src/src_user/Applications/UserDefined/Com/COM_cmd_COBC.d \
./src/src_user/Applications/UserDefined/Com/COM_cmd_INTERSAT.d \
./src/src_user/Applications/UserDefined/Com/COM_cmd_Large_packet.d \
./src/src_user/Applications/UserDefined/Com/COM_cmd_LoRa.d \
./src/src_user/Applications/UserDefined/Com/COM_cmd_RXPIC.d \
./src/src_user/Applications/UserDefined/Com/COM_cmd_Relay.d \
./src/src_user/Applications/UserDefined/Com/COM_cmd_SOBC.d \
./src/src_user/Applications/UserDefined/Com/COM_cmd_TXPIC.d \
./src/src_user/Applications/UserDefined/Com/COM_cmd_response.d 

OBJS += \
./src/src_user/Applications/UserDefined/Com/COM_cmd_COBC.o \
./src/src_user/Applications/UserDefined/Com/COM_cmd_INTERSAT.o \
./src/src_user/Applications/UserDefined/Com/COM_cmd_Large_packet.o \
./src/src_user/Applications/UserDefined/Com/COM_cmd_LoRa.o \
./src/src_user/Applications/UserDefined/Com/COM_cmd_RXPIC.o \
./src/src_user/Applications/UserDefined/Com/COM_cmd_Relay.o \
./src/src_user/Applications/UserDefined/Com/COM_cmd_SOBC.o \
./src/src_user/Applications/UserDefined/Com/COM_cmd_TXPIC.o \
./src/src_user/Applications/UserDefined/Com/COM_cmd_response.o 


# Each subdirectory must supply rules for building sources it contributes
src/src_user/Applications/UserDefined/Com/%.o src/src_user/Applications/UserDefined/Com/%.su src/src_user/Applications/UserDefined/Com/%.cyclo: ../src/src_user/Applications/UserDefined/Com/%.c src/src_user/Applications/UserDefined/Com/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F469xx -DONE_U -c -I../src -I../1U_config/Inc -I../1U_config/Drivers/STM32F4xx_HAL_Driver/Inc -I../1U_config/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../1U_config/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../1U_config/Drivers/CMSIS/Include -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-src-2f-src_user-2f-Applications-2f-UserDefined-2f-Com

clean-src-2f-src_user-2f-Applications-2f-UserDefined-2f-Com:
	-$(RM) ./src/src_user/Applications/UserDefined/Com/COM_cmd_COBC.cyclo ./src/src_user/Applications/UserDefined/Com/COM_cmd_COBC.d ./src/src_user/Applications/UserDefined/Com/COM_cmd_COBC.o ./src/src_user/Applications/UserDefined/Com/COM_cmd_COBC.su ./src/src_user/Applications/UserDefined/Com/COM_cmd_INTERSAT.cyclo ./src/src_user/Applications/UserDefined/Com/COM_cmd_INTERSAT.d ./src/src_user/Applications/UserDefined/Com/COM_cmd_INTERSAT.o ./src/src_user/Applications/UserDefined/Com/COM_cmd_INTERSAT.su ./src/src_user/Applications/UserDefined/Com/COM_cmd_Large_packet.cyclo ./src/src_user/Applications/UserDefined/Com/COM_cmd_Large_packet.d ./src/src_user/Applications/UserDefined/Com/COM_cmd_Large_packet.o ./src/src_user/Applications/UserDefined/Com/COM_cmd_Large_packet.su ./src/src_user/Applications/UserDefined/Com/COM_cmd_LoRa.cyclo ./src/src_user/Applications/UserDefined/Com/COM_cmd_LoRa.d ./src/src_user/Applications/UserDefined/Com/COM_cmd_LoRa.o ./src/src_user/Applications/UserDefined/Com/COM_cmd_LoRa.su ./src/src_user/Applications/UserDefined/Com/COM_cmd_RXPIC.cyclo ./src/src_user/Applications/UserDefined/Com/COM_cmd_RXPIC.d ./src/src_user/Applications/UserDefined/Com/COM_cmd_RXPIC.o ./src/src_user/Applications/UserDefined/Com/COM_cmd_RXPIC.su ./src/src_user/Applications/UserDefined/Com/COM_cmd_Relay.cyclo ./src/src_user/Applications/UserDefined/Com/COM_cmd_Relay.d ./src/src_user/Applications/UserDefined/Com/COM_cmd_Relay.o ./src/src_user/Applications/UserDefined/Com/COM_cmd_Relay.su ./src/src_user/Applications/UserDefined/Com/COM_cmd_SOBC.cyclo ./src/src_user/Applications/UserDefined/Com/COM_cmd_SOBC.d ./src/src_user/Applications/UserDefined/Com/COM_cmd_SOBC.o ./src/src_user/Applications/UserDefined/Com/COM_cmd_SOBC.su ./src/src_user/Applications/UserDefined/Com/COM_cmd_TXPIC.cyclo ./src/src_user/Applications/UserDefined/Com/COM_cmd_TXPIC.d ./src/src_user/Applications/UserDefined/Com/COM_cmd_TXPIC.o ./src/src_user/Applications/UserDefined/Com/COM_cmd_TXPIC.su ./src/src_user/Applications/UserDefined/Com/COM_cmd_response.cyclo ./src/src_user/Applications/UserDefined/Com/COM_cmd_response.d ./src/src_user/Applications/UserDefined/Com/COM_cmd_response.o ./src/src_user/Applications/UserDefined/Com/COM_cmd_response.su

.PHONY: clean-src-2f-src_user-2f-Applications-2f-UserDefined-2f-Com

