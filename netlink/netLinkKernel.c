#include <linux/module.h>
#include <net/sock.h>
#include <linux/netlink.h>
#include <linux/skbuff.h>
#include <linux/kernel.h>   /* Needed for KERN_INFO */

MODULE_LICENSE("GPL");

MODULE_AUTHOR("Bernardo Felipeto");	// The author -- visible when you use modinfo

#define NETLINK_USER 31

struct sock *nl_sk = NULL;

static void netlink_recv_msg(struct sk_buff *skb) {

	struct nlmsghdr *nlh;

	int pid, msg_size, res;

	struct sk_buff *skb_out;

	char *msg="Sup, I'm the kernel =D I got your message but I'm not able to respond.\nIf you wish to edit this message come and change the code. I'm at line 23 at netLinkKernel.c file";


	printk(KERN_INFO "Entering: %s\n", __FUNCTION__);

	msg_size=strlen(msg);

	nlh=(struct nlmsghdr*)skb->data;

	printk(KERN_INFO "Netlink received msg payload:%s\n",(char*)nlmsg_data(nlh));

	pid = nlh->nlmsg_pid; /*pid of sending process */

	skb_out = nlmsg_new(msg_size,0);

	if(!skb_out){

		printk(KERN_ERR "Failed to allocate new skb\n");
		return;

	}

	nlh=nlmsg_put(skb_out,0,0,NLMSG_DONE,msg_size,0);  

	NETLINK_CB(skb_out).dst_group = 0; /* not in mcast group */

	strncpy(nlmsg_data(nlh),msg,msg_size);

	res=nlmsg_unicast(nl_sk,skb_out,pid);

	if(res<0){    
		printk(KERN_INFO "Error while sending bak to user\n");
	}
}


	static int __init netlink_init(void) {

		printk("Entering: %s\n",__FUNCTION__);
	//This is for 3.6 kernels and above.
		struct netlink_kernel_cfg cfg = {
			.input = netlink_recv_msg,
		};

		nl_sk = netlink_kernel_create(&init_net, NETLINK_USER, &cfg);
	//nl_sk = netlink_kernel_create(&init_net, NETLINK_USER, 0, netlink_recv_msg,NULL,THIS_MODULE);
		if(!nl_sk)
		{

			printk(KERN_ALERT "Error creating socket.\n");
			return -10;

		}

		return 0;
	}

	static void __exit netlink_exit(void) {

		printk(KERN_INFO "exiting netlink module\n");
		netlink_kernel_release(nl_sk);

	}

	module_init(netlink_init); 
	module_exit(netlink_exit);
