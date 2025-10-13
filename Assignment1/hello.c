// Need to handle proc_init
// Need to display the time
// Need proc_read (and do somethinf else)
// Need proc_exit (and something else)

#include <linux/version.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>

#define BUFFER_SIZE 128

#define PROC_NAME "hello"
#define MESSAGE "Hello World\n"

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,6,0)
#define HAVE_PROC_OPS
#endif

static ssize_t proc_read(struct file *file, char *buf, size_t count, loff_t *pos);

#ifdef HAVE_PROC_OPS
static const struct proc_ops proc_ops = {
	.proc_read = proc_read;
};
#else
static const struct file_operations proc_ops = {
	.owner = THIS_MODULE,
	.read = proc_read,
}
#endif

static int proc_init(void){
	proc_create(PROC_NAME, 0, NULL, &proc_ops);
	printk(KERN_INFO "/proc/%s created\n", PROC_NAME);
	return 0;
}

static void proc_exit(void){
	remove_proc_entry(PROC_NAME, NULL);
	printf(KERN_INFO "/proc/%s removed\n", PROC_NAME);
}

static ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos) {
	int rv = 0;
	char buffer[BUFFER_SIZE];
	static int completed = 0;

	if(completed) {
		completed = 0;
		return 0;
	}

	completed = 1;

	rv = sprintf(buffer, "Hello World\n");

	copy_to_user(usr_buf, buffer, rv);
	return rv;
}

module_init(proc_init);
module_exit(proc_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Hello Module");
MODULE_AUTHOR("SGG");
