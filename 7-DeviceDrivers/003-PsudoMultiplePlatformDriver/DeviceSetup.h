#ifndef      DEVICESETUP_H
#define      DEVICESETUP_H
#define No_Of_Devices						4

#define pcd1_MemSize                       (1024)
#define pcd2_MemSize                       (512)
#define pcd3_MemSize                       (1024)
#define pcd4_MemSize                       (512)

char pcd1_buffer[pcd1_MemSize];
char pcd2_buffer[pcd2_MemSize];
char pcd3_buffer[pcd3_MemSize];
char pcd4_buffer[pcd4_MemSize];
struct pcdev_platform_data {
	int size;
	int perm;
	char *serial_number;

};

struct pcdev_PrivateData {
	struct pcdev_platform_data pdata;
	dev_t dev_deviceNumber;
	struct cdev dev_cdev;
	char *buffer;

};

void pDevice_release(struct device *dev);

#endif
