# U-Boot
## Introduction
U-Boot is an open-source, cross-platform boot loader that provides out-of-box support for hundreds of embedded boards and many CPUs, including PowerPC, ARM, XScale, MIPS, Coldfire, NIOS, Microblaze, and x86.

## 1- Installing U-Boot
```
git clone https://github.com/u-boot/u-boot.git
cd u-boot
```
## 2- Install missing dependencies
* it will ask you for the sudo password , Enter the passord :D
```
sudo apt -y install gcc-arm-linux-gnueabihf binutils-arm-linux-gnueabihf
sudo apt-get -y install bison flex bc libssl-dev make gcc 
```

## 3- List configurations and choose what suits you
`ls configs/ |grep [your board]`
* in our case the command would be `ls configs/ |grep vex`

## 4- Use the `make` command to generate the MakeFile
`make [your Board] ARCH=[Board Architecture] CROSS_COMPILE=[your Custom Compiler path]`
* in our case the command would be
```
make vexpress_ca9x4_defconfig ARCH=arm CROSS_COMPILE=~/x-tools/arm-cortexa9_neon-linux-musleabihf/bin/arm-cortexa9_neon-linux-musleabihf-
```
## 5- Configure your own settings
```
make menuconfig ARCH=arm CROSS_COMPILE=~/x-tools/arm-cortexa9_neon-linux-musleabihf/bin/arm-cortexa9_neon-linux-musleabihf-
```
### our configuration 
* Command line interface ---> Enviroment commands ---> editenv
* Command line interface ---> Boot commands ---> bootd
* Enviroment ---> uncheck flash ---> check FAT
* Enviroment ---> Device and partition where to store the enviroment in FAT ---> (0:1)

## 6- Run `make` command to build the binary
```
make -j 6 ARCH=arm CROSS_COMPILE=~/x-tools/arm-cortexa9_neon-linux-musleabihf/bin/arm-cortexa9_neon-linux-musleabihf-
```

# SD Card Emulation 
## 1- Create directory to hold sd.img and partions
```
cd ..
mkdir sd_emulation
cd sd_emulation/
```
## 2- make the sd.img by copying 1 Gb of /dev/zero
`dd if=/dev/zero of=sd.img bs=1M count=1024`

## 3- Configure the Partition Table for the SD card
`cfdisk sd.img`
### our configuration
* New--> 200M -->type = FAT16(6)  --> bootable --> write
* New-->823M  -->type = Linux(83) -->write
* Quit
## 3.1- To emulate the sd.img file as a sd card we need to attach it to loop driver to be as a block storage
```
# attach the sd.img to be treated as block storage

sudo losetup -f --show --partscan sd.img

# Assign the Block device as global variable to be treated as MACRO

export DISK=/dev/loop<x>
```


## 3.2- Format Partition Table
```
# Formating the first partition as FAT

sudo mkfs.vfat -F 16 -n boot ${DISK}p1

# format the created partition by ext4

sudo mkfs.ext4 -L rootfs ${DISK}p2
```

## 4- Install qemu
`sudo apt install qemu-system-arm`

## 5- I created a script to attach the image and format the partion table and mount the partions you just have to edit the paths in the script
* attached to the github repo under the name of "qemu_launch_script.sh"

# Initialize TFTP protocol
```
# Download tftp protocol

sudo apt-get install tftpd-hpa

# Change the configuration of tftp

nano /etc/default/tftpd-hpa

# write inside the file

tftf_option = “--secure --create”

# Restart the protocal

systemctl restart tftpd-hpa

# Make sure the tftp protocol is running

systemctl status tftpd-hpa

# Change the file owner

cd /srv
chown tftp:tftp tftp 
```