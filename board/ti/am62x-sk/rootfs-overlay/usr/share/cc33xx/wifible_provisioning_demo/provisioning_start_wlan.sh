cc33xx_ip_address=$1
ping_ip_address=$2

echo "Provisioning start" > cred.txt
echo "Provisioning start" > btgatt.txt

echo "Waiting for GATT connection to start"
for i in {1..99999}
do
	sleep 1

	btgatt_server=$(grep "Write Cmd" btgatt.txt | head -n1)

	if [ "$btgatt_server" == "" ]; then
		echo "Waiting for WLAN Credentials"
        continue
	fi

	echo "Credentials Received"

	killall btgatt-server
	killall btmon
	break
done

echo "Waiting for WLAN network information"
for i in {1..99999}
do
	sleep 1

	raw_data=$(grep "Data:" cred.txt | tail -1)
	echo $raw_data
	if [ "$raw_data" == "" ]; then
		echo "Waiting for provisioning to complete"
        continue
	fi

	hex=${raw_data//Data:/}
	trimmed=$(echo $hex | xxd -r -p)
	ssid=${trimmed%,*}
	password=${trimmed#*,}
	echo "SSID: $ssid, Password: $password"
	break
done

echo "Connecting to WiFi network"

security="WPA-PSK"
if [ "$password" == "" ]; then
	security="NONE"
fi
/usr/share/cc33xx/sta_connect-ex.sh $ssid $security $password
sleep 2
if [[ "$cc33xx_ip_address" != "" ]]
then
	echo "Setting WLAN static IP to $cc33xx_ip_address"
	ifconfig wlan0 $cc33xx_ip_address netmask 255.255.255.0
fi

echo "**************************"
echo "**************************"
echo "Network Status:"
wpa_cli status
if [[ "$ping_ip_address" != "" ]]
then
	echo "*****************************"
	echo "Pinging peer $ping_ip_address"
	ping -c4 $ping_ip_address
fi
