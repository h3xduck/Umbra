#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("marsan27 marcossbajo@gmail.com");
MODULE_DESCRIPTION("LKM main");
MODULE_VERSION("1");

static int __init example_init(void){
    printk(KERN_INFO "Hello, world!\n");
    return 0;
}

static void __exit example_exit(void){
    printk(KERN_INFO "Goodbye, world!\n");
}

// Registering functions for when loading/unloading kernel module
module_init(example_init);
module_exit(example_exit);
