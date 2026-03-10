echo " == Lagu dengan Streaming Terbanyak == "
max_title=$(awk -F, 'NR>1 && $2 ~ /[0-9]/ {user[$2]++;songs[$3]++;}
END {
        max_count = 0;
        for(song_title in songs){
        if (songs[song_title] > max_count){
        max_count = songs[song_title];
        max_title = song_title;
                }
        }
	print max_title;
}
' DataStreamer.csv)
echo "Lagu terpopuler adalah $max_title dengan Album :  "
awk -F, -v title="$max_title" 'NR>1 && $2 == title {print "Album:", $1, "| Tahun:", $3}' AlbumDetails.csv

