#!/bin/bash
usr="/home/freddskiii/cloud_storage/users.txt"
log="/home/freddskiii/cloud_storage/cloud_log.txt"

if grep -q "userLogged" "$log"; then
    echo "Error: User lain sedang login!"
    exit 1
fi

log_message() {
    echo "$(date +'%y/%m/%d %T') $1" >> "$log"
}

echo -n "Username: "
read usn
echo -n "Password: "
read pw

usrEntry=$(grep "^$usn;" "$usr")
if [[ -z "$usrEntry" ]]; then
    log_message "LOGIN: ERROR Failed login attempt for user $usn"
    echo "Error: Invalid credentials!"
    exit 1
fi

pwUsr=$(echo "$usrEntry" | awk -F';' '{print $2}')

if [[ "$pw" != "$pwUsr" ]]; then
    log_message "LOGIN: ERROR Failed login attempt for user $usn (wrong password)"
    echo "Error: Password salah!"
    exit 1
fi

log_message "LOGIN: INFO User $usn berhasil log in"
echo "Login berhasil!"
echo "userLogged;$usn" >> "$log" 

while true; do
    echo -n "Input 1 untuk logout: "
    read choice
    if [[ "$choice" == "1" ]]; then
        sed -i '$d' "$log"
        log_message "LOGOUT: INFO User $usn berhasil log out"
        echo "Logout berhasil!"
        exit 0
    else
        echo "Input tidak valid"
    fi
done
