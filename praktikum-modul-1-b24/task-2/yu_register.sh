#!/bin/bash

read np
read dep
read fak
read jud
read dp
read skema
nomor=$(awk -F, 'END{print $1+1}' ~/praktikum-modul-1-b24/resources/data-pkm.csv)
awk -v nomor="$nomor"  -v dep="$dep" -v fak="$fak" -v  jud="$jud" -v dp="$dp" -v skema="$skema" -v np="$np"  -F, 'BEGIN{f=0} {if(np==$2){f=1}}END{if(f==0){
OFS=","; print nomor,np,dep,fak,jud,dp,skema >> "temp.csv";
print "REGISTER: SUCCESS "np" is registered. Proposal "nomor" is added" >> "log.txt";
}else {print strftime("%Y/%m/%d %H:%M:%S",systime())" REGISTER : ERROR "np" is already existed">>"log.txt"}}' ~/praktikum-modul-1-b24/resources/data-pkm.csv

if [ -f "temp.csv" ]; then
cat  "temp.csv"  >> ~/praktikum-modul-1-b24/resources/data-pkm.csv
rm "temp.csv"
fi