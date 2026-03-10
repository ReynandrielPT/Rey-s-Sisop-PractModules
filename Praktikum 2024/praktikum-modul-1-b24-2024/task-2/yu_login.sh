#!/bin/bash

read username
read password
awk -v username=$username -v password=$password -F, '
BEGIN {found=0}
{for(i=6; i<=NF; i++) {if(match($i, "\([0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9]\)")>0) {currentpassword=$i}}}
{split($2, arr, "_")} 
{gsub(".*\(", "", currentpassword)}
{gsub("\).*", "", currentpassword)}
{if(arr[1]==username) {if(password==$4currentpassword){found=1}}}
END {if(found==1) 
  {print strftime("%Y/%m/%d %H:%M:%S", systime()) " LOGIN: SUCCESS "username" is logged in" >> "log.txt"} 
else 
  {print strftime("%Y/%m/%d %H:%M:%S", systime()) " LOGIN: ERROR Failed login attempt on "username >> "log.txt"}}' ~/praktikum-modul-1-b24/resources/data-pkm.csv 