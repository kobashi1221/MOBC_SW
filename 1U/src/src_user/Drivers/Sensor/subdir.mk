################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/src_user/Drivers/Sensor/DAC.c 

CPP_SRCS += \
../src/src_user/Drivers/Sensor/9AX.cpp \
../src/src_user/Drivers/Sensor/ADC.cpp \
../src/src_user/Drivers/Sensor/CURS.cpp \
../src/src_user/Drivers/Sensor/GPS.cpp \
../src/src_user/Drivers/Sensor/GYRO.cpp \
../src/src_user/Drivers/Sensor/MAGS.cpp \
../src/src_user/Drivers/Sensor/MTQ.cpp \
../src/src_user/Drivers/Sensor/PLASMA.cpp \
../src/src_user/Drivers/Sensor/PSD.cpp \
../src/src_user/Drivers/Sensor/PlatinumSensor.cpp \
../src/src_user/Drivers/Sensor/SUBCAM.cpp \
../src/src_user/Drivers/Sensor/SUNS.cpp \
../src/src_user/Drivers/Sensor/TOA2BOA.cpp \
../src/src_user/Drivers/Sensor/VOLS.cpp \
../src/src_user/Drivers/Sensor/cSUNS.cpp \
../src/src_user/Drivers/Sensor/countFreq.cpp 

C_DEPS += \
./src/src_user/Drivers/Sensor/DAC.d 

OBJS += \
./src/src_user/Drivers/Sensor/9AX.o \
./src/src_user/Drivers/Sensor/ADC.o \
./src/src_user/Drivers/Sensor/CURS.o \
./src/src_user/Drivers/Sensor/DAC.o \
./src/src_user/Drivers/Sensor/GPS.o \
./src/src_user/Drivers/Sensor/GYRO.o \
./src/src_user/Drivers/Sensor/MAGS.o \
./src/src_user/Drivers/Sensor/MTQ.o \
./src/src_user/Drivers/Sensor/PLASMA.o \
./src/src_user/Drivers/Sensor/PSD.o \
./src/src_user/Drivers/Sensor/PlatinumSensor.o \
./src/src_user/Drivers/Sensor/SUBCAM.o \
./src/src_user/Drivers/Sensor/SUNS.o \
./src/src_user/Drivers/Sensor/TOA2BOA.o \
./src/src_user/Drivers/Sensor/VOLS.o \
./src/src_user/Drivers/Sensor/cSUNS.o \
./src/src_user/Drivers/Sensor/countFreq.o 

CPP_DEPS += \
./src/src_user/Drivers/Sensor/9AX.d \
./src/src_user/Drivers/Sensor/ADC.d \
./src/src_user/Drivers/Sensor/CURS.d \
./src/src_user/Drivers/Sensor/GPS.d \
./src/src_user/Drivers/Sensor/GYRO.d \
./src/src_user/Drivers/Sensor/MAGS.d \
./src/src_user/Drivers/Sensor/MTQ.d \
./src/src_user/Drivers/Sensor/PLASMA.d \
./src/src_user/Drivers/Sensor/PSD.d \
./src/src_user/Drivers/Sensor/PlatinumSensor.d \
./src/src_user/Drivers/Sensor/SUBCAM.d \
./src/src_user/Drivers/Sensor/SUNS.d \
./src/src_user/Drivers/Sensor/TOA2BOA.d \
./src/src_user/Drivers/Sensor/VOLS.d \
./src/src_user/Drivers/Sensor/cSUNS.d \
./src/src_user/Drivers/Sensor/countFreq.d 


# Each subdirectory must supply rules for building sources it contributes
src/src_user/Drivers/Sensor/%.o src/src_user/Drivers/Sensor/%.su src/src_user/Drivers/Sensor/%.cyclo: ../src/src_user/Drivers/Sensor/%.cpp src/src_user/Drivers/Sensor/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DUSE_HAL_DRIVER -DSTM32F469xx -DONE_U -c -I../src -I../1U_config/Inc -I../1U_config/Drivers/STM32F4xx_HAL_Driver/Inc -I../1U_config/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../1U_config/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../1U_config/Drivers/CMSIS/Include -Og -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src/src_user/Drivers/Sensor/%.o src/src_user/Drivers/Sensor/%.su src/src_user/Drivers/Sensor/%.cyclo: ../src/src_user/Drivers/Sensor/%.c src/src_user/Drivers/Sensor/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F469xx -DONE_U -c -I../src -I../1U_config/Inc -I../1U_config/Drivers/STM32F4xx_HAL_Driver/Inc -I../1U_config/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../1U_config/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../1U_config/Drivers/CMSIS/Include -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-src-2f-src_user-2f-Drivers-2f-Sensor

