//334
#include <linux/linkage.h>
#include <linux/kernel.h>

asmlinkage void sys_printk(int pid, long start, long end)
{
	static const long C = 1000000000;
	printk(KERN_INFO "[Project1] %d %ld.%09ld %ld.%09ld\n", pid, start/C, start%C, end/C, end%C);
	return ;
}



