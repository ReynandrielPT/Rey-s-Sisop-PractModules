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

if ! [[ -e /home/freddskiii/cloud_storage/users.txt ]]; then
    mkdir cloud_storage
    touch /home/freddskiii/cloud_storage/cloud_log.txt
    touch /home/freddskiii/cloud_storage/users.txt
fi

echo -n "Username : "
read usn
echo -n "Password : "
read pw

if grep -q "^$usn;" "$usr"; then
    log_message "REGISTER: ERROR Username tersebut sudah terdaftar!"
    echo "Error: Username tersebut sudah terdaftar!"
    exit 1
fi

error=""
if [[ ${#pw} -lt 8 ]]; then
    error="Password terlalu pendek"
elif ! [[ "$pw" =~ [A-Z] ]]; then
    error="Password harus memiliki huruf kapital"
elif ! [[ "$pw" =~ [0-9] ]]; then
    error="Password harus memiliki digit"
elif ! [[ "$pw" =~ [^a-zA-Z0-9] ]]; then
    error="Password harus memiliki karakter special {ex : !@#$%^&*}"
elif [[ "$pw" == "$usn" ]]; then
    error="Password tidak boleh sama dengan username"
elif [[ "$pw" =~ [Cc][Ll][Oo][Uu][Dd] || "$pw" =~ [Ss][Tt][Oo][Rr][Aa][Gg][Ee] ]]; then
    error="Password mengandung kata terlarang : cloud / storage"
fi

if [ "$error" ]; then
    log_message "REGISTER: ERROR $error"
    echo "$error"
    exit 1
fi

echo "$usn;$pw" >> "$usr"
log_message "REGISTER: User baru berhasil didaftarkan"
echo "Registrasi berhasil!"
