#!/bin/bash

nama_lagu=$(awk -F',' '{if ($2 ~ /[0-9]/ && !($2 ~ /_/)) count[$3]++} 
END {
    for (song in count) {
        if (count[song] > max_count) {
            max_count = count[song]
            max_song = song
        }
    }
}
END {print max_song}
' DataStreamer.csv)

awk -F',' -v song=$nama_lagu '
{
    if ($2 == song) {
    print "Lagu", song, "dari album", $1, "rilis tahun", $3
    }
}
' AlbumDetails.csv