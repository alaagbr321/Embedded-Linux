#include <linux/module.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/platform_device.h>
#include "DeviceSetup.h"
#include "permission.h"
#undef pr_fmt
#define pr_fmt(fmt) "[%s]: " fmt, __func__

MODULE_LICENSE("GPL");
MODULE_AUTHOR("AlaaGbr");

struct pcdev_platform_data platformDev_data[No_Of_Devices] = {
	[0] = {.size = pcd1_MemSize,.perm = RDWR,.serial_number = "PCDABC1111"},
	[1] = {.size = pcd2_MemSize,.perm = RDWR,.serial_number = "PCDABC2222"},
	[2] = {.size = pcd3_MemSize,.perm = RDWR,.serial_number = "PCDABC3333"},
	[3] = {.size = pcd4_MemSize,.perm = RDWR,.serial_number = "PCDABC4444"},
};

struct platform_device platform_dev[No_Of_Devices] = {
	[0] = {.name = "pcd_dev",
	       .id = 0,
	       .dev = {
		       .platform_data = &platformDev_data[0],
		       .release = pDevice_release}
	       },
	[1] = {.name = "pcd_dev",
	       .id = 1,
	       .dev = {
		       .platform_data = &platformDev_data[1],
		       .release = pDevice_release}
	       },
	[2] = {.name = "pcd_dev",
	       .id = 2,
	       .dev = {
		       .platform_data = &platformDev_data[2],
		       .release = pDevice_release}
	       },
	[3] = {.name = "pcd_dev",
	       .id = 3,
	       .dev = {
		       .platform_data = &platformDev_data[3],
		       .release = pDevice_release}
	       },
};

static int __init DeviceSetupInit(void)
{

	platform_device_register(&platform_dev[0]);
	platform_device_register(&platform_dev[1]);

	printk("Device Setup module loaded\n");
	return 0;
}

static void __exit DeviceSetupExit(void)
{

	platform_device_unregister(&platform_dev[0]);
	platform_device_unregister(&platform_dev[1]);

	printk("Device Setup module removed\n");

}

module_init(DeviceSetupInit);

module_exit(DeviceSetupExit);

void pDevice_release(struct device *dev)
{

	pr_info("%s function was called\n", __FUNCTION__);

}
