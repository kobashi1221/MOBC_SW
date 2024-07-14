################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/src_user/Applications/UserDefined/Power/PWR_cmd_AnomalyHandle.c 

CPP_SRCS += \
../src/src_user/Applications/UserDefined/Power/PWR_cmd_ADC_MOBC.cpp \
../src/src_user/Applications/UserDefined/Power/PWR_cmd_ADC_PWR2.cpp \
../src/src_user/Applications/UserDefined/Power/PWR_cmd_AnomalyDetect.cpp \
../src/src_user/Applications/UserDefined/Power/PWR_cmd_FETREG.cpp \
../src/src_user/Applications/UserDefined/Power/PWR_cmd_GPIO_input_MOBC.cpp \
../src/src_user/Applications/UserDefined/Power/PWR_cmd_IOEX_PWR2.cpp 

C_DEPS += \
./src/src_user/Applications/UserDefined/Power/PWR_cmd_AnomalyHandle.d 

OBJS += \
./src/src_user/Applications/UserDefined/Power/PWR_cmd_ADC_MOBC.o \
./src/src_user/Applications/UserDefined/Power/PWR_cmd_ADC_PWR2.o \
./src/src_user/Applications/UserDefined/Power/PWR_cmd_AnomalyDetect.o \
./src/src_user/Applications/UserDefined/Power/PWR_cmd_AnomalyHandle.o \
./src/src_user/Applications/UserDefined/Power/PWR_cmd_FETREG.o \
./src/src_user/Applications/UserDefined/Power/PWR_cmd_GPIO_input_MOBC.o \
./src/src_user/Applications/UserDefined/Power/PWR_cmd_IOEX_PWR2.o 

CPP_DEPS += \
./src/src_user/Applications/UserDefined/Power/PWR_cmd_ADC_MOBC.d \
./src/src_user/Applications/UserDefined/Power/PWR_cmd_ADC_PWR2.d \
./src/src_user/Applications/UserDefined/Power/PWR_cmd_AnomalyDetect.d \
./src/src_user/Applications/UserDefined/Power/PWR_cmd_FETREG.d \
./src/src_user/Applications/UserDefined/Power/PWR_cmd_GPIO_input_MOBC.d \
./src/src_user/Applications/UserDefined/Power/PWR_cmd_IOEX_PWR2.d 


# Each subdirectory must supply rules for building sources it contributes
src/src_user/Applications/UserDefined/Power/%.o src/src_user/Applications/UserDefined/Power/%.su src/src_user/Applications/UserDefined/Power/%.cyclo: ../src/src_user/Applications/UserDefined/Power/%.cpp src/src_user/Applications/UserDefined/Power/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DUSE_HAL_DRIVER -DSTM32F469xx -DTWO_U -c -I../2U_config/Inc -I../2U_config/Drivers/STM32F4xx_HAL_Driver/Inc -I../2U_config/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../2U_config/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../2U_config/Drivers/CMSIS/Include -I../src -Og -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src/src_user/Applications/UserDefined/Power/%.o src/src_user/Applications/UserDefined/Power/%.su src/src_user/Applications/UserDefined/Power/%.cyclo: ../src/src_user/Applications/UserDefined/Power/%.c src/src_user/Applications/UserDefined/Power/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F469xx -DTWO_U -c -I../2U_config/Inc -I../2U_config/Drivers/STM32F4xx_HAL_Driver/Inc -I../2U_config/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../2U_config/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../2U_config/Drivers/CMSIS/Include -I../src -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-src-2f-src_user-2f-Applications-2f-UserDefined-2f-Power

clean-src-2f-src_user-2f-Applications-2f-UserDefined-2f-Power:
	-$(RM) ./src/src_user/Applications/UserDefined/Power/PWR_cmd_ADC_MOBC.cyclo ./src/src_user/Applications/UserDefined/Power/PWR_cmd_ADC_MOBC.d ./src/src_user/Applications/UserDefined/Power/PWR_cmd_ADC_MOBC.o ./src/src_user/Applications/UserDefined/Power/PWR_cmd_ADC_MOBC.su ./src/src_user/Applications/UserDefined/Power/PWR_cmd_ADC_PWR2.cyclo ./src/src_user/Applications/UserDefined/Power/PWR_cmd_ADC_PWR2.d ./src/src_user/Applications/UserDefined/Power/PWR_cmd_ADC_PWR2.o ./src/src_user/Applications/UserDefined/Power/PWR_cmd_ADC_PWR2.su ./src/src_user/Applications/UserDefined/Power/PWR_cmd_AnomalyDetect.cyclo ./src/src_user/Applications/UserDefined/Power/PWR_cmd_AnomalyDetect.d ./src/src_user/Applications/UserDefined/Power/PWR_cmd_AnomalyDetect.o ./src/src_user/Applications/UserDefined/Power/PWR_cmd_AnomalyDetect.su ./src/src_user/Applications/UserDefined/Power/PWR_cmd_AnomalyHandle.cyclo ./src/src_user/Applications/UserDefined/Power/PWR_cmd_AnomalyHandle.d ./src/src_user/Applications/UserDefined/Power/PWR_cmd_AnomalyHandle.o ./src/src_user/Applications/UserDefined/Power/PWR_cmd_AnomalyHandle.su ./src/src_user/Applications/UserDefined/Power/PWR_cmd_FETREG.cyclo ./src/src_user/Applications/UserDefined/Power/PWR_cmd_FETREG.d ./src/src_user/Applications/UserDefined/Power/PWR_cmd_FETREG.o ./src/src_user/Applications/UserDefined/Power/PWR_cmd_FETREG.su ./src/src_user/Applications/UserDefined/Power/PWR_cmd_GPIO_input_MOBC.cyclo ./src/src_user/Applications/UserDefined/Power/PWR_cmd_GPIO_input_MOBC.d ./src/src_user/Applications/UserDefined/Power/PWR_cmd_GPIO_input_MOBC.o ./src/src_user/Applications/UserDefined/Power/PWR_cmd_GPIO_input_MOBC.su ./src/src_user/Applications/UserDefined/Power/PWR_cmd_IOEX_PWR2.cyclo ./src/src_user/Applications/UserDefined/Power/PWR_cmd_IOEX_PWR2.d ./src/src_user/Applications/UserDefined/Power/PWR_cmd_IOEX_PWR2.o ./src/src_user/Applications/UserDefined/Power/PWR_cmd_IOEX_PWR2.su

.PHONY: clean-src-2f-src_user-2f-Applications-2f-UserDefined-2f-Power

