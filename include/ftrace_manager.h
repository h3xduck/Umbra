#ifndef HEADER_FTRACE_MANAGER
#define HEADER_FTRACE_MANAGER

//#define CONFIG_FUNCTION_TRACER 0

#include <linux/version.h>
//#include <asm/ftrace.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/linkage.h>
#include <linux/module.h>
#include <linux/ftrace.h>
#include <linux/kallsyms.h>


//If x64, we prepend a __x64_ to the syscall name since that's the format on the syscall table
#if defined(CONFIG_X86_64) && (LINUX_VERSION_CODE >= KERNEL_VERSION(4,17,0))
#define SYSCALL_NAME(name) ("__x64_" name)
#else
#define SYSCALL_NAME(name) (name)
#endif

//Simple macro to get the elements which the user will be filling to call the syscall
#define HOOK(_name, _hook, _orig){\
    .name = SYSCALL_NAME(_name),\
    .function = (_hook),        \
    .original = (_orig),        \
}

struct ftrace_hook {
        const char *name;
        void *function;
        void *original;
 
        unsigned long address;
        struct ftrace_ops ops;
}; 



/**
 * Locate address of syscall in kernel memory.
 */ 
int resolve_hook_address (struct ftrace_hook *hook);

void notrace ftrace_thunk(unsigned long ip, unsigned long parent_ip, struct ftrace_ops *ops, struct pt_regs *regs);

int install_hook(struct ftrace_hook *hook);

void remove_hook(struct ftrace_hook *hook);

int install_hooks_set(struct ftrace_hook *hooks, size_t count);

void remove_hooks_set(struct ftrace_hook *hooks, size_t count);



#endif