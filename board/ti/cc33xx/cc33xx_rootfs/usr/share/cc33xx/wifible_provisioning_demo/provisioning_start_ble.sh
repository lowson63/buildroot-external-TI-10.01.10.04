echo "Loading Osprey WLAN Driver"
/usr/share/cc33xx/unload_cc33xx.sh
/usr/share/cc33xx/sdio_rebind.sh
sleep 1

echo "Start WLAN STA"
/usr/share/cc33xx/sta_start.sh  > /dev/null 2>&1  &
ifconfig | grep wlan
sleep 1

echo "Enabling BLE controller"
/usr/share/cc33xx/ble_enable.sh
sleep 1

echo "Starting BLE host"
./__cc33xx_ble_adv_gatt_server.sh
