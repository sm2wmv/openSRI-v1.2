################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: SRI_v1.axf

# Tool invocations
SRI_v1.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"C:\Users\micke\Documents\openSRI\sri_v1_2\LPCUSBLib\LPC17xx_Device" -L"C:\Users\micke\Documents\openSRI\sri_v1_2\CDL" -Xlinker -Map="SRI_v1.map" -Xlinker --gc-sections -mcpu=cortex-m3 -mthumb -T "SRI_v1_LPC17xx.ld" -o "SRI_v1.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) SRI_v1.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-objcopy -O binary "SRI_v1.axf" "SRI_v1.bin";checksum -v "SRI_v1.bin";arm-none-eabi-objcopy -I binary "SRI_v1.bin" -O ihex "SRI_v1.hex";
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
