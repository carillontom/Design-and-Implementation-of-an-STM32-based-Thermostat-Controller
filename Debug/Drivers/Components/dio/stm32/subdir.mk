################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/Components/dio/stm32/dio.c 

OBJS += \
./Drivers/Components/dio/stm32/dio.o 

C_DEPS += \
./Drivers/Components/dio/stm32/dio.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Components/dio/stm32/%.o Drivers/Components/dio/stm32/%.su Drivers/Components/dio/stm32/%.cyclo: ../Drivers/Components/dio/stm32/%.c Drivers/Components/dio/stm32/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32F401xE -DUSE_FULL_LL_DRIVER -DHSE_VALUE=25000000 -DHSE_STARTUP_TIMEOUT=100 -DLSE_STARTUP_TIMEOUT=5000 -DLSE_VALUE=32768 -DEXTERNAL_CLOCK_VALUE=12288000 -DHSI_VALUE=16000000 -DLSI_VALUE=32000 -DVDD_VALUE=3300 -DPREFETCH_ENABLE=1 -DINSTRUCTION_CACHE_ENABLE=1 -DDATA_CACHE_ENABLE=1 -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-Components-2f-dio-2f-stm32

clean-Drivers-2f-Components-2f-dio-2f-stm32:
	-$(RM) ./Drivers/Components/dio/stm32/dio.cyclo ./Drivers/Components/dio/stm32/dio.d ./Drivers/Components/dio/stm32/dio.o ./Drivers/Components/dio/stm32/dio.su

.PHONY: clean-Drivers-2f-Components-2f-dio-2f-stm32

