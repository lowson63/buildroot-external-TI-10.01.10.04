#./bt_agent.exp DisplayOnly &
#./bt_agent.exp DisplayYesNo &
#./bt_agent.exp KeyboardOnly &
#./bt_agent.exp NoInputNoOutput &
./bt_agent.exp KeyboardDisplay &

btmon > cred.txt &
sleep 1

echo 160 >> /sys/kernel/debug/bluetooth/hci0/adv_min_interval
echo 160 >> /sys/kernel/debug/bluetooth/hci0/adv_max_interval

btmgmt -i hci0 info

btmgmt -i hci0 power off

btmgmt -i hci0 le on

btmgmt -i hci0 connectable on

btmgmt -i hci0 debug-keys off

btmgmt -i hci0 sc on

btmgmt -i hci0 bondable on

btmgmt -i hci0 pairable on

btmgmt -i hci0 privacy off

btmgmt -i hci0 name cc33xxble

btmgmt -i hci0 advertising on

btmgmt -i hci0 power on

hcitool -i hci0 lerlon

for device in $(bluetoothctl paired-devices  | grep -o "[[:xdigit:]:]\{8,17\}"); do
    echo "removing bluetooth device: $device | $(bluetoothctl remove $device)"
done

btgatt-server -i hci0 -s low -t public -r -v > btgatt.txt 
