################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/src_user/Drivers/Power/MCP23017.cpp \
../src/src_user/Drivers/Power/MCP3421.cpp \
../src/src_user/Drivers/Power/PWR_Operation.cpp \
../src/src_user/Drivers/Power/PWR_State.cpp 

OBJS += \
./src/src_user/Drivers/Power/MCP23017.o \
./src/src_user/Drivers/Power/MCP3421.o \
./src/src_user/Drivers/Power/PWR_Operation.o \
./src/src_user/Drivers/Power/PWR_State.o 

CPP_DEPS += \
./src/src_user/Drivers/Power/MCP23017.d \
./src/src_user/Drivers/Power/MCP3421.d \
./src/src_user/Drivers/Power/PWR_Operation.d \
./src/src_user/Drivers/Power/PWR_State.d 


# Each subdirectory must supply rules for building sources it contributes
src/src_user/Drivers/Power/%.o src/src_user/Drivers/Power/%.su src/src_user/Drivers/Power/%.cyclo: ../src/src_user/Drivers/Power/%.cpp src/src_user/Drivers/Power/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DUSE_HAL_DRIVER -DSTM32F469xx -DONE_U -c -I../src -I../1U_config/Inc -I../1U_config/Drivers/STM32F4xx_HAL_Driver/Inc -I../1U_config/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../1U_config/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../1U_config/Drivers/CMSIS/Include -Og -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-src-2f-src_user-2f-Drivers-2f-Power

clean-src-2f-src_user-2f-Drivers-2f-Power:
	-$(RM) ./src/src_user/Drivers/Power/MCP23017.cyclo ./src/src_user/Drivers/Power/MCP23017.d ./src/src_user/Drivers/Power/MCP23017.o ./src/src_user/Drivers/Power/MCP23017.su ./src/src_user/Drivers/Power/MCP3421.cyclo ./src/src_user/Drivers/Power/MCP3421.d ./src/src_user/Drivers/Power/MCP3421.o ./src/src_user/Drivers/Power/MCP3421.su ./src/src_user/Drivers/Power/PWR_Operation.cyclo ./src/src_user/Drivers/Power/PWR_Operation.d ./src/src_user/Drivers/Power/PWR_Operation.o ./src/src_user/Drivers/Power/PWR_Operation.su ./src/src_user/Drivers/Power/PWR_State.cyclo ./src/src_user/Drivers/Power/PWR_State.d ./src/src_user/Drivers/Power/PWR_State.o ./src/src_user/Drivers/Power/PWR_State.su

.PHONY: clean-src-2f-src_user-2f-Drivers-2f-Power

