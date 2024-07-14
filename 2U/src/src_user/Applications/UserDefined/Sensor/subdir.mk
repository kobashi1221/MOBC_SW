################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/src_user/Applications/UserDefined/Sensor/ACS23_cmd_DAC.c \
../src/src_user/Applications/UserDefined/Sensor/ACS23_cmd_PLASMA.c 

CPP_SRCS += \
../src/src_user/Applications/UserDefined/Sensor/ACS13_cmd_GPS.cpp \
../src/src_user/Applications/UserDefined/Sensor/ACS13_cmd_SUNS.cpp \
../src/src_user/Applications/UserDefined/Sensor/ACS13_cmd_SUNS_PSD.cpp \
../src/src_user/Applications/UserDefined/Sensor/ACS1_cmd_ADC.cpp \
../src/src_user/Applications/UserDefined/Sensor/ACS23_cmd_9AX.cpp \
../src/src_user/Applications/UserDefined/Sensor/ACS23_cmd_MTQ.cpp \
../src/src_user/Applications/UserDefined/Sensor/ACS3_cmd_Gyro.cpp \
../src/src_user/Applications/UserDefined/Sensor/ACS3_cmd_SUBCAM.cpp 

C_DEPS += \
./src/src_user/Applications/UserDefined/Sensor/ACS23_cmd_DAC.d \
./src/src_user/Applications/UserDefined/Sensor/ACS23_cmd_PLASMA.d 

OBJS += \
./src/src_user/Applications/UserDefined/Sensor/ACS13_cmd_GPS.o \
./src/src_user/Applications/UserDefined/Sensor/ACS13_cmd_SUNS.o \
./src/src_user/Applications/UserDefined/Sensor/ACS13_cmd_SUNS_PSD.o \
./src/src_user/Applications/UserDefined/Sensor/ACS1_cmd_ADC.o \
./src/src_user/Applications/UserDefined/Sensor/ACS23_cmd_9AX.o \
./src/src_user/Applications/UserDefined/Sensor/ACS23_cmd_DAC.o \
./src/src_user/Applications/UserDefined/Sensor/ACS23_cmd_MTQ.o \
./src/src_user/Applications/UserDefined/Sensor/ACS23_cmd_PLASMA.o \
./src/src_user/Applications/UserDefined/Sensor/ACS3_cmd_Gyro.o \
./src/src_user/Applications/UserDefined/Sensor/ACS3_cmd_SUBCAM.o 

CPP_DEPS += \
./src/src_user/Applications/UserDefined/Sensor/ACS13_cmd_GPS.d \
./src/src_user/Applications/UserDefined/Sensor/ACS13_cmd_SUNS.d \
./src/src_user/Applications/UserDefined/Sensor/ACS13_cmd_SUNS_PSD.d \
./src/src_user/Applications/UserDefined/Sensor/ACS1_cmd_ADC.d \
./src/src_user/Applications/UserDefined/Sensor/ACS23_cmd_9AX.d \
./src/src_user/Applications/UserDefined/Sensor/ACS23_cmd_MTQ.d \
./src/src_user/Applications/UserDefined/Sensor/ACS3_cmd_Gyro.d \
./src/src_user/Applications/UserDefined/Sensor/ACS3_cmd_SUBCAM.d 


# Each subdirectory must supply rules for building sources it contributes
src/src_user/Applications/UserDefined/Sensor/%.o src/src_user/Applications/UserDefined/Sensor/%.su src/src_user/Applications/UserDefined/Sensor/%.cyclo: ../src/src_user/Applications/UserDefined/Sensor/%.cpp src/src_user/Applications/UserDefined/Sensor/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DUSE_HAL_DRIVER -DSTM32F469xx -DTWO_U -c -I../2U_config/Inc -I../2U_config/Drivers/STM32F4xx_HAL_Driver/Inc -I../2U_config/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../2U_config/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../2U_config/Drivers/CMSIS/Include -I../src -Og -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src/src_user/Applications/UserDefined/Sensor/%.o src/src_user/Applications/UserDefined/Sensor/%.su src/src_user/Applications/UserDefined/Sensor/%.cyclo: ../src/src_user/Applications/UserDefined/Sensor/%.c src/src_user/Applications/UserDefined/Sensor/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F469xx -DTWO_U -c -I../2U_config/Inc -I../2U_config/Drivers/STM32F4xx_HAL_Driver/Inc -I../2U_config/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../2U_config/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../2U_config/Drivers/CMSIS/Include -I../src -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-src-2f-src_user-2f-Applications-2f-UserDefined-2f-Sensor

