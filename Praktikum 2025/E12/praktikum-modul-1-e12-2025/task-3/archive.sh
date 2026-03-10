#!/bin/bash


path="/home/sakuya/tugas_3"
loggggggggggg="$path/cloud_storage/cloud_log.txt"


ada_ga=$(grep "LOGIN: INFO User" "$loggggggggggg" | awk '{print $6}' | sort | uniq)


if [ -z "$ada_ga" ]; then
    echo "$(date '+%Y/%m/%d %H:%M:%S') ARCHIVE: ERROR " >> "$loggggggggggg"
    exit 1
fi

for USER in $ada_ga; do
   
    downdir="$path/cloud_storage/downloads/$USER"
    arcdir="$path/cloud_storage/archives/$USER"
    
    mkdir -p "$arcdir"

    jam=$(date '+%H-%M_%d-%m-%Y')
    archive="$arcdir/archive_$jam.zip"
   
    if [ -z "$(ls -A "$downdir" 2>/dev/null)" ]; then
        echo "$(date '+%Y/%m/%d %H:%M:%S') ARCHIVE: INFO No files to archive for user $USER" >> "$loggggggggggg"
        continue
    fi
    
    zip -r "$archive" "$downdir"/* && rm -rf "$downdir"/*

    echo "$(date '+%Y/%m/%d %H:%M:%S') ARCHIVE: INFO Archived files for user $USER as $archive" >> "$loggggggggggg"
done
