################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/src_user/TlmCmd/NormalBlockCommandDefinition/subdir.mk
-include src/src_user/TlmCmd/Ccsds/subdir.mk
-include src/src_user/TlmCmd/subdir.mk
-include src/src_user/Settings/TlmCmd/Ccsds/subdir.mk
-include src/src_user/Settings/TlmCmd/subdir.mk
-include src/src_user/Settings/System/EventHandlerRules/subdir.mk
-include src/src_user/Settings/System/subdir.mk
-include src/src_user/Settings/Modes/Transitions/subdir.mk
-include src/src_user/Settings/Modes/TaskLists/Elements/subdir.mk
-include src/src_user/Settings/Modes/TaskLists/subdir.mk
-include src/src_user/Settings/Modes/Phase/subdir.mk
-include src/src_user/Settings/Modes/Eqlipse/subdir.mk
-include src/src_user/Settings/Modes/subdir.mk
-include src/src_user/ReProgramming/Command/subdir.mk
-include src/src_user/Library/SDcard/subdir.mk
-include src/src_user/Library/General/subdir.mk
-include src/src_user/Library/subdir.mk
-include src/src_user/IfWrapper/SilsMockup/subdir.mk
-include src/src_user/IfWrapper/subdir.mk
-include src/src_user/Drivers/Sensor/subdir.mk
-include src/src_user/Drivers/Power/subdir.mk
-include src/src_user/Drivers/Etc/subdir.mk
-include src/src_user/Drivers/Dbg/subdir.mk
-include src/src_user/Drivers/Com/TXPIC/subdir.mk
-include src/src_user/Drivers/Com/SOBC/subdir.mk
-include src/src_user/Drivers/Com/RXPIC/subdir.mk
-include src/src_user/Drivers/Com/INTER_SAT/subdir.mk
-include src/src_user/Drivers/Com/GS/subdir.mk
-include src/src_user/Drivers/Com/COBC/subdir.mk
-include src/src_user/Drivers/Com/subdir.mk
-include src/src_user/Drivers/Cdh/WDT/subdir.mk
-include src/src_user/Drivers/Cdh/SPIPROM/subdir.mk
-include src/src_user/Drivers/Cdh/SD/subdir.mk
-include src/src_user/Drivers/Cdh/RTC/subdir.mk
-include src/src_user/Drivers/Cdh/MRAM/subdir.mk
-include src/src_user/Drivers/Cdh/LoRa/subdir.mk
-include src/src_user/Drivers/Cdh/Flash/subdir.mk
-include src/src_user/Drivers/Cdh/EEPROM/subdir.mk
-include src/src_user/Drivers/Cdh/subdir.mk
-include src/src_user/Drivers/Aocs/LinearAlgebra/subdir.mk
-include src/src_user/Drivers/Aocs/subdir.mk
-include src/src_user/Drivers/subdir.mk
-include src/src_user/Applications/UserDefined/Sensor/subdir.mk
-include src/src_user/Applications/UserDefined/Power/subdir.mk
-include src/src_user/Applications/UserDefined/Mission/subdir.mk
-include src/src_user/Applications/UserDefined/Memory/subdir.mk
-include src/src_user/Applications/UserDefined/Com/subdir.mk
-include src/src_user/Applications/UserDefined/Cdh/Repro/subdir.mk
-include src/src_user/Applications/UserDefined/Cdh/subdir.mk
-include src/src_user/Applications/UserDefined/Aocs/ODS/subdir.mk
-include src/src_user/Applications/UserDefined/Aocs/OCS/subdir.mk
-include src/src_user/Applications/UserDefined/Aocs/ADS/subdir.mk
-include src/src_user/Applications/UserDefined/Aocs/ACS/subdir.mk
-include src/src_user/Applications/UserDefined/Aocs/subdir.mk
-include src/src_user/Applications/UserDefined/subdir.mk
-include src/src_user/Applications/Middleware/subdir.mk
-include src/src_user/Applications/DriverInstances/subdir.mk
-include src/src_user/Applications/subdir.mk
-include src/src_user/subdir.mk
-include src/src_core/TlmCmd/Ccsds/subdir.mk
-include src/src_core/TlmCmd/subdir.mk
-include src/src_core/System/WatchdogTimer/subdir.mk
-include src/src_core/System/TimeManager/subdir.mk
-include src/src_core/System/TaskManager/subdir.mk
-include src/src_core/System/ModeManager/subdir.mk
-include src/src_core/System/EventManager/subdir.mk
-include src/src_core/System/ApplicationManager/subdir.mk
-include src/src_core/Library/libc/subdir.mk
-include src/src_core/Library/subdir.mk
-include src/src_core/IfWrapper/Common/subdir.mk
-include src/src_core/Drivers/Super/subdir.mk
-include src/src_core/Drivers/Protocol/subdir.mk
-include src/src_core/Applications/TestApp/subdir.mk
-include src/src_core/Applications/subdir.mk
-include src/src_core/subdir.mk
-include Startup/subdir.mk
-include 1U_config/Src/subdir.mk
-include 1U_config/Drivers/STM32F4xx_HAL_Driver/Src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := Nucube_C2A_MOBC
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
Nucube_C2A_MOBC.elf \

MAP_FILES += \
Nucube_C2A_MOBC.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
Nucube_C2A_MOBC.list \

OBJCOPY_BIN += \
Nucube_C2A_MOBC.bin \


# All Target
all: main-build

# Main-build Target
main-build: Nucube_C2A_MOBC.elf secondary-outputs

# Tool invocations
Nucube_C2A_MOBC.elf Nucube_C2A_MOBC.map: $(OBJS) $(USER_OBJS) C:\INAMORILAB\cubesat\MAGNAROII\MAGNAROII_SW\CDH\CDH1_main_for_blockcommand\STM32F469BITX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-g++ -o "Nucube_C2A_MOBC.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\INAMORILAB\cubesat\MAGNAROII\MAGNAROII_SW\CDH\CDH1_main_for_blockcommand\STM32F469BITX_FLASH.ld" --specs=nosys.specs -Wl,-Map="Nucube_C2A_MOBC.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -u _printf_float -Wl,--start-group -lc -lm -lstdc++ -lsupc++ -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

Nucube_C2A_MOBC.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "Nucube_C2A_MOBC.list"
	@echo 'Finished building: $@'
	@echo ' '

Nucube_C2A_MOBC.bin: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "Nucube_C2A_MOBC.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) Nucube_C2A_MOBC.bin Nucube_C2A_MOBC.elf Nucube_C2A_MOBC.list Nucube_C2A_MOBC.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
