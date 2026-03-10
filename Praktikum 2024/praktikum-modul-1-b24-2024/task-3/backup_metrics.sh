#!/bin/bash

DATE=$(date '+%Y%m%d')
patt="metrics_agg_$DATE*"

files=$(find ~/praktikum-modul-1-b24/task-3/ -maxdepth 1 -name "$patt")

if [ -z "$files" ]; then
    echo "No files were found for backup."
    exit 0
fi

tar -czf ~/praktikum-modul-1-b24/task-3/backup_metrics_${DATE}.tar.gz -C ~/praktikum-modul-1-b24/task-3 $(echo $files | xargs -n 1 basename) 
