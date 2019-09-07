################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../modbus/mb.c 

OBJS += \
./modbus/mb.o 

C_DEPS += \
./modbus/mb.d 


# Each subdirectory must supply rules for building sources it contributes
modbus/%.o: ../modbus/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -DCPU_MKE06Z128VLD4_cm0plus -DFSL_RTOS_FREE_RTOS -DCPU_MKE06Z128VLD4 -DSDK_OS_FREE_RTOS -I../CMSIS -I../board -I../drivers -I../freertos/include -I../freertos/portable -I../source -I../utilities -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


