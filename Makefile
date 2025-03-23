TARGET ?= startup
BOARD ?= stm32vldiscovery
CPU ?= cortex-m3

CC_PREFIX ?= arm-none-eabi
QEMU_ARM ?= qemu-system-arm

# ?= means assign value only if previously not done
SRC ?= main1.c main2.c main3.c startup.S


bin: 
	$(CC_PREFIX)-gcc -Wall -mcpu=$(CPU) -Tlinker.ld -Wl,-Map=main.map -mthumb -g -nostartfiles $(SRC) -o $(TARGET).elf
	
	$(CC_PREFIX)-objdump -D -S $(TARGET).elf > $(TARGET).lst
	$(CC_PREFIX)-readelf -a $(TARGET).elf > $(TARGET).debug
	$(CC_PREFIX)-objcopy -O binary $(TARGET).elf $(TARGET).bin
	
# -S = stop cpu at starting
# feed elf as kernel file
emulate: bin
	$(QEMU_ARM) -S -M $(BOARD) -cpu $(CPU) -nographic -kernel $(TARGET).elf -gdb tcp::1234


gdb:
	gdb-multiarch -q $(TARGET).elf -ex "target remote localhost:1234"


clean:
	rm -rf *.elf *.o *.map *.lst *.debug *.bin .gdb_history
