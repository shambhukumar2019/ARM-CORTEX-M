# one shot makefile to compile and upload code to STM32F407VGTX MCU

all: clean elf bin size
	@echo "!! bin file generate success !!"

# generate binary file and upload to MCU
burn: build_files/main.bin
	@st-flash --reset write build_files/main.bin 0x08000000
	@echo "flash write success....."

# generate binary file
bin: build_files/main.bin

build_files/main.bin: build_files/main.elf
	@echo "create binary file $@"
	@arm-none-eabi-objcopy -O binary $^ $@

# generate elf file
elf: build_files/main.elf

#  '@' in front of cmd prevents it from appearing during execution

C_OBJECTS := $(patsubst source/%.c, build_files/%.o, $(wildcard source/*.c))
OBJECTS := build_files/startup_stm32f407vgtx.o $(C_OBJECTS)


build_files/main.elf: $(OBJECTS)
	@echo 'compile and link $^'
	@arm-none-eabi-gcc -Wall -Wextra -mcpu=cortex-m4 -Og -g -T"STM32F407VGTX_FLASH.ld" -Wl,--print-memory-usage --specs=nosys.specs -Wl,-Map="build_files/main.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group -o $@ $^

build_files/startup_stm32f407vgtx.o: startup_stm32f407vgtx.s
	@arm-none-eabi-gcc -Wall -Wextra -mcpu=cortex-m4 -Og -g --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -c $< -o $@
	

build_files/%.o: source/%.c
	@arm-none-eabi-gcc -Wall -Wextra -mcpu=cortex-m4 -Og -g --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Iinclude -c $< -o $@

# get size of elf file
size: 
	@echo "calculat size of elf file...."
	@arm-none-eabi-size -B build_files/main.elf

# clean all build files
clean: 
	@echo 'clear all build files.....'
	@rm -rf build_files/*
	@echo "done"

# phony targets
.PHONY: all bin elf size burn clean
