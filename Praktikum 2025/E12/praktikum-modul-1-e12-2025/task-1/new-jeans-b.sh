echo " == Lagu dengan Streaming Terbanyak == "
awk -F, 'NR>1 && $2 ~ /[0-9]/ {songs[$3]++;}
END {
	max_count = 0;
	for(song_title in songs){
	if (songs[song_title] > max_count){
	max_count = songs[song_title];
	max_title = song_title;
		}
	}

print "Judul Lagu : " max_title "\nTotal User : " max_count "\n";
if (max_count < 24){
print "Maaf, Minji, total streamingnya tidak sesuai harapan :(";
}
else{
print "Keren, Minji! Kamu hebat <3!";
}
}
' DataStreamer.csv
