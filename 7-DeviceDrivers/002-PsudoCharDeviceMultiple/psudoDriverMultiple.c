#include <linux/module.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include "/home/alaagbr/Desktop/DeviceDriver/002-PsudoCharDeviceMultiple/psudoDriverMultiple.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("AlaaGbr");

struct file_operations pcd_fops = {
	.read = pcd_myread,
	.write = pcd_mywrite,
	.open = pcd_myopen,
	.llseek = pcd_myllseek
};

struct pcdrv_PrivateData pcdrv_data = {
	.TotalDevicesNum = No_Of_Devices,
	.pcdev_data = {
		       [0] = {
			      .buffer = pcd1_DataBuffer,
			      .size = pcd1_MemSize,
			      .perm = RDONLY},
		       [1] = {
			      .buffer = pcd2_DataBuffer,
			      .size = pcd2_MemSize,
			      .perm = WRONLY},
		       [2] = {
			      .buffer = pcd3_DataBuffer,
			      .size = pcd3_MemSize,
			      .perm = RDWR},
		       [3] = {
			      .buffer = pcd4_DataBuffer,
			      .size = pcd4_MemSize,
			      .perm = RDWR}

		       }

};

static __init int PsudoDriverInit(void)
{
	int i = 0;
	printk("%s was called\n", __FUNCTION__);

	int returnVal = 0;
	/* reserve a device number for our psudoDevice/s */
	returnVal =
	    alloc_chrdev_region(&pcdrv_data.deviceNum, base_minor,
				No_Of_Devices, "psudoDevices");
	if (returnVal < 0) {
		pr_info("couldn`t allocate region for the device\n");
		goto alloc_ERR;
	}

	/*create a file under /sys/class */

	pcdrv_data.pcd_class = class_create("pcd_class");
	if (IS_ERR(pcdrv_data.pcd_class)) {
		returnVal = PTR_ERR(pcdrv_data.pcd_class);
		pr_info("Class Creation Failed");
		goto class_ERR;
	}

	/* initialize cdev sturct with zeros then assign our pcd_fops to the cdev struct member fops */
	for (i = 0; i < No_Of_Devices; i++) {

		cdev_init(&pcdrv_data.pcdev_data[i].pcd_cdev, &pcd_fops);

		pcdrv_data.pcdev_data[i].pcd_cdev.owner = THIS_MODULE;
		/* link our reserved deviceNumber to the initialized cdev struct to let the VFS know what to use when we invoke userspace function */

		returnVal =
		    cdev_add(&pcdrv_data.pcdev_data[i].pcd_cdev,
			     pcdrv_data.deviceNum + i, 1);
		pcdrv_data.pcdev_data[i].pcd_MinorNum =
		    MINOR(pcdrv_data.deviceNum + i);
		if (returnVal < 0) {
			pr_info("couldn`t add operations to cdev struct\n");
			goto cdev_ERR;
		}

		/*populate sysfs with the device information */
		pcdrv_data.pcdev_data[i].pcd_device =
		    device_create(pcdrv_data.pcd_class, NULL,
				  pcdrv_data.deviceNum + i, NULL, "pcd#%d",
				  i + 1);
		if (IS_ERR(pcdrv_data.pcdev_data[i].pcd_device)) {
			returnVal =
			    PTR_ERR(pcdrv_data.pcdev_data[i].pcd_device);
			pr_info("Device Creation Failed\n");
			goto device_ERR;
		}

	}

	pr_info("module init was successful\n");

	return 0;

 device_ERR:
	class_destroy(pcdrv_data.pcd_class);
 class_ERR:
	for (i = 0; i < No_Of_Devices; i++) {
		cdev_del(&pcdrv_data.pcdev_data[i].pcd_cdev);
	}
 cdev_ERR:
	unregister_chrdev_region(pcdrv_data.deviceNum, 1);
 alloc_ERR:
	return returnVal;

}

