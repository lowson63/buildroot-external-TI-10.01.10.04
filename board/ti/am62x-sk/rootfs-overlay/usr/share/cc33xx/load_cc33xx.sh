#!/bin/sh

if ! mount | grep debugfs > /dev/null; then
        echo "Mount debugfs"
        mount -t debugfs none /sys/kernel/debug
fi

modprobe cc33xx
modprobe cc33xx_sdio
modprobe cc33xx_spi
