#!/bin/sh

PHY=`ls /sys/class/ieee80211/`

echo "$1" > /sys/kernel/debug/ieee80211/$PHY/cc33xx/antenna_diversity_set_rssi_threshold