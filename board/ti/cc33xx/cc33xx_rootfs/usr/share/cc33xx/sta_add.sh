#!/bin/sh

IW=/system/bin/iw
PHY=`ls /sys/class/ieee80211/`
SUPPLICANT_CONF=/usr/share/cc33xx/wpa_supplicant2.conf
WLAN_IF=wlan1

if ps -w | grep -v grep | grep wpa_supplicant | grep $WLAN_IF > /dev/null
then
    echo "wpa_supplicant is already running"
    exit 0
fi


echo "adding interface wlan1"
iw phy $PHY interface add $WLAN_IF type managed

wpa_supplicant -c$SUPPLICANT_CONF -i$WLAN_IF -Dnl80211 &