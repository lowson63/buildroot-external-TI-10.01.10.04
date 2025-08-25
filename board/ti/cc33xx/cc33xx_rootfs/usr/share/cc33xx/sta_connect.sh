#!/bin/sh

# Set default params
ssid=""
security_type="NONE"
sec_key=""
pmf="" 
unicast=0  #disabled
intf_name="wlan0" #default
# Enterprise params
eap_method="TLS" #default (ONLY)
identity=""
priv_key_passwd=""
client_cert="" # for example cert.pem
priv_key=""    # for example key.pem
disable_he="0"  #default HE enabled


# globals
NETID=0


print_help() {
    echo "Usage: $0 [OPTIONS]"
    echo "Options:"
    echo "  -h,   --help       Display this help"
    echo "  -n,   --network    Network name (aka SSID) of the AP (required)"
    echo "  -s,   --sec        Security Type: NONE, WPA-PSK, SAE, WPA-EAP, WPA-EAP-SHA256"
    echo "  -p,   --password   Security Key passphrase"
    echo "  -f,   --pmf        PMF: 0 - disabled, 1 - optional, 2 - required"
    echo "  -u,   --unicast    Scan unicast or broadcast: 0 - broadcast (default), 1 - unicast"
    echo "  -i,   --intf       Interface name: wlan0 (default), wlan1"
    echo "  -d,                Disable HE: 0 - default, 1 - disable"
    echo "  ------------------------------------------------------------- "
    echo "  Following options for enterprise only (WPA-EAP/WPA-EAP-SHA256)"
    echo "  ------------------------------------------------------------- "
    echo "  -c    Path to client certificate file"
    echo "  -K    Path to private key cert file"
    echo "  -I    Identity"
    echo "  -P    Private key password"
    exit 0
}

validate_params(){
    # SSID argument is required
    if [[ -z $ssid ]]; then
        echo "SSID is required"
        exit
    fi

    # validate security types
    if [[ "$security_type" != "NONE" && "$security_type" != "WPA-PSK" && "$security_type" != "SAE" && "$security_type" != "WPA-EAP" && "$security_type" != "WPA-EAP-SHA256" ]]; then
        echo "Security type supported: NONE, WPA-PSK, SAE, WPA-EAP, WPA-EAP-SHA256"
        exit 1
    fi

    if [[ "$security_type" == "SAE" || "$security_type" == "WPA-EAP-SHA256" ]]; then
        $pmf="2"
    fi

    if [[ -z $sec_key ]]; then
        if [[ "$security_type" != "NONE" ]]; then
            echo "WARNING: Security password key is required for given security type!"
            exit 1
        fi
    fi

    if [[ "$security_type" == "WPA-EAP" ||  "$security_type" == "WPA-EAP-SHA256" ]]; then
        if [[ -z client_cert || -z priv_key ]]; then
            echo "Missing client cert or key files for entherprise security"
            exit 1
        fi
        if [[ -z identity || -z priv_key_passwd ]]; then
            echo "Missing identity or private key password"
            exit 1
        fi 

    fi

    #valid PMF options
    if [[ -n $pmf ]]; then
        if [[ $pmf != "0" && $pmf != "1" && $pmf != "2" ]]; then
            echo "PMF: Options must be 0, 1 or 2"
            exit 1
        fi
    fi

    #validate interface name
    if [[ $intf_name != "wlan0" && $intf_name != "wlan1" ]]; then
        echo "Invalid interface name"
        exit 1
    fi



}

while getopts ":hn:s:p:f:u:i:c:K:I:P:d:" opt; do
    case ${opt} in
        h)
            print_help
            ;;
        n)
            ssid="$OPTARG"
            ;;
        s)
            security_type="$OPTARG"
            ;;
        p)
            sec_key="$OPTARG"
            ;;
        f)
            pmf="$OPTARG"
            ;;
        u)
            unicast="$OPTARG"
            ;;
        i)
            intf_name="$OPTARG"
            ;;
        c)
            client_cert="$OPTARG"
            ;;
        K)
            priv_key="$OPTARG"
            ;;
        I)
            identity="$OPTARG"
            ;;
        P)
            priv_key_passwd="$OPTARG"
            ;;  
        d)
            disable_he="$OPTARG"
            ;;
        \?)
            echo "Invalid option: -$OPTARG" >&2
            print_help
            ;;
    esac
done

validate_params

echo "Connect to SSID: $ssid, sec type: $security_type, key: $sec_key, pmf: $pmf"

WPA_CLI="wpa_cli -i $intf_name" 
NETID=`$WPA_CLI add_network | grep -v Using | grep -v Selected`

