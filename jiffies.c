//jiffies.c
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/jiffies.h>

#define BUFFER_SIZE 128
#define PROC_NAME "jiffies"

ssize_t proc_read(struct file *file, char *buf, size_t count, loff_t *pos);

//Use struct proc_ops for modern kernels
static const struct proc_ops proc_ops = {
	.proc_read = proc_read
};

/*
static struct file_operations proc_ops = {
	.owner = THIS_MODULE,
	.read = proc_read
};
*/

//Module load initialization
static int __init proc_init(void) {
    proc_create(PROC_NAME, 0666, NULL, &proc_ops);
    return 0;
}

//Module unload exit
static void __exit proc_exit(void) {
    remove_proc_entry(PROC_NAME, NULL);
}

//Read Jiffies Function
ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos){
	int rv = 0;
	char buffer[BUFFER_SIZE];
	static int completed = 0;

	if(completed){
		completed = 0;
		return 0;
	}

	completed = 1;

	rv = sprintf(buffer, "Elapsed jiffies: %lu\n", jiffies);

	//Copies the contents of the buffer to userspace usr_buf
	if (copy_to_user(usr_buf, buffer, rv)) {	
		return -1;
	}
	
	return rv;
}

//Macros for registering module entry and exit points
module_init(proc_init);
module_exit(proc_exit);

// Module metadata
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Assignment 1 Jiffies");
MODULE_AUTHOR("Alexander Boutselis");