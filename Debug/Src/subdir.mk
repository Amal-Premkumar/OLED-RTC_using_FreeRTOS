################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/OLED_ssd1306.c \
../Src/RTC_ds3231.c \
../Src/main.c \
../Src/syscalls.c \
../Src/system_coreclock.c 

OBJS += \
./Src/OLED_ssd1306.o \
./Src/RTC_ds3231.o \
./Src/main.o \
./Src/syscalls.o \
./Src/system_coreclock.o 

C_DEPS += \
./Src/OLED_ssd1306.d \
./Src/RTC_ds3231.d \
./Src/main.d \
./Src/syscalls.d \
./Src/system_coreclock.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32F40_41xxx -DSTM32F407xx -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -c -I"C:/Users/Amal/RTOS_workspace/OLED_RTC_using_FreeRTOS/Inc" -I"C:/Users/Amal/RTOS_workspace/OLED_RTC_using_FreeRTOS/ThirdParty/FreeRTOS/portable/GCC/ARM_CM4F" -I"C:/Users/Amal/RTOS_workspace/OLED_RTC_using_FreeRTOS/ThirdParty/FreeRTOS/portable/MemMang" -I"C:/Users/Amal/RTOS_workspace/OLED_RTC_using_FreeRTOS/ThirdParty/FreeRTOS/portable" -I"C:/Users/Amal/RTOS_workspace/OLED_RTC_using_FreeRTOS/ThirdParty/FreeRTOS/include" -I"C:/Users/Amal/RTOS_workspace/OLED_RTC_using_FreeRTOS/CMSIS/device" -I"C:/Users/Amal/RTOS_workspace/OLED_RTC_using_FreeRTOS/config" -I"C:/Users/Amal/RTOS_workspace/OLED_RTC_using_FreeRTOS/CMSIS/core" -I"C:/Users/Amal/RTOS_workspace/OLED_RTC_using_FreeRTOS/CMSIS/device" -I"C:/Users/Amal/RTOS_workspace/OLED_RTC_using_FreeRTOS/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/OLED_ssd1306.cyclo ./Src/OLED_ssd1306.d ./Src/OLED_ssd1306.o ./Src/OLED_ssd1306.su ./Src/RTC_ds3231.cyclo ./Src/RTC_ds3231.d ./Src/RTC_ds3231.o ./Src/RTC_ds3231.su ./Src/main.cyclo ./Src/main.d ./Src/main.o ./Src/main.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/system_coreclock.cyclo ./Src/system_coreclock.d ./Src/system_coreclock.o ./Src/system_coreclock.su

.PHONY: clean-Src

