################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/src_user/Settings/Modes/Transitions/CheckoutSL.c \
../src/src_user/Settings/Modes/Transitions/DampingSL.c \
../src/src_user/Settings/Modes/Transitions/DebugSL.c \
../src/src_user/Settings/Modes/Transitions/EarthCenterPointing.c \
../src/src_user/Settings/Modes/Transitions/GroundTracking.c \
../src/src_user/Settings/Modes/Transitions/HiSpinSL.c \
../src/src_user/Settings/Modes/Transitions/InertialPointing.c \
../src/src_user/Settings/Modes/Transitions/OtherSatellitePointing.c \
../src/src_user/Settings/Modes/Transitions/RelativeOrbitalControl.c \
../src/src_user/Settings/Modes/Transitions/ReservedSL.c \
../src/src_user/Settings/Modes/Transitions/StandbySL.c \
../src/src_user/Settings/Modes/Transitions/StartupSL.c \
../src/src_user/Settings/Modes/Transitions/SunPointingSL.c \
../src/src_user/Settings/Modes/Transitions/VelocityDirectionPointing.c \
../src/src_user/Settings/Modes/Transitions/sequence_items.c \
../src/src_user/Settings/Modes/Transitions/sl_initial.c \
../src/src_user/Settings/Modes/Transitions/sl_nop.c 

C_DEPS += \
./src/src_user/Settings/Modes/Transitions/CheckoutSL.d \
./src/src_user/Settings/Modes/Transitions/DampingSL.d \
./src/src_user/Settings/Modes/Transitions/DebugSL.d \
./src/src_user/Settings/Modes/Transitions/EarthCenterPointing.d \
./src/src_user/Settings/Modes/Transitions/GroundTracking.d \
./src/src_user/Settings/Modes/Transitions/HiSpinSL.d \
./src/src_user/Settings/Modes/Transitions/InertialPointing.d \
./src/src_user/Settings/Modes/Transitions/OtherSatellitePointing.d \
./src/src_user/Settings/Modes/Transitions/RelativeOrbitalControl.d \
./src/src_user/Settings/Modes/Transitions/ReservedSL.d \
./src/src_user/Settings/Modes/Transitions/StandbySL.d \
./src/src_user/Settings/Modes/Transitions/StartupSL.d \
./src/src_user/Settings/Modes/Transitions/SunPointingSL.d \
./src/src_user/Settings/Modes/Transitions/VelocityDirectionPointing.d \
./src/src_user/Settings/Modes/Transitions/sequence_items.d \
./src/src_user/Settings/Modes/Transitions/sl_initial.d \
./src/src_user/Settings/Modes/Transitions/sl_nop.d 

OBJS += \
./src/src_user/Settings/Modes/Transitions/CheckoutSL.o \
./src/src_user/Settings/Modes/Transitions/DampingSL.o \
./src/src_user/Settings/Modes/Transitions/DebugSL.o \
./src/src_user/Settings/Modes/Transitions/EarthCenterPointing.o \
./src/src_user/Settings/Modes/Transitions/GroundTracking.o \
./src/src_user/Settings/Modes/Transitions/HiSpinSL.o \
./src/src_user/Settings/Modes/Transitions/InertialPointing.o \
./src/src_user/Settings/Modes/Transitions/OtherSatellitePointing.o \
./src/src_user/Settings/Modes/Transitions/RelativeOrbitalControl.o \
./src/src_user/Settings/Modes/Transitions/ReservedSL.o \
./src/src_user/Settings/Modes/Transitions/StandbySL.o \
./src/src_user/Settings/Modes/Transitions/StartupSL.o \
./src/src_user/Settings/Modes/Transitions/SunPointingSL.o \
./src/src_user/Settings/Modes/Transitions/VelocityDirectionPointing.o \
./src/src_user/Settings/Modes/Transitions/sequence_items.o \
./src/src_user/Settings/Modes/Transitions/sl_initial.o \
./src/src_user/Settings/Modes/Transitions/sl_nop.o 


# Each subdirectory must supply rules for building sources it contributes
src/src_user/Settings/Modes/Transitions/%.o src/src_user/Settings/Modes/Transitions/%.su src/src_user/Settings/Modes/Transitions/%.cyclo: ../src/src_user/Settings/Modes/Transitions/%.c src/src_user/Settings/Modes/Transitions/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F469xx -DTWO_U -c -I../2U_config/Inc -I../2U_config/Drivers/STM32F4xx_HAL_Driver/Inc -I../2U_config/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../2U_config/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../2U_config/Drivers/CMSIS/Include -I../src -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-src-2f-src_user-2f-Settings-2f-Modes-2f-Transitions

