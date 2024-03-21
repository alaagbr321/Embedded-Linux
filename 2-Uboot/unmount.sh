#!/bin/bash
sudo umount  ~/Desktop/Image_Building/sd_emulation/boot/
sudo umount  ~/Desktop/Image_Building/sd_emulation/rootfs/

sudo losetup -d $var
