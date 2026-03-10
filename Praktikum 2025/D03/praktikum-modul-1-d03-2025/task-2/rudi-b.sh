#!/bin/bash

convert_date_format() {
    temp_date=$1
    month=${temp_date:0:2}
    day=${temp_date:3:2}
    year=${temp_date:6:4}

    months=("Jan")
    month_name=${months[$((month-1))]}
    formatted_date="$day/$month_name/$year"

    echo "$formatted_date"
}

echo -e "Masukkan tanggal (MM/DD/YYYY)"
read tanggal
echo -e "Masukkan IP Address (192.168.1.X)"
read ip_address

number_pc=$(echo "$ip_address" | awk -F'.' '{print $4}')

echo "Nomor komputer berdasarkan IP adalah: $number_pc"

nama_pengguna=$(grep "$tanggal,$number_pc" peminjaman_computer.csv | awk -F',' '{print $3}')

if [ -z "$nama_pengguna" ]; then
    echo "Nama pengguna tidak ditemukan"
else
    echo "Pengguna saat itu adalah $nama_pengguna"
    echo "Log Aktivitas $nama_pengguna"
    formatted_date=$(convert_date_format "$tanggal")
    waktu=$(date | awk '{print $4}' | awk -F':' '{print $1$2$3}')
    dir="${nama_pengguna}_$(echo $tanggal | awk -F'/' '{print $1$2$3}')_$waktu.log"
    mkdir -p "backup"
    awk -v tgl=$formatted_date -v ip=$ip_address '
        $1 == ip && $4 ~ tgl {
        date = substr($4, 2, 20)
        metohod = substr($6, 2)
        print "[" date "]:", metohod, "-", $7, "-", $9}' access.log > backup/$dir
    echo "$dir berhasil dibuat"
fi
