objs	:= start.o main.o
ld 		:= arm-linux-gnueabihf-ld
gcc		:= arm-linux-gnueabihf-gcc
objcopy	:= arm-linux-gnueabihf-objcopy
objdump := arm-linux-gnueabihf-objdump

ledc.bin : $(objs)
	$(ld) -TSoc_IMX6U.lds -o ledc.elf $^
	$(objcopy) -O binary -S ledc.elf $@
	$(objdump) -D -m arm ledc.elf > ledc.dis


%.o : %.c
	$(gcc) -Wall -nostdlib -c -O2 -o $@ $<

%.o : %.S
	$(gcc) -Wall -nostdlib -c -O2 -o $@ $<

clean : 
	rm -rf *.o ledc.bin ledc.elf ledc.dis