################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../driver/rtc+oled_driver.c 

OBJS += \
./driver/rtc+oled_driver.o 

C_DEPS += \
./driver/rtc+oled_driver.d 


# Each subdirectory must supply rules for building sources it contributes
driver/%.o driver/%.su driver/%.cyclo: ../driver/%.c driver/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32F40_41xxx -DSTM32F407xx -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -c -I"C:/Users/Amal/RTOS_workspace/OLED_RTC_using_FreeRTOS/Inc" -I"C:/Users/Amal/RTOS_workspace/OLED_RTC_using_FreeRTOS/ThirdParty/FreeRTOS/portable/GCC/ARM_CM4F" -I"C:/Users/Amal/RTOS_workspace/OLED_RTC_using_FreeRTOS/ThirdParty/FreeRTOS/portable/MemMang" -I"C:/Users/Amal/RTOS_workspace/OLED_RTC_using_FreeRTOS/ThirdParty/FreeRTOS/portable" -I"C:/Users/Amal/RTOS_workspace/OLED_RTC_using_FreeRTOS/ThirdParty/FreeRTOS/include" -I"C:/Users/Amal/RTOS_workspace/OLED_RTC_using_FreeRTOS/CMSIS/device" -I"C:/Users/Amal/RTOS_workspace/OLED_RTC_using_FreeRTOS/config" -I"C:/Users/Amal/RTOS_workspace/OLED_RTC_using_FreeRTOS/CMSIS/core" -I"C:/Users/Amal/RTOS_workspace/OLED_RTC_using_FreeRTOS/CMSIS/device" -I"C:/Users/Amal/RTOS_workspace/OLED_RTC_using_FreeRTOS/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-driver

clean-driver:
	-$(RM) ./driver/rtc+oled_driver.cyclo ./driver/rtc+oled_driver.d ./driver/rtc+oled_driver.o ./driver/rtc+oled_driver.su

.PHONY: clean-driver

