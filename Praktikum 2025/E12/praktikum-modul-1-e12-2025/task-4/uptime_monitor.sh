#!/bin/bash

user=$(whoami)
path="/home/$user/metrics"
jam=$(date +"%Y%m%d%H")


file="$path/uptime_$jam.log"


UPTIME=$(uptime | awk '{print $1, $2, $3}' )
load_avg=$(cat /proc/loadavg | awk '{print $1, $2, $3}')

echo "uptime,load_avg_1min,load_avg_5min,load_avg_15min" > "$file"
echo "$UPTIME,$load_avg" >> "$file"

