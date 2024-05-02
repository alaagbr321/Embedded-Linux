#define base_minor                        (0U)
#define Dev_Mem_Size                       (512)

char Device_Buffer[Dev_Mem_Size];
dev_t deviceNum;


ssize_t pcd_myread (struct file *, char __user *, size_t, loff_t *);
ssize_t pcd_mywrite (struct file *, const char __user *, size_t, loff_t *);
int pcd_myopen (struct inode *, struct file *);
int pcd_myrelease (struct inode *, struct file *);
loff_t pcd_myllseek (struct file *, loff_t, int);



struct cdev pcd_cdev;
struct file_operations pcd_fops;

struct class *pcd_class;
struct device *pcd_device;





