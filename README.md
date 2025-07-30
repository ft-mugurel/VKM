# Initial setup for debugging

1. Install the VKM project for our vurnable kernel module.
``` bash
git clone https://github.com/ft-mugurel/VKM.git
cd VKM
```

- Now we need an kernel with debugging enabled.

1. Install the linux kernel.
``` bash
wget https://cdn.kernel.org/pub/linux/kernel/v6.x/linux-6.9.tar.xz
tar xvf linux-6.9.tar.xz
mv linux-6.9 linux
cd linux
```

2. Create the default configuration file.
``` bash 
make defconfig
make kvm_guest.config
```

3. Enable the following options by adding them to the `.config` file:
``` bash
# Coverage collection.
CONFIG_KCOV=y

# Debug info for symbolization.
CONFIG_DEBUG_INFO_DWARF4=y

# Memory bug detector
CONFIG_KASAN=y
CONFIG_KASAN_INLINE=y
CONFIG_KASAN_MULTI_SHOT=y

# Required for Debian Stretch and later
CONFIG_CONFIGFS_FS=y
CONFIG_SECURITYFS=y
```

Or just run the following command:
``` bash
echo -e "\nCONFIG_KCOV=y\nCONFIG_DEBUG_INFO_DWARF4=y\nCONFIG_KASAN=y\nCONFIG_KASAN_INLINE=y\nCONFIG_KASAN_MULTI_SHOT=y\nCONFIG_CONFIGFS_FS=y\nCONFIG_SECURITYFS=y" >> .config
```

After that you need to run this command to update the configuration:
``` bash
make olddefconfig
```

4. Compile the kernel.
``` bash
make CC="gcc -std=gnu11" -j16
```

4. Create and debootstrap. First you need to install debootstrap for your distribution. Then run:
``` bash
cd ..
mkdir image
cd image
wget https://raw.githubusercontent.com/google/syzkaller/master/tools/create-image.sh -O create-image.sh
chmod +x create-image.sh
./create-image.sh
cd ..
```

# Building the kernel module and runnning inside of the qemu:
1. Build the kernel module.
``` bash
make build
```
2. Run the kernel module inside of the qemu.
``` bash
make loadmodule
```
This will upload the module in to qemu and load it into the kernel.

3. To unload the module, run:
``` bash
make unloadmodule
```

# Vulnerabilities in the VKM module
## Stack buffer overflow [x]
- Stack boffer overflow happens when the product name readed in to a fixt lengt buffer is longer than the buffer size, causing the overflow of the stack memory.
## Heap buffer overflow
- The channel count wil be readed and stored in a int variable, but there is no check for the size of the channel count, and it is not unsined int and can be negative, if the channel count is smaller then the actual channels it will couse a heap buffer overflow.
## Use after free
- When the device is connected, there will be key for security, but if the device is send reset command it will be freed, and if we don't send the key again, it will read the freed memory and couse a use after free.
## Double free
- If after reset command send we call it again it will couse a double free, because the device is already freed.
## Integer overflow
- There is no limit to channel count it will couse the integer overflow, if the channel count is bigger then the max value of the int, it will couse the integer overflow.
## Integer underflow
- There is no limit to channel count it will couse the integer underflow, if the channel count is smaller then the min value of the int, it will couse the integer underflow.

