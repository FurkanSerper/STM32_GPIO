################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/spi_tx_testing.c \
../Src/syscalls.c 

OBJS += \
./Src/spi_tx_testing.o \
./Src/syscalls.o 

C_DEPS += \
./Src/spi_tx_testing.d \
./Src/syscalls.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -c -I"C:/LowLevelDriverDevelopment/Workspace_LLDD/STM32_GPIO/GPIO_DriverDevelopment/drivers/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/spi_tx_testing.cyclo ./Src/spi_tx_testing.d ./Src/spi_tx_testing.o ./Src/spi_tx_testing.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su

.PHONY: clean-Src

