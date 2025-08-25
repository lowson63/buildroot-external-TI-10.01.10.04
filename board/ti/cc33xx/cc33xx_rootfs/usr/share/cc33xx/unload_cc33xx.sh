#!/bin/sh

# Set default param
mac_cfg_modules="0"

print_help() {
    echo "Usage: $0 [OPTIONS]"
    echo "Options:"
    echo "  -h,   --help       Display this help"
    echo "  -r,                Remove mac80211 and cfg80211 modules"
    exit 0
}

while getopts ":hr" opt; do
    case ${opt} in
        h)
            print_help
            ;;
        r)
            mac_cfg_modules="1"
            ;;
        \?)
            echo "Invalid option: -$OPTARG" >&2
            print_help
            ;;
    esac
done

killall wpa_supplicant
killall hostapd

rmmod cc33xx_sdio
rmmod cc33xx_spi
rmmod cc33xx
if [[ $mac_cfg_modules != "0" ]]; then
        rmmod mac80211
        rmmod cfg80211
fi
rmmod compat
rmmod btti_sdio
rmmod btti


