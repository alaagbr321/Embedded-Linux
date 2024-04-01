# Yocto Project
## Introduction
The Yocto Project is a Linux Foundation collaborative open source project whose goal is to produce tools and processes that enable the creation of Linux distributions for embedded and IoT software that are independent of the underlying architecture of the embedded hardware.
 
## Heads Up
* it needs atleast 50 GB of free space

## 1. Install missing dependencies
```
sudo apt-get install gawk wget git-core diffstat unzip texinfo gcc-multilib \
build-essential chrpath socat cpio python3 python3-pip python3-pexpect \
xz-utils debianutils iputils-ping python3-git python3-jinja2 libegl1-mesa libsdl1.2-dev \
pylint3 xterm
```
## 2. Download Yocto 
```
git clone -b dunfell git://git.yoctoproject.org/poky.git --depth=1
```
## 3. Download the meta layer for the Raspberrypi
```
git clone  -b dunfell https://github.com/agherzan/meta-raspberrypi.git --depth=1
```

## 4. Poky Configuration (for rpi4-64)
### 4.1 source the given script to setup the enviroment --> this will make a directory called build 

`source oe-init-build-env`

`cd conf/`

### 4.2 open the file called `local.conf` using preferred editor
`gedit local.conf &` 
### 4.3 edit variable called `MACHINE` with our machine's name
`MACHINE ??= "raspberrypi4-64" `

### 4.4 add a variable called `IMAGE_FSTYPES` to generate sd_img to be used later on

`IMAGE_FSTYPES = "tar.xz ext3 rpi-sdimg" `
### 4.5 add a variable called `ENALBE_UART` to enable usb-ttl connection if needed
`ENABLE_UART = "1"`

### 4.6 add a variable called `IMAGE_INSTALL_append ` to add any package wanted

`IMAGE_INSTALL_append = "openssh" `


## 5. add the `meta-raspberrypi` path to bblayer.conf file
![bblayers](https://github.com/alaagbr321/Embedded-Linux/assets/145403129/0a200d10-c02c-4c64-b0ad-a4959b2a55d6)

## 6. Time to build our first yocto image
* it will take some time so stay patient ~ 2-3 hours on a good machine :)
* you need a stable and good internet speed so it doesn`t fail
  
`bitbake core-image-minimal -k `



