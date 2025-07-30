FILE ?= default.txt
obj-m += vulndrv.o
KDIR := /lib/modules/$(shell uname -r)/build
PWD  := $(shell pwd)

run_qemu: 
	sudo qemu-system-x86_64 \
		-m 2G \
		-smp 2 \
		-kernel linux/arch/x86/boot/bzImage \
		-append "console=ttyS0 root=/dev/sda earlyprintk=serial net.ifnames=0" \
		-drive file=./image/bullseye.img,format=raw \
		-net user,host=10.0.2.10,hostfwd=tcp:127.0.0.1:10021-:22 \
		-net nic,model=e1000 \
		-enable-kvm \
		-nographic \
		-pidfile vm.pid \
		-usb \
		-device usb-host,vendorid=0x1a86,productid=0x7523 \
		2>&1 | tee vm.log


build_module:
	$(MAKE) -C ./src/module/

clean_module:
	$(MAKE) clean -C ./src/module/

send_to_qemu:
	@scp -i $(shell pwd)/image/bullseye.id_rsa -P 10021 -r $(FILE) root@localhost:

loadmodule: build_module
	@scp -i $(shell pwd)/image/bullseye.id_rsa -P 10021 -r ./src/module/vulndrv.ko root@localhost:
	@ssh -i ./image/bullseye.id_rsa -p 10021 -o "StrictHostKeyChecking no" root@localhost 'insmod /root/vulndrv.ko; dmesg | grep vulndrv | tail'

unloadmodule:
	@ssh -i ./image/bullseye.id_rsa -p 10021 -o "StrictHostKeyChecking no" root@localhost 'rmmod vulndrv; dmesg | grep vulndrv | tail'

reloadmodule: unloadmodule loadmodule

ssh_qemu:
	 ssh -i ./image/bullseye.id_rsa -p 10021 -o "StrictHostKeyChecking no" root@localhost
