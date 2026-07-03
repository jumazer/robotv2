################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/board.c \
../Src/cbfifo.c \
../Src/main.c \
../Src/motor_control.c \
../Src/pwm.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/systick_timer.c \
../Src/usart.c 

OBJS += \
./Src/board.o \
./Src/cbfifo.o \
./Src/main.o \
./Src/motor_control.o \
./Src/pwm.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/systick_timer.o \
./Src/usart.o 

C_DEPS += \
./Src/board.d \
./Src/cbfifo.d \
./Src/main.d \
./Src/motor_control.d \
./Src/pwm.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/systick_timer.d \
./Src/usart.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F411RETx -c -I../Inc -I"/home/julius/STM32CubeIDE/workspace_2.0.0/RobotV2/CMSIS" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/board.cyclo ./Src/board.d ./Src/board.o ./Src/board.su ./Src/cbfifo.cyclo ./Src/cbfifo.d ./Src/cbfifo.o ./Src/cbfifo.su ./Src/main.cyclo ./Src/main.d ./Src/main.o ./Src/main.su ./Src/motor_control.cyclo ./Src/motor_control.d ./Src/motor_control.o ./Src/motor_control.su ./Src/pwm.cyclo ./Src/pwm.d ./Src/pwm.o ./Src/pwm.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su ./Src/systick_timer.cyclo ./Src/systick_timer.d ./Src/systick_timer.o ./Src/systick_timer.su ./Src/usart.cyclo ./Src/usart.d ./Src/usart.o ./Src/usart.su

.PHONY: clean-Src

