################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/src_user/Applications/UserDefined/Aocs/AOCS_hils.c \
../src/src_user/Applications/UserDefined/Aocs/AOCS_tlm_param.c 

CPP_SRCS += \
../src/src_user/Applications/UserDefined/Aocs/AOCS_cmd_EKF.cpp \
../src/src_user/Applications/UserDefined/Aocs/AOCS_cmd_Photo_COBC.cpp \
../src/src_user/Applications/UserDefined/Aocs/AOCS_cmd_Separation.cpp \
../src/src_user/Applications/UserDefined/Aocs/Global_AOCS_Param_Set.cpp \
../src/src_user/Applications/UserDefined/Aocs/Global_AOCS_Parameter.cpp 

C_DEPS += \
./src/src_user/Applications/UserDefined/Aocs/AOCS_hils.d \
./src/src_user/Applications/UserDefined/Aocs/AOCS_tlm_param.d 

OBJS += \
./src/src_user/Applications/UserDefined/Aocs/AOCS_cmd_EKF.o \
./src/src_user/Applications/UserDefined/Aocs/AOCS_cmd_Photo_COBC.o \
./src/src_user/Applications/UserDefined/Aocs/AOCS_cmd_Separation.o \
./src/src_user/Applications/UserDefined/Aocs/AOCS_hils.o \
./src/src_user/Applications/UserDefined/Aocs/AOCS_tlm_param.o \
./src/src_user/Applications/UserDefined/Aocs/Global_AOCS_Param_Set.o \
./src/src_user/Applications/UserDefined/Aocs/Global_AOCS_Parameter.o 

CPP_DEPS += \
./src/src_user/Applications/UserDefined/Aocs/AOCS_cmd_EKF.d \
./src/src_user/Applications/UserDefined/Aocs/AOCS_cmd_Photo_COBC.d \
./src/src_user/Applications/UserDefined/Aocs/AOCS_cmd_Separation.d \
./src/src_user/Applications/UserDefined/Aocs/Global_AOCS_Param_Set.d \
./src/src_user/Applications/UserDefined/Aocs/Global_AOCS_Parameter.d 


# Each subdirectory must supply rules for building sources it contributes
src/src_user/Applications/UserDefined/Aocs/%.o src/src_user/Applications/UserDefined/Aocs/%.su src/src_user/Applications/UserDefined/Aocs/%.cyclo: ../src/src_user/Applications/UserDefined/Aocs/%.cpp src/src_user/Applications/UserDefined/Aocs/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DUSE_HAL_DRIVER -DSTM32F469xx -DTWO_U -c -I../2U_config/Inc -I../2U_config/Drivers/STM32F4xx_HAL_Driver/Inc -I../2U_config/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../2U_config/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../2U_config/Drivers/CMSIS/Include -I../src -Og -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src/src_user/Applications/UserDefined/Aocs/%.o src/src_user/Applications/UserDefined/Aocs/%.su src/src_user/Applications/UserDefined/Aocs/%.cyclo: ../src/src_user/Applications/UserDefined/Aocs/%.c src/src_user/Applications/UserDefined/Aocs/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F469xx -DTWO_U -c -I../2U_config/Inc -I../2U_config/Drivers/STM32F4xx_HAL_Driver/Inc -I../2U_config/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../2U_config/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../2U_config/Drivers/CMSIS/Include -I../src -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-src-2f-src_user-2f-Applications-2f-UserDefined-2f-Aocs

clean-src-2f-src_user-2f-Applications-2f-UserDefined-2f-Aocs:
	-$(RM) ./src/src_user/Applications/UserDefined/Aocs/AOCS_cmd_EKF.cyclo ./src/src_user/Applications/UserDefined/Aocs/AOCS_cmd_EKF.d ./src/src_user/Applications/UserDefined/Aocs/AOCS_cmd_EKF.o ./src/src_user/Applications/UserDefined/Aocs/AOCS_cmd_EKF.su ./src/src_user/Applications/UserDefined/Aocs/AOCS_cmd_Photo_COBC.cyclo ./src/src_user/Applications/UserDefined/Aocs/AOCS_cmd_Photo_COBC.d ./src/src_user/Applications/UserDefined/Aocs/AOCS_cmd_Photo_COBC.o ./src/src_user/Applications/UserDefined/Aocs/AOCS_cmd_Photo_COBC.su ./src/src_user/Applications/UserDefined/Aocs/AOCS_cmd_Separation.cyclo ./src/src_user/Applications/UserDefined/Aocs/AOCS_cmd_Separation.d ./src/src_user/Applications/UserDefined/Aocs/AOCS_cmd_Separation.o ./src/src_user/Applications/UserDefined/Aocs/AOCS_cmd_Separation.su ./src/src_user/Applications/UserDefined/Aocs/AOCS_hils.cyclo ./src/src_user/Applications/UserDefined/Aocs/AOCS_hils.d ./src/src_user/Applications/UserDefined/Aocs/AOCS_hils.o ./src/src_user/Applications/UserDefined/Aocs/AOCS_hils.su ./src/src_user/Applications/UserDefined/Aocs/AOCS_tlm_param.cyclo ./src/src_user/Applications/UserDefined/Aocs/AOCS_tlm_param.d ./src/src_user/Applications/UserDefined/Aocs/AOCS_tlm_param.o ./src/src_user/Applications/UserDefined/Aocs/AOCS_tlm_param.su ./src/src_user/Applications/UserDefined/Aocs/Global_AOCS_Param_Set.cyclo ./src/src_user/Applications/UserDefined/Aocs/Global_AOCS_Param_Set.d ./src/src_user/Applications/UserDefined/Aocs/Global_AOCS_Param_Set.o ./src/src_user/Applications/UserDefined/Aocs/Global_AOCS_Param_Set.su ./src/src_user/Applications/UserDefined/Aocs/Global_AOCS_Parameter.cyclo ./src/src_user/Applications/UserDefined/Aocs/Global_AOCS_Parameter.d ./src/src_user/Applications/UserDefined/Aocs/Global_AOCS_Parameter.o ./src/src_user/Applications/UserDefined/Aocs/Global_AOCS_Parameter.su

.PHONY: clean-src-2f-src_user-2f-Applications-2f-UserDefined-2f-Aocs

