#!/bin/sh

########## variables ##########

WLAN=wlan1
DHCP_CONF=ud[h]cpd.conf

########## body ##########

echo "Terminating DHCP"
if [ -d /sys/class/net/$WLAN1 ]
then 
  output=`ps | grep $DHCP_CONF`
  set -- $output
  if [ -n "$output" ]; then
    kill $1
  fi
fi

echo "Terminating hostapd"
killall hostapd
iw $WLAN del
