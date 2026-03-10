str1=$(cat access.log | awk '{print $1}' | awk '{count[$1]++} END {for (ip in count) print "Jumlah request dari", ip, "=", count[ip]}')
str2=$(cat access.log | awk '{count[$9]++} END {for (code in count) print "Jumlah status code", code, "=", count[code]}')

echo "$str1"
echo "$str2"