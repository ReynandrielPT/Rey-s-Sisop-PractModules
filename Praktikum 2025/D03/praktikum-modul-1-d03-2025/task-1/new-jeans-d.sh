#!/bin/bash
awk -F',' '
BEGIN {
  waktu_tablet = 0;
  waktu_desktop = 0;
  waktu_smart_tv = 0;
  waktu_mobile = 0;
  count_tablet = 0;
  count_desktop = 0;
  count_smart_tv = 0;
  count_mobile = 0;
  popular_devices = 0;
  popular_index = 0;
  most_stream = 0;
  most_index = 0;
  loyal = 0;
  loyal_index = 0;
}

/Tablet/ {
    waktu_tablet = $4 + waktu_tablet;
    count_tablet++;
}

/Desktop/ {
    waktu_desktop = $4 + waktu_desktop;
    count_desktop++;
}

/Smart TV/ {
    waktu_smart_tv = $4 + waktu_smart_tv;
    count_smart_tv++;
}

/Mobile/ {
    waktu_mobile = $4 + waktu_mobile;
    count_mobile++;
}

END {
    count[1] = count_tablet;
    count[2] = count_desktop;
    count[3] = count_smart_tv;
    count[4] = count_mobile;

    waktu[1] = waktu_tablet;
    waktu[2] = waktu_desktop;
    waktu[3] = waktu_smart_tv;
    waktu[4] = waktu_mobile;
    
    devices[1] = "Tablet";
    devices[2] = "Desktop";
    devices[3] = "Smart TV";
    devices[4] = "Mobile";
    
    for (i = 1; i <= 4; i++) {
        device = devices[i];
        print "Jumlah stream", device,":", count[i], "devices Total stream:", waktu[i], "second";
    }
    for (j = 1; j <= 4; j++) {
        if (count[j] > popular_devices) {
          popular_devices = count[j];
          popular_index = j;
        }
        if (waktu[j] > most_stream) {
          most_stream = waktu[j];
          most_index = j;
        }
        if (waktu[j]/count[j] > loyal) {
          loyal = waktu[j]/count[j];
          loyal_index = j;
        }
    }
    print "Popular device:", devices[popular_index];
    print "Most stream device:", devices[most_index];
    print "Most loyal device:", devices[loyal_index];
}
' DataStreamer.csv

