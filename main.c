#include <asm-generic/barrier.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/syscalls.h>
#include <linux/kernel.h>
#include <linux/linkage.h>
#include <linux/version.h>
#include <linux/namei.h>

#include "ftrace_manager.h"



MODULE_LICENSE("GPL");
MODULE_AUTHOR("marsan27 marcossbajo@gmail.com");
MODULE_DESCRIPTION("LKM main");
MODULE_VERSION("1");

static asmlinkage long (*orig_mkdir)(const char __user *pathname, umode_t mode);

asmlinkage int hook_mkdir(const char __user *pathname, umode_t mode){
    char dir_name[NAME_MAX] = {0};

    //Getting pathname from userspace, which is out of our module address space
    long err = strncpy_from_user(dir_name, pathname, NAME_MAX);
    // Returns -EFAULT if error, number of copied bytes otherwise
    if (err>0){
        printk(KERN_INFO "UMBRA:: Trying to create directory with name %s\n", dir_name);
    }
        

    orig_mkdir(pathname, mode);
    return 0;
}

// Syscalls to be hooked using frace
static struct ftrace_hook hooks[] = {
    HOOK("sys_mkdir", hook_mkdir, &orig_mkdir),
};



static int __init example_init(void){
    int err = install_hooks_set(hooks, ARRAY_SIZE(hooks));
    if(err){
        return err;
    }
    printk(KERN_INFO "UMBRA:: Successfully loaded\n");
    return 0;
}

static void __exit example_exit(void){
    remove_hooks_set(hooks, ARRAY_SIZE(hooks));
    printk(KERN_INFO "UMBRA:: Successfully unloaded\n");
}

// Registering functions for when loading/unloading kernel module
module_init(example_init);
module_exit(example_exit);
