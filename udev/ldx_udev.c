/* LDX Module, using UDEV instead of Major/Minor Numbers */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "LDX_UDEV"

#define MAX_DATA_BUFFER 1024
#define SAMPLE_DATA "Hello Device!"

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("LDX, using UDEV");
MODULE_VERSION("0.1");
MODULE_AUTHOR("Milo");

/* F-Ops Functions */

static int open_device(struct inode*, struct file*);
static ssize_t read_device(struct file*, char*, size_t, loff_t*);
static ssize_t write_device(struct file*, const char*, size_t, loff_t*);
static int close_device(struct inode*, struct file*);

/* F-Ops Functions */

static dev_t device_no;
static struct cdev* driver_obj;
struct class* dev_class;

static int device_use_count = 0;
static char data_buffer[ MAX_DATA_BUFFER ];
static char* data_p;

static struct file_operations f_ops = {

    .open = open_device,
    .read = read_device,
    .write = write_device,
    .release = close_device,
    .owner = THIS_MODULE
};

static int __init init_mod(void) {

    printk(KERN_INFO "LDX_UDEV: Loading Kernel Module...\n");

    if(alloc_chrdev_region(&device_no, 0, 1, DEVICE_NAME) < 0)
        return -EIO;

    if((driver_obj = cdev_alloc()) == NULL)
        goto clean_up_devno;

    driver_obj->owner = THIS_MODULE;
    driver_obj->ops = &f_ops;

    if(cdev_add(driver_obj, device_no, 1) < 0)
        goto clean_up_cdev;

    dev_class = class_create(THIS_MODULE, DEVICE_NAME);

    if(IS_ERR(dev_class) == 1)
        goto clean_up_cdev;

    device_create(dev_class, NULL, device_no, NULL, "%s", DEVICE_NAME);

    strncpy(data_buffer, SAMPLE_DATA, MAX_DATA_BUFFER);
    data_p = data_buffer;  

    return 0;

clean_up_cdev:
    kobject_put( &driver_obj->kobj );

clean_up_devno:
    unregister_chrdev_region(device_no, 1);

    return -EIO;    
}

static void __exit exit_mod(void) {

    printk(KERN_INFO "LDX_UDEV: Kernel Module unloaded.\n");

    device_destroy(dev_class, device_no);
    class_destroy(dev_class);

    cdev_del(driver_obj);

    unregister_chrdev_region(device_no, 1);
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

static int close_device(struct inode* node, struct file* device_file) {

    if(device_use_count == 0) {

        printk(KERN_INFO "No LDX Devices avaiable to close.\n");
        return 0;
    }
	
    module_put( THIS_MODULE );
    device_use_count -= 1;

    printk(KERN_INFO "LDX Device has beed closed.\n");
	
    return 0;
}

module_init( init_mod );
module_exit( exit_mod );

