#!/bin/bash

awk -F',' '{ if ($2 ~ /[0-9]/ && !($2 ~ /_/)) count[$3]++} 
END {
    for (song in count) {
        if (count[song] > max_count) {
            max_count = count[song]
            max_song = song
        }
    }
    print "Pendengar lagu", max_song, "berjumlah", max_count, "pendengar"
    if (max_count > 24) {
        print "Keren, Minji! Kamu hebat <3!"
    } else {
        print "Maaf, Minji, total streamingnya tidak sesuai harapan :("}}
' DataStreamer.csv

