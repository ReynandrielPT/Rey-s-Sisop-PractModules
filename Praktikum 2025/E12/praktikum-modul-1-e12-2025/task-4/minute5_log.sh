#!/bin/bash

user=$(whoami)
path="/home/$user/"


jam=$(date +"%Y%m%d%H%M%S")
log_file="$path/metrics/metrics_$jam.log"

mem_total=$(free -m | awk '/Mem:/ {print $2}')
mem_used=$(free -m | awk '/Mem:/ {print $3}')
mem_free=$(free -m | awk '/Mem:/ {print $4}')
mem_shared=$(free -m | awk '/Mem:/ {print $5}')
mem_buff_cache=$(free -m | awk '/Mem:/ {print $6}')
mem_available=$(free -m | awk '/Mem:/ {print $7}')


swap_total=$(free -m | awk '/Swap:/ {print $2}')
swap_used=$(free -m | awk '/Swap:/ {print $3}')
swap_free=$(free -m | awk '/Swap:/ {print $4}')


directory_size=$(du -sh "$path"  | awk '{print $1}')

uptime=$(uptime -p )

load_avg=$(awk '{print $1","$2","$3}' /proc/loadavg)
{   
    echo "mem_total,mem_used,mem_free,mem_shared,mem_buff,mem_available,swap_total,swap_used,swap_free,path,path_size"
    echo $mem_total,$mem_used,$mem_free,$mem_shared,$mem_buff_cache,$mem_available,$directory_size,$path, $directory_size
    
} >> "$log_file" 




