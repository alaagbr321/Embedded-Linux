#include <linux/module.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include "/home/alaagbr/Desktop/DeviceDriver/psudoDriver/psudoDriver.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("AlaaGbr");


struct file_operations pcd_fops =
{	
.read=pcd_myread,
.write=pcd_mywrite,
.open=pcd_myopen,
.llseek=pcd_myllseek
};







static __init int PsudoDriverInit(void)
{
	printk("%s was called\n",__FUNCTION__);

	int returnVal=0;
	/* reserve a device number for our psudoDevice/s */
	returnVal=alloc_chrdev_region(&deviceNum,base_minor,1,"psudoDevice");
	if(returnVal<0)
	{
		pr_info("couldn`t allocate region for the device\n");
		goto alloc_ERR;
	}

	/* initialize cdev sturct with zeros then assign our pcd_fops to the cdev struct member fops*/
	cdev_init(&pcd_cdev,&pcd_fops);
	/* link our reserved deviceNumber to the initialized cdev struct to let the VFS know what to use when we invoke userspace function */
	pcd_cdev.owner=THIS_MODULE;
	returnVal=cdev_add(&pcd_cdev,deviceNum,1);
	if(returnVal<0){
		pr_info("couldn`t add operations to cdev struct\n");
		goto cdev_ERR;
	}


	/*create a file under /sys/class */
	pcd_class=class_create("pcd_class");
	if(IS_ERR(pcd_class)){
		returnVal=PTR_ERR(pcd_class);
		pr_info("Class Creation Failed");
		goto class_ERR;
	}


	/*populate sysfs with the device information */
	pcd_device=device_create(pcd_class,NULL,deviceNum,NULL,"pcd");
	if(IS_ERR(pcd_device)){
		returnVal=PTR_ERR(pcd_device);
		pr_info("Device Creation Failed");
		goto device_ERR;
	}


	pr_info("module init was successful\n");

return 0;

device_ERR:
class_destroy(pcd_class);
class_ERR:
cdev_del(&pcd_cdev);
cdev_ERR:
	unregister_chrdev_region(deviceNum,1);
alloc_ERR:
	return returnVal;

}

static __exit void PsudoDriverExit(void)
{

printk("%s Function was called\n",__FUNCTION__);

device_destroy(pcd_class,deviceNum);

class_destroy(pcd_class);

cdev_del(&pcd_cdev);

unregister_chrdev_region(deviceNum,1);

	

}

module_init(PsudoDriverInit);

module_exit(PsudoDriverExit);


ssize_t pcd_myread (struct file *filp , char __user *buff, size_t count, loff_t *f_pos)
{
printk("%s Function was called\n",__FUNCTION__);


pr_info ("user requested to read  %zu bytes from device's buffer\n",count);
/*check if count exceeds our device limit */
if((*f_pos + count) > Dev_Mem_Size)
	count = Dev_Mem_Size- *f_pos;

/*copy data from our device buffer to kernel space buffer*/
if(copy_to_user(buff,&Device_Buffer[*f_pos],count))
{
	return -EFAULT;
}



/*update current f_pos for future read*/
*f_pos += count; 
pr_info("number of bytes successfully read =%zu \n",count);
pr_info("current file postion is %lld\n", *f_pos);

return count;

}
ssize_t pcd_mywrite (struct file *filp, const char __user *buff, size_t count , loff_t *f_pos)
{
	printk("%s Function was called\n",__FUNCTION__);


pr_info ("user requested to write  %zu bytes to device's buffer\n",count);
/*check if count exceeds our device limit */
if((*f_pos + count) > Dev_Mem_Size)
	count = Dev_Mem_Size - *f_pos;

/*check if count equals zero then there is no space left to write*/
if(!count)	
	return -ENOMEM;

if(copy_from_user(&Device_Buffer[*f_pos],buff,count))
{
	return -EFAULT;
}

*f_pos += count;

pr_info("number of bytes successfully Written =%zu \n",count);
pr_info("current file postion is %lld\n", *f_pos);

return count;
}
int pcd_myopen (struct inode * inode, struct file * filp)
{
printk("%s Function was called\n",__FUNCTION__);

return 0;
}
int pcd_myrelease (struct inode * inode, struct file *filp)
{
printk("%s Function was called\n",__FUNCTION__);

return 0;
}

loff_t pcd_myllseek (struct file *filp, loff_t off , int)
{

printk("%s Function was called\n",__FUNCTION__);

return 0;
}



