################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/src_user/Applications/UserDefined/Aocs/ADS/ADS_cmd_AD_EKF.cpp \
../src/src_user/Applications/UserDefined/Aocs/ADS/ADS_cmd_BBdot.cpp \
../src/src_user/Applications/UserDefined/Aocs/ADS/ADS_cmd_GST.cpp \
../src/src_user/Applications/UserDefined/Aocs/ADS/ADS_cmd_IGRF.cpp \
../src/src_user/Applications/UserDefined/Aocs/ADS/ADS_cmd_Qmethod.cpp \
../src/src_user/Applications/UserDefined/Aocs/ADS/ADS_cmd_Quest.cpp \
../src/src_user/Applications/UserDefined/Aocs/ADS/ADS_cmd_SunVector.cpp \
../src/src_user/Applications/UserDefined/Aocs/ADS/ADS_cmd_TRIAD.cpp 

OBJS += \
./src/src_user/Applications/UserDefined/Aocs/ADS/ADS_cmd_AD_EKF.o \
./src/src_user/Applications/UserDefined/Aocs/ADS/ADS_cmd_BBdot.o \
./src/src_user/Applications/UserDefined/Aocs/ADS/ADS_cmd_GST.o \
./src/src_user/Applications/UserDefined/Aocs/ADS/ADS_cmd_IGRF.o \
./src/src_user/Applications/UserDefined/Aocs/ADS/ADS_cmd_Qmethod.o \
./src/src_user/Applications/UserDefined/Aocs/ADS/ADS_cmd_Quest.o \
./src/src_user/Applications/UserDefined/Aocs/ADS/ADS_cmd_SunVector.o \
./src/src_user/Applications/UserDefined/Aocs/ADS/ADS_cmd_TRIAD.o 

CPP_DEPS += \
./src/src_user/Applications/UserDefined/Aocs/ADS/ADS_cmd_AD_EKF.d \
./src/src_user/Applications/UserDefined/Aocs/ADS/ADS_cmd_BBdot.d \
./src/src_user/Applications/UserDefined/Aocs/ADS/ADS_cmd_GST.d \
./src/src_user/Applications/UserDefined/Aocs/ADS/ADS_cmd_IGRF.d \
./src/src_user/Applications/UserDefined/Aocs/ADS/ADS_cmd_Qmethod.d \
./src/src_user/Applications/UserDefined/Aocs/ADS/ADS_cmd_Quest.d \
./src/src_user/Applications/UserDefined/Aocs/ADS/ADS_cmd_SunVector.d \
./src/src_user/Applications/UserDefined/Aocs/ADS/ADS_cmd_TRIAD.d 


# Each subdirectory must supply rules for building sources it contributes
src/src_user/Applications/UserDefined/Aocs/ADS/%.o src/src_user/Applications/UserDefined/Aocs/ADS/%.su src/src_user/Applications/UserDefined/Aocs/ADS/%.cyclo: ../src/src_user/Applications/UserDefined/Aocs/ADS/%.cpp src/src_user/Applications/UserDefined/Aocs/ADS/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DUSE_HAL_DRIVER -DSTM32F469xx -DTWO_U -c -I../2U_config/Inc -I../2U_config/Drivers/STM32F4xx_HAL_Driver/Inc -I../2U_config/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../2U_config/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../2U_config/Drivers/CMSIS/Include -I../src -Og -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-src-2f-src_user-2f-Applications-2f-UserDefined-2f-Aocs-2f-ADS

clean-src-2f-src_user-2f-Applications-2f-UserDefined-2f-Aocs-2f-ADS:
	-$(RM) ./src/src_user/Applications/UserDefined/Aocs/ADS/ADS_cmd_AD_EKF.cyclo ./src/src_user/Applications/UserDefined/Aocs/ADS/ADS_cmd_AD_EKF.d ./src/src_user/Applications/UserDefined/Aocs/ADS/ADS_cmd_AD_EKF.o ./src/src_user/Applications/UserDefined/Aocs/ADS/ADS_cmd_AD_EKF.su ./src/src_user/Applications/UserDefined/Aocs/ADS/ADS_cmd_BBdot.cyclo ./src/src_user/Applications/UserDefined/Aocs/ADS/ADS_cmd_BBdot.d ./src/src_user/Applications/UserDefined/Aocs/ADS/ADS_cmd_BBdot.o ./src/src_user/Applications/UserDefined/Aocs/ADS/ADS_cmd_BBdot.su ./src/src_user/Applications/UserDefined/Aocs/ADS/ADS_cmd_GST.cyclo ./src/src_user/Applications/UserDefined/Aocs/ADS/ADS_cmd_GST.d ./src/src_user/Applications/UserDefined/Aocs/ADS/ADS_cmd_GST.o ./src/src_user/Applications/UserDefined/Aocs/ADS/ADS_cmd_GST.su ./src/src_user/Applications/UserDefined/Aocs/ADS/ADS_cmd_IGRF.cyclo ./src/src_user/Applications/UserDefined/Aocs/ADS/ADS_cmd_IGRF.d ./src/src_user/Applications/UserDefined/Aocs/ADS/ADS_cmd_IGRF.o ./src/src_user/Applications/UserDefined/Aocs/ADS/ADS_cmd_IGRF.su ./src/src_user/Applications/UserDefined/Aocs/ADS/ADS_cmd_Qmethod.cyclo ./src/src_user/Applications/UserDefined/Aocs/ADS/ADS_cmd_Qmethod.d ./src/src_user/Applications/UserDefined/Aocs/ADS/ADS_cmd_Qmethod.o ./src/src_user/Applications/UserDefined/Aocs/ADS/ADS_cmd_Qmethod.su ./src/src_user/Applications/UserDefined/Aocs/ADS/ADS_cmd_Quest.cyclo ./src/src_user/Applications/UserDefined/Aocs/ADS/ADS_cmd_Quest.d ./src/src_user/Applications/UserDefined/Aocs/ADS/ADS_cmd_Quest.o ./src/src_user/Applications/UserDefined/Aocs/ADS/ADS_cmd_Quest.su ./src/src_user/Applications/UserDefined/Aocs/ADS/ADS_cmd_SunVector.cyclo ./src/src_user/Applications/UserDefined/Aocs/ADS/ADS_cmd_SunVector.d ./src/src_user/Applications/UserDefined/Aocs/ADS/ADS_cmd_SunVector.o ./src/src_user/Applications/UserDefined/Aocs/ADS/ADS_cmd_SunVector.su ./src/src_user/Applications/UserDefined/Aocs/ADS/ADS_cmd_TRIAD.cyclo ./src/src_user/Applications/UserDefined/Aocs/ADS/ADS_cmd_TRIAD.d ./src/src_user/Applications/UserDefined/Aocs/ADS/ADS_cmd_TRIAD.o ./src/src_user/Applications/UserDefined/Aocs/ADS/ADS_cmd_TRIAD.su

.PHONY: clean-src-2f-src_user-2f-Applications-2f-UserDefined-2f-Aocs-2f-ADS

