// stack buffer overflow [x]
// heap buffer overflow
// use after free
// double free
// integer overflow
// integer underflow
// out of bounds

#include "vulndrv.h"

char latest_data[64] = {0};
char product_name[32] = {0};
int channel_count = 0;

static struct usb_device_id arduino_table[] = {
		{ USB_DEVICE(USB_ARDUINO_VENDOR_ID, USB_ARDUINO_PRODUCT_ID) },
		{} 
};

MODULE_DEVICE_TABLE(usb, arduino_table);

static int read_data_from_device(struct usb_arduino *dev, int write_device_flag)
{
		int retval;
		int actual_length;

		retval = usb_bulk_msg(dev->udev,
												usb_rcvbulkpipe(dev->udev, dev->bulk_in_endpointAddr),
												dev->bulk_in_buffer,
												dev->bulk_in_size,
												&actual_length,
												5000);

		dev->bulk_in_buffer[actual_length - 1] = '\0';
		if (write_device_flag) {
				mutex_lock(&char_dev_mutex);
				memcpy(latest_data, dev->bulk_in_buffer, min(actual_length, DATA_BUF_SIZE));
				mutex_unlock(&char_dev_mutex);
		}
		msleep(100);
		return retval;
}

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
				printk(KERN_ERR "Vulndrv: No bulk in endpoint found\n");
				kfree(dev);
				return -ENODEV;
		}

		usb_set_intfdata(interface, dev);

		while (1) {
				retval = read_data_from_device(dev, 0);
				if (retval) {
						printk(KERN_ERR "Vulndrv: Failed to read data: %d\n", retval);
						break;
				}
				printk(KERN_INFO "Vulndrv: Received data: %s\n", dev->bulk_in_buffer);
				if (strcmp(dev->bulk_in_buffer, "done") == 0) {
						printk(KERN_INFO "Vulndrv: Device initialization complete\n");
						break;
				} else if (strcmp(dev->bulk_in_buffer, "pname") == 0) {
						// Stack buffer overflow will occur here if the product name is longer than 32 bytes
						retval = read_data_from_device(dev, 0);
						strcpy(product_name, dev->bulk_in_buffer);
						if (retval) {
								printk(KERN_ERR "Vulndrv: Failed to read data: %d\n", retval);
								break;
						}
						printk(KERN_INFO "Vulndrv: Product name %s\n", dev->bulk_in_buffer);
				} else if (strcmp(dev->bulk_in_buffer, "ccount") == 0) {
						retval = read_data_from_device(dev, 0);
						channel_count = simple_strtol(dev->bulk_in_buffer, NULL, 10); 
						if (retval) {
								printk(KERN_ERR "Vulndrv: Failed to read data: %d\n", retval);
								break;
						}
						printk(KERN_INFO "Vulndrv: Chanel count %s\n", dev->bulk_in_buffer);
				} 
		}

		while (!retval){
				retval = read_data_from_device(dev, 1);
				if (retval) {
						printk(KERN_ERR "Vulndrv: Failed to read data: %d\n", retval);
						break;
				}
		}

		printk(KERN_INFO "Vulndrv: Device (%04X:%04X) connected\n", id->idVendor, id->idProduct);
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

		printk(KERN_INFO "Vulndrv: Device disconnected\n");
}

static struct usb_driver arduino_driver = {
		.name = "vulndrv",
		.id_table = arduino_table,
		.probe = arduino_probe,
		.disconnect = arduino_disconnect,
};

module_usb_driver(arduino_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("MTU");
MODULE_DESCRIPTION("Vulnerable USB Driver");

