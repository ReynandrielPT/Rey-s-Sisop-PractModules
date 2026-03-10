#!/bin/bash

while true; do
    sleep 120
    if [ -f "$ACTIVE_USER_FILE" ]; then
        logged_user=$(cat "$ACTIVE_USER_FILE")
        bash /home/sakuya/tugas_3/download.sh "$logged_user"
    fi
done 
