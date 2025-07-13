#ifndef VULNDRV_H
# define VULNDRV_H

# include <linux/module.h>
# include <linux/kernel.h>

// For USB device handling

# include <linux/usb.h>

#define USB_ARDUINO_VENDOR_ID  0x1a86
#define USB_ARDUINO_PRODUCT_ID 0x7523
#define BULK_BUFFER_SIZE 64

struct usb_arduino {
		struct usb_device *udev;
		struct usb_interface *interface;
		unsigned char *bulk_in_buffer;
		size_t bulk_in_size;
		__u8 bulk_in_endpointAddr;
};

// For Character device handling

# include <linux/fs.h>
# include <linux/device.h>
# include <linux/uaccess.h>

int  chrdev_init(void);
int chrdev_exit(void);

#endif // VULNDRV_H

