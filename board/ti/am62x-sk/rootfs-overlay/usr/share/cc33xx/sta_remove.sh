#!/bin/sh

WLAN_IF=wlan1

echo "removing station interface $WLAN_IF"
wpa_cli -i$WLAN_IF terminate
iw dev $WLAN_IF del

