################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/src_user/Settings/System/EventHandlerRules/event_handler_rule_PWR.c \
../src/src_user/Settings/System/EventHandlerRules/event_handler_rule_test.c \
../src/src_user/Settings/System/EventHandlerRules/event_handler_rules.c 

C_DEPS += \
./src/src_user/Settings/System/EventHandlerRules/event_handler_rule_PWR.d \
./src/src_user/Settings/System/EventHandlerRules/event_handler_rule_test.d \
./src/src_user/Settings/System/EventHandlerRules/event_handler_rules.d 

OBJS += \
./src/src_user/Settings/System/EventHandlerRules/event_handler_rule_PWR.o \
./src/src_user/Settings/System/EventHandlerRules/event_handler_rule_test.o \
./src/src_user/Settings/System/EventHandlerRules/event_handler_rules.o 


# Each subdirectory must supply rules for building sources it contributes
src/src_user/Settings/System/EventHandlerRules/%.o src/src_user/Settings/System/EventHandlerRules/%.su src/src_user/Settings/System/EventHandlerRules/%.cyclo: ../src/src_user/Settings/System/EventHandlerRules/%.c src/src_user/Settings/System/EventHandlerRules/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F469xx -DTWO_U -c -I../2U_config/Inc -I../2U_config/Drivers/STM32F4xx_HAL_Driver/Inc -I../2U_config/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../2U_config/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../2U_config/Drivers/CMSIS/Include -I../src -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-src-2f-src_user-2f-Settings-2f-System-2f-EventHandlerRules

clean-src-2f-src_user-2f-Settings-2f-System-2f-EventHandlerRules:
	-$(RM) ./src/src_user/Settings/System/EventHandlerRules/event_handler_rule_PWR.cyclo ./src/src_user/Settings/System/EventHandlerRules/event_handler_rule_PWR.d ./src/src_user/Settings/System/EventHandlerRules/event_handler_rule_PWR.o ./src/src_user/Settings/System/EventHandlerRules/event_handler_rule_PWR.su ./src/src_user/Settings/System/EventHandlerRules/event_handler_rule_test.cyclo ./src/src_user/Settings/System/EventHandlerRules/event_handler_rule_test.d ./src/src_user/Settings/System/EventHandlerRules/event_handler_rule_test.o ./src/src_user/Settings/System/EventHandlerRules/event_handler_rule_test.su ./src/src_user/Settings/System/EventHandlerRules/event_handler_rules.cyclo ./src/src_user/Settings/System/EventHandlerRules/event_handler_rules.d ./src/src_user/Settings/System/EventHandlerRules/event_handler_rules.o ./src/src_user/Settings/System/EventHandlerRules/event_handler_rules.su

.PHONY: clean-src-2f-src_user-2f-Settings-2f-System-2f-EventHandlerRules

