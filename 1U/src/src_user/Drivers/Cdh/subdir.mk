################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/src_user/Drivers/Cdh/CDH_Mode_Transition_driver.c \
../src/src_user/Drivers/Cdh/Com_Fault_Detection_driver.c \
../src/src_user/Drivers/Cdh/reset_counter.c 

C_DEPS += \
./src/src_user/Drivers/Cdh/CDH_Mode_Transition_driver.d \
./src/src_user/Drivers/Cdh/Com_Fault_Detection_driver.d \
./src/src_user/Drivers/Cdh/reset_counter.d 

OBJS += \
./src/src_user/Drivers/Cdh/CDH_Mode_Transition_driver.o \
./src/src_user/Drivers/Cdh/Com_Fault_Detection_driver.o \
./src/src_user/Drivers/Cdh/reset_counter.o 


# Each subdirectory must supply rules for building sources it contributes
src/src_user/Drivers/Cdh/%.o src/src_user/Drivers/Cdh/%.su src/src_user/Drivers/Cdh/%.cyclo: ../src/src_user/Drivers/Cdh/%.c src/src_user/Drivers/Cdh/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F469xx -DONE_U -c -I../src -I../1U_config/Inc -I../1U_config/Drivers/STM32F4xx_HAL_Driver/Inc -I../1U_config/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../1U_config/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../1U_config/Drivers/CMSIS/Include -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-src-2f-src_user-2f-Drivers-2f-Cdh

clean-src-2f-src_user-2f-Drivers-2f-Cdh:
	-$(RM) ./src/src_user/Drivers/Cdh/CDH_Mode_Transition_driver.cyclo ./src/src_user/Drivers/Cdh/CDH_Mode_Transition_driver.d ./src/src_user/Drivers/Cdh/CDH_Mode_Transition_driver.o ./src/src_user/Drivers/Cdh/CDH_Mode_Transition_driver.su ./src/src_user/Drivers/Cdh/Com_Fault_Detection_driver.cyclo ./src/src_user/Drivers/Cdh/Com_Fault_Detection_driver.d ./src/src_user/Drivers/Cdh/Com_Fault_Detection_driver.o ./src/src_user/Drivers/Cdh/Com_Fault_Detection_driver.su ./src/src_user/Drivers/Cdh/reset_counter.cyclo ./src/src_user/Drivers/Cdh/reset_counter.d ./src/src_user/Drivers/Cdh/reset_counter.o ./src/src_user/Drivers/Cdh/reset_counter.su

.PHONY: clean-src-2f-src_user-2f-Drivers-2f-Cdh

