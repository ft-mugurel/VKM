#include <linux/module.h>
#include <linux/kernel.h>

int init_module(void)
{
	printk(KERN_INFO "Hello, Kernel World!\n");
	return 0;
}
