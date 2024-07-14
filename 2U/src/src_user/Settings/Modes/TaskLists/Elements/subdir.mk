################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/src_user/Settings/Modes/TaskLists/Elements/tl_elem_debug_display.c \
../src/src_user/Settings/Modes/TaskLists/Elements/tl_elem_drivers_update.c \
../src/src_user/Settings/Modes/TaskLists/Elements/tl_elem_gs_related_process.c \
../src/src_user/Settings/Modes/TaskLists/Elements/tl_elem_tlm_cmd_hirate.c 

C_DEPS += \
./src/src_user/Settings/Modes/TaskLists/Elements/tl_elem_debug_display.d \
./src/src_user/Settings/Modes/TaskLists/Elements/tl_elem_drivers_update.d \
./src/src_user/Settings/Modes/TaskLists/Elements/tl_elem_gs_related_process.d \
./src/src_user/Settings/Modes/TaskLists/Elements/tl_elem_tlm_cmd_hirate.d 

OBJS += \
./src/src_user/Settings/Modes/TaskLists/Elements/tl_elem_debug_display.o \
./src/src_user/Settings/Modes/TaskLists/Elements/tl_elem_drivers_update.o \
./src/src_user/Settings/Modes/TaskLists/Elements/tl_elem_gs_related_process.o \
./src/src_user/Settings/Modes/TaskLists/Elements/tl_elem_tlm_cmd_hirate.o 


# Each subdirectory must supply rules for building sources it contributes
src/src_user/Settings/Modes/TaskLists/Elements/%.o src/src_user/Settings/Modes/TaskLists/Elements/%.su src/src_user/Settings/Modes/TaskLists/Elements/%.cyclo: ../src/src_user/Settings/Modes/TaskLists/Elements/%.c src/src_user/Settings/Modes/TaskLists/Elements/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F469xx -DTWO_U -c -I../2U_config/Inc -I../2U_config/Drivers/STM32F4xx_HAL_Driver/Inc -I../2U_config/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../2U_config/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../2U_config/Drivers/CMSIS/Include -I../src -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-src-2f-src_user-2f-Settings-2f-Modes-2f-TaskLists-2f-Elements

clean-src-2f-src_user-2f-Settings-2f-Modes-2f-TaskLists-2f-Elements:
	-$(RM) ./src/src_user/Settings/Modes/TaskLists/Elements/tl_elem_debug_display.cyclo ./src/src_user/Settings/Modes/TaskLists/Elements/tl_elem_debug_display.d ./src/src_user/Settings/Modes/TaskLists/Elements/tl_elem_debug_display.o ./src/src_user/Settings/Modes/TaskLists/Elements/tl_elem_debug_display.su ./src/src_user/Settings/Modes/TaskLists/Elements/tl_elem_drivers_update.cyclo ./src/src_user/Settings/Modes/TaskLists/Elements/tl_elem_drivers_update.d ./src/src_user/Settings/Modes/TaskLists/Elements/tl_elem_drivers_update.o ./src/src_user/Settings/Modes/TaskLists/Elements/tl_elem_drivers_update.su ./src/src_user/Settings/Modes/TaskLists/Elements/tl_elem_gs_related_process.cyclo ./src/src_user/Settings/Modes/TaskLists/Elements/tl_elem_gs_related_process.d ./src/src_user/Settings/Modes/TaskLists/Elements/tl_elem_gs_related_process.o ./src/src_user/Settings/Modes/TaskLists/Elements/tl_elem_gs_related_process.su ./src/src_user/Settings/Modes/TaskLists/Elements/tl_elem_tlm_cmd_hirate.cyclo ./src/src_user/Settings/Modes/TaskLists/Elements/tl_elem_tlm_cmd_hirate.d ./src/src_user/Settings/Modes/TaskLists/Elements/tl_elem_tlm_cmd_hirate.o ./src/src_user/Settings/Modes/TaskLists/Elements/tl_elem_tlm_cmd_hirate.su

.PHONY: clean-src-2f-src_user-2f-Settings-2f-Modes-2f-TaskLists-2f-Elements

