#!/bin/sh
# run ./be_reyax_dev.sh first on RPi3B
# assume already login
# set up wlan0 and eth1
ip link set eth1 down
ip link set wlan0 down
ip link set wlan0 address b8:27:eb:6b:2e:9e
ip link set wlan0 up
cd /usr/share/cc33xx
./sta_start.sh
sleep 1
# you may change the ssid and password here
./sta_connect.sh -n Deco99N2F -s WPA-PSK -p Chao5P@Chiron
sleep 5
# static ip to wlan0
ip addr add 192.168.68.77/24 dev wlan0
# enable ip forwarding
echo 1 | tee /proc/sys/net/ipv4/ip_forward

# set up eth0
ip netns add eth_ns
ip link set eth0 netns eth_ns
ip netns exec eth_ns ip addr add 168.254.199.99/32 dev eth0
ip netns exec eth_ns ip link set eth0 up
ip netns exec eth_ns route add -host 192.168.68.77 eth0
ip netns exec eth_ns arp -i eth0 -s 192.168.68.77 b8:27:eb:6b:2e:8d
# run relayd
relayd -d -d -d -d -I eth0 -N eth0:eth_ns -I wlan0 -B -D &
# TCP port forwarding between wlan0 and eth0 of namespace eth_ns
# open TCP forwarding
# chmod +x /usr/bin/netns_socat.sh
socat TCP-LISTEN:9100,reuseaddr,fork,max-children=2 EXEC:/usr/bin/netns_socat.sh 
# open debugging
#socat -d4 TCP-LISTEN:9100,reuseaddr,fork,max-children=2 EXEC:/usr/bin/netns_socat.sh

# reserved
#ip link set dev eth0 arp off