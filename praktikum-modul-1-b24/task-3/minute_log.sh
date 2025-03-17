#!/bin/bash

read mt mu mf ms mb ma < <(free -m | awk 'NR==2{print $2,$3,$4,$5,$6,$7}')
read st su sf < <(free -m | awk 'NR==3{print $2,$3,$4}')

Date=$(date +'%Y%m%d%H%M%S')

echo "mem_total,mem_used,mem_free,mem_shared,mem_buff,mem_available,swap_total,swap_used,swap_free,path,path_size" > "metrics_${Date}.log"

echo "$mt,$mu,$mf,$ms,$mb,$ma,$st,$su,$sf,~/,$(du -sh ~/ | awk '{print $1}'))" >> "metrics_${Date}.log"

chmod go-rwx "metrics_${Date}.log"
chmod u+rwx "metrics_${Date}.log"