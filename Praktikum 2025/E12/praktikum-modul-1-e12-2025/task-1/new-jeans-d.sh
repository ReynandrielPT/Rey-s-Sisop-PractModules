awk -F, 'NR > 1 {device = $7;user=$2;duration = $4 + 0;song_title = $3;

	new_jeans[device]++;
	

	if (!(device "," user in seen_all)){
	seen_all[device "," user] = 1;
	user_count[device]++;}

	duration_sum[device] += duration;
}
END{
	max_users = 0;
	max_duration = 0;
	max_loyalty = 0;

	for (device_name in user_count){
	loyalty = duration_sum[device_name]/user_count[device_name];
	if (user_count[device_name]>max_users){
	max_users = user_count[device_name];
	popular_device = device_name;
}
	if (duration_sum[device_name] > max_duration){
	max_duration = duration_sum[device_name];
	duration_device = device_name;
}
	if (loyalty > max_loyalty){
	max_loyalty = loyalty;
	loyal_device = device_name;
}
}
print "\n== Total User Streaming New Jeans per Device ==";
	for (device_name in new_jeans){
	print "Device :", device_name, "| Total User:", new_jeans[device_name];
}
print "\n== Total Streaming per Device ==";
	for (device_name in duration_sum){
	print "Device :", device_name, "| Total Streaming :", duration_sum[device_name], "detik";
}
print "\n== Statistik Device ==";
print "Device ter-populer :", popular_device;
print "Device total streaming tertinggi: ",duration_device;
print "Device ter-loyal :",loyal_device;

}' DataStreamer.csv



