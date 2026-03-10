#!/bin/bash

timestamp=$(date +"%Y%m%d%H%M%S")
filename="uptime_$timestamp.log"
filename="/home/riyan/metrics/$filename"

touch $filename
echo "uptime,load_avg_1min,load_avg_5min,load_avg_15min" >> $filename
uptime_=$(uptime | awk '{print $1,$2,$3}')
average_=$(cat /proc/loadavg | awk '{OFS=",";print $1,$2,$3}')
echo ${uptime_}${average_} >> $filename
chmod 400 "$filename"