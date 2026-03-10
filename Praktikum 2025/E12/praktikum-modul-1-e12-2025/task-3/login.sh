#!/bin/bash

user=$(whoami)
path=/home/$user/tugas_3
USER_FILE="$path/cloud_storage/users.txt"
LOG_FILE="$path/cloud_storage/cloud_log.txt"

echo "Enter username: "
read username
echo "Enter password: "
read password

jam=$(date +"%y/%m/%d %H:%M:%S")


user_ingpo=$(grep "$username:$password" "$USER_FILE")
if [[ -z "$user_ingpo" ]]; then
    echo "$jam LOGIN: ERROR Failed login attempt on user $username" >> "$LOG_FILE"
    echo "Login failed. Try again."
    exit 1
fi


user_status=$(echo "$user_ingpo" | cut -d ':' -f 3)

if [[ "$user_status" == "online" ]]; then
   
    echo "User $username is already logged in. Do you want  log out? (y/n)"
    read choise

    if [[ "$choise" == "y" || "$choise" == "Y" ]]; then
        # Log out the user by updating users.txt and writing the log
        sed -i "s/^$username:$password:online\$/$username:$password:offline/" "$USER_FILE"
        echo "$jam LOGOUT: INFO User $username logged out" >> "$LOG_FILE"
        echo "You have been logged out."
        exit 0
    else
        echo "You remain logged in."
        exit 0
    fi
fi





sed -i "s/^$username:$password:offline\$/$username:$password:online/" "$USER_FILE"
echo "$jam LOGIN: INFO User $username logged in" >> "$LOG_FILE"
echo "Login successful."


echo "Do you want  log out? (y/n)"
read choise

if [[ "$choise" == "y" || "$choise" == "Y" ]]; then
  
    sed -i "s/^$username:$password:online\$/$username:$password:offline/" "$USER_FILE"
    echo "$jam LOGOUT: INFO User $username logged out" >> "$LOG_FILE"
    echo "You have been logged out."
    exit 0
else
    echo "You logged in."
    exit 0
fi
