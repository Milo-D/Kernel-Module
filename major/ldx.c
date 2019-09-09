/* Simple Char Device */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

#define DEVICE_NAME "LDX"

#define MAX_DATA_BUFFER 1024
#define SAMPLE_DATA "Hello Device!"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Milo");
MODULE_DESCRIPTION("Linux Kernel Module.");
MODULE_VERSION("0.1");

/* F-Ops Function Prototypes */

static int open_device(struct inode*, struct file*);
static ssize_t read_device(struct file*, char*, size_t, loff_t*);
static ssize_t write_device(struct file*, const char*, size_t, loff_t*);
static int release_device(struct inode*, struct file*);

/* F-Ops Function Prototypes */

static int device_use_count = 0;

static char data_buffer[ MAX_DATA_BUFFER ];
static char* data_p;

static int major;

/* Setting up Device Functions */
static struct file_operations f_ops = {

    .open = open_device,
    .read = read_device,
    .write = write_device,
    .release = release_device,
    .owner = THIS_MODULE
};

static int __init mod_load(void) {

    printk(KERN_INFO "LDX: Loading Kernel Module...\n");

    strncpy(data_buffer, SAMPLE_DATA, MAX_DATA_BUFFER);
    data_p = data_buffer;

    major = register_chrdev(0, DEVICE_NAME, &f_ops);

    if(major < 0) {

	    printk(KERN_ALERT "Could not register Char Device: %s\n", DEVICE_NAME);
	    return major;
    }

    printk(KERN_INFO "Registered Char Device %s with major number: %d\n", DEVICE_NAME, major);
    return 0;
}

static void __exit mod_unload(void) {

    unregister_chrdev(major, DEVICE_NAME);
    printk(KERN_INFO "LDX: Kernel Module unloaded.\n");
}

static int open_device(struct inode* node, struct file* device_file) {	/* Proccess is opening Device File */

    if(device_use_count > 0)
        return -EBUSY;

    try_module_get( THIS_MODULE );
    device_use_count += 1;

    printk(KERN_INFO "A LDX Device has been opened.\n");

    return 0;	
}

static ssize_t read_device(struct file* device_file, char* buffer, size_t buff_len, loff_t* offset) {

    ssize_t total_bytes_read = 0;

    if(*data_p == '\0')
	    data_p = data_buffer;

    while(buff_len != 0 && *data_p != '\0') {

        put_user(*(data_p++), buffer++);

        buff_len -= 1;
        total_bytes_read += 1;
    }

    return total_bytes_read;		
}

static ssize_t write_device(struct file* device_file, const char* buffer, size_t buff_len, loff_t* offset) {

    ssize_t total_bytes_written = 0; int i = 0;

    data_p = data_buffer;

    if(buff_len >= MAX_DATA_BUFFER)
        return -EINVAL;

    for(i = 0; i < buff_len; i++) {
	
        if(__get_user(*(data_p++), buffer++) != 0)
            return -EFAULT;
			
        total_bytes_written += 1;
    }

    *data_p = '\0';
	
    return total_bytes_written;
}

static int release_device(struct inode* node, struct file* device_file) {

    if(device_use_count == 0) {

        printk(KERN_INFO "No LDX Devices available to close.\n");
        return 0;
    }
	
    module_put( THIS_MODULE );
    device_use_count -= 1;

    printk(KERN_INFO "LDX Device has beed closed.\n");
	
    return 0;
}

module_init( mod_load );
module_exit( mod_unload );