static __exit void PsudoDriverExit(void)
{
	int i = 0;
	printk("%s Function was called\n", __FUNCTION__);

	for (i = 0; i < No_Of_Devices; i++) {
		device_destroy(pcdrv_data.pcd_class, pcdrv_data.deviceNum + i);
	}

	class_destroy(pcdrv_data.pcd_class);

	for (i = 0; i < No_Of_Devices; i++) {
		cdev_del(&pcdrv_data.pcdev_data[i].pcd_cdev);
	}

	unregister_chrdev_region(pcdrv_data.deviceNum, 1);

	pr_info("module Exit was successful\n");

}

module_init(PsudoDriverInit);

module_exit(PsudoDriverExit);

ssize_t pcd_myread(struct file *filp, char __user * buff, size_t count,
		   loff_t * f_pos)
{
	printk("%s Function was called\n", __FUNCTION__);

	struct pcdev_PrivateData *dev_privateData =
	    (struct pcdev_PrivateData *)filp->private_data;

	pr_info("user requested to read  %zu bytes from device's buffer\n",
		count);
/*check if count exceeds our device limit */
	if ((*f_pos + count) > dev_privateData->size)
		count = dev_privateData->size - *f_pos;

/*copy data from our device buffer to kernel space buffer*/
	if (copy_to_user(buff, dev_privateData->buffer + (*f_pos), count)) {
		return -EFAULT;
	}

/*update current f_pos for future read*/
	*f_pos += count;
	pr_info("number of bytes successfully read =%zu \n", count);
	pr_info("current file postion is %lld\n", *f_pos);

	return count;

}

ssize_t pcd_mywrite(struct file *filp, const char __user * buff, size_t count,
		    loff_t * f_pos)
{
	printk("%s Function was called\n", __FUNCTION__);

	struct pcdev_PrivateData *dev_privateData =
	    (struct pcdev_PrivateData *)filp->private_data;

	pr_info("user requested to write  %zu bytes to device's buffer\n",
		count);
/*check if count exceeds our device limit */
	if ((*f_pos + count) > dev_privateData->size)
		count = dev_privateData->size - *f_pos;

/*check if count equals zero then there is no space left to write*/
	if (!count)
		return -ENOMEM;

	if (copy_from_user(dev_privateData->buffer + (*f_pos), buff, count)) {
		return -EFAULT;
	}

	*f_pos += count;

	pr_info("number of bytes successfully Written =%zu \n", count);
	pr_info("current file postion is %lld\n", *f_pos);

	return count;
}

int pcd_myopen(struct inode *inode, struct file *filp)
{
	printk("%s Function was called\n", __FUNCTION__);

	int minor_num, ret;
	struct pcdev_PrivateData *privateData;
	/*find out which file was requested to open */

	minor_num = MINOR(inode->i_rdev);
	pr_info("minor access = %d\n", minor_num);

	privateData =
	    container_of(inode->i_cdev, struct pcdev_PrivateData, pcd_cdev);

	filp->private_data = privateData;

	ret = check_permssion(privateData->perm, filp->f_mode);

	if (!ret)
		pr_info("Open was successful\n");
	else
		pr_info("Open was unsuccessful\n");

	return ret;

}

int pcd_myrelease(struct inode *inode, struct file *filp)
{
	printk("%s Function was called\n", __FUNCTION__);

	return 0;
}

loff_t pcd_myllseek(struct file *filp, loff_t off, int)
{

	printk("%s Function was called\n", __FUNCTION__);

	return 0;
}

int check_permssion(int dev_perm, int access_mode)
{
	if (dev_perm == RDWR)
		return 0;

	/*ensure read only access */
	if ((dev_perm == RDONLY)
	    && ((access_mode & FMODE_READ) && !(access_mode & FMODE_WRITE)))
		return 0;
	/*ensure write only access */
	if ((dev_perm == WRONLY)
	    && ((access_mode & FMODE_WRITE) && !(access_mode & FMODE_READ)))
		return 0;
	return -EPERM;

}
