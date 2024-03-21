#!/bin/bash
export var=$(sudo losetup -f --show --partscan ~/Desktop/Image_Building/sd_emulation/sd.img)

export DISK=$var
sudo mount ${DISK}p1 ~/Desktop/Image_Building/sd_emulation/boot/
sudo mount ${DISK}p2 ~/Desktop/Image_Building/sd_emulation/rootfs/

sudo qemu-system-arm -M vexpress-a9 -m 128M -nographic \
-kernel ~/Desktop/Image_Building/u-boot/u-boot \
-sd ~/Desktop/Image_Building/sd_emulation/sd.img \
-net tap,script= ~/Desktop/Image_Building/sd_emulation/qemu-ifup -net nic
