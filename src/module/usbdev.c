#include "vulndrv.h"

static struct usb_device_id arduino_table[] = {
    { USB_DEVICE(USB_ARDUINO_VENDOR_ID, USB_ARDUINO_PRODUCT_ID) },
    {} 
};

MODULE_DEVICE_TABLE(usb, arduino_table);


static int arduino_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
    struct usb_device *udev = interface_to_usbdev(interface);
    struct usb_host_interface *iface_desc;
    struct usb_endpoint_descriptor *endpoint;
    struct usb_arduino *dev;
    int i, retval;

		chrdev_init();

    dev = kzalloc(sizeof(struct usb_arduino), GFP_KERNEL);
    if (!dev) return -ENOMEM;

    dev->udev = usb_get_dev(udev);
		dev->bulk_in_buffer = NULL;
    dev->interface = interface;

    iface_desc = interface->cur_altsetting;

    for (i = 0; i < iface_desc->desc.bNumEndpoints; ++i) {
        endpoint = &iface_desc->endpoint[i].desc;

        if (!dev->bulk_in_endpointAddr &&
            usb_endpoint_is_bulk_in(endpoint)) {

            dev->bulk_in_size = usb_endpoint_maxp(endpoint);
            dev->bulk_in_endpointAddr = endpoint->bEndpointAddress;
            dev->bulk_in_buffer = kmalloc(dev->bulk_in_size, GFP_KERNEL);
            break;
        }
    }

    if (!dev->bulk_in_endpointAddr) {
        printk(KERN_ERR "Arduino: No bulk in endpoint found\n");
        kfree(dev);
        return -ENODEV;
    }

    usb_set_intfdata(interface, dev);

    {
        int actual_length;
        retval = usb_bulk_msg(dev->udev,
                              usb_rcvbulkpipe(dev->udev, dev->bulk_in_endpointAddr),
                              dev->bulk_in_buffer,
                              dev->bulk_in_size,
                              &actual_length,
                              5000); // 5 saniye timeout

        if (retval) {
            printk(KERN_ERR "Arduino: Failed to read data: %d\n", retval);
        } else {
            dev->bulk_in_buffer[actual_length] = '\0';
            printk(KERN_INFO "Arduino: Received data: %s\n", dev->bulk_in_buffer);
        }
    }

    printk(KERN_INFO "Arduino: Device (%04X:%04X) connected\n", id->idVendor, id->idProduct);
    return 0;
}

static void arduino_disconnect(struct usb_interface *interface)
{
    struct usb_arduino *dev;

		chrdev_exit();
    dev = usb_get_intfdata(interface);
    usb_set_intfdata(interface, NULL);

    if (dev) {
        usb_put_dev(dev->udev);
        kfree(dev->bulk_in_buffer);
        kfree(dev);
    }

    printk(KERN_INFO "Arduino: Device disconnected\n");
}

static struct usb_driver arduino_driver = {
    .name = "arduino_custom",
    .id_table = arduino_table,
    .probe = arduino_probe,
    .disconnect = arduino_disconnect,
};

module_usb_driver(arduino_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("MTU");
MODULE_DESCRIPTION("Arduino serial driver example");

