################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ThirdParty/FreeRTOS/portable/MemMang/heap_4.c 

OBJS += \
./ThirdParty/FreeRTOS/portable/MemMang/heap_4.o 

C_DEPS += \
./ThirdParty/FreeRTOS/portable/MemMang/heap_4.d 


# Each subdirectory must supply rules for building sources it contributes
ThirdParty/FreeRTOS/portable/MemMang/%.o ThirdParty/FreeRTOS/portable/MemMang/%.su ThirdParty/FreeRTOS/portable/MemMang/%.cyclo: ../ThirdParty/FreeRTOS/portable/MemMang/%.c ThirdParty/FreeRTOS/portable/MemMang/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32F40_41xxx -DSTM32F407xx -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -c -I"C:/Users/Amal/RTOS_workspace/OLED_RTC_using_FreeRTOS/Inc" -I"C:/Users/Amal/RTOS_workspace/OLED_RTC_using_FreeRTOS/ThirdParty/FreeRTOS/portable/GCC/ARM_CM4F" -I"C:/Users/Amal/RTOS_workspace/OLED_RTC_using_FreeRTOS/ThirdParty/FreeRTOS/portable/MemMang" -I"C:/Users/Amal/RTOS_workspace/OLED_RTC_using_FreeRTOS/ThirdParty/FreeRTOS/portable" -I"C:/Users/Amal/RTOS_workspace/OLED_RTC_using_FreeRTOS/ThirdParty/FreeRTOS/include" -I"C:/Users/Amal/RTOS_workspace/OLED_RTC_using_FreeRTOS/CMSIS/device" -I"C:/Users/Amal/RTOS_workspace/OLED_RTC_using_FreeRTOS/config" -I"C:/Users/Amal/RTOS_workspace/OLED_RTC_using_FreeRTOS/CMSIS/core" -I"C:/Users/Amal/RTOS_workspace/OLED_RTC_using_FreeRTOS/CMSIS/device" -I"C:/Users/Amal/RTOS_workspace/OLED_RTC_using_FreeRTOS/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-ThirdParty-2f-FreeRTOS-2f-portable-2f-MemMang

clean-ThirdParty-2f-FreeRTOS-2f-portable-2f-MemMang:
	-$(RM) ./ThirdParty/FreeRTOS/portable/MemMang/heap_4.cyclo ./ThirdParty/FreeRTOS/portable/MemMang/heap_4.d ./ThirdParty/FreeRTOS/portable/MemMang/heap_4.o ./ThirdParty/FreeRTOS/portable/MemMang/heap_4.su

.PHONY: clean-ThirdParty-2f-FreeRTOS-2f-portable-2f-MemMang

