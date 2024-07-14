################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/src_user/Drivers/Aocs/aobc.c \
../src/src_user/Drivers/Aocs/aobc_telemetry_buffer.c \
../src/src_user/Drivers/Aocs/sgdp4.c 

CPP_SRCS += \
../src/src_user/Drivers/Aocs/AD_EKF.cpp \
../src/src_user/Drivers/Aocs/AOCSVector.cpp \
../src/src_user/Drivers/Aocs/AOCScalc.cpp \
../src/src_user/Drivers/Aocs/Bdot.cpp \
../src/src_user/Drivers/Aocs/Compensation.cpp \
../src/src_user/Drivers/Aocs/CrossProduct.cpp \
../src/src_user/Drivers/Aocs/EKF.cpp \
../src/src_user/Drivers/Aocs/EarthVector.cpp \
../src/src_user/Drivers/Aocs/Element2Cartesian.cpp \
../src/src_user/Drivers/Aocs/GST.cpp \
../src/src_user/Drivers/Aocs/IGRF.cpp \
../src/src_user/Drivers/Aocs/Integrater.cpp \
../src/src_user/Drivers/Aocs/JulianDate.cpp \
../src/src_user/Drivers/Aocs/OD_EKF.cpp \
../src/src_user/Drivers/Aocs/OrbitDynamics.cpp \
../src/src_user/Drivers/Aocs/ProjectedArea.cpp \
../src/src_user/Drivers/Aocs/Qmethod.cpp \
../src/src_user/Drivers/Aocs/Quest.cpp \
../src/src_user/Drivers/Aocs/SunVector.cpp \
../src/src_user/Drivers/Aocs/TRIAD.cpp \
../src/src_user/Drivers/Aocs/sgp4.cpp 

C_DEPS += \
./src/src_user/Drivers/Aocs/aobc.d \
./src/src_user/Drivers/Aocs/aobc_telemetry_buffer.d \
./src/src_user/Drivers/Aocs/sgdp4.d 

OBJS += \
./src/src_user/Drivers/Aocs/AD_EKF.o \
./src/src_user/Drivers/Aocs/AOCSVector.o \
./src/src_user/Drivers/Aocs/AOCScalc.o \
./src/src_user/Drivers/Aocs/Bdot.o \
./src/src_user/Drivers/Aocs/Compensation.o \
./src/src_user/Drivers/Aocs/CrossProduct.o \
./src/src_user/Drivers/Aocs/EKF.o \
./src/src_user/Drivers/Aocs/EarthVector.o \
./src/src_user/Drivers/Aocs/Element2Cartesian.o \
./src/src_user/Drivers/Aocs/GST.o \
./src/src_user/Drivers/Aocs/IGRF.o \
./src/src_user/Drivers/Aocs/Integrater.o \
./src/src_user/Drivers/Aocs/JulianDate.o \
./src/src_user/Drivers/Aocs/OD_EKF.o \
./src/src_user/Drivers/Aocs/OrbitDynamics.o \
./src/src_user/Drivers/Aocs/ProjectedArea.o \
./src/src_user/Drivers/Aocs/Qmethod.o \
./src/src_user/Drivers/Aocs/Quest.o \
./src/src_user/Drivers/Aocs/SunVector.o \
./src/src_user/Drivers/Aocs/TRIAD.o \
./src/src_user/Drivers/Aocs/aobc.o \
./src/src_user/Drivers/Aocs/aobc_telemetry_buffer.o \
./src/src_user/Drivers/Aocs/sgdp4.o \
./src/src_user/Drivers/Aocs/sgp4.o 

