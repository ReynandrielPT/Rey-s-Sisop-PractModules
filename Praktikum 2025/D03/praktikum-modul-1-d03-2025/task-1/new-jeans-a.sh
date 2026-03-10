#!/bin/bash
awk -F',' 'NR > 1 {print $2}' DataStreamer.csv | grep "2" | grep -v "_" | sort | awk ' {{print} counter++}; END {print "Jumlah Username sesuai kriteria:", counter}'