echo "netid="$NETID
echo "=============================================="
echo $WPA_CLI set_network $NETID ssid \'\""$ssid"\"\' > /usr/share/cc33xx/temp.txt


if [[ "$security_type" == "WPA-PSK" || "$security_type" == "NONE" ]] ; then
    echo $WPA_CLI set_network $NETID key_mgmt "$security_type" >> /usr/share/cc33xx/temp.txt
elif [[ "$security_type" == "SAE" ]] ; then
    echo $WPA_CLI set_network $NETID key_mgmt SAE >> /usr/share/cc33xx/temp.txt
elif [[ "$security_type" == "WPA-EAP" ]] ; then
    echo $WPA_CLI set_network $NETID key_mgmt WPA-EAP >> /usr/share/cc33xx/temp.txt
elif [[ "$security_type" == "WPA-EAP-SHA256" ]] ; then
    echo $WPA_CLI set_network $NETID key_mgmt WPA-EAP-SHA256 >> /usr/share/cc33xx/temp.txt
else
    echo "Default security is NONE"
    echo $WPA_CLI set_network $NETID auth_alg OPEN >> /usr/share/cc33xx/temp.txt
    echo $WPA_CLI set_network $NETID key_mgmt NONE >> /usr/share/cc33xx/temp.txt
fi

if [[ -n $sec_key ]]; then
    echo "Set password: $sec_key"
    if [[ "$security_type" == "WPA-EAP" || "$security_type" == "WPA-EAP-SHA256" ]] ; then
        echo $WPA_CLI set_network $NETID password \'\""$sec_key"\"\' >> /usr/share/cc33xx/temp.txt
    else
        echo $WPA_CLI set_network $NETID psk \'\""$sec_key"\"\' >> /usr/share/cc33xx/temp.txt
    fi
fi

if [[ -n $pmf ]]; then
    echo "Set pmf: $pmf"
    if [[ $pmf == "0" ]]; then
        echo $WPA_CLI set_network $NETID ieee80211w "$pmf" >> /usr/share/cc33xx/temp.txt
    else
        echo $WPA_CLI set_network $NETID ieee80211w "$pmf" >> /usr/share/cc33xx/temp.txt
        echo $WPA_CLI set_network $NETID group CCMP  >> /usr/share/cc33xx/temp.txt
        echo $WPA_CLI set_network $NETID group_mgmt AES-128-CMAC  >> /usr/share/cc33xx/temp.txt
    fi

    if [[ "$security_type" == "WPA-PSK" ]]; then
        echo $WPA_CLI set_network $NETID key_mgmt WPA-PSK WPA-PSK-SHA256 >> /usr/share/cc33xx/temp.txt
    fi
fi

if [[ "$security_type" == "WPA-EAP" ||  "$security_type" == "WPA-EAP-SHA256" ]]; then
    echo "Setting enterprise security"
    echo $WPA_CLI set_network $NETID auth_alg OPEN >> /usr/share/cc33xx/temp.txt
    echo $WPA_CLI set_network $NETID pairwise CCMP >> /usr/share/cc33xx/temp.txt
    echo $WPA_CLI set_network $NETID group CCMP >> /usr/share/cc33xx/temp.txt
    echo $WPA_CLI set_network $NETID proto RSN >> /usr/share/cc33xx/temp.txt
    echo $WPA_CLI set_network $NETID eap "$eap_method" >> /usr/share/cc33xx/temp.txt
    echo $WPA_CLI set_network $NETID identity \'\""$identity"\"\' >> /usr/share/cc33xx/temp.txt
    echo $WPA_CLI set_network $NETID private_key_passwd \'\""$priv_key_passwd"\"\' >> /usr/share/cc33xx/temp.txt
    echo $WPA_CLI set_network $NETID client_cert \'\""$client_cert"\"\' >> /usr/share/cc33xx/temp.txt
    echo $WPA_CLI set_network $NETID private_key \'\""$priv_key"\"\' >> /usr/share/cc33xx/temp.txt
fi

if [ $unicast != 0 ]; then
    echo "Enable unicast scan"
    echo $WPA_CLI set_network $NETID scan_ssid 1 >> /usr/share/cc33xx/temp.txt
fi

if [[ $disable_he == "1" ]]; then
    echo "Disabling HE"
    echo $WPA_CLI set_network $NETID disable_ht 0 >> /usr/share/cc33xx/temp.txt
    echo $WPA_CLI set_network $NETID disable_he 1 >> /usr/share/cc33xx/temp.txt
fi

echo $WPA_CLI select_network $NETID >> /usr/share/cc33xx/temp.txt
chmod 777 /usr/share/cc33xx/temp.txt
sh /usr/share/cc33xx/temp.txt
rm /usr/share/cc33xx/temp.txt