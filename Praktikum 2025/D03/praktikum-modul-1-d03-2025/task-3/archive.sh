#!/bin/bash
downDir="/home/freddskiii/cloud_storage/downloads"
arcDir="/home/freddskiii/cloud_storage/archives"

for usr in "$downDir"/*; do
    user=$(echo "$usr" | awk -F'/' '{print $NF}')

    mkdir -p "$arcDir/$user"
    
    time=$(date +'%H-%d-%m-%Y')
    zip -r "$arcDir/$user/archive_$time.zip" "$usr"/*
    rm -f "$usr"/*
done
