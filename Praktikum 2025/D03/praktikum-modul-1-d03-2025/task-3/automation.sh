#!/bin/bash
usr="/home/freddskiii/cloud_storage/users.txt"
log="/home/freddskiii/cloud_storage/cloud_log.txt"

usrEntry=$(awk -F ";" '/userLogged/ {print $2}' $log)

if ! [[ -z "$usrEntry" ]]; then
    if ! crontab -l | grep -q "download.sh"; then
        (crontab -l ; echo "*/10 * * * * /home/freddskiii/download.sh") | crontab -
        (crontab -l ; echo "* */2 * * * /home/freddskiii/archive.sh") | crontab -
    fi
else
    crontab -l | grep -v "download.sh" | grep -v "archive.sh" | crontab -
fi

if ! crontab -l | grep -q "automation.sh"; then
    (crontab -l ; echo "*/2 * * * * /home/freddskiii/automation.sh") | crontab -
fi
