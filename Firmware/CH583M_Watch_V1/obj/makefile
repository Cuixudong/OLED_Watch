################################################################################
# MRS Version: 1.9.1
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include StdPeriphDriver/subdir.mk
-include Startup/subdir.mk
-include RVMSIS/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
CH583_Watch.hex \

SECONDARY_LIST += \
CH583_Watch.lst \

SECONDARY_SIZE += \
CH583_Watch.siz \


# All Target
all: CH583_Watch.elf secondary-outputs

# Tool invocations
CH583_Watch.elf: $(OBJS) $(USER_OBJS)
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common  -g -T "../Ld/Link.ld" -nostartfiles -Xlinker --gc-sections -L"../" -L"../StdPeriphDriver" -Xlinker --print-memory-usage -Wl,-Map,"CH583_Watch.map" --specs=nano.specs --specs=nosys.specs -o "CH583_Watch.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@	@
CH583_Watch.hex: CH583_Watch.elf
	@	riscv-none-embed-objcopy -O ihex "CH583_Watch.elf"  "CH583_Watch.hex"
	@	@
CH583_Watch.lst: CH583_Watch.elf
	@	riscv-none-embed-objdump --source --all-headers --demangle --line-numbers --wide "CH583_Watch.elf" > "CH583_Watch.lst"
	@	@
CH583_Watch.siz: CH583_Watch.elf
	@	riscv-none-embed-size --format=berkeley "CH583_Watch.elf"
	@	@
# Other Targets
clean:
	-$(RM) $(ASM_UPPER_DEPS)$(OBJS)$(SECONDARY_FLASH)$(SECONDARY_LIST)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_DEPS)$(S_UPPER_DEPS)$(C_DEPS) CH583_Watch.elf
	-@
secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents

-include ../makefile.targets
