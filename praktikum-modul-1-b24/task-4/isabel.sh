#!/bin/bash

curr_H=$(date +'%H')
curr_M=$(date +'%M')

CRON_FILE="~/praktikum-modul-1-b24/task-4/crontabs"

function update_crontab {
    JOB="$1"
    FUNC="$2"
    sed -i '/'$FUNC'/d' ~/praktikum-modul-1-b24/task-4/crontabs
    if ! grep -Fq "$JOB" ~/praktikum-modul-1-b24/task-4/crontabs; then
        echo "$JOB * * * /bin/bash ~/praktikum-modul-1-b24/task-4/isabel.sh $FUNC" >> ~/praktikum-modul-1-b24/task-4/crontabs
        crontab /bin/bash ~/praktikum-modul-1-b24/task-4/crontabs
    fi
}

if ! grep -Fq "0 2 * * * /bin/bash ~/praktikum-modul-1-b24/task-4/isabel.sh isabel-c" ~/praktikum-modul-1-b24/task-4/crontabs; then
    echo "0 2 * * * /bin/bash ~/praktikum-modul-1-b24/task-4/isabel.sh isabel-c" >> ~/praktikum-modul-1-b24/task-4/crontabs
    crontab /bin/bash ~/praktikum-modul-1-b24/task-4/crontabs
fi

if ! grep -Fq "0 0 * * * /bin/bash ~/praktikum-modul-1-b24/task-4/isabel.sh isabel-d" ~/praktikum-modul-1-b24/task-4/crontabs; then
    echo "0 0 * * * /bin/bash ~/praktikum-modul-1-b24/task-4/isabel.sh isabel-d" >> ~/praktikum-modul-1-b24/task-4/crontabs
    crontab /bin/bash ~/praktikum-modul-1-b24/task-4/crontabs
fi

function isabel_a {
    newH=$(($curr_H + 5))
    if [ $newH -ge 24 ]; then
        newH=$((newH-24))
    fi
    update_crontab "$curr_M $newH" "isabel-a"

    if [[ "$curr_H" == "00" && "$curr_M" == "00" ]]; then
        count=10
    elif ((curr_H % 2 == 0)); then
        count=5
    else
        count=3
    fi

    jumlah=$(find ~/praktikum-modul-1-b24/task-4/ -type d -name 'folder_*' | wc -l)
    num=$((jumlah + 1))
    folder="folder_${num}"

    mkdir -p ~/praktikum-modul-1-b24/task-4/"$folder"

    for ((i=1; i<=count; i++)); do
        wget -O ~/praktikum-modul-1-b24/task-4/${folder}/foto_${i}.jpg "https://media.discordapp.net/attachments/1121728188935045164/1220633219662086144/20240321_143812.jpg?ex=660fa645&is=65fd3145&hm=6e86655a8cb573d1d2ff2bf0320e840c78528d91c1b9eabe3fc42676aefc46e4&=&format=webp&width=1247&height=627"
    done
}

function isabel_b {
    newH=$(($curr_H + 10))
    if [ $newH -ge 24 ]; then
        newH=$((newH-24))
    fi
    update_crontab "$curr_M $newH" "isabel-b"

    folders=$(find ~/praktikum-modul-1-b24/task-4/ -maxdepth 1 -type d -name 'folder_*' | sort)

    cnt=1

    for folder in $folders; do
        namazip="ayang_${cnt}.zip"
        if [ ! -f "$namazip" ]; then
            zip -r "$namazip" "$folder"
        fi
        cnt=$((cnt + 1))
    done
}

function isabel_c {
    rm -rf ~/praktikum-modul-1-b24/task-4/folder_*
    rm -rf ~/praktikum-modul-1-b24/task-4/ayang_*.zip
}

function isabel_d {
    urlL="https://media.discordapp.net/attachments/1121728188935045164/1220633219662086144/20240321_143812.jpg?ex=660fa645&is=65fd3145&hm=6e86655a8cb573d1d2ff2bf0320e840c78528d91c1b9eabe3fc42676aefc46e4&=&format=webp&width=1247&height=627"
    urlE="https://api.duniagames.co.id/api/content/upload/file/8237210571695204665.jpg"

    trakir=$(ls -Art ~/praktikum-modul-1-b24/task-4 | egrep "levi_|eren_" | tail -n 1)
    waktu=$(date +'%Y%m%d')

    if [[ $trakir == "levi_"* ]]; then
        wget "$urlE" -O "eren_$waktu.jpg"
        touch "eren_$waktu.jpg"
    else
        wget "$urlL" -O "levi_$waktu.jpg"
        touch "levi_$waktu.jpg"
    fi
}

if [[ $1 == "isabel-a" ]]; then
    isabel_a
elif [[ $1 == "isabel-b" ]]; then    
    isabel_b
elif [[ $1 == "isabel-c" ]]; then
    isabel_c
elif [[ $1 == "isabel-d" ]]; then
    isabel_d
fi