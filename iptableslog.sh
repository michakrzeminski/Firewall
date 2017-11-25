#!/bin/bash
sudo iptables -A INPUT -j LOG --log-level 4 --log-prefix "denied: "
sudo iptables -A FORWARD -j LOG --log-level 4 --log-prefix "denied: "
sudo iptables -A OUTPUT -j LOG --log-level 4 --log-prefix "denied: "
