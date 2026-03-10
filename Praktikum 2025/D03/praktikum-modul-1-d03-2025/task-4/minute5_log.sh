#!/bin/bash

timestamp=$(date +"%Y%m%d%H%M%S")
filename="metrics_$timestamp.log"
filename="/home/riyan/metrics/$filename"

metrics=$(free -m | awk 'NR==2 {OFS=","; print $2,$3,$4,$5,$6,$7}') 
metrics2=$(free -m | awk 'NR==3 {OFS=","; print $2,$3,$4}')
metrics3=$(du -sh "/home/$USER/" | awk '{print $2}')
metrics4=$(du -sh "/home/$USER/" | awk '{print $1}')

touch $filename
echo "mem_total,mem_used,mem_free,mem_shared,mem_buff,mem_available,swap_total,swap_used,swap_free,path,path_size" >> $filename
echo ${metrics},${metrics2},${metrics3},${metrics4} >> $filename 

chmod 400 "$filename"