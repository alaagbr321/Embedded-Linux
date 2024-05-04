#ifndef      MULTDRIVER_H
#define      MULTDRIVER_H

#define base_minor                        (0U)

ssize_t pcd_myread(struct file *, char __user *, size_t, loff_t *);
ssize_t pcd_mywrite(struct file *, const char __user *, size_t, loff_t *);
int pcd_myopen(struct inode *, struct file *);
int pcd_myrelease(struct inode *, struct file *);
loff_t pcd_myllseek(struct file *, loff_t, int);
int check_permssion(int dev_perm, int access_mode);
int pcd_myprobe(struct platform_device *);
int pcd_myremove(struct platform_device *);

struct pcdrv_PrivateData {
	int TotalDevicesNum;
	dev_t drv_deviceNum;
	struct class *pcd_class;
	struct device *pcd_device;
	int count;
};

struct file_operations pcd_fops;

#endif
