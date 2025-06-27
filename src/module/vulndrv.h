#ifndef VULNDRV_H
#define VULNDRV_H

#include <linux/usb.h>

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

static int arduino_probe(struct usb_interface *interface, const struct usb_device_id *id);
static void arduino_disconnect(struct usb_interface *interface);

#endif // VULNDRV_H

