# Introduction
 Linux device driver is a software component that enables interaction between the operating system and specific hardware devices.

 ## 1. Make the source and make file
 `touch psudoDriver.c  Makefile` 

 ## 2. include some headers in the psudoDriver.c 
```
#include <linux/module.h>
#include <linux/init.h> // to use init and exit functions for the modules 
#include <linux/moduleparam.h> // to pass arguments to modules while injecting into kernel
#include <linux/fs.h> // to use some of the filesystem operations 
````
## 3. add the type of license you want to use in your module/driver 
### in our case we are using GPL license
` MODULE_LICENSE("GPL");`

# commonly used functions
```

// gives a pre-defined MajorNum to a device and register it under /proc/devices

// returns 0 if it successfully allocated the device 

// returns -ve if it failed to allocate


register_chrdev(MajorNum,"device Name",poniter to file operation struct)

// de-allocate the device allocated with ( register_chrdev() )

 unregister_chrdev(MajorNum,1"device Name");


*****************************************************************************************

// automatically allocate a device and register it under /proc/devices

// returns 0 if it successfully allocated the device 

// returns -ve if it failed to allocate


alloc_chrdev_region(dev_t deviceNum , minor_base , number of devices , device name)

// de-allocate the device allocated with ( alloc_chrdev_region() )

unregister_chrdev_region(deviceNum,number of devices);

*****************************************************************************************
// macros that return major and minor numbers

MAJOR(deviceNum)

MINOR(deviceNum)

*****************************************************************************************
// these functions are invoked whenever insmod or rmmod commands are invoked in shell

module_init(pointer to init function);

module_exit(pointer to exit function);

*****************************************************************************************

```





