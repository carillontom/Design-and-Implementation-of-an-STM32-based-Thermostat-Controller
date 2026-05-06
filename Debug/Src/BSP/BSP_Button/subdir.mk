################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/BSP/BSP_Button/bsp_button.c 

OBJS += \
./Src/BSP/BSP_Button/bsp_button.o 

C_DEPS += \
./Src/BSP/BSP_Button/bsp_button.d 


# Each subdirectory must supply rules for building sources it contributes
Src/BSP/BSP_Button/%.o Src/BSP/BSP_Button/%.su Src/BSP/BSP_Button/%.cyclo: ../Src/BSP/BSP_Button/%.c Src/BSP/BSP_Button/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32F401xE -DUSE_FULL_LL_DRIVER -DHSE_VALUE=25000000 -DHSE_STARTUP_TIMEOUT=100 -DLSE_STARTUP_TIMEOUT=5000 -DLSE_VALUE=32768 -DEXTERNAL_CLOCK_VALUE=12288000 -DHSI_VALUE=16000000 -DLSI_VALUE=32000 -DVDD_VALUE=3300 -DPREFETCH_ENABLE=1 -DINSTRUCTION_CACHE_ENABLE=1 -DDATA_CACHE_ENABLE=1 -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Admin/OneDrive/252/Embedded System Programming/btl/Thermostat/Src" -I"C:/Users/Admin/OneDrive/252/Embedded System Programming/btl/Thermostat/Src/Drivers" -I"C:/Users/Admin/OneDrive/252/Embedded System Programming/btl/Thermostat/Src/Drivers/Components" -I"C:/Users/Admin/OneDrive/252/Embedded System Programming/btl/Thermostat/Src/Drivers/Components/dio" -I"C:/Users/Admin/OneDrive/252/Embedded System Programming/btl/Thermostat/Src/Drivers/Components/systick" -I"C:/Users/Admin/OneDrive/252/Embedded System Programming/btl/Thermostat/Src/Drivers/Components/delay" -I"C:/Users/Admin/OneDrive/252/Embedded System Programming/btl/Thermostat/Src/BSP/BSP_Button" -I"C:/Users/Admin/OneDrive/252/Embedded System Programming/btl/Thermostat/Src/BSP/BSP_LCD" -I"C:/Users/Admin/OneDrive/252/Embedded System Programming/btl/Thermostat/Src/BSP/BSP_DS18B20" -I"C:/Users/Admin/OneDrive/252/Embedded System Programming/btl/Thermostat/Src/App" -I"C:/Users/Admin/OneDrive/252/Embedded System Programming/btl/Thermostat/Src/Drivers/Components/dio/stm32" -I"C:/Users/Admin/OneDrive/252/Embedded System Programming/btl/Thermostat/Src/BSP" -I"C:/Users/Admin/OneDrive/252/Embedded System Programming/btl/Thermostat/Src/BSP/BSP_Fan" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src-2f-BSP-2f-BSP_Button

clean-Src-2f-BSP-2f-BSP_Button:
	-$(RM) ./Src/BSP/BSP_Button/bsp_button.cyclo ./Src/BSP/BSP_Button/bsp_button.d ./Src/BSP/BSP_Button/bsp_button.o ./Src/BSP/BSP_Button/bsp_button.su

.PHONY: clean-Src-2f-BSP-2f-BSP_Button

