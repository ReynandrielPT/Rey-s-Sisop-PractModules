#!/bin/bash

current_date=$(date -d "yesterday" +%Y%m%d)

for file in /home/riyan/metrics/metrics_agg_"$current_date"*; do
    base=$(basename "$file")
    hour=${base#metrics_agg_${current_date}}
    hour=${hour:0:2}
    if [[ "$hour" -lt 13 ]]; then
        rm "$file"
    fi
done