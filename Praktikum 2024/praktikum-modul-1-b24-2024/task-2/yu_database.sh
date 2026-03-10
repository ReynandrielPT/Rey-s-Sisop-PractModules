#!/bin/bash

awk -F, '
{if(NR==1){next}}
{for(i=6; i<=NF; i++) {if(match($i, "\([0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9]\)")>0) {currentpassword=$i}}}
{split($2, arr, "_")} 
{gsub(".*\(", "", currentpassword)}
{gsub("\).*", "", currentpassword)}
{print arr[1] " " $4 currentpassword > "users.txt"}
' ~/praktikum-modul-1-b24/resources/data-pkm.csv