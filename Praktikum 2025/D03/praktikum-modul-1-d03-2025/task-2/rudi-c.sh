#!/bin/bash
declare -A count

str1=$(awk -F' ' 'BEGIN {months["Jan"]="01"}
$9 ~ 500 {
    tgl = substr($4, 2, 11)
    split(tgl, splitted, "/")
    peminjam = months[splitted[2]] "/" splitted[1] "/" splitted[3] " " substr($1, 11, 1)
    count[peminjam]++
}
END {for (i in count) print i " " count[i]}' access.log)

while read tanggal komputer aktivitas; do
    index=$(awk -v tgl=$tanggal -v komp=$komputer -F',' '{if ($1 ~ tgl && $2 ~ komp) print $3}' peminjaman_computer.csv)
    ((count[$index]+=$aktivitas))
    # echo "${count[$index]}"
done <<< "$str1"

for peminjam in "${!count[@]}"; do
    echo "$peminjam mendapatkan Status Code 500 sebanyak ${count[$peminjam]} kali"
    ((total+=count[$peminjam]))
    if [[ $max_count -lt ${count[$peminjam]} ]]; then
        max_count=${count[$peminjam]}
        max_name=$peminjam
    fi
done

echo "Total Status Code yang ditemukan adalah $total kali"
echo "Selamatt!!! $max_name mendapatkan hadiah dari Rudi"