clean-src-2f-src_user-2f-Applications-2f-UserDefined-2f-Sensor:
	-$(RM) ./src/src_user/Applications/UserDefined/Sensor/ACS13_cmd_GPS.cyclo ./src/src_user/Applications/UserDefined/Sensor/ACS13_cmd_GPS.d ./src/src_user/Applications/UserDefined/Sensor/ACS13_cmd_GPS.o ./src/src_user/Applications/UserDefined/Sensor/ACS13_cmd_GPS.su ./src/src_user/Applications/UserDefined/Sensor/ACS13_cmd_SUNS.cyclo ./src/src_user/Applications/UserDefined/Sensor/ACS13_cmd_SUNS.d ./src/src_user/Applications/UserDefined/Sensor/ACS13_cmd_SUNS.o ./src/src_user/Applications/UserDefined/Sensor/ACS13_cmd_SUNS.su ./src/src_user/Applications/UserDefined/Sensor/ACS13_cmd_SUNS_PSD.cyclo ./src/src_user/Applications/UserDefined/Sensor/ACS13_cmd_SUNS_PSD.d ./src/src_user/Applications/UserDefined/Sensor/ACS13_cmd_SUNS_PSD.o ./src/src_user/Applications/UserDefined/Sensor/ACS13_cmd_SUNS_PSD.su ./src/src_user/Applications/UserDefined/Sensor/ACS1_cmd_ADC.cyclo ./src/src_user/Applications/UserDefined/Sensor/ACS1_cmd_ADC.d ./src/src_user/Applications/UserDefined/Sensor/ACS1_cmd_ADC.o ./src/src_user/Applications/UserDefined/Sensor/ACS1_cmd_ADC.su ./src/src_user/Applications/UserDefined/Sensor/ACS23_cmd_9AX.cyclo ./src/src_user/Applications/UserDefined/Sensor/ACS23_cmd_9AX.d ./src/src_user/Applications/UserDefined/Sensor/ACS23_cmd_9AX.o ./src/src_user/Applications/UserDefined/Sensor/ACS23_cmd_9AX.su ./src/src_user/Applications/UserDefined/Sensor/ACS23_cmd_DAC.cyclo ./src/src_user/Applications/UserDefined/Sensor/ACS23_cmd_DAC.d ./src/src_user/Applications/UserDefined/Sensor/ACS23_cmd_DAC.o ./src/src_user/Applications/UserDefined/Sensor/ACS23_cmd_DAC.su ./src/src_user/Applications/UserDefined/Sensor/ACS23_cmd_MTQ.cyclo ./src/src_user/Applications/UserDefined/Sensor/ACS23_cmd_MTQ.d ./src/src_user/Applications/UserDefined/Sensor/ACS23_cmd_MTQ.o ./src/src_user/Applications/UserDefined/Sensor/ACS23_cmd_MTQ.su ./src/src_user/Applications/UserDefined/Sensor/ACS23_cmd_PLASMA.cyclo ./src/src_user/Applications/UserDefined/Sensor/ACS23_cmd_PLASMA.d ./src/src_user/Applications/UserDefined/Sensor/ACS23_cmd_PLASMA.o ./src/src_user/Applications/UserDefined/Sensor/ACS23_cmd_PLASMA.su ./src/src_user/Applications/UserDefined/Sensor/ACS3_cmd_Gyro.cyclo ./src/src_user/Applications/UserDefined/Sensor/ACS3_cmd_Gyro.d ./src/src_user/Applications/UserDefined/Sensor/ACS3_cmd_Gyro.o ./src/src_user/Applications/UserDefined/Sensor/ACS3_cmd_Gyro.su ./src/src_user/Applications/UserDefined/Sensor/ACS3_cmd_SUBCAM.cyclo ./src/src_user/Applications/UserDefined/Sensor/ACS3_cmd_SUBCAM.d ./src/src_user/Applications/UserDefined/Sensor/ACS3_cmd_SUBCAM.o ./src/src_user/Applications/UserDefined/Sensor/ACS3_cmd_SUBCAM.su

.PHONY: clean-src-2f-src_user-2f-Applications-2f-UserDefined-2f-Sensor

