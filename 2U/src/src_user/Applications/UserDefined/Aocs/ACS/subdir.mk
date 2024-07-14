################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/src_user/Applications/UserDefined/Aocs/ACS/ACS_cmd_Bdot.cpp \
../src/src_user/Applications/UserDefined/Aocs/ACS/ACS_cmd_Compensation.cpp \
../src/src_user/Applications/UserDefined/Aocs/ACS/ACS_cmd_CrossProduct.cpp \
../src/src_user/Applications/UserDefined/Aocs/ACS/ACS_cmd_EarthVector.cpp 

OBJS += \
./src/src_user/Applications/UserDefined/Aocs/ACS/ACS_cmd_Bdot.o \
./src/src_user/Applications/UserDefined/Aocs/ACS/ACS_cmd_Compensation.o \
./src/src_user/Applications/UserDefined/Aocs/ACS/ACS_cmd_CrossProduct.o \
./src/src_user/Applications/UserDefined/Aocs/ACS/ACS_cmd_EarthVector.o 

CPP_DEPS += \
./src/src_user/Applications/UserDefined/Aocs/ACS/ACS_cmd_Bdot.d \
./src/src_user/Applications/UserDefined/Aocs/ACS/ACS_cmd_Compensation.d \
./src/src_user/Applications/UserDefined/Aocs/ACS/ACS_cmd_CrossProduct.d \
./src/src_user/Applications/UserDefined/Aocs/ACS/ACS_cmd_EarthVector.d 


# Each subdirectory must supply rules for building sources it contributes
src/src_user/Applications/UserDefined/Aocs/ACS/%.o src/src_user/Applications/UserDefined/Aocs/ACS/%.su src/src_user/Applications/UserDefined/Aocs/ACS/%.cyclo: ../src/src_user/Applications/UserDefined/Aocs/ACS/%.cpp src/src_user/Applications/UserDefined/Aocs/ACS/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DUSE_HAL_DRIVER -DSTM32F469xx -DTWO_U -c -I../2U_config/Inc -I../2U_config/Drivers/STM32F4xx_HAL_Driver/Inc -I../2U_config/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../2U_config/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../2U_config/Drivers/CMSIS/Include -I../src -Og -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-src-2f-src_user-2f-Applications-2f-UserDefined-2f-Aocs-2f-ACS

clean-src-2f-src_user-2f-Applications-2f-UserDefined-2f-Aocs-2f-ACS:
	-$(RM) ./src/src_user/Applications/UserDefined/Aocs/ACS/ACS_cmd_Bdot.cyclo ./src/src_user/Applications/UserDefined/Aocs/ACS/ACS_cmd_Bdot.d ./src/src_user/Applications/UserDefined/Aocs/ACS/ACS_cmd_Bdot.o ./src/src_user/Applications/UserDefined/Aocs/ACS/ACS_cmd_Bdot.su ./src/src_user/Applications/UserDefined/Aocs/ACS/ACS_cmd_Compensation.cyclo ./src/src_user/Applications/UserDefined/Aocs/ACS/ACS_cmd_Compensation.d ./src/src_user/Applications/UserDefined/Aocs/ACS/ACS_cmd_Compensation.o ./src/src_user/Applications/UserDefined/Aocs/ACS/ACS_cmd_Compensation.su ./src/src_user/Applications/UserDefined/Aocs/ACS/ACS_cmd_CrossProduct.cyclo ./src/src_user/Applications/UserDefined/Aocs/ACS/ACS_cmd_CrossProduct.d ./src/src_user/Applications/UserDefined/Aocs/ACS/ACS_cmd_CrossProduct.o ./src/src_user/Applications/UserDefined/Aocs/ACS/ACS_cmd_CrossProduct.su ./src/src_user/Applications/UserDefined/Aocs/ACS/ACS_cmd_EarthVector.cyclo ./src/src_user/Applications/UserDefined/Aocs/ACS/ACS_cmd_EarthVector.d ./src/src_user/Applications/UserDefined/Aocs/ACS/ACS_cmd_EarthVector.o ./src/src_user/Applications/UserDefined/Aocs/ACS/ACS_cmd_EarthVector.su

.PHONY: clean-src-2f-src_user-2f-Applications-2f-UserDefined-2f-Aocs-2f-ACS

