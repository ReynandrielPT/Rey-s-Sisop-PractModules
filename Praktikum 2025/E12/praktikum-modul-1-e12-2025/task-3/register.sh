#!/bin/bash


user=$(whoami)
path=/home/$user/tugas_3
user_data="$path/cloud_storage/users.txt"
cloudfile="$path/cloud_storage/cloud_log.txt"

jam=$(date +'%y/%m/%d %H:%M:%S')


echo "Current time: $jam"

log_event() {
    echo "$(date +'%y/%m/%d %H:%M:%S') $1" >> "$cloudfile"
}


read -p "Enter username: " username
read  -p "Enter password: " password



if grep -q "$username" "$user_data"; then
    
    log_event "$jam REGISTER: ERROR User already exists"
    echo "$jam REGISTER: ERROR User already exists"
    exit 1
fi



if [[ ${#password} -lt 8 ]]; then
    log_event "$jam REGISTER: ERROR Password is too short. Must be at least 8 characters."
    echo "$jam REGISTER: ERROR Password is too short. Must be at least 8 characters."
    exit 1
fi


if [[ ! "$password" =~ [A-Z] ]]; then
    log_event "$jam REGISTER: ERROR Password must contain at least one uppercase letter"
    echo "$jam REGISTER: ERROR: Password must contain at least one uppercase letter."
    exit 1
fi


if [[ ! "$password" =~ [0-9] ]]; then
    log_event "$jam REGISTER: ERROR Password must contain at least one number"
    echo "$jam REGISTER: ERROR: Password must contain at least one number."
    exit 1
fi


if [[ ! "$password" =~ [@#$%*!] ]]; then
    log_event "$jam REGISTER: ERROR Password must contain at least one special character"
    echo "$jam REGISTER: ERROR: Password must contain at least one special character."
    exit 1
fi


if [[ "$password" == "$username" ]]; then
    log_event "$jam REGISTER: ERROR Password cannot be the same as username"
    echo "$jam REGISTER: ERROR: Password cannot be the same as username."
    exit 1
fi


if [[ "$password" =~ [Cc][Ll][Oo][Uu][Dd] ]]; then
    log_event "$jam REGISTER: ERROR Password cannot contain the word 'cloud'"
    echo "$jam REGISTER: ERROR: Password cannot contain the word 'cloud'."
    exit 1
fi


if [[ "$password" =~ [Ss][Tt][Oo][Rr][Aa][Gg][Ee] ]]; then
    log_event "$jam REGISTER: ERROR Password cannot contain the word 'storage'"
    echo "$jam REGISTER: ERROR: Password cannot contain the word 'storage'."
    exit 1
fi


echo "$username:$password:offline" >> "$user_data"
log_event "$jam REGISTER:INFO User registered successfully"

echo "$jam REGISTER:INFO User registered successfully "
