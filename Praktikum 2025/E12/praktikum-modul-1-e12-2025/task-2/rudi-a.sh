#!/bin/bash

log_file="access.log"

if [ ! -f "$log_file" ]; then
    echo "File $log_file tidak ditemukan!"
    exit 1
fi

echo "Total Request per IP:"
awk '{print $1}' "$log_file" | sort | uniq -c | sort -nr

echo -e "Jumlah Setiap Status Code:"
awk '{print $9}' "$log_file" | sort | uniq -c | sort -nr
