################################################################################
# MRS Version: 1.9.1
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Main.c \
../src/app.c \
../src/fifo.c \
../src/key.c \
../src/oled.c 

OBJS += \
./src/Main.o \
./src/app.o \
./src/fifo.o \
./src/key.o \
./src/oled.o 

C_DEPS += \
./src/Main.d \
./src/app.d \
./src/fifo.d \
./src/key.d \
./src/oled.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common  -g -DDEBUG=1 -I"../StdPeriphDriver/inc" -I"../RVMSIS" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

