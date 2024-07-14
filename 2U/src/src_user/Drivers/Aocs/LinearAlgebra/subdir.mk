################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/src_user/Drivers/Aocs/LinearAlgebra/LAFunction.cpp 

OBJS += \
./src/src_user/Drivers/Aocs/LinearAlgebra/LAFunction.o 

CPP_DEPS += \
./src/src_user/Drivers/Aocs/LinearAlgebra/LAFunction.d 


# Each subdirectory must supply rules for building sources it contributes
src/src_user/Drivers/Aocs/LinearAlgebra/%.o src/src_user/Drivers/Aocs/LinearAlgebra/%.su src/src_user/Drivers/Aocs/LinearAlgebra/%.cyclo: ../src/src_user/Drivers/Aocs/LinearAlgebra/%.cpp src/src_user/Drivers/Aocs/LinearAlgebra/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DUSE_HAL_DRIVER -DSTM32F469xx -DTWO_U -c -I../2U_config/Inc -I../2U_config/Drivers/STM32F4xx_HAL_Driver/Inc -I../2U_config/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../2U_config/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../2U_config/Drivers/CMSIS/Include -I../src -Og -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-src-2f-src_user-2f-Drivers-2f-Aocs-2f-LinearAlgebra

clean-src-2f-src_user-2f-Drivers-2f-Aocs-2f-LinearAlgebra:
	-$(RM) ./src/src_user/Drivers/Aocs/LinearAlgebra/LAFunction.cyclo ./src/src_user/Drivers/Aocs/LinearAlgebra/LAFunction.d ./src/src_user/Drivers/Aocs/LinearAlgebra/LAFunction.o ./src/src_user/Drivers/Aocs/LinearAlgebra/LAFunction.su

.PHONY: clean-src-2f-src_user-2f-Drivers-2f-Aocs-2f-LinearAlgebra

