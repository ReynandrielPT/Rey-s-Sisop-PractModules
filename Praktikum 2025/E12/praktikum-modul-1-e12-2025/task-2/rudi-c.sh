#!/bin/bash

log_file="access.log"
peminjaman_file="peminjaman_computer.csv"

declare -A ip_to_name
while IFS="," read -r tanggal komputer peminjam; do
    if [[ "$komputer" =~ [0-9]+ ]]; then
        ip="192.168.1.$komputer"
        ip_to_name[$ip]="$peminjam"
    fi
done < <(tail -n +2 "$peminjaman_file")

errors=$(awk '$9 == 500 {print $1}' "$log_file" | sort | uniq -c | sort -nr | head -1)

while read -r count ip; do
    name="Unknown"
    for key in "${!ip_to_name[@]}"; do
        if [[ "$ip" == "$key" ]]; then
            name="${ip_to_name[$key]}"
            break
        fi
    done
    echo "pemenang: $name menemukan error $count kali"
done <<< "$errors"
