################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/src_user/Applications/UserDefined/Aocs/OCS/OCS_cmd_CrossProduct.cpp 

OBJS += \
./src/src_user/Applications/UserDefined/Aocs/OCS/OCS_cmd_CrossProduct.o 

CPP_DEPS += \
./src/src_user/Applications/UserDefined/Aocs/OCS/OCS_cmd_CrossProduct.d 


# Each subdirectory must supply rules for building sources it contributes
src/src_user/Applications/UserDefined/Aocs/OCS/%.o src/src_user/Applications/UserDefined/Aocs/OCS/%.su src/src_user/Applications/UserDefined/Aocs/OCS/%.cyclo: ../src/src_user/Applications/UserDefined/Aocs/OCS/%.cpp src/src_user/Applications/UserDefined/Aocs/OCS/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DUSE_HAL_DRIVER -DSTM32F469xx -DTWO_U -c -I../2U_config/Inc -I../2U_config/Drivers/STM32F4xx_HAL_Driver/Inc -I../2U_config/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../2U_config/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../2U_config/Drivers/CMSIS/Include -I../src -Og -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-src-2f-src_user-2f-Applications-2f-UserDefined-2f-Aocs-2f-OCS

clean-src-2f-src_user-2f-Applications-2f-UserDefined-2f-Aocs-2f-OCS:
	-$(RM) ./src/src_user/Applications/UserDefined/Aocs/OCS/OCS_cmd_CrossProduct.cyclo ./src/src_user/Applications/UserDefined/Aocs/OCS/OCS_cmd_CrossProduct.d ./src/src_user/Applications/UserDefined/Aocs/OCS/OCS_cmd_CrossProduct.o ./src/src_user/Applications/UserDefined/Aocs/OCS/OCS_cmd_CrossProduct.su

.PHONY: clean-src-2f-src_user-2f-Applications-2f-UserDefined-2f-Aocs-2f-OCS

