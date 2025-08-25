#!/bin/sh
# Wrapper script to execute socat in a network namespace
ip netns exec eth_ns socat STDIO TCP:192.168.68.77:9100