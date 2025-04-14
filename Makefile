obj-m += vulndrv.o

KDIR := /lib/modules/$(shell uname -r)/build
PWD  := $(shell pwd)

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

load: all
	sudo insmod vulndrv.ko
	dmesg | grep vulndrv | tail

unload:
	sudo rmmod vulndrv
	dmesg | grep vulndrv | tail

