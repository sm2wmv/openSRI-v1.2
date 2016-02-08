################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += 

OBJS += 

C_DEPS += 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__LPC17XX__ -DUSB_DEVICE_ONLY -D__USE_CMSIS=CMSISv2p00_LPC17xx -D__REDLIB__ -D__CODE_RED -I"C:\Users\micke\Documents\LPCXpresso_6.1.0_164\workspace_sri_v1\CDL\CMSISv2p00_LPC17xx\inc" -I"C:\Users\micke\Documents\LPCXpresso_6.1.0_164\workspace_sri_v1\LPCUSBlib\Drivers\USB" -I"C:\Users\micke\Documents\LPCXpresso_6.1.0_164\workspace_sri_v1\VirtualSerialDevice" -I"C:\Users\micke\Documents\LPCXpresso_6.1.0_164\workspace_sri_v1\CDL\CMSISv2p00_LPC17xx\lpcinc" -I"C:\Users\micke\Documents\LPCXpresso_6.1.0_164\workspace_sri_v1\LPCUSBlib\Drivers\USB" -I"C:\Users\micke\Documents\LPCXpresso_6.1.0_164\workspace_sri_v1\CDL\LPC17xxLib\inc" -I"C:\Users\micke\Documents\LPCXpresso_6.1.0_164\workspace_sri_v1\SRI_v1\include" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -std=gnu99 -mcpu=cortex-m3 -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


