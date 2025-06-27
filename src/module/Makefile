obj-m += vulndrv.o

KDIR := ../../linux/
PWD  := $(shell pwd)
BUILD_DIR := $(PWD)/build

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
	mkdir -p $(BUILD_DIR)
	mv *.o *.ko *.mod.c Module.symvers modules.order $(BUILD_DIR)

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	rm -rf $(BUILD_DIR)

