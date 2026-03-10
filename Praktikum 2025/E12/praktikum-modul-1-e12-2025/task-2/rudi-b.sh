#!/bin/bash

read -p "Masukkan Tanggal (DD/MM/YYYY): " tanggal_input
read -p "Masukkan IP Address (192.168.1.X): " ip_input

log_file="access.log"
peminjaman_file="peminjaman_computer.csv"
backup_dir="/backup"

mkdir -p "$backup_dir"

nomor_komputer=${ip_input##*.}

nama_pengguna=$(awk -F',' -v tgl="$tanggal_input" -v komp="$nomor_komputer" '$1==tgl && $2==komp {print $3}' "$peminjaman_file")

if [[ -z "$nama_pengguna" ]]; then
    echo "Data yang kamu cari tidak ada"
    exit 1
fi

echo "Pengguna saat itu adalah $nama_pengguna"
echo "Log Aktivitas [$nama_pengguna]"

user_backup_dir="$backup_dir/$nama_pengguna"
mkdir -p "$user_backup_dir"

timestamp=$(date +"%H%M%S")
tanggal_format=$(echo "$tanggal_input" | sed 's/\///g')
backup_file="$user_backup_dir/${nama_pengguna}_${tanggal_format}_${timestamp}.log"

log_aktivitas=$(grep "$ip_input" "$log_file" | grep "$tanggal_input" | awk '{print "[" $4 "]:" $6 " - " $7 " - " $9}')

echo -e "$log_aktivitas" > "$backup_file"

echo "Log Aktivitas $nama_pengguna telah disimpan di $backup_file"
