################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/src_user/Applications/UserDefined/debug_apps.c \
../src/src_user/Applications/UserDefined/debug_nop.c \
../src/src_user/Applications/UserDefined/sample_code.c \
../src/src_user/Applications/UserDefined/tlm_mem_dump.c 

CPP_SRCS += \
../src/src_user/Applications/UserDefined/GlobalValDefine.cpp \
../src/src_user/Applications/UserDefined/Global_PWR_Parameter.cpp \
../src/src_user/Applications/UserDefined/IF_Instances.cpp 

C_DEPS += \
./src/src_user/Applications/UserDefined/debug_apps.d \
./src/src_user/Applications/UserDefined/debug_nop.d \
./src/src_user/Applications/UserDefined/sample_code.d \
./src/src_user/Applications/UserDefined/tlm_mem_dump.d 

OBJS += \
./src/src_user/Applications/UserDefined/GlobalValDefine.o \
./src/src_user/Applications/UserDefined/Global_PWR_Parameter.o \
./src/src_user/Applications/UserDefined/IF_Instances.o \
./src/src_user/Applications/UserDefined/debug_apps.o \
./src/src_user/Applications/UserDefined/debug_nop.o \
./src/src_user/Applications/UserDefined/sample_code.o \
./src/src_user/Applications/UserDefined/tlm_mem_dump.o 

CPP_DEPS += \
./src/src_user/Applications/UserDefined/GlobalValDefine.d \
./src/src_user/Applications/UserDefined/Global_PWR_Parameter.d \
./src/src_user/Applications/UserDefined/IF_Instances.d 


# Each subdirectory must supply rules for building sources it contributes
src/src_user/Applications/UserDefined/%.o src/src_user/Applications/UserDefined/%.su src/src_user/Applications/UserDefined/%.cyclo: ../src/src_user/Applications/UserDefined/%.cpp src/src_user/Applications/UserDefined/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DUSE_HAL_DRIVER -DSTM32F469xx -DONE_U -c -I../src -I../1U_config/Inc -I../1U_config/Drivers/STM32F4xx_HAL_Driver/Inc -I../1U_config/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../1U_config/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../1U_config/Drivers/CMSIS/Include -Og -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src/src_user/Applications/UserDefined/%.o src/src_user/Applications/UserDefined/%.su src/src_user/Applications/UserDefined/%.cyclo: ../src/src_user/Applications/UserDefined/%.c src/src_user/Applications/UserDefined/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F469xx -DONE_U -c -I../src -I../1U_config/Inc -I../1U_config/Drivers/STM32F4xx_HAL_Driver/Inc -I../1U_config/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../1U_config/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../1U_config/Drivers/CMSIS/Include -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-src-2f-src_user-2f-Applications-2f-UserDefined

clean-src-2f-src_user-2f-Applications-2f-UserDefined:
	-$(RM) ./src/src_user/Applications/UserDefined/GlobalValDefine.cyclo ./src/src_user/Applications/UserDefined/GlobalValDefine.d ./src/src_user/Applications/UserDefined/GlobalValDefine.o ./src/src_user/Applications/UserDefined/GlobalValDefine.su ./src/src_user/Applications/UserDefined/Global_PWR_Parameter.cyclo ./src/src_user/Applications/UserDefined/Global_PWR_Parameter.d ./src/src_user/Applications/UserDefined/Global_PWR_Parameter.o ./src/src_user/Applications/UserDefined/Global_PWR_Parameter.su ./src/src_user/Applications/UserDefined/IF_Instances.cyclo ./src/src_user/Applications/UserDefined/IF_Instances.d ./src/src_user/Applications/UserDefined/IF_Instances.o ./src/src_user/Applications/UserDefined/IF_Instances.su ./src/src_user/Applications/UserDefined/debug_apps.cyclo ./src/src_user/Applications/UserDefined/debug_apps.d ./src/src_user/Applications/UserDefined/debug_apps.o ./src/src_user/Applications/UserDefined/debug_apps.su ./src/src_user/Applications/UserDefined/debug_nop.cyclo ./src/src_user/Applications/UserDefined/debug_nop.d ./src/src_user/Applications/UserDefined/debug_nop.o ./src/src_user/Applications/UserDefined/debug_nop.su ./src/src_user/Applications/UserDefined/sample_code.cyclo ./src/src_user/Applications/UserDefined/sample_code.d ./src/src_user/Applications/UserDefined/sample_code.o ./src/src_user/Applications/UserDefined/sample_code.su ./src/src_user/Applications/UserDefined/tlm_mem_dump.cyclo ./src/src_user/Applications/UserDefined/tlm_mem_dump.d ./src/src_user/Applications/UserDefined/tlm_mem_dump.o ./src/src_user/Applications/UserDefined/tlm_mem_dump.su

.PHONY: clean-src-2f-src_user-2f-Applications-2f-UserDefined

