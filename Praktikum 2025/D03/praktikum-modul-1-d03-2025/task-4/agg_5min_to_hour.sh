#!/bin/bash

current=$(date +%s)
current_time=$(date +"%Y%m%d%H")
output=/home/riyan/metrics/metrics_agg_"$current_time".log
log_files=()

for file in /home/$USER/metrics/metrics_*; do
    base=$(basename "$file")
    if [[ "$base" == metrics_agg* ]]; then
        continue
    fi
    timestamp=${file#/home/$USER/metrics/metrics_}
    timestamp=${timestamp%.log}
    formatted_timestamp=$(echo "$timestamp" | sed 's/\(....\)\(..\)\(..\)\(..\)\(..\)\(..\)/\1-\2-\3 \4:\5:\6/')
    epoch=$(date -d "$formatted_timestamp" +%s)

    if((current - epoch < 3600)); then
        log_files+=("$file")
    fi 

done

awk -F',' 'BEGIN { OFS="," }
FNR==2 {
    fieldCount = NF;
    for(i = 1; i<=NF; i++){
        if(i==11){
            if($i ~ /G$/){
                num = substr($i, 1, length($i)-1) + 0;
                value = num * 1024;
            }
            else{
                value = substr($i, 1, length($i)-1) + 0;
            }
            sum[i] += value;
            count[i]++;
            if(!(i in min) || value < min[i])
                min[i] = value;
            if(!(i in max) || value > max[i]) 
                max[i] = value;
        }
        else if (i!=10){
            sum[i] += $i;
            count[i]++;
            if(!(i in min) || $i < min[i])
                min[i] = $i;
            if(!(i in max) || $i > max[i]) 
                max[i] = $i;
        }
    }
}

END {
    print "type,mem_total,mem_used,mem_free,mem_shared,mem_buff,mem_available,swap_total,swap_used,swap_free,path,path_size";
    type[1] = "minimum"; type[2] = "maximum"; type[3] = "average";

    for(i = 1; i<=3; i++){
        printf "%s", type[i];
        for(j = 1; j<=fieldCount; j++){
            if(j!=10){
                if(i==1)
                    printf ",%s", min[j];            
                else if(i==2)
                    printf ",%s", max[j];
                else
                    printf ",%s", sum[j]/count[j];
            }
            else
                printf ",/home/riyan/"
        }
        print ""
    }
}' "${log_files[@]}" > "$output"

chmod 400 "$output"

