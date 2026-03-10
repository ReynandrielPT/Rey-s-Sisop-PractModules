#!/bin/bash

USERNAME="$1"
path=/home/sakuya/tugas_3
downpath="$path/cloud_storage/downloads/"

users="$path/cloud_storage/users.txt"

while IFS=':' read -r username pass status; do
    if [[ "$status" == "online" ]]; then
       
        user_folder="$downpath/$username"
        
        if [ ! -d "$user_folder" ]; then
            mkdir -p "$user_folder"
           

           
        fi

        
        API_KEY="dummy"
        CX="51bd222acf62343c7"
        
     
        URL="https://www.googleapis.com/customsearch/v1?key=${API_KEY}&cx=${CX}&q=nature&searchType=image"

        
        image_urls=$(curl -s "${URL}" | grep -o '"link": "[^"]*"' | sed 's/"link": "//g' | sed 's/"//g')

       
        selected_image=$(echo "$image_urls" | shuf -n1)

        
        current_time=$(date +"%H-%M_%d-%m-%Y")

       
        url_without_query="${selected_image%%\?*}"
        extension="${url_without_query##*.}"

       
       

        
        filename="${user_folder}/${current_time}.zip"

       
        curl -s "$selected_image" -o "$filename"

        
    fi
done < "$users"
