#include <linux/linkage.h>
#include <linux/kernel.h>
#include <linux/timer.h>

asmlinkage long sys_gettime(void)
{
	static const long C = 1000000000;
	struct	timespec tv;
	getnstimeofday(&tv);
	return tv.tv_sec * C + tv.tv_nsec;
}






