################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../APP/Src/\ udp_handller.c \
../APP/Src/app_callbackes.c \
../APP/Src/app_main.c \
../APP/Src/test_adc.c \
../APP/Src/test_i2c.c \
../APP/Src/test_select_ctrl.c \
../APP/Src/test_spi.c \
../APP/Src/test_timer.c \
../APP/Src/test_uart.c \
../APP/Src/tools.c 

OBJS += \
./APP/Src/\ udp_handller.o \
./APP/Src/app_callbackes.o \
./APP/Src/app_main.o \
./APP/Src/test_adc.o \
./APP/Src/test_i2c.o \
./APP/Src/test_select_ctrl.o \
./APP/Src/test_spi.o \
./APP/Src/test_timer.o \
./APP/Src/test_uart.o \
./APP/Src/tools.o 

C_DEPS += \
./APP/Src/\ udp_handller.d \
./APP/Src/app_callbackes.d \
./APP/Src/app_main.d \
./APP/Src/test_adc.d \
./APP/Src/test_i2c.d \
./APP/Src/test_select_ctrl.d \
./APP/Src/test_spi.d \
./APP/Src/test_timer.d \
./APP/Src/test_uart.d \
./APP/Src/tools.d 


# Each subdirectory must supply rules for building sources it contributes
APP/Src/\ udp_handller.o: ../APP/Src/\ udp_handller.c APP/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F756xx -c -I../APP/Inc -I../Core/Inc -I../LWIP/App -I../LWIP/Target -I../Middlewares/Third_Party/LwIP/src/include -I../Middlewares/Third_Party/LwIP/system -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I../Drivers/BSP/Components/lan8742 -I../Middlewares/Third_Party/LwIP/src/include/netif/ppp -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Middlewares/Third_Party/LwIP/src/include/lwip -I../Middlewares/Third_Party/LwIP/src/include/lwip/apps -I../Middlewares/Third_Party/LwIP/src/include/lwip/priv -I../Middlewares/Third_Party/LwIP/src/include/lwip/prot -I../Middlewares/Third_Party/LwIP/src/include/netif -I../Middlewares/Third_Party/LwIP/src/include/compat/posix -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/arpa -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/net -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/sys -I../Middlewares/Third_Party/LwIP/src/include/compat/stdc -I../Middlewares/Third_Party/LwIP/system/arch -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"APP/Src/ udp_handller.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
APP/Src/%.o APP/Src/%.su APP/Src/%.cyclo: ../APP/Src/%.c APP/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F756xx -c -I../APP/Inc -I../Core/Inc -I../LWIP/App -I../LWIP/Target -I../Middlewares/Third_Party/LwIP/src/include -I../Middlewares/Third_Party/LwIP/system -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I../Drivers/BSP/Components/lan8742 -I../Middlewares/Third_Party/LwIP/src/include/netif/ppp -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Middlewares/Third_Party/LwIP/src/include/lwip -I../Middlewares/Third_Party/LwIP/src/include/lwip/apps -I../Middlewares/Third_Party/LwIP/src/include/lwip/priv -I../Middlewares/Third_Party/LwIP/src/include/lwip/prot -I../Middlewares/Third_Party/LwIP/src/include/netif -I../Middlewares/Third_Party/LwIP/src/include/compat/posix -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/arpa -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/net -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/sys -I../Middlewares/Third_Party/LwIP/src/include/compat/stdc -I../Middlewares/Third_Party/LwIP/system/arch -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-APP-2f-Src

clean-APP-2f-Src:
	-$(RM) ./APP/Src/\ udp_handller.cyclo ./APP/Src/\ udp_handller.d ./APP/Src/\ udp_handller.o ./APP/Src/\ udp_handller.su ./APP/Src/app_callbackes.cyclo ./APP/Src/app_callbackes.d ./APP/Src/app_callbackes.o ./APP/Src/app_callbackes.su ./APP/Src/app_main.cyclo ./APP/Src/app_main.d ./APP/Src/app_main.o ./APP/Src/app_main.su ./APP/Src/test_adc.cyclo ./APP/Src/test_adc.d ./APP/Src/test_adc.o ./APP/Src/test_adc.su ./APP/Src/test_i2c.cyclo ./APP/Src/test_i2c.d ./APP/Src/test_i2c.o ./APP/Src/test_i2c.su ./APP/Src/test_select_ctrl.cyclo ./APP/Src/test_select_ctrl.d ./APP/Src/test_select_ctrl.o ./APP/Src/test_select_ctrl.su ./APP/Src/test_spi.cyclo ./APP/Src/test_spi.d ./APP/Src/test_spi.o ./APP/Src/test_spi.su ./APP/Src/test_timer.cyclo ./APP/Src/test_timer.d ./APP/Src/test_timer.o ./APP/Src/test_timer.su ./APP/Src/test_uart.cyclo ./APP/Src/test_uart.d ./APP/Src/test_uart.o ./APP/Src/test_uart.su ./APP/Src/tools.cyclo ./APP/Src/tools.d ./APP/Src/tools.o ./APP/Src/tools.su

.PHONY: clean-APP-2f-Src

