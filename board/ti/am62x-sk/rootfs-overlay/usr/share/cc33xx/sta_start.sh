#!/bin/sh

if ps -w | grep -v grep | grep wpa_supplicant | grep wlan0 > /dev/null
then
    echo "wpa_supplicant is already running"
    exit 0
fi

if [ ! -f /usr/share/cc33xx/wpa_supplicant.conf ] 
then
 if [ ! -f /etc/wpa_supplicant.conf ]
 then
  echo "error - no default wpa_supplicant.conf"
  exit 1
 fi
 cp /etc/wpa_supplicant.conf /usr/share/cc33xx/wpa_supplicant.conf
fi

wpa_supplicant  -e/usr/share/cc33xx/entropy.bin \
        -iwlan0 -Dnl80211 -c/usr/share/cc33xx/wpa_supplicant.conf &

