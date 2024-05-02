#define base_minor                        (0U)
#define No_Of_Devices                      (4)

#define pcd1_MemSize                       (1024)
#define pcd2_MemSize                       (512)
#define pcd3_MemSize                       (1024)
#define pcd4_MemSize                       (512)

#define RDONLY                              0x1
#define WRONLY                              0x2
#define RDWR                                0x3

char pcd1_DataBuffer[pcd1_MemSize];
char pcd2_DataBuffer[pcd2_MemSize];
char pcd3_DataBuffer[pcd3_MemSize];
char pcd4_DataBuffer[pcd4_MemSize];

ssize_t pcd_myread(struct file *, char __user *, size_t, loff_t *);
ssize_t pcd_mywrite(struct file *, const char __user *, size_t, loff_t *);
int pcd_myopen(struct inode *, struct file *);
int pcd_myrelease(struct inode *, struct file *);
loff_t pcd_myllseek(struct file *, loff_t, int);
int check_permssion(int dev_perm, int access_mode);

struct pcdev_PrivateData {
	char *buffer;
	int size;
	struct cdev pcd_cdev;
	int perm;
	struct device *pcd_device;
	int pcd_MinorNum;
};

struct pcdrv_PrivateData {
	int TotalDevicesNum;
	dev_t deviceNum;
	struct class *pcd_class;

	struct pcdev_PrivateData pcdev_data[No_Of_Devices];

};

struct file_operations pcd_fops;
