################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/src_user/IfWrapper/HAL.c \
../src/src_user/IfWrapper/if_list.c 

CPP_SRCS += \
../src/src_user/IfWrapper/IF_class.cpp 

C_DEPS += \
./src/src_user/IfWrapper/HAL.d \
./src/src_user/IfWrapper/if_list.d 

OBJS += \
./src/src_user/IfWrapper/HAL.o \
./src/src_user/IfWrapper/IF_class.o \
./src/src_user/IfWrapper/if_list.o 

CPP_DEPS += \
./src/src_user/IfWrapper/IF_class.d 


# Each subdirectory must supply rules for building sources it contributes
src/src_user/IfWrapper/%.o src/src_user/IfWrapper/%.su src/src_user/IfWrapper/%.cyclo: ../src/src_user/IfWrapper/%.c src/src_user/IfWrapper/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F469xx -DTWO_U -c -I../2U_config/Inc -I../2U_config/Drivers/STM32F4xx_HAL_Driver/Inc -I../2U_config/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../2U_config/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../2U_config/Drivers/CMSIS/Include -I../src -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src/src_user/IfWrapper/%.o src/src_user/IfWrapper/%.su src/src_user/IfWrapper/%.cyclo: ../src/src_user/IfWrapper/%.cpp src/src_user/IfWrapper/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DUSE_HAL_DRIVER -DSTM32F469xx -DTWO_U -c -I../2U_config/Inc -I../2U_config/Drivers/STM32F4xx_HAL_Driver/Inc -I../2U_config/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../2U_config/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../2U_config/Drivers/CMSIS/Include -I../src -Og -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-src-2f-src_user-2f-IfWrapper

clean-src-2f-src_user-2f-IfWrapper:
	-$(RM) ./src/src_user/IfWrapper/HAL.cyclo ./src/src_user/IfWrapper/HAL.d ./src/src_user/IfWrapper/HAL.o ./src/src_user/IfWrapper/HAL.su ./src/src_user/IfWrapper/IF_class.cyclo ./src/src_user/IfWrapper/IF_class.d ./src/src_user/IfWrapper/IF_class.o ./src/src_user/IfWrapper/IF_class.su ./src/src_user/IfWrapper/if_list.cyclo ./src/src_user/IfWrapper/if_list.d ./src/src_user/IfWrapper/if_list.o ./src/src_user/IfWrapper/if_list.su

.PHONY: clean-src-2f-src_user-2f-IfWrapper