CPP_DEPS += \
./src/src_user/Drivers/Aocs/AD_EKF.d \
./src/src_user/Drivers/Aocs/AOCSVector.d \
./src/src_user/Drivers/Aocs/AOCScalc.d \
./src/src_user/Drivers/Aocs/Bdot.d \
./src/src_user/Drivers/Aocs/Compensation.d \
./src/src_user/Drivers/Aocs/CrossProduct.d \
./src/src_user/Drivers/Aocs/EKF.d \
./src/src_user/Drivers/Aocs/EarthVector.d \
./src/src_user/Drivers/Aocs/Element2Cartesian.d \
./src/src_user/Drivers/Aocs/GST.d \
./src/src_user/Drivers/Aocs/IGRF.d \
./src/src_user/Drivers/Aocs/Integrater.d \
./src/src_user/Drivers/Aocs/JulianDate.d \
./src/src_user/Drivers/Aocs/OD_EKF.d \
./src/src_user/Drivers/Aocs/OrbitDynamics.d \
./src/src_user/Drivers/Aocs/ProjectedArea.d \
./src/src_user/Drivers/Aocs/Qmethod.d \
./src/src_user/Drivers/Aocs/Quest.d \
./src/src_user/Drivers/Aocs/SunVector.d \
./src/src_user/Drivers/Aocs/TRIAD.d \
./src/src_user/Drivers/Aocs/sgp4.d 


# Each subdirectory must supply rules for building sources it contributes
src/src_user/Drivers/Aocs/%.o src/src_user/Drivers/Aocs/%.su src/src_user/Drivers/Aocs/%.cyclo: ../src/src_user/Drivers/Aocs/%.cpp src/src_user/Drivers/Aocs/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DUSE_HAL_DRIVER -DSTM32F469xx -DTWO_U -c -I../2U_config/Inc -I../2U_config/Drivers/STM32F4xx_HAL_Driver/Inc -I../2U_config/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../2U_config/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../2U_config/Drivers/CMSIS/Include -I../src -Og -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src/src_user/Drivers/Aocs/%.o src/src_user/Drivers/Aocs/%.su src/src_user/Drivers/Aocs/%.cyclo: ../src/src_user/Drivers/Aocs/%.c src/src_user/Drivers/Aocs/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F469xx -DTWO_U -c -I../2U_config/Inc -I../2U_config/Drivers/STM32F4xx_HAL_Driver/Inc -I../2U_config/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../2U_config/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../2U_config/Drivers/CMSIS/Include -I../src -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-src-2f-src_user-2f-Drivers-2f-Aocs

