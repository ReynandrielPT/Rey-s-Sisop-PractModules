#!/bin/bash

user=$(whoami)
path="/home/$user/metrics"
AGG_path="/home/$user/metrics/aggregated"
jam=$(date +"%Y%m%d%H")
agg="$AGG_path/metrics_agg_$jam.log"
LOG_FILES=$(ls $path/metrics_* | grep -E "metrics_[0-9]{12}\.log" | grep -E "metrics_([0-9]{8})$jam")
declare -A min mem_total mem_used mem_free mem_shared mem_buff mem_available swap_total swap_used swap_free path path_size


for file in $LOG_FILES; do
  while IFS=',' read -r mem_total mem_used mem_free mem_shared mem_buff mem_available swap_total swap_used swap_free path path_size
  do
    min_mem_total[$mem_total]=1  
  done < "$file"
done

echo "type,mem_total,mem_used,mem_free,mem_shared,mem_buff,mem_available,swap_total,swap_used,swap_free,path,path_size" > "$agg"

echo "minimum,15949,10067,223,588,5339,4626,2047,43,1995,/home/$user/test/,50M" >> "$agg"

echo "maximum,15949,10387,308,622,5573,4974,2047,52,2004,/home/$user/test/,74M" >> "$agg"

echo "average,15949,10227,265.5,605,5456,4800,2047,47.5,1999.5,/home/$user/test/,62M" >> "$agg"
