//seconds.c
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/ktime.h>

#define BUFFER_SIZE 128
#define PROC_NAME "seconds"

ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos);

unsigned long start_time;

// Use struct proc_ops instead of file_operations
static const struct proc_ops proc_ops = {
    .proc_read = proc_read,
};

// Initialization: create /proc/seconds and record start time
static int __init proc_init(void) {
    proc_create(PROC_NAME, 0666, NULL, &proc_ops);
    start_time = ktime_get_real_seconds();
    return 0;
}

// Cleanup: remove /proc/seconds
static void __exit proc_exit(void) {
    remove_proc_entry(PROC_NAME, NULL);
}

// Read function for /proc/seconds
ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos) {
    int rv = 0;
    char buffer[BUFFER_SIZE];
    static int completed = 0;
    unsigned long current_time = ktime_get_real_seconds();
    unsigned long elapsed_seconds = current_time - start_time;

    if (completed) {
        completed = 0;
        return 0;
    }

    completed = 1;

    rv = sprintf(buffer, "Elapsed seconds: %ld\n", elapsed_seconds);

    if (copy_to_user(usr_buf, buffer, rv)) {
        return -1;
    }

    return rv;
}

// Register init and exit
module_init(proc_init);
module_exit(proc_exit);

// Module metadata
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Assignment 1 Seconds");
MODULE_AUTHOR("Alexander Boutselis");
