CROSS_COMPILE 	?= arm-linux-gnueabihf-
TARGET		  	?= ICM20608_SPI

CC 				:= $(CROSS_COMPILE)gcc
LD				:= $(CROSS_COMPILE)ld
OBJCOPY 		:= $(CROSS_COMPILE)objcopy
OBJDUMP 		:= $(CROSS_COMPILE)objdump

LIBPATH			:= -lgcc -L /usr/arm/gcc-linaro-14.0.0-2023.06-x86_64_arm-linux-gnueabihf/lib/gcc/arm-linux-gnueabihf/14.0.0

INCDIRS 		:= imx6u	 	\
				   bsp/clk 		\
				   bsp/led 		\
				   bsp/beep		\
				   bsp/key		\
				   bsp/gpio		\
				   bsp/interrupt\
				   bsp/exti		\
				   bsp/epit		\
				   bsp/keyFilter\
				   bsp/uart		\
				   bsp/delay 	\
				   bsp/lcd		\
				   bsp/RTC		\
				   bsp/I2C		\
				   bsp/ap3216c	\
				   bsp/icm20608	\
				   bsp/SPI		\
				   stdio/include\
				   			   
SRCDIRS			:= project 		\
				   bsp/clk 		\
				   bsp/led 		\
				   bsp/beep		\
				   bsp/key		\
				   bsp/gpio		\
				   bsp/interrupt\
				   bsp/exti		\
				   bsp/epit		\
				   bsp/keyFilter\
				   bsp/uart		\
				   bsp/delay 	\
				   bsp/lcd		\
				   bsp/RTC		\
				   bsp/I2C		\
				   bsp/ap3216c	\
				   bsp/icm20608	\
				   bsp/SPI		\
				   stdio/lib
				   
				   
INCLUDE			:= $(patsubst %, -I %, $(INCDIRS))

SFILES			:= $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.S))
CFILES			:= $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.c))

SFILENDIR		:= $(notdir  $(SFILES))
CFILENDIR		:= $(notdir  $(CFILES))

SOBJS			:= $(patsubst %, obj/%, $(SFILENDIR:.S=.o))
COBJS			:= $(patsubst %, obj/%, $(CFILENDIR:.c=.o))
OBJS			:= $(SOBJS) $(COBJS)

VPATH			:= $(SRCDIRS)

.PHONY: clean
	
$(TARGET).bin : $(OBJS)
	$(LD) -TSoc_IMX6U.lds -o $(TARGET).elf $^ $(LIBPATH)
	$(OBJCOPY) -O binary -S $(TARGET).elf $@
	$(OBJDUMP) -D -m arm $(TARGET).elf > $(TARGET).dis

$(SOBJS) : obj/%.o : %.S
	$(CC) -Wall -nostdlib -fno-builtin -c -O2  $(INCLUDE) -o $@ $<

$(COBJS) : obj/%.o : %.c
	$(CC) -Wall -march=armv7-a -mfpu=neon-vfpv4 -mfloat-abi=hard -Wa,-mimplicit-it=thumb -nostdlib -fno-builtin -c -O2  $(INCLUDE) -o $@ $<
	
clean:
	rm -rf $(TARGET).elf $(TARGET).dis $(TARGET).bin $(COBJS) $(SOBJS)

	