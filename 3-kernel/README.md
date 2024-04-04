# Linux Kernel Configuring 
The Linux kernel is the core component of the Linux operating system, responsible for managing system resources such as memory, processes, input/output (I/O), and device drivers. It is an open-source software project that was initially developed by Linus Torvalds in 1991 and is now maintained by a large community of developers around the world.

## 1. Install Linux Kernel
```
git clone --depth=1 git://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git

cd linux/

make ARCH=arm vexpress_defconfig CROSS_COMPILE=~/x-tools/arm-cortexa9_neon-linux-musleabihf/bin/arm-cortexa9_neon-linux-musleabihf-
```

## 2. Configurations for our kernel
* Enable Automate devtmps at /dev, after the kernel mounted the rootfs
* Change kernel compression to XZ
```
make menuconfig ARCH=arm vexpress_defconfig CROSS_COMPILE=~/x-tools/arm-cortexa9_neon-linux-musleabihf/bin/arm-cortexa9_neon-linux-musleabihf-
```

## 3. Build The Kernel
``` 
make  zImage modules dtbs  ARCH=arm vexpress_defconfig CROSS_COMPILE=~/x-tools/arm-cortexa9_neon-linux-musleabihf/bin/arm-cortexa9_neon-linux-musleabihf-
```
## 4. Compiling modules and store them in rootfs
`make -j4 ARCH=arm CROSS_COMPILE=~/x-tools/arm-cortexa9_neon-linux-musleabihf/bin/arm-cortexa9_neon-linux-musleabihf-
INSTALL_MOD_PATH=$HOME/rootfs modules_install
`

## 5. Copy The `zImage` and `dtb` to boot partion

```
#first run our script to mount the partions of virtual sd.img (you will find it in U-boot)

sudo cp linux/arch/arm/boot/zImage sd_emulation/boot/

sudo cp linux/arch/arm/boot/dts/arm/vexpress-v2p-ca9.dtb  sd_emulation/boot/
```

## 6. set `bootcmd` to load kernel and dtb to ram 

```
setenv bootcmd "fatload mmc 0:1 $kernel_addr_r zImage ; fatload mmc 0:1 $fdt_addr_r vexpress-v2p-ca9.dtb ; bootz $kernel_addr_r - $fdt_addr_r"

setenv bootargs console=ttyAMA0 

saveenv 
```

##  The Kernel will panic as there is no file system  



