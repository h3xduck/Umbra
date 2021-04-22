#include <asm-generic/barrier.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/syscalls.h>
#include <linux/kernel.h>
#include <linux/linkage.h>
#include <linux/version.h>
#include <linux/namei.h>

#include "ftrace_manager.h"

#if defined(CONFIG_X86_64) && (LINUX_VERSION_CODE >= KERNEL_VERSION(4,17,0))
#define USE_NEW_FORMAT 1
#endif

MODULE_LICENSE("GPL");
MODULE_AUTHOR("marsan27 marcossbajo@gmail.com");
MODULE_DESCRIPTION("LKM main");
MODULE_VERSION("1");

#ifdef USE_NEW_FORMAT
static asmlinkage long (*orig_mkdir)(const struct pt_regs *);

asmlinkage int hook_mkdir(const struct pt_regs *regs){
    char __user *pathname = (char *)regs->di;
   
    char dir_name[NAME_MAX] = {0};
    long err;

    printk(KERN_INFO "UMBRA:: mkdir hooked x64!");
    //Getting pathname from userspace, which is out of our module address space
    err = strncpy_from_user(dir_name, pathname, NAME_MAX);
    // Returns -EFAULT if error, number of copied bytes otherwise
    if (err>0){
        printk(KERN_INFO "UMBRA:: Trying to create directory with name %s\n", dir_name);
    }

    orig_mkdir(regs);
    return 0;
}

#else

static asmlinkage long (*orig_mkdir)(const char __user *pathname, umode_t mode);

asmlinkage int hook_mkdir(const char __user *pathname, umode_t mode){
    char dir_name[NAME_MAX] = {0};
    long err;

    printk(KERN_INFO "UMBRA:: mkdir hooked!");
    //Getting pathname from userspace, which is out of our module address space
    err = strncpy_from_user(dir_name, pathname, NAME_MAX);
    // Returns -EFAULT if error, number of copied bytes otherwise
    if (err>0){
        printk(KERN_INFO "UMBRA:: Trying to create directory with name %s\n", dir_name);
    }
        printk(KERN_INFO "UMBRA:: Trying to create directory with name %s\n", dir_name);

    orig_mkdir(pathname, mode);
    return 0;
}

#endif

// Syscalls to be hooked using frace
static struct ftrace_hook hooks[] = {
    HOOK("sys_mkdir", hook_mkdir, &orig_mkdir),
};



static int __init mod_init(void){
    int err = install_hooks_set(hooks, ARRAY_SIZE(hooks));
    if(err){
        return err;
    }
    printk(KERN_INFO "UMBRA:: Successfully loaded\n");
    return 0;
}

static void __exit mod_exit(void){
    remove_hooks_set(hooks, ARRAY_SIZE(hooks));
    printk(KERN_INFO "UMBRA:: Successfully unloaded\n");
}

// Registering functions for when loading/unloading kernel module
module_init(mod_init);
module_exit(mod_exit);
