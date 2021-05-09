#ifndef HEADER_NETFILTER_MANAGER
#define HEADER_NETFILTER_MANAGER

#include <linux/kernel.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/ip.h>
#include <linux/tcp.h>

//unsigned int nf_hookfn(unsigned int hook_num, struct sk_buff *skb, const struct net_device *in, const struct net_device *out, int (*okfn)(struct sk_buff *));

//Netfilter hook options. Newer versions require building the struct ourselves
/*struct def_nf_hook_ops {
	struct list_head list;
    nf_hookfn *hook; //Function to be called
	struct module *owner;
	u_int8_t pf;
	unsigned int hooknum;
	int priority;
};*/

int register_netfilter_hook(void);
void unregister_netfilter_hook(void);

#endif