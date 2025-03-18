TARGET ?= startup
BOARD ?= stm32vldiscovery
CPU ?= cortex-m3

CC_PREFIX = arm-none-eabi
QEMU_SYS = qemu-system-arm

qemu: startup.s
	$(CC_PREFIX)-as -mcpu=$(CPU) -mthumb -g --gstabs+ -c $^ -o $(TARGET).o
	$(CC_PREFIX)-ld -Tlinker.ld $(TARGET).o -o $(TARGET).elf
	$(CC_PREFIX)-objdump -D -S $(TARGET).elf > $(TARGET).lst
	$(CC_PREFIX)-readelf -a $(TARGET).elf > $(TARGET).debug
	$(CC_PREFIX)-objcopy -O binary $(TARGET).elf $(TARGET).bin
	$(QEMU_SYS) -S -M $(BOARD) -cpu $(CPU) -nographic -kernel $(TARGET).elf -gdb tcp::1234


gdb:
	gdb-multiarch -q $(TARGET).elf -ex "target remote localhost:1234"


clean:
	rm -rf *.elf *.o *.map *.lst *.debug *.bin