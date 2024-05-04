#include <linux/module.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include "MultiplePlatformDriver.h"
#include "permission.h"
#include "DeviceSetup.h"

#undef pr_fmt
#define pr_fmt(fmt) "[%s]: " fmt, __func__

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
	.count = 0,
};

struct platform_driver pcd_platform_driver = {
	.probe = pcd_myprobe,
	.remove = pcd_myremove,
	.driver = {.name = "pcd_dev"}
};

static int __init PsudoDriverInit(void)
{
	// int i = 0;
	printk("%s was called\n", __FUNCTION__);
	int returnVal = 0;

	/* reserve a device number for our psudoDevice/s  you can find it under /proc/devices */
	returnVal =
	    alloc_chrdev_region(&pcdrv_data.drv_deviceNum, base_minor,
				pcdrv_data.TotalDevicesNum, "psudoDevices");
	if (returnVal < 0) {
		pr_info("couldn`t allocate region for the device\n");
		goto alloc_ERR;
	}

	/*create a file under /sys/class */

	pcdrv_data.pcd_class = class_create("pcdClass");
	if (IS_ERR(pcdrv_data.pcd_class)) {
		returnVal = PTR_ERR(pcdrv_data.pcd_class);
		pr_info("Class Creation Failed");
		goto class_ERR;
	}

	platform_driver_register(&pcd_platform_driver);
	pr_info("Platform Driver module was loaded\n");

	return 0;
 class_ERR:
	unregister_chrdev_region(pcdrv_data.drv_deviceNum,
				 pcdrv_data.TotalDevicesNum);
 alloc_ERR:
	return returnVal;

}

static void __exit PsudoDriverExit(void)
{
	platform_driver_unregister(&pcd_platform_driver);

	class_destroy(pcdrv_data.pcd_class);

	unregister_chrdev_region(pcdrv_data.drv_deviceNum,
				 pcdrv_data.TotalDevicesNum);

	pr_info("Platform Driver module was removed\n");
}

module_init(PsudoDriverInit);

module_exit(PsudoDriverExit);

ssize_t pcd_myread(struct file *filp, char __user * buff, size_t count,
		   loff_t * f_pos)
{

	int max_size;
	struct pcdev_PrivateData *pcdev_data =
	    (struct pcdev_PrivateData *)filp->private_data;
	max_size = pcdev_data->pdata.size;

	pr_info("Read requested for %zu bytes \n", count);
	pr_info("Current file position = %lld\n", *f_pos);

	/* Ajust the count argument */
	if ((*f_pos + count) > max_size)
		count = max_size - *f_pos;

	if (copy_to_user(buff, pcdev_data->buffer, count))
		return -EFAULT;

	/* Update current file position */
	*f_pos += count;
	pr_info("Number of bytes successfully read = %zu\n", count);
	pr_info("Updated file position = %lld\n", *f_pos);

	return count;

}

ssize_t pcd_mywrite(struct file *filp, const char __user * buff, size_t count,
		    loff_t * f_pos)
{
	int max_size;
    struct pcdev_PrivateData *pcdev_data = (struct pcdev_PrivateData *)filp->private_data;
    max_size = pcdev_data->pdata.size;

    pr_info("Write requested %zu bytes \n",count);
    pr_info("Current file position = %lld\n",*f_pos);

    /* Ajust the count argument */
    if ((*f_pos + count) > max_size)
        count = max_size - *f_pos;

    if (!count)
        return -ENOMEM;

    if (copy_from_user(pcdev_data->buffer, buff, count))
        return -EFAULT;

    /* Update current file position */
    *f_pos += count;
    pr_info("Number of bytes successfully written = %zu\n", count);
    pr_info("Updated file position = %lld\n",*f_pos);

    return count;
}

int pcd_myopen(struct inode *inode, struct file *filp)
{
 int minor_no, ret;
    struct pcdev_PrivateData *pcdev_data;

    /* Find out on on which device file open was attempted by user space */
    minor_no = MINOR(inode->i_rdev);

    /* Get device's private data structure */
    pcdev_data = container_of(inode->i_cdev, struct pcdev_PrivateData, dev_cdev);

    /* Supply device private data to other method of the driver */
    filp->private_data = pcdev_data;

    /* Check permission */
    ret = check_permssion(pcdev_data->pdata.perm, filp->f_mode);
    if (!ret)
        pr_info("Open was successful\n");
    else
        pr_info("Open was unsuccessful\n");

    return ret;

}

int pcd_myrelease(struct inode *inode, struct file *filp)
{

	return 0;
}

loff_t pcd_myllseek(struct file *filp, loff_t off, int)
{

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

int pcd_myprobe(struct platform_device *pdev)
{
	int ret;

	pr_info("device detected \n");
	/*Get Platform Data */
	struct pcdev_PrivateData *dev_data;
	struct pcdev_platform_data *pdata;
	pdata = (struct pcdev_platform_data *)dev_get_platdata(&pdev->dev);
	if (!pdata) {
		pr_info("No platform data\n");
		return -EINVAL;

	}

	/*Dynamically allocate memory for each device */

	dev_data = devm_kzalloc(&pdev->dev, sizeof(*dev_data), GFP_KERNEL);
	if (!dev_data) {
		pr_info("Cannot allocate memory\n");
		return -ENOMEM;
	}

	/*save device private data in pdev to use in remove */
	dev_set_drvdata(&pdev->dev, dev_data);
	dev_data->pdata.size = pdata->size;
	dev_data->pdata.perm = pdata->perm;
	dev_data->pdata.serial_number = pdata->serial_number;

	pr_info("Device size %d\n", dev_data->pdata.size);
	pr_info("Device permission %d\n", dev_data->pdata.perm);
	pr_info("Device serial number %s\n", dev_data->pdata.serial_number);

	/*Dynamically allocate each device`s buffer */
	dev_data->buffer =
	    devm_kzalloc(&pdev->dev, dev_data->pdata.size, GFP_KERNEL);
	if (!dev_data->buffer) {
		pr_info("Cannot allocate memory\n");
		return -ENOMEM;
	}

	/*Get the device number */
	dev_data->dev_deviceNumber = pcdrv_data.drv_deviceNum + pdev->id;

	/*do cdev init */
	cdev_init(&dev_data->dev_cdev, &pcd_fops);
	dev_data->dev_cdev.owner = THIS_MODULE;

	ret = cdev_add(&dev_data->dev_cdev, dev_data->dev_deviceNumber, 1);
	if (ret < 0) {
		pr_info("Cdev add failed\n");
		return ret;
	}
	/*create device file for the detected platform device */
	pcdrv_data.pcd_device =
	    device_create(pcdrv_data.pcd_class, NULL,
			  dev_data->dev_deviceNumber, NULL, "pcdev-%d",
			  pdev->id);
	if (IS_ERR(pcdrv_data.pcd_device)) {
		ret = PTR_ERR(pcdrv_data.pcd_device);
		cdev_del(&dev_data->dev_cdev);
		return ret;
	}

	pcdrv_data.count++;

	pr_info("probing was successfull \n");

	pr_info("total probed devices = %d\n", pcdrv_data.count);
	return 0;
}

int pcd_myremove(struct platform_device *pdev)
{

	struct pcdev_PrivateData *dev_data = dev_get_drvdata(&pdev->dev);

	device_destroy(pcdrv_data.pcd_class, dev_data->dev_deviceNumber);
	cdev_del(&dev_data->dev_cdev);

	pcdrv_data.count--;
	pr_info("device removed \n");
	pr_info("total available devices = %d\n", pcdrv_data.count);

	return 0;
}
