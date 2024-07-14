################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/src_user/Library/SDcard/stm32_matsuzawa_sd.c 

C_DEPS += \
./src/src_user/Library/SDcard/stm32_matsuzawa_sd.d 

OBJS += \
./src/src_user/Library/SDcard/stm32_matsuzawa_sd.o 


# Each subdirectory must supply rules for building sources it contributes
src/src_user/Library/SDcard/%.o src/src_user/Library/SDcard/%.su src/src_user/Library/SDcard/%.cyclo: ../src/src_user/Library/SDcard/%.c src/src_user/Library/SDcard/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F469xx -DTWO_U -c -I../2U_config/Inc -I../2U_config/Drivers/STM32F4xx_HAL_Driver/Inc -I../2U_config/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../2U_config/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../2U_config/Drivers/CMSIS/Include -I../src -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-src-2f-src_user-2f-Library-2f-SDcard

clean-src-2f-src_user-2f-Library-2f-SDcard:
	-$(RM) ./src/src_user/Library/SDcard/stm32_matsuzawa_sd.cyclo ./src/src_user/Library/SDcard/stm32_matsuzawa_sd.d ./src/src_user/Library/SDcard/stm32_matsuzawa_sd.o ./src/src_user/Library/SDcard/stm32_matsuzawa_sd.su

.PHONY: clean-src-2f-src_user-2f-Library-2f-SDcard