clean-src-2f-src_user-2f-Drivers-2f-Aocs:
	-$(RM) ./src/src_user/Drivers/Aocs/AD_EKF.cyclo ./src/src_user/Drivers/Aocs/AD_EKF.d ./src/src_user/Drivers/Aocs/AD_EKF.o ./src/src_user/Drivers/Aocs/AD_EKF.su ./src/src_user/Drivers/Aocs/AOCSVector.cyclo ./src/src_user/Drivers/Aocs/AOCSVector.d ./src/src_user/Drivers/Aocs/AOCSVector.o ./src/src_user/Drivers/Aocs/AOCSVector.su ./src/src_user/Drivers/Aocs/AOCScalc.cyclo ./src/src_user/Drivers/Aocs/AOCScalc.d ./src/src_user/Drivers/Aocs/AOCScalc.o ./src/src_user/Drivers/Aocs/AOCScalc.su ./src/src_user/Drivers/Aocs/Bdot.cyclo ./src/src_user/Drivers/Aocs/Bdot.d ./src/src_user/Drivers/Aocs/Bdot.o ./src/src_user/Drivers/Aocs/Bdot.su ./src/src_user/Drivers/Aocs/Compensation.cyclo ./src/src_user/Drivers/Aocs/Compensation.d ./src/src_user/Drivers/Aocs/Compensation.o ./src/src_user/Drivers/Aocs/Compensation.su ./src/src_user/Drivers/Aocs/CrossProduct.cyclo ./src/src_user/Drivers/Aocs/CrossProduct.d ./src/src_user/Drivers/Aocs/CrossProduct.o ./src/src_user/Drivers/Aocs/CrossProduct.su ./src/src_user/Drivers/Aocs/EKF.cyclo ./src/src_user/Drivers/Aocs/EKF.d ./src/src_user/Drivers/Aocs/EKF.o ./src/src_user/Drivers/Aocs/EKF.su ./src/src_user/Drivers/Aocs/EarthVector.cyclo ./src/src_user/Drivers/Aocs/EarthVector.d ./src/src_user/Drivers/Aocs/EarthVector.o ./src/src_user/Drivers/Aocs/EarthVector.su ./src/src_user/Drivers/Aocs/Element2Cartesian.cyclo ./src/src_user/Drivers/Aocs/Element2Cartesian.d ./src/src_user/Drivers/Aocs/Element2Cartesian.o ./src/src_user/Drivers/Aocs/Element2Cartesian.su ./src/src_user/Drivers/Aocs/GST.cyclo ./src/src_user/Drivers/Aocs/GST.d ./src/src_user/Drivers/Aocs/GST.o ./src/src_user/Drivers/Aocs/GST.su ./src/src_user/Drivers/Aocs/IGRF.cyclo ./src/src_user/Drivers/Aocs/IGRF.d ./src/src_user/Drivers/Aocs/IGRF.o ./src/src_user/Drivers/Aocs/IGRF.su ./src/src_user/Drivers/Aocs/Integrater.cyclo ./src/src_user/Drivers/Aocs/Integrater.d ./src/src_user/Drivers/Aocs/Integrater.o ./src/src_user/Drivers/Aocs/Integrater.su ./src/src_user/Drivers/Aocs/JulianDate.cyclo ./src/src_user/Drivers/Aocs/JulianDate.d ./src/src_user/Drivers/Aocs/JulianDate.o ./src/src_user/Drivers/Aocs/JulianDate.su ./src/src_user/Drivers/Aocs/OD_EKF.cyclo ./src/src_user/Drivers/Aocs/OD_EKF.d ./src/src_user/Drivers/Aocs/OD_EKF.o ./src/src_user/Drivers/Aocs/OD_EKF.su ./src/src_user/Drivers/Aocs/OrbitDynamics.cyclo ./src/src_user/Drivers/Aocs/OrbitDynamics.d ./src/src_user/Drivers/Aocs/OrbitDynamics.o ./src/src_user/Drivers/Aocs/OrbitDynamics.su ./src/src_user/Drivers/Aocs/ProjectedArea.cyclo ./src/src_user/Drivers/Aocs/ProjectedArea.d ./src/src_user/Drivers/Aocs/ProjectedArea.o ./src/src_user/Drivers/Aocs/ProjectedArea.su ./src/src_user/Drivers/Aocs/Qmethod.cyclo ./src/src_user/Drivers/Aocs/Qmethod.d ./src/src_user/Drivers/Aocs/Qmethod.o ./src/src_user/Drivers/Aocs/Qmethod.su ./src/src_user/Drivers/Aocs/Quest.cyclo ./src/src_user/Drivers/Aocs/Quest.d ./src/src_user/Drivers/Aocs/Quest.o ./src/src_user/Drivers/Aocs/Quest.su ./src/src_user/Drivers/Aocs/SunVector.cyclo ./src/src_user/Drivers/Aocs/SunVector.d ./src/src_user/Drivers/Aocs/SunVector.o ./src/src_user/Drivers/Aocs/SunVector.su ./src/src_user/Drivers/Aocs/TRIAD.cyclo ./src/src_user/Drivers/Aocs/TRIAD.d ./src/src_user/Drivers/Aocs/TRIAD.o ./src/src_user/Drivers/Aocs/TRIAD.su ./src/src_user/Drivers/Aocs/aobc.cyclo ./src/src_user/Drivers/Aocs/aobc.d ./src/src_user/Drivers/Aocs/aobc.o ./src/src_user/Drivers/Aocs/aobc.su ./src/src_user/Drivers/Aocs/aobc_telemetry_buffer.cyclo ./src/src_user/Drivers/Aocs/aobc_telemetry_buffer.d ./src/src_user/Drivers/Aocs/aobc_telemetry_buffer.o ./src/src_user/Drivers/Aocs/aobc_telemetry_buffer.su ./src/src_user/Drivers/Aocs/sgdp4.cyclo ./src/src_user/Drivers/Aocs/sgdp4.d ./src/src_user/Drivers/Aocs/sgdp4.o ./src/src_user/Drivers/Aocs/sgdp4.su ./src/src_user/Drivers/Aocs/sgp4.cyclo ./src/src_user/Drivers/Aocs/sgp4.d ./src/src_user/Drivers/Aocs/sgp4.o ./src/src_user/Drivers/Aocs/sgp4.su

.PHONY: clean-src-2f-src_user-2f-Drivers-2f-Aocs

