################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/audio.c \
../src/cat_interface.c \
../src/comm_interface.c \
../src/computer_interface.c \
../src/cr_startup_lpc176x.c \
../src/ctrl.c \
../src/cw_ctrl.c \
../src/display.c \
../src/eeprom_m24.c \
../src/event_handler.c \
../src/event_queue.c \
../src/init.c \
../src/lcd.c \
../src/main.c \
../src/misc.c \
../src/qei.c \
../src/queue.c \
../src/sc16is7x0.c \
../src/sequencer.c \
../src/settings.c \
../src/status.c \
../src/timer.c \
../src/winkey.c 

OBJS += \
./src/audio.o \
./src/cat_interface.o \
./src/comm_interface.o \
./src/computer_interface.o \
./src/cr_startup_lpc176x.o \
./src/ctrl.o \
./src/cw_ctrl.o \
./src/display.o \
./src/eeprom_m24.o \
./src/event_handler.o \
./src/event_queue.o \
./src/init.o \
./src/lcd.o \
./src/main.o \
./src/misc.o \
./src/qei.o \
./src/queue.o \
./src/sc16is7x0.o \
./src/sequencer.o \
./src/settings.o \
./src/status.o \
./src/timer.o \
./src/winkey.o 

C_DEPS += \
./src/audio.d \
./src/cat_interface.d \
./src/comm_interface.d \
./src/computer_interface.d \
./src/cr_startup_lpc176x.d \
./src/ctrl.d \
./src/cw_ctrl.d \
./src/display.d \
./src/eeprom_m24.d \
./src/event_handler.d \
./src/event_queue.d \
./src/init.d \
./src/lcd.d \
./src/main.d \
./src/misc.d \
./src/qei.d \
./src/queue.d \
./src/sc16is7x0.d \
./src/sequencer.d \
./src/settings.d \
./src/status.d \
./src/timer.d \
./src/winkey.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__LPC17XX__ -DUSB_DEVICE_ONLY -D__USE_CMSIS=CMSISv2p00_LPC17xx -D__REDLIB__ -D__CODE_RED -I"C:\Users\micke\Documents\LPCXpresso_8.0.0_526\workspace_sri1_1\CDL\CMSISv2p00_LPC17xx\inc" -I"C:\Users\micke\Documents\LPCXpresso_8.0.0_526\workspace_sri1_1\CDL\CMSISv2p00_LPC17xx\lpcinc" -I"C:\Users\micke\Documents\LPCXpresso_8.0.0_526\workspace_sri1_1\CDL\LPC17xxLib\inc" -I"C:\Users\micke\Documents\LPCXpresso_8.0.0_526\workspace_sri1_1\SRI_v1\include" -O0 -g3 -Wall -Wextra -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -std=gnu99 -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/main.o: ../src/main.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__LPC17XX__ -DUSB_DEVICE_ONLY -D__USE_CMSIS=CMSISv2p00_LPC17xx -D__REDLIB__ -D__CODE_RED -I"C:\Users\micke\Documents\LPCXpresso_8.0.0_526\workspace_sri1_1\CDL\CMSISv2p00_LPC17xx\inc" -I"C:\Users\micke\Documents\LPCXpresso_8.0.0_526\workspace_sri1_1\CDL\CMSISv2p00_LPC17xx\lpcinc" -I"C:\Users\micke\Documents\LPCXpresso_8.0.0_526\workspace_sri1_1\CDL\LPC17xxLib\inc" -I"C:\Users\micke\Documents\LPCXpresso_8.0.0_526\workspace_sri1_1\SRI_v1\include" -O0 -g3 -Wall -Wextra -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -std=gnu99 -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"src/main.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


