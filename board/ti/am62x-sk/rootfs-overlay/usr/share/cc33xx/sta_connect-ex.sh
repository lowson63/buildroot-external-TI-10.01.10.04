#!/bin/sh

NETID=0
# I/F name can be set externally or default to WLAN0
INTERFACE_NAME=${INTERFACE_NAME:-"wlan0"} 
[ "$INTERFACE_NAME" == "wlan2" ] && DISABLE_HE=true # wlan2 is incapable of HE
WPA_CLI="wpa_cli -i $INTERFACE_NAME"

if [ $# -eq 0 ] || [ "$1" == "--help" ] || [ "$1" == "-h" ]; then
    echo ""
    echo "You must specify at least the network ssid"
    echo "format: $0 <ssid> [key_mgmt] [psk] [ieee80211w] [scan_ssid]"
    echo ""
    echo "supported key-mgmt: NONE, WPA-PSK, PMF, SAE"
    echo "supported ieee80211w (PMF):  0 = disabled (default), 1 = optional, 2 = required"
    echo "supported scan_ssid: 1 = enable unicast scan. In default is disabled"
    exit
fi

NETID=`$WPA_CLI add_network | grep -v Using | grep -v Selected`

echo "netid="$NETID
echo "========================="
echo $WPA_CLI set_network $NETID ssid \'\""$1"\"\' > /usr/share/cc33xx/temp.txt

if [ $# -gt 1 ]; then
    if [ "$2" == "WPA-PSK" ] || [ "$2" == "NONE" ] ; then
       echo $WPA_CLI set_network $NETID key_mgmt "$2" >> /usr/share/cc33xx/temp.txt
    elif [ "$2" == "PMF" ] ; then
        echo $WPA_CLI set_network $NETID key_mgmt WPA-PSK WPA-PSK-SHA256 >> /usr/share/cc33xx/temp.txt
    elif [ "$2" == "SAE" ] ; then
        echo $WPA_CLI set_network $NETID key_mgmt SAE >> /usr/share/cc33xx/temp.txt
    else
       echo "Sorry, but only WPA-PSK , NONE , PMF and SAE key_mgmt is supported"
       exit
    fi
else
    echo $WPA_CLI set_network $NETID key_mgmt NONE >> /usr/share/cc33xx/temp.txt
fi

if [ $# -gt 2 ]; then
    echo $WPA_CLI set_network $NETID psk \'\""$3"\"\' >> /usr/share/cc33xx/temp.txt
fi

if [ $# -gt 3 ]; then
    echo $WPA_CLI set_network $NETID ieee80211w "$4" >> /usr/share/cc33xx/temp.txt
    echo $WPA_CLI set_network $NETID group CCMP  >> /usr/share/cc33xx/temp.txt
    echo $WPA_CLI set_network $NETID group_mgmt AES-128-CMAC  >> /usr/share/cc33xx/temp.txt
fi

if [ $# -gt 4 ]; then
    echo "Enable unicast scan"
    echo $WPA_CLI set_network $NETID scan_ssid 1 >> /usr/share/cc33xx/temp.txt
fi

if [ "$DISABLE_HE" == true ]; then
    echo "Disabling HE"
    echo $WPA_CLI set_network $NETID disable_ht 0 >> /usr/share/cc33xx/temp.txt
    echo $WPA_CLI set_network $NETID disable_he 1 >> /usr/share/cc33xx/temp.txt
fi
echo $WPA_CLI select_network $NETID >> /usr/share/cc33xx/temp.txt
chmod 777 /usr/share/cc33xx/temp.txt
sh /usr/share/cc33xx/temp.txt
rm /usr/share/cc33xx/temp.txt

#udhcpc -i wlan0

