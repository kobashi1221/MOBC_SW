################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/src_user/Settings/Modes/TaskLists/CheckoutTL.c \
../src/src_user/Settings/Modes/TaskLists/HiSpinTL.c \
../src/src_user/Settings/Modes/TaskLists/MissionTL.c \
../src/src_user/Settings/Modes/TaskLists/Momentum_ControlTL.c \
../src/src_user/Settings/Modes/TaskLists/ReservedTL.c \
../src/src_user/Settings/Modes/TaskLists/SeparetionTL.c \
../src/src_user/Settings/Modes/TaskLists/StandbyTL.c \
../src/src_user/Settings/Modes/TaskLists/StartupTL.c \
../src/src_user/Settings/Modes/TaskLists/tl_initial.c 

C_DEPS += \
./src/src_user/Settings/Modes/TaskLists/CheckoutTL.d \
./src/src_user/Settings/Modes/TaskLists/HiSpinTL.d \
./src/src_user/Settings/Modes/TaskLists/MissionTL.d \
./src/src_user/Settings/Modes/TaskLists/Momentum_ControlTL.d \
./src/src_user/Settings/Modes/TaskLists/ReservedTL.d \
./src/src_user/Settings/Modes/TaskLists/SeparetionTL.d \
./src/src_user/Settings/Modes/TaskLists/StandbyTL.d \
./src/src_user/Settings/Modes/TaskLists/StartupTL.d \
./src/src_user/Settings/Modes/TaskLists/tl_initial.d 

OBJS += \
./src/src_user/Settings/Modes/TaskLists/CheckoutTL.o \
./src/src_user/Settings/Modes/TaskLists/HiSpinTL.o \
./src/src_user/Settings/Modes/TaskLists/MissionTL.o \
./src/src_user/Settings/Modes/TaskLists/Momentum_ControlTL.o \
./src/src_user/Settings/Modes/TaskLists/ReservedTL.o \
./src/src_user/Settings/Modes/TaskLists/SeparetionTL.o \
./src/src_user/Settings/Modes/TaskLists/StandbyTL.o \
./src/src_user/Settings/Modes/TaskLists/StartupTL.o \
./src/src_user/Settings/Modes/TaskLists/tl_initial.o 


# Each subdirectory must supply rules for building sources it contributes
src/src_user/Settings/Modes/TaskLists/%.o src/src_user/Settings/Modes/TaskLists/%.su src/src_user/Settings/Modes/TaskLists/%.cyclo: ../src/src_user/Settings/Modes/TaskLists/%.c src/src_user/Settings/Modes/TaskLists/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F469xx -DONE_U -c -I../src -I../1U_config/Inc -I../1U_config/Drivers/STM32F4xx_HAL_Driver/Inc -I../1U_config/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../1U_config/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../1U_config/Drivers/CMSIS/Include -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-src-2f-src_user-2f-Settings-2f-Modes-2f-TaskLists

clean-src-2f-src_user-2f-Settings-2f-Modes-2f-TaskLists:
	-$(RM) ./src/src_user/Settings/Modes/TaskLists/CheckoutTL.cyclo ./src/src_user/Settings/Modes/TaskLists/CheckoutTL.d ./src/src_user/Settings/Modes/TaskLists/CheckoutTL.o ./src/src_user/Settings/Modes/TaskLists/CheckoutTL.su ./src/src_user/Settings/Modes/TaskLists/HiSpinTL.cyclo ./src/src_user/Settings/Modes/TaskLists/HiSpinTL.d ./src/src_user/Settings/Modes/TaskLists/HiSpinTL.o ./src/src_user/Settings/Modes/TaskLists/HiSpinTL.su ./src/src_user/Settings/Modes/TaskLists/MissionTL.cyclo ./src/src_user/Settings/Modes/TaskLists/MissionTL.d ./src/src_user/Settings/Modes/TaskLists/MissionTL.o ./src/src_user/Settings/Modes/TaskLists/MissionTL.su ./src/src_user/Settings/Modes/TaskLists/Momentum_ControlTL.cyclo ./src/src_user/Settings/Modes/TaskLists/Momentum_ControlTL.d ./src/src_user/Settings/Modes/TaskLists/Momentum_ControlTL.o ./src/src_user/Settings/Modes/TaskLists/Momentum_ControlTL.su ./src/src_user/Settings/Modes/TaskLists/ReservedTL.cyclo ./src/src_user/Settings/Modes/TaskLists/ReservedTL.d ./src/src_user/Settings/Modes/TaskLists/ReservedTL.o ./src/src_user/Settings/Modes/TaskLists/ReservedTL.su ./src/src_user/Settings/Modes/TaskLists/SeparetionTL.cyclo ./src/src_user/Settings/Modes/TaskLists/SeparetionTL.d ./src/src_user/Settings/Modes/TaskLists/SeparetionTL.o ./src/src_user/Settings/Modes/TaskLists/SeparetionTL.su ./src/src_user/Settings/Modes/TaskLists/StandbyTL.cyclo ./src/src_user/Settings/Modes/TaskLists/StandbyTL.d ./src/src_user/Settings/Modes/TaskLists/StandbyTL.o ./src/src_user/Settings/Modes/TaskLists/StandbyTL.su ./src/src_user/Settings/Modes/TaskLists/StartupTL.cyclo ./src/src_user/Settings/Modes/TaskLists/StartupTL.d ./src/src_user/Settings/Modes/TaskLists/StartupTL.o ./src/src_user/Settings/Modes/TaskLists/StartupTL.su ./src/src_user/Settings/Modes/TaskLists/tl_initial.cyclo ./src/src_user/Settings/Modes/TaskLists/tl_initial.d ./src/src_user/Settings/Modes/TaskLists/tl_initial.o ./src/src_user/Settings/Modes/TaskLists/tl_initial.su

.PHONY: clean-src-2f-src_user-2f-Settings-2f-Modes-2f-TaskLists