clean-src-2f-src_user-2f-Drivers-2f-Sensor:
	-$(RM) ./src/src_user/Drivers/Sensor/9AX.cyclo ./src/src_user/Drivers/Sensor/9AX.d ./src/src_user/Drivers/Sensor/9AX.o ./src/src_user/Drivers/Sensor/9AX.su ./src/src_user/Drivers/Sensor/ADC.cyclo ./src/src_user/Drivers/Sensor/ADC.d ./src/src_user/Drivers/Sensor/ADC.o ./src/src_user/Drivers/Sensor/ADC.su ./src/src_user/Drivers/Sensor/CURS.cyclo ./src/src_user/Drivers/Sensor/CURS.d ./src/src_user/Drivers/Sensor/CURS.o ./src/src_user/Drivers/Sensor/CURS.su ./src/src_user/Drivers/Sensor/DAC.cyclo ./src/src_user/Drivers/Sensor/DAC.d ./src/src_user/Drivers/Sensor/DAC.o ./src/src_user/Drivers/Sensor/DAC.su ./src/src_user/Drivers/Sensor/GPS.cyclo ./src/src_user/Drivers/Sensor/GPS.d ./src/src_user/Drivers/Sensor/GPS.o ./src/src_user/Drivers/Sensor/GPS.su ./src/src_user/Drivers/Sensor/GYRO.cyclo ./src/src_user/Drivers/Sensor/GYRO.d ./src/src_user/Drivers/Sensor/GYRO.o ./src/src_user/Drivers/Sensor/GYRO.su ./src/src_user/Drivers/Sensor/MAGS.cyclo ./src/src_user/Drivers/Sensor/MAGS.d ./src/src_user/Drivers/Sensor/MAGS.o ./src/src_user/Drivers/Sensor/MAGS.su ./src/src_user/Drivers/Sensor/MTQ.cyclo ./src/src_user/Drivers/Sensor/MTQ.d ./src/src_user/Drivers/Sensor/MTQ.o ./src/src_user/Drivers/Sensor/MTQ.su ./src/src_user/Drivers/Sensor/PLASMA.cyclo ./src/src_user/Drivers/Sensor/PLASMA.d ./src/src_user/Drivers/Sensor/PLASMA.o ./src/src_user/Drivers/Sensor/PLASMA.su ./src/src_user/Drivers/Sensor/PSD.cyclo ./src/src_user/Drivers/Sensor/PSD.d ./src/src_user/Drivers/Sensor/PSD.o ./src/src_user/Drivers/Sensor/PSD.su ./src/src_user/Drivers/Sensor/PlatinumSensor.cyclo ./src/src_user/Drivers/Sensor/PlatinumSensor.d ./src/src_user/Drivers/Sensor/PlatinumSensor.o ./src/src_user/Drivers/Sensor/PlatinumSensor.su ./src/src_user/Drivers/Sensor/SUBCAM.cyclo ./src/src_user/Drivers/Sensor/SUBCAM.d ./src/src_user/Drivers/Sensor/SUBCAM.o ./src/src_user/Drivers/Sensor/SUBCAM.su ./src/src_user/Drivers/Sensor/SUNS.cyclo ./src/src_user/Drivers/Sensor/SUNS.d ./src/src_user/Drivers/Sensor/SUNS.o ./src/src_user/Drivers/Sensor/SUNS.su ./src/src_user/Drivers/Sensor/TOA2BOA.cyclo ./src/src_user/Drivers/Sensor/TOA2BOA.d ./src/src_user/Drivers/Sensor/TOA2BOA.o ./src/src_user/Drivers/Sensor/TOA2BOA.su ./src/src_user/Drivers/Sensor/VOLS.cyclo ./src/src_user/Drivers/Sensor/VOLS.d ./src/src_user/Drivers/Sensor/VOLS.o ./src/src_user/Drivers/Sensor/VOLS.su ./src/src_user/Drivers/Sensor/cSUNS.cyclo ./src/src_user/Drivers/Sensor/cSUNS.d ./src/src_user/Drivers/Sensor/cSUNS.o ./src/src_user/Drivers/Sensor/cSUNS.su ./src/src_user/Drivers/Sensor/countFreq.cyclo ./src/src_user/Drivers/Sensor/countFreq.d ./src/src_user/Drivers/Sensor/countFreq.o ./src/src_user/Drivers/Sensor/countFreq.su

.PHONY: clean-src-2f-src_user-2f-Drivers-2f-Sensor

