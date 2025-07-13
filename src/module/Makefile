obj-m := vulndrv.o

vulndrv-objs := chrdev.o usbdev.o


KDIR := ../../linux/
PWD  := $(shell pwd)

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	rm -f *.o *.ko *.mod.* *.symvers *.order

load: all
	sudo insmod vulndrv.ko
	dmesg | grep vulndrv | tail

unload:
	sudo rmmod vulndrv
	dmesg | grep vulndrv | tail

