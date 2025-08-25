#!/bin/sh

twt_action=$1
min_wake_duration_usec=$2
min_wake_interval_mantissa=$3
min_wake_interval_exponent=$4
max_wake_interval_mantissa=$5
max_wake_interval_exponent=$6

echo "$twt_action $min_wake_duration_usec $min_wake_interval_mantissa $min_wake_interval_exponent \
$max_wake_interval_mantissa $max_wake_interval_exponent" > /sys/kernel/debug/ieee80211/phy0/cc33xx/twt_action