clean-src-2f-src_user-2f-Settings-2f-Modes-2f-Transitions:
	-$(RM) ./src/src_user/Settings/Modes/Transitions/CheckoutSL.cyclo ./src/src_user/Settings/Modes/Transitions/CheckoutSL.d ./src/src_user/Settings/Modes/Transitions/CheckoutSL.o ./src/src_user/Settings/Modes/Transitions/CheckoutSL.su ./src/src_user/Settings/Modes/Transitions/DampingSL.cyclo ./src/src_user/Settings/Modes/Transitions/DampingSL.d ./src/src_user/Settings/Modes/Transitions/DampingSL.o ./src/src_user/Settings/Modes/Transitions/DampingSL.su ./src/src_user/Settings/Modes/Transitions/DebugSL.cyclo ./src/src_user/Settings/Modes/Transitions/DebugSL.d ./src/src_user/Settings/Modes/Transitions/DebugSL.o ./src/src_user/Settings/Modes/Transitions/DebugSL.su ./src/src_user/Settings/Modes/Transitions/EarthCenterPointing.cyclo ./src/src_user/Settings/Modes/Transitions/EarthCenterPointing.d ./src/src_user/Settings/Modes/Transitions/EarthCenterPointing.o ./src/src_user/Settings/Modes/Transitions/EarthCenterPointing.su ./src/src_user/Settings/Modes/Transitions/GroundTracking.cyclo ./src/src_user/Settings/Modes/Transitions/GroundTracking.d ./src/src_user/Settings/Modes/Transitions/GroundTracking.o ./src/src_user/Settings/Modes/Transitions/GroundTracking.su ./src/src_user/Settings/Modes/Transitions/HiSpinSL.cyclo ./src/src_user/Settings/Modes/Transitions/HiSpinSL.d ./src/src_user/Settings/Modes/Transitions/HiSpinSL.o ./src/src_user/Settings/Modes/Transitions/HiSpinSL.su ./src/src_user/Settings/Modes/Transitions/InertialPointing.cyclo ./src/src_user/Settings/Modes/Transitions/InertialPointing.d ./src/src_user/Settings/Modes/Transitions/InertialPointing.o ./src/src_user/Settings/Modes/Transitions/InertialPointing.su ./src/src_user/Settings/Modes/Transitions/OtherSatellitePointing.cyclo ./src/src_user/Settings/Modes/Transitions/OtherSatellitePointing.d ./src/src_user/Settings/Modes/Transitions/OtherSatellitePointing.o ./src/src_user/Settings/Modes/Transitions/OtherSatellitePointing.su ./src/src_user/Settings/Modes/Transitions/RelativeOrbitalControl.cyclo ./src/src_user/Settings/Modes/Transitions/RelativeOrbitalControl.d ./src/src_user/Settings/Modes/Transitions/RelativeOrbitalControl.o ./src/src_user/Settings/Modes/Transitions/RelativeOrbitalControl.su ./src/src_user/Settings/Modes/Transitions/ReservedSL.cyclo ./src/src_user/Settings/Modes/Transitions/ReservedSL.d ./src/src_user/Settings/Modes/Transitions/ReservedSL.o ./src/src_user/Settings/Modes/Transitions/ReservedSL.su ./src/src_user/Settings/Modes/Transitions/StandbySL.cyclo ./src/src_user/Settings/Modes/Transitions/StandbySL.d ./src/src_user/Settings/Modes/Transitions/StandbySL.o ./src/src_user/Settings/Modes/Transitions/StandbySL.su ./src/src_user/Settings/Modes/Transitions/StartupSL.cyclo ./src/src_user/Settings/Modes/Transitions/StartupSL.d ./src/src_user/Settings/Modes/Transitions/StartupSL.o ./src/src_user/Settings/Modes/Transitions/StartupSL.su ./src/src_user/Settings/Modes/Transitions/SunPointingSL.cyclo ./src/src_user/Settings/Modes/Transitions/SunPointingSL.d ./src/src_user/Settings/Modes/Transitions/SunPointingSL.o ./src/src_user/Settings/Modes/Transitions/SunPointingSL.su ./src/src_user/Settings/Modes/Transitions/VelocityDirectionPointing.cyclo ./src/src_user/Settings/Modes/Transitions/VelocityDirectionPointing.d ./src/src_user/Settings/Modes/Transitions/VelocityDirectionPointing.o ./src/src_user/Settings/Modes/Transitions/VelocityDirectionPointing.su ./src/src_user/Settings/Modes/Transitions/sequence_items.cyclo ./src/src_user/Settings/Modes/Transitions/sequence_items.d ./src/src_user/Settings/Modes/Transitions/sequence_items.o ./src/src_user/Settings/Modes/Transitions/sequence_items.su ./src/src_user/Settings/Modes/Transitions/sl_initial.cyclo ./src/src_user/Settings/Modes/Transitions/sl_initial.d ./src/src_user/Settings/Modes/Transitions/sl_initial.o ./src/src_user/Settings/Modes/Transitions/sl_initial.su ./src/src_user/Settings/Modes/Transitions/sl_nop.cyclo ./src/src_user/Settings/Modes/Transitions/sl_nop.d ./src/src_user/Settings/Modes/Transitions/sl_nop.o ./src/src_user/Settings/Modes/Transitions/sl_nop.su

.PHONY: clean-src-2f-src_user-2f-Settings-2f-Modes-2f-Transitions

