################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Core/Startup/startup_stm32h7a3vitx.s 

OBJS += \
./Core/Startup/startup_stm32h7a3vitx.o 

S_DEPS += \
./Core/Startup/startup_stm32h7a3vitx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Startup/%.o: ../Core/Startup/%.s Core/Startup/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m7 -g3 -DDEBUG -c -I"C:/Skola/VUT/CubeSat/BP/software/thesis/Drivers" -I"C:/Skola/VUT/CubeSat/BP/software/thesis/Core/Inc" -I"C:/Skola/VUT/CubeSat/BP/software/thesis/Core/Src" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-Core-2f-Startup

clean-Core-2f-Startup:
	-$(RM) ./Core/Startup/startup_stm32h7a3vitx.d ./Core/Startup/startup_stm32h7a3vitx.o

.PHONY: clean-Core-2f-Startup

