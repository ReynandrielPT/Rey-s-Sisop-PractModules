#!/bin/bash

log_dir="/home/mob/praktikum-modul-1-b24/task-3"

datetime=$(date +'%Y%m%d%H')

cat "$log_dir"/metrics_"$datetime"*.log > "$log_dir/metrics_agg_$datetime.log"

rown=0

min_total=9999999999
min_used=9999999999
min_free=9999999999
min_shared=9999999999
min_buff=9999999999
min_available=9999999999
min_swaptotal=9999999999
min_swapused=9999999999
min_swapfree=9999999999
min_pathsize=9999999999

max_total=0
max_used=0
max_free=0
max_shared=0
max_buff=0
max_available=0
max_swaptotal=0
max_swapused=0
max_swapfree=0
max_pathsize=0

avg_total=0
avg_used=0
avg_free=0
avg_shared=0
avg_buff=0
avg_available=0
avg_swaptotal=0
avg_swapused=0
avg_swapfree=0
avg_pathsize=0

min() {
  local a=$1
  local b=$2
  if ((a<b)); then
    echo "$a"
  else
    echo "$b"
  fi
}

max() {
  local a=$1
  local b=$2
  if ((a>b)); then
    echo "$a"
  else
    echo "$b"
  fi
}

while IFS= read -r line; do
  ((rown++))
  if [ $((rown % 2)) -eq 0 ]; then
    IFS=',' read -ra data <<< "$line"
    cnt=0
    for i in "${data[@]}"; do
      ((cnt++))
      if [ $((cnt)) -eq 1 ]; then
        min_total=$(min "$min_total" "$i")
        max_total=$(max "$max_total" "$i")
        ((avg_total+=$i))
      elif [ $((cnt)) -eq 2 ]; then
        min_used=$(min "$min_used" "$i")
        max_used=$(max "$max_used" "$i")
        ((avg_used+=$i))
      elif [ $((cnt)) -eq 3 ]; then
        min_free=$(min "$min_free" "$i")
        max_free=$(max "$max_free" "$i")
        ((avg_free+=$i))
      elif [ $((cnt)) -eq 4 ]; then
        min_shared=$(min "$min_shared" "$i")
        max_shared=$(max "$max_shared" "$i")
        ((avg_shared+=$i))
      elif [ $((cnt)) -eq 5 ]; then
        min_buff=$(min "$min_buff" "$i")
        max_buff=$(max "$max_buff" "$i")
        ((avg_buff+=$i))
      elif [ $((cnt)) -eq 6 ]; then
        min_available=$(min "$min_available" "$i")
        max_available=$(max "$max_available" "$i")
        ((avg_available+=$i))
      elif [ $((cnt)) -eq 7 ]; then
        min_swaptotal=$(min "$min_swaptotal" "$i")
        max_swaptotal=$(max "$max_swaptotal" "$i")
        ((avg_swaptotal+=$i))
      elif [ $((cnt)) -eq 8 ]; then
        min_swapused=$(min "$min_swapused" "$i")
        max_swapused=$(max "$max_swapused" "$i")
        ((avg_swapused+=$i))
      elif [ $((cnt)) -eq 9 ]; then
        min_swapfree=$(min "$min_swapfree" "$i")
        max_swapfree=$(max "$max_swapfree" "$i")
        ((avg_swapfree+=$i))
      elif [ $((cnt)) -eq 11 ]; then
        num=$(echo "$i" | tr -cd '[:digit:]')
        suf=$(echo "$i" | tr -cd '[:alpha:]')
        if [ "$suf" = "G" ]; then
          num=$((num * 1048576))
        elif [ "$suf" = "M"]; then
          num=$((num * 1024))
        fi
        min_pathsize=$(min "$min_pathsize" "$((num))")
        max_pathsize=$(max "$max_pathsize" "$((num))")
        ((avg_pathsize+=$((num))))
      fi
    done
  fi
done < "$log_dir/metrics_agg_$datetime.log"

avg_rn=$((rown / 2))

avg_total=$((avg_total / avg_rn))
avg_used=$((avg_used / avg_rn))
avg_free=$((avg_free / avg_rn))
avg_shared=$((avg_shared / avg_rn))
avg_buff=$((avg_buff / avg_rn))
avg_available=$((avg_available / avg_rn)) 
avg_swaptotal=$((avg_swaptotal / avg_rn))
avg_swapused=$((avg_swapused / avg_rn))
avg_swapfree=$((avg_swapfree / avg_rn))
avg_pathsize=$((avg_pathsize / avg_rn))

echo "type,mem_total,mem_used,mem_free,mem_shared,mem_buff,mem_available,swap_total,swap_used,swap_free,swap_cache,path,path_size" > "$log_dir/metrics_agg_$datetime.log"
echo "minimum,$min_total,$min_used,$min_free,$min_shared,$min_buff,$min_available,$min_swaptotal,$min_swapused,$min_swapfree,$log_dir,$min_pathsize""K" >> "$log_dir/metrics_agg_$datetime.log"
echo "maximum,$max_total,$max_used,$max_free,$max_shared,$max_buff,$max_available,$max_swaptotal,$max_swapused,$max_swapfree,$log_dir,$max_pathsize""K">> "$log_dir/metrics_agg_$datetime.log"
echo "average,$avg_total,$avg_used,$avg_free,$avg_shared,$avg_buff,$avg_available,$avg_swaptotal,$avg_swapused,$avg_swapfree,$log_dir,$avg_pathsize""K">> "$log_dir/metrics_agg_$datetime.log"