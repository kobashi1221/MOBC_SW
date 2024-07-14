################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/src_user/Settings/Modes/Eqlipse/eclipse_setting.c \
../src/src_user/Settings/Modes/Eqlipse/eclipse_setting_driver.c 

C_DEPS += \
./src/src_user/Settings/Modes/Eqlipse/eclipse_setting.d \
./src/src_user/Settings/Modes/Eqlipse/eclipse_setting_driver.d 

OBJS += \
./src/src_user/Settings/Modes/Eqlipse/eclipse_setting.o \
./src/src_user/Settings/Modes/Eqlipse/eclipse_setting_driver.o 


# Each subdirectory must supply rules for building sources it contributes
src/src_user/Settings/Modes/Eqlipse/%.o src/src_user/Settings/Modes/Eqlipse/%.su src/src_user/Settings/Modes/Eqlipse/%.cyclo: ../src/src_user/Settings/Modes/Eqlipse/%.c src/src_user/Settings/Modes/Eqlipse/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F469xx -DTWO_U -c -I../2U_config/Inc -I../2U_config/Drivers/STM32F4xx_HAL_Driver/Inc -I../2U_config/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../2U_config/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../2U_config/Drivers/CMSIS/Include -I../src -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-src-2f-src_user-2f-Settings-2f-Modes-2f-Eqlipse

clean-src-2f-src_user-2f-Settings-2f-Modes-2f-Eqlipse:
	-$(RM) ./src/src_user/Settings/Modes/Eqlipse/eclipse_setting.cyclo ./src/src_user/Settings/Modes/Eqlipse/eclipse_setting.d ./src/src_user/Settings/Modes/Eqlipse/eclipse_setting.o ./src/src_user/Settings/Modes/Eqlipse/eclipse_setting.su ./src/src_user/Settings/Modes/Eqlipse/eclipse_setting_driver.cyclo ./src/src_user/Settings/Modes/Eqlipse/eclipse_setting_driver.d ./src/src_user/Settings/Modes/Eqlipse/eclipse_setting_driver.o ./src/src_user/Settings/Modes/Eqlipse/eclipse_setting_driver.su

.PHONY: clean-src-2f-src_user-2f-Settings-2f-Modes-2f-Eqlipse

