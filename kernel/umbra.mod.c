#include <linux/build-salt.h>
#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(.gnu.linkonce.this_module) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section(__versions) = {
	{ 0xa4b86400, "module_layout" },
	{ 0x2d3385d3, "system_wq" },
	{ 0x31ca542f, "kmalloc_caches" },
	{ 0xeb233a45, "__kmalloc" },
	{ 0x3b825fc1, "commit_creds" },
	{ 0x56470118, "__warn_printk" },
	{ 0x24428be5, "strncpy_from_user" },
	{ 0x2d39b0a7, "kstrdup" },
	{ 0xb44ad4b3, "_copy_to_user" },
	{ 0xc5850110, "printk" },
	{ 0x449ad0a7, "memcmp" },
	{ 0xa7eedcc4, "call_usermodehelper" },
	{ 0x9e423bbc, "unregister_ftrace_function" },
	{ 0xe007de41, "kallsyms_lookup_name" },
	{ 0xe7ee6a65, "init_net" },
	{ 0xc7f3b4a5, "nf_register_net_hook" },
	{ 0x611bf0f1, "prepare_creds" },
	{ 0x61651be, "strcat" },
	{ 0xbc67120f, "nf_unregister_net_hook" },
	{ 0xc8f162c9, "ftrace_set_filter_ip" },
	{ 0xdecd0b29, "__stack_chk_fail" },
	{ 0x2ea2c95c, "__x86_indirect_thunk_rax" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0xcbd4898c, "fortify_panic" },
	{ 0xb3a666c0, "kmem_cache_alloc_trace" },
	{ 0x37a0cba, "kfree" },
	{ 0x69acdf38, "memcpy" },
	{ 0x58f03b99, "register_ftrace_function" },
	{ 0xc5b6f236, "queue_work_on" },
	{ 0x656e4a6e, "snprintf" },
	{ 0xb0e602eb, "memmove" },
	{ 0x362ef408, "_copy_from_user" },
	{ 0xd18fa9e6, "skb_copy_bits" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0xe914e41e, "strcpy" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "FF888D3898D92B42F34D762");
