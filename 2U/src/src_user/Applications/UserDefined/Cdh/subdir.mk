################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/src_user/Applications/UserDefined/Cdh/Communication_Fault_Detection.c \
../src/src_user/Applications/UserDefined/Cdh/Developer_message.c \
../src/src_user/Applications/UserDefined/Cdh/RTC_handler.c \
../src/src_user/Applications/UserDefined/Cdh/ResetUtility.c \
../src/src_user/Applications/UserDefined/Cdh/telemetry_user.c 

C_DEPS += \
./src/src_user/Applications/UserDefined/Cdh/Communication_Fault_Detection.d \
./src/src_user/Applications/UserDefined/Cdh/Developer_message.d \
./src/src_user/Applications/UserDefined/Cdh/RTC_handler.d \
./src/src_user/Applications/UserDefined/Cdh/ResetUtility.d \
./src/src_user/Applications/UserDefined/Cdh/telemetry_user.d 

OBJS += \
./src/src_user/Applications/UserDefined/Cdh/Communication_Fault_Detection.o \
./src/src_user/Applications/UserDefined/Cdh/Developer_message.o \
./src/src_user/Applications/UserDefined/Cdh/RTC_handler.o \
./src/src_user/Applications/UserDefined/Cdh/ResetUtility.o \
./src/src_user/Applications/UserDefined/Cdh/telemetry_user.o 


# Each subdirectory must supply rules for building sources it contributes
src/src_user/Applications/UserDefined/Cdh/%.o src/src_user/Applications/UserDefined/Cdh/%.su src/src_user/Applications/UserDefined/Cdh/%.cyclo: ../src/src_user/Applications/UserDefined/Cdh/%.c src/src_user/Applications/UserDefined/Cdh/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F469xx -DTWO_U -c -I../2U_config/Inc -I../2U_config/Drivers/STM32F4xx_HAL_Driver/Inc -I../2U_config/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../2U_config/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../2U_config/Drivers/CMSIS/Include -I../src -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-src-2f-src_user-2f-Applications-2f-UserDefined-2f-Cdh

clean-src-2f-src_user-2f-Applications-2f-UserDefined-2f-Cdh:
	-$(RM) ./src/src_user/Applications/UserDefined/Cdh/Communication_Fault_Detection.cyclo ./src/src_user/Applications/UserDefined/Cdh/Communication_Fault_Detection.d ./src/src_user/Applications/UserDefined/Cdh/Communication_Fault_Detection.o ./src/src_user/Applications/UserDefined/Cdh/Communication_Fault_Detection.su ./src/src_user/Applications/UserDefined/Cdh/Developer_message.cyclo ./src/src_user/Applications/UserDefined/Cdh/Developer_message.d ./src/src_user/Applications/UserDefined/Cdh/Developer_message.o ./src/src_user/Applications/UserDefined/Cdh/Developer_message.su ./src/src_user/Applications/UserDefined/Cdh/RTC_handler.cyclo ./src/src_user/Applications/UserDefined/Cdh/RTC_handler.d ./src/src_user/Applications/UserDefined/Cdh/RTC_handler.o ./src/src_user/Applications/UserDefined/Cdh/RTC_handler.su ./src/src_user/Applications/UserDefined/Cdh/ResetUtility.cyclo ./src/src_user/Applications/UserDefined/Cdh/ResetUtility.d ./src/src_user/Applications/UserDefined/Cdh/ResetUtility.o ./src/src_user/Applications/UserDefined/Cdh/ResetUtility.su ./src/src_user/Applications/UserDefined/Cdh/telemetry_user.cyclo ./src/src_user/Applications/UserDefined/Cdh/telemetry_user.d ./src/src_user/Applications/UserDefined/Cdh/telemetry_user.o ./src/src_user/Applications/UserDefined/Cdh/telemetry_user.su

.PHONY: clean-src-2f-src_user-2f-Applications-2f-UserDefined-2f-Cdh

