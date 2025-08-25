Embedded Linux development with Buildroot
## U-boot
- use "make uboot-rebuild" command in /media/buildroot-2024.05.3 to build U-boot
## Busybox
- use "make busybox-rebuild" command in /media/buildroot-2024.05.3 to build Busybox
## linux-10.01.10
- use "make linux-rebuild" command in /media/buildroot-2024.05.3 to build Kernel
- location is "/media/buildroot-2024.05.3/output/build/linux-10.01.10/
## Buildroot
- folder location is "/media/buildroot-2024.05.3"
- use "make TI_K3_BOOT_FIRMWARE_VERSION=10.01.10" in /media/buildroot-2024.05.3 to build buildroot image
## others programs or application
- toolchain is "/media/buildroot-2024.05.3/output/host"
- compiler is "/media/buildroot-2024.05.3/output/host/bin/aarch64-linux-gcc"
- include path is "/media/buildroot-2024.05.3/output/target/usr/include"
- library path is "/media/buildroot-2024.05.3/output/target/usr/lib" and "/media/buildroot-2024.05.3/output/target/lib"
