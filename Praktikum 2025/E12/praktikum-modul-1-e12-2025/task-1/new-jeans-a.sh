echo "List nama user sesuai kriteria Minji : "
awk -F, '(NR>1) && $2 ~ /2/ && $2 !~ /_/ {print $2} ' DataStreamer.csv | LC_ALL=C sort -u |
awk '{print NR "." $0} END {print "Jumlah: " NR}'
 

