# BusyBox
## Introduction
  BusyBox combines tiny versions of many common UNIX utilities into a single
  small executable.  It provides minimalist replacements for most of the
  utilities you usually find in bzip2, coreutils, dhcp, diffutils, e2fsprogs,
  file, findutils, gawk, grep, inetutils, less, modutils, net-tools, procps,
  sed, shadow, sysklogd, sysvinit, tar, util-linux, and vim.  The utilities
  in BusyBox often have fewer options than their full-featured cousins;
  however, the options that are included provide the expected functionality
  and behave very much like their larger counterparts.

  ## 1. Download BusyBox
  ```
  git clone https://github.com/mirror/busybox.git --depth=1
  cd busybox/
  ```

  ## 2. Add your own configuration
  ```
  export CROSS_COMPILE=~/x-tools/arm-cortexa9_neon-linux-musleabihf/bin/arm-cortexa9_neon-linux-musleabihf-

  export ARCH=arm
  
  make menuconfig 
  ```
* Settings --> Build static binary(no shared libs)

## 3. use the `make` command to build busybox
```
make 

# generate the rootfs
make install
```

## 4. we need to create the rootfs by copying the directories inside _install to rootfs
```
# go back one step
cd ..

# create rootfs directory
mkdir rootfs

# go into the dir
cd rootfs

# copy contents of _install
cp -r ../busybox/_install/*  ./

# make the rest of directories
mkdir -p etc/ dev/ mnt/ proc/ srv/ sys/ 
```

## 5. configure inittab
### We need to setup inittab file because it's the first thing that the init process look at

```
cd /etc 

mkdir init.d

vim inittab
```
## inside the inittab write:
```
# inittab file 
::sysinit:/etc/init.d/rcS
# Start an "askfirst" shell on the console (whatever that may be)
ttyAMA0::askfirst:-/bin/sh
# Stuff to do when restarting the init process
::restart:/sbin/init
```
```
cd init.d/

vim rcS
```
## inside the rcS write:
```
#!/bin/sh
# mount a filesystem of type `proc` to /proc
mount -t proc nodev /proc
# mount a filesystem of type `sysfs` to /sys
mount -t sysfs nodev /sys
# you can create `/dev` and execute `mdev -s` if you missed the `devtmpfs` configuration  
```
```
# make it executable 
chmod +x rcS
```

## 6. run our script to start qemu from (U-boot)
`./qemu_launch_script.sh`

## 7. copy our new rootfs into sd_emulation-->rootfs
`sudo cp -r rootfs/* sd_emulation/rootfs/`



