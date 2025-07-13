#include "vulndrv.h"

#define DEVICE_NAME "vulndrv"
#define CLASS_NAME  "vulndrv_chrdev_class"

static int major;
static struct class *cls;
static struct device *dev;

static ssize_t my_read(struct file *filp, char __user *buf, size_t len, loff_t *offset)
{
		const char *msg = "hello from kernel\n";
		size_t msg_len = strlen(msg);

		if (*offset >= msg_len)
				return 0;  // zaten okunduysa tekrar verme

		if (len > msg_len - *offset)
				len = msg_len - *offset;

		if (copy_to_user(buf, msg + *offset, len))
				return -EFAULT;

		*offset += len;
		return len;
}

static ssize_t my_write(struct file *filp, const char __user *user_buf, size_t len, loff_t *off)
{
	return 0;
}

static struct file_operations fops = {
	.read = my_read,
	.write = my_write
};

// @brief Register a character device with the kernel
int  chrdev_init(void)
{
		major = register_chrdev(0, DEVICE_NAME, &fops);
		if (major < 0)
				return major;

		cls = class_create(CLASS_NAME);
		if (IS_ERR(cls)) {
				unregister_chrdev(major, DEVICE_NAME);
				return PTR_ERR(cls);
		}

		dev = device_create(cls, NULL, MKDEV(major, 0), NULL, DEVICE_NAME);
		if (IS_ERR(dev)) {
				class_destroy(cls);
				unregister_chrdev(major, DEVICE_NAME);
				return PTR_ERR(dev);
		}
		printk(KERN_INFO "vulndrv: registered with major number %d\n", major);
		return 0;
}

int chrdev_exit(void)
{
		device_destroy(cls, MKDEV(major, 0));
		class_destroy(cls);
		unregister_chrdev(major, DEVICE_NAME);
		return 0;
}
