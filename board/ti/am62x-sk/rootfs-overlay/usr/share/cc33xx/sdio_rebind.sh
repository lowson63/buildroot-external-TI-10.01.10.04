#!/bin/sh

CC33XX_MMC_DEVICE="481d8000.mmc"

echo  "$CC33XX_MMC_DEVICE" > /sys/bus/platform/drivers/sdhci-omap/unbind 
sleep 1
echo  "$CC33XX_MMC_DEVICE" > /sys/bus/platform/drivers/sdhci-omap/bind 

# Workaround for systemd-networkd not initializing default STA role after
# SDIO rebind (root-cause still pending).
sleep 3
systemctl restart systemd-networkd
echo "Done"