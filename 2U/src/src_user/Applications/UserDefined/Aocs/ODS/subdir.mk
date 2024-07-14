################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/src_user/Applications/UserDefined/Aocs/ODS/ODS_cmd_BallCoef.cpp \
../src/src_user/Applications/UserDefined/Aocs/ODS/ODS_cmd_ORBEKF.cpp \
../src/src_user/Applications/UserDefined/Aocs/ODS/ODS_cmd_ORBPROP.cpp \
../src/src_user/Applications/UserDefined/Aocs/ODS/ODS_cmd_SGP4.cpp 

OBJS += \
./src/src_user/Applications/UserDefined/Aocs/ODS/ODS_cmd_BallCoef.o \
./src/src_user/Applications/UserDefined/Aocs/ODS/ODS_cmd_ORBEKF.o \
./src/src_user/Applications/UserDefined/Aocs/ODS/ODS_cmd_ORBPROP.o \
./src/src_user/Applications/UserDefined/Aocs/ODS/ODS_cmd_SGP4.o 

CPP_DEPS += \
./src/src_user/Applications/UserDefined/Aocs/ODS/ODS_cmd_BallCoef.d \
./src/src_user/Applications/UserDefined/Aocs/ODS/ODS_cmd_ORBEKF.d \
./src/src_user/Applications/UserDefined/Aocs/ODS/ODS_cmd_ORBPROP.d \
./src/src_user/Applications/UserDefined/Aocs/ODS/ODS_cmd_SGP4.d 


# Each subdirectory must supply rules for building sources it contributes
src/src_user/Applications/UserDefined/Aocs/ODS/%.o src/src_user/Applications/UserDefined/Aocs/ODS/%.su src/src_user/Applications/UserDefined/Aocs/ODS/%.cyclo: ../src/src_user/Applications/UserDefined/Aocs/ODS/%.cpp src/src_user/Applications/UserDefined/Aocs/ODS/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DUSE_HAL_DRIVER -DSTM32F469xx -DTWO_U -c -I../2U_config/Inc -I../2U_config/Drivers/STM32F4xx_HAL_Driver/Inc -I../2U_config/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../2U_config/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../2U_config/Drivers/CMSIS/Include -I../src -Og -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-src-2f-src_user-2f-Applications-2f-UserDefined-2f-Aocs-2f-ODS

clean-src-2f-src_user-2f-Applications-2f-UserDefined-2f-Aocs-2f-ODS:
	-$(RM) ./src/src_user/Applications/UserDefined/Aocs/ODS/ODS_cmd_BallCoef.cyclo ./src/src_user/Applications/UserDefined/Aocs/ODS/ODS_cmd_BallCoef.d ./src/src_user/Applications/UserDefined/Aocs/ODS/ODS_cmd_BallCoef.o ./src/src_user/Applications/UserDefined/Aocs/ODS/ODS_cmd_BallCoef.su ./src/src_user/Applications/UserDefined/Aocs/ODS/ODS_cmd_ORBEKF.cyclo ./src/src_user/Applications/UserDefined/Aocs/ODS/ODS_cmd_ORBEKF.d ./src/src_user/Applications/UserDefined/Aocs/ODS/ODS_cmd_ORBEKF.o ./src/src_user/Applications/UserDefined/Aocs/ODS/ODS_cmd_ORBEKF.su ./src/src_user/Applications/UserDefined/Aocs/ODS/ODS_cmd_ORBPROP.cyclo ./src/src_user/Applications/UserDefined/Aocs/ODS/ODS_cmd_ORBPROP.d ./src/src_user/Applications/UserDefined/Aocs/ODS/ODS_cmd_ORBPROP.o ./src/src_user/Applications/UserDefined/Aocs/ODS/ODS_cmd_ORBPROP.su ./src/src_user/Applications/UserDefined/Aocs/ODS/ODS_cmd_SGP4.cyclo ./src/src_user/Applications/UserDefined/Aocs/ODS/ODS_cmd_SGP4.d ./src/src_user/Applications/UserDefined/Aocs/ODS/ODS_cmd_SGP4.o ./src/src_user/Applications/UserDefined/Aocs/ODS/ODS_cmd_SGP4.su

.PHONY: clean-src-2f-src_user-2f-Applications-2f-UserDefined-2f-Aocs-2f-ODS

