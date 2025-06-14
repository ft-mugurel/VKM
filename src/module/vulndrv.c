#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>

int param = 0;

module_param(param, int, S_IRUSR | S_IWUSR);

static int __init vuln_init(void)
{
		printk(KERN_INFO "vulndrv: Module loaded!\n");
		return 0;
}

static void __exit vuln_exit(void)
{
		printk(KERN_INFO "vulndrv: Module unloaded!\n");
}

module_init(vuln_init);
module_exit(vuln_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("MTU");
MODULE_DESCRIPTION("KM");
