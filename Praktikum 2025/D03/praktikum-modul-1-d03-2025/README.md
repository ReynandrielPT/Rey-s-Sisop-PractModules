[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/Ph837wyE)
|    NRP     |      Name      |
| :--------: | :------------: |
| 5025241068 | Riyan Fadli Amazzadin |
| 5025241100 | Liem, Alfred Haryanto |
| 5025221120 | Raditya Zhafran Pranuja |

# Praktikum Modul 1 _(Module 1 Lab Work)_

</div>

### Daftar Soal _(Task List)_

- [Task 1 - New Jeans Global Teratas _(Top Global New Jeans)_](/task-1/)

- [Task 2 - Liburan Bersama Rudi _(Holiday with Rudi)_](/task-2/)

- [Task 3 - Ignatius Si Cloud Engineer _(Ignatius The Cloud Engineer)_](/task-3/)

- [Task 4 - Proxy Terbaik di New Eridu _(Best Proxy in New Eridu)_](/task-4/)

### Laporan Resmi Praktikum Modul 1 _(Module 1 Lab Work Report)_

# Task 1 _(Top Global New Jeans)_
Setelah comeback besar-besaran, NewJeans memecahkan berbagai rekor di chart musik global. Oleh karena itu, muncul lah niat Minji untuk menganalisis performa album mereka selama 2024. Manajer Minji memberikan kepada Minji data streaming dari platform “N” melalui link [newjeans_analysis.zip](https://drive.google.com/file/d/1poxmppuB7zOXOVm9Xn7_FtSRwtC10dbw/view?usp=sharing). Data tersebut mencakup data streamer dan detail album NewJeans. Sebagai penggemar NewJeans dan juga seorang suhu dalam dunia Linux, kamu ditantang oleh Minji untuk memenuhi rasa ingin tahunya. Minji ingin kamu untuk menganalisis data yang diberikan oleh manajernya menggunakan shell scripting dan awk.
#### Contoh ``AlbumDetails.csv``
```
Album,Song Title,Release Year,Label
New Jeans,Attention,2022,ADOR
New Jeans,Hype Boy,2022,ADOR
New Jeans,Cookie,2022,ADOR
OMG,OMG,2023,ADOR
```
#### Contoh ``DataStreamer.csv``
```
id,username,song_title,total_stream_time,country,last_stream_date,last_device_used
1,Anomaly,New Jeans,11868,Australia,2024-07-26,Tablet
2,Asmongold,OMG,10016,Argentina,2024-10-03,Smart TV
3,sho12tpeeps,Hype Boy,12242,Saudi Arabia,2024-02-08,Desktop
4,ibai,Cookie,19002,Chile,2024-12-02,Smart TV
5,sodapoppin,Ditto,11640,Italy,2024-12-01,Desktop
```
### Poin A
Minji penasaran dengan nama-nama user yang mendengarkan lagunya pada platform “N”. Berhubung Minji sangat menyukai angka **2** dan ia sangat membenci orang yang membuat username menggunakan **underscore (\_)**, bantulah Minji untuk mendapatkan username yang sesuai dengan kriterianya (**terdapat angka 2 dan tidak terdapat underscore**) dan tampilkan jumlahnya. **Pastikan username yang ditampilkan berurutan berdasarkan ASCII Table (dari kecil ke besar).**
#### Solusi
```bash
#!/bin/bash
awk -F',' 'NR > 1 {print $2}' DataStreamer.csv | grep "2" | grep -v "_" | sort | 
  awk '{{print} counter++}; 
  END {
    print "Jumlah Username sesuai kriteria:", counter}'
```
#### Penjelasan
```bash
#!/bin/bash
awk -F',' 'NR > 1 {print $2}' DataStreamer.csv | grep "2" | grep -v "_" | sort 
```
- ``awk`` disini berfungsi untuk mengambil username yang ada di kolom ke-2 dari file ``DataStreamer.csv`` dengan cara ``print $2``.
- ``grep`` disini untuk mengambil username yang terdapat angka "2" dan menambah ``-v`` untuk mengambil username yang tidak terdapat "_".
- ``sort`` disini berfungsi untuk mengurutkan hasil username tadi berdasarkan ASCII dari kecil ke besar.
```bash
awk '{{print} counter++}; 
END {print "Jumlah Username sesuai kriteria:", counter}'
```
Fungsi ``awk`` yang kedua ini untuk menghitung username yang sudah tersaring dengan kriteria ada angka "2" dan tidak ada "_" dengan cara increment sebuah variabel setiap membaca satu baris.
#### Output
![image](https://github.com/rdtzaa/assets/blob/358914fa4d8f240c2a3d59bd173643d18bddb024/Sistem%20Operasi/newjeans_a_1.png)
![image](https://github.com/rdtzaa/assets/blob/358914fa4d8f240c2a3d59bd173643d18bddb024/Sistem%20Operasi/newjean_a_2.png)

### Poin B
Ternyata, Minji tidak hanya menyukai angka 2, tetapi semua angka. Ia ingin mengetahui lagu apa yang paling banyak di-streaming oleh user yang memiliki username berangka. Beri tahu Minji nama lagu tersebut dan jumlah usernya. Jika jumlah user tersebut tidak mencapai 24, maka berikan pesan kepada Minji “Maaf, Minji, total streamingnya tidak sesuai harapan :(“. Jika jumlah user mencapai atau melebihi 24, berikan pesan kepada Minji “Keren, Minji! Kamu hebat <3!”.
#### Solusi
```bash
#!/bin/bash

awk -F',' '{ if ($2 ~ /[0-9]/ && !($2 ~ /_/)) count[$3]++} 
END {
    for (song in count) {
        if (count[song] > max_count) {
            max_count = count[song]
            max_song = song
        }
    }
    print "Pendengar lagu", max_song, "berjumlah", max_count, "pendengar"
    if (max_count > 24) {
        print "Keren, Minji! Kamu hebat <3!"
    } else {
        print "Maaf, Minji, total streamingnya tidak sesuai harapan :("}}
' DataStreamer.csv
```
#### Penjelasan
```bash
awk -F',' '{ if ($2 ~ /[0-9]/ && !($2 ~ /_/)) count[$3]++}
```
- ``awk -F','`` disini untuk membaca baris data dengan pemisah antar kata nya adalah tanda ','
- ``if ($2 ~ /[0-9]/ && !($2 ~ /_/))``, kondisi tersebut untuk mencari baris data yang kolom kedua yaitu username memiliki sebuah angka dan tidak memiliki "_".
- ``count[$3]++``, untuk menghitung banyak lagu yang diputar oleh para username yang memenuhi kriteria berdasarkan judul lagu nya dengan cara counter setiap baris yang memenuhi dengan index judul lagu yang diputar.
```bash
END {
    for (song in count) {
        if (count[song] > max_count) {
            max_count = count[song]
            max_song = song
        }
    }
    print "Pendengar lagu", max_song, "berjumlah", max_count, "pendengar"
    if (max_count > 24) {
        print "Keren, Minji! Kamu hebat <3!"
    } else {
        print "Maaf, Minji, total streamingnya tidak sesuai harapan :("}}
' DataStreamer.csv
```
- ``END`` adalah penanda fungsi-fungsi yang ada di dalam tanda kurung kurawal setelah fungsi ``END`` akan berjalan setelah semua baris data diseleksi.
- ``if (count[song] > max_count)``, kondisi tersebut untuk mencari lagu yang paling banyak diputar dengan melakukan perulangan terhadap index variabel counter berupa judul lagu yang telah dibuat sebelumnya.
- ``print`` disana untuk menampilkan hasil lagu yang paling banyak didengar beserta jumlah pendengarnya
- ``if (max_count > 24)`` melakukan pengecekan jumlah pendengar yang mana jika jumlah pendengar lebih dari 24 pendengar akan mengeluarkan output "Keren, Minji! Kamu hebat <3!" sebaliknya jika tidak memenuhi kondisi akan menghasilkan output "Maaf, Minji, total streamingnya tidak sesuai harapan :("
- ``DataStreamer.csv`` adalah file data dari tiap-tiap username yang mendengarkan lagu melalui platform "N".
#### Output
![image](https://github.com/rdtzaa/assets/blob/358914fa4d8f240c2a3d59bd173643d18bddb024/Sistem%20Operasi/newjeans_b.png)
### Poin C
Tiba-tiba Minji lupa, di album mana lagu yang paling banyak di-streaming tersebut berada. Carikan Minji nama album dari lagu yang paling banyak di-streaming di platform tersebut, beserta tahun rilisnya!
#### Solusi
```bash
#!/bin/bash

nama_lagu=$(awk -F',' '{if ($2 ~ /[0-9]/ && !($2 ~ /_/)) count[$3]++} 
END {
    for (song in count) {
        if (count[song] > max_count) {
            max_count = count[song]
            max_song = song
        }
    }
}
END {print max_song}
' DataStreamer.csv)

awk -F',' -v song=$nama_lagu '
{
    if ($2 == song) {
    print "Lagu", song, "dari album", $1, "rilis tahun", $3
    }
}
' AlbumDetails.csv
```
#### Penjelasan
```bash
nama_lagu=$(awk -F',' '{if ($2 ~ /[0-9]/ && !($2 ~ /_/)) count[$3]++} 
END {
    for (song in count) {
        if (count[song] > max_count) {
            max_count = count[song]
            max_song = song
        }
    }
}
END {print max_song}
' DataStreamer.csv)
```
Fungsi diatas hampir sama dengan poin b sebelumnya yaitu mencari lagu yang paling banyak diputar oleh username yang memiliki setidaknya satu angka dan tidak ada "_". Perbedaanya adalah disini hanya mencetak nama lagu di bagian ``END {print max_song}`` lalu diassign ke variabel ``nama_lagu`` 
```bash
awk -F',' -v song=$nama_lagu '
{
    if ($2 == song) {
    print "Lagu", song, "dari album", $1, "rilis tahun", $3
    }
}
' AlbumDetails.csv
```
- ``awk -F',' -v song=$nama_lagu`` disini kita membaca file data album dengan pemisah tiap bagiannya adalah tanda koma lalu ditambahkan juga variabel ``nama_lagu`` dengan cara menambah ``-v [variabel]`` untuk kita cocokkan dengan file data album yang kita punya.
- ``if ($2 == song)`` kondisi ini untuk mencari nama lagu yang sesuai di dalam file data album yang kemudian diikuti ``print "Lagu", song, "dari album", $1, "rilis tahun", $3`` untuk mencetak nama album beserta tahun rilis yang berada di kolom pertama dan ketiga.
- ``AlbumDetails.csv`` adalah file data album yang akan dicocokkan dengan fungsi ``awk``
#### Output
![image](https://github.com/rdtzaa/assets/blob/358914fa4d8f240c2a3d59bd173643d18bddb024/Sistem%20Operasi/newjeans_c.png)
### Poin D
Minji ingin mengajukan ke agensi untuk membuat campaign iklan supaya lagu-lagu NewJeans lebih dikenal, tapi Minji bingung mau optimasi iklan untuk device apa. Berikut tugas kamu untuk membantu Minji menyusun proposal iklan sebelum diberikan ke CEO ADOR:

1.  **Hitung berapa banyak pengguna** yang menggunakan setiap jenis device (**Tablet, Smart TV, Desktop, Mobile**) untuk streaming lagu NewJeans.
2.  **Hitung total durasi streaming (detik)** untuk setiap device.
3.  **Tentukan:**

    - **Device paling populer** yang punya user terbanyak.
    - **Device menang durasi** yang memiliki total streaming tertinggi.
    - **Device ter-“loyal”** berdasarkan **rasio (total durasi / user) tertinggi**.
#### Solusi
```sh
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
```
#### Penjelasan
```bash
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
```
Disini kita menggunakan fungsi ``awk`` dengan pemisah kata nya adalah tanda koma. Selanjutnya, terdapat bagian ``BEGIN`` yang akan berjalan sebelum pengecekan tiap baris dimulai berisi variabel-variabel yaitu ``count`` untuk jumlah devices, ``waktu`` adalah jumlah streaming dengan satuan detik, ``popular_x`` yang akan digunakan untuk menyimpan nilai device paling populer, ``most_stream`` digunakan untuk menyimpan nilai device yang paling banyak melakukan streaming, dan variabel ``loyal`` yang akan digunakan kaitannya mencari device yang paling loyal. 
```bash
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
```
Fungsi diatas adalah bagian yang akan dijalankan setiap ganti baris data ketika pengecekan. ``/....../`` berguna untuk membandingkan setiap baris data yang mana jika kata yang ada dalam bagian tersebut akan berjalan fungsi yang berada setelah bagian tersebut. Untuk menghitung waktu kita menjumlahkan nilai waktu dalam detik yang terdapat di kolom ke empat ditambah nilai waktu sebelumnya lalu melakukan increment variabel counter.
```bash
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
```
Mengassign beberapa variabel array dengan variabel yang terdapat dibagian ``BEGIN`` untuk memudahkan dalam melakukan looping.
```bash
for (i = 1; i <= 4; i++) {
  device = devices[i];
  print "Jumlah stream", device,":", count[i], "devices Total stream:", waktu[i], "second";
}
```
Melakukan looping untuk mencetak jumlah setiap device yang digunakan untuk streaming beserta total waktu dengan menggunakan variabel array ``count`` dan ``waktu``.
```bash
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
```
Melakukan sebuah looping kembali untuk mencari popular device dengan kondisi ``count[j] > popular_devices``, paling banyak stream dengan kondisi ``waktu[j] > most_stream``, dan terakhir menghitung loyalitas dengan membagi waktu stream dan jumlah device ``waktu[j]/count[j] > loyal``
- ``print`` untuk mencetak device paling populer, device paling banyak digunakan, dan device yang paling loyal.
- ``DataStreamer.csv`` adalah file data streaming.
#### Output
![image](https://github.com/rdtzaa/assets/blob/358914fa4d8f240c2a3d59bd173643d18bddb024/Sistem%20Operasi/newjeans_d.png)
## Kendala
Pada soal poin D, saya terlalu banyak inisialisasi variabel untuk menghitung device dan waktu yang seharusnya bisa dipersingkat menggunakan array associative dengan index yaitu nama device. Ketika menggunakan array associative saya tidak perlu menuliskan secara manual device apa saja yang ada sehingga lebih fleksibel untuk membaca data-data lainnya.
# Task 2 _(Liburan Bersama Rudi)_
Mengisi waktu liburan, Rudi membuat sebuah website untuk personal brandingnya yang sementara berjalan di local pada komputer laboratorium. Rudi yang baru belajar kebingungan karena sering muncul error yang tidak ia pahami. Untuk itu dia meminta ketiga temannya Andi, Budi, dan Caca untuk melakukan pengecekan secara berkala pada website Rudi. Dari pengecekan secara berkala, Rudi mendapatkan sebuah file [access.log](https://drive.google.com/file/d/1yf4lWB4lUgq4uxKP8Pr8pqcAWytc3eR4/view?usp=sharing) yang berisi catatan akses dari ketiga temannya. 
### Poin A
Karena melihat ada IP dan Status Code pada file access.log. Rudi meminta praktikan untuk menampilkan total request yang dibuat oleh setiap IP dan menampilkan jumlah dari setiap status code.
#### Solusi
```bash
str1=$(cat access.log | awk '{print $1}' | awk '{count[$1]++} END {for (ip in count) print "Jumlah request dari", ip, "=", count[ip]}')
str2=$(cat access.log | awk '{count[$9]++} END {for (code in count) print "Jumlah status code", code, "=", count[code]}')

echo "$str1"
echo "$str2"
```
#### Penjelasan
```bash
str1=$(cat access.log | awk '{print $1}'
```
- ``str1=${...}`` untuk menyimpan data hasil operasi yang berada di dalam tanda kurung kurawal.
- ``cat.access.log`` untuk mencetak isi dari file ``access.log`` berupa log akses sebuah web.
- ``awk '{print $1}'`` untuk mencetak kolom pertama dari hasil dari cetakan ``cat`` sebelumnya yaitu berupa ip address.
```bash
awk '{count[$1]++} END {for (ip in count) print "Jumlah request dari", ip, "=", count[ip]}')
```
Fungsi ``awk`` disini membuat sebuah variabel yang akan increment dengan index yang berasal dari kolom pertama hasil cetakan operasi sebelumnya. Di bagian ``END`` yang akan dijalankan setelah semua baris data dilewati, kita menambahkan looping terhadap index yang terdapat pada variabel ``count`` dengan cara ``(ip in count)`` lalu kita cetak jumlah request setiap ip.
```bash
str2=$(cat access.log | awk '{count[$9]++} END {for (code in count) print "Jumlah status code", code, "=", count[code]}')
```
- ``str2=${....}`` untuk menyimpan data hasil operasi yang berada di dalam tanda kurung kurawal.
- ``cat.access.log`` untuk mencetak isi dari file ``access.log`` berupa log akses sebuah web.
- ``awk`` disini diikuti operasi seperti di variabel ``str1`` yaitu kita membuat variabel counter tetapi disini index nya adalah kolom ke sembilan dari file log yaitu status code nya. Di bagian ``END`` hampir sama dengan variabel ``str1`` juga, kita akan melakukan looping terhadap index-index yang ada dalam variabel ``count`` dengan cara ``(code in count)`` lalu kita cetak jumlah dari setiap status code.
#### Output
![image](https://github.com/rdtzaa/assets/blob/e603d07387e822ee2f74d0e51ffaf069a5e84929/Sistem%20Operasi/rudi-a.png)
### Poin B
Karena banyaknya status code error, Rudi ingin tahu siapa yang menemukan error tersebut. Setelah melihat-lihat, ternyata IP komputer selalu sama. Dengan bantuan [peminjaman_komputer.csv](https://drive.google.com/file/d/1-aN4Ca0M3IQdp6xh3PiS_rLQeLVT1IWt/view?usp=drive_link), Rudi meminta kamu untuk membuat sebuah program bash yang akan menerima inputan tanggal dan IP serta menampilkan siapa pengguna dan membuat file backup log aktivitas, dengan format berikut:

- **Tanggal** (format: `MM/DD/YYYY`)

- **IP Address** (format: `192.168.1.X`, karena menggunakan jaringan lokal, di mana `X` adalah nomor komputer)

- Setelah pengecekan, program akan memberikan **message pengguna dan log aktivitas** dengan format berikut:

  ```
  Pengguna saat itu adalah [Nama Pengguna Komputer]
  Log Aktivitas [Nama Pengguna Komputer]
  ```

  atau jika data tidak ditemukan:

  ```
  Data yang kamu cari tidak ada
  ```

- File akan disimpan pada directory “/backup/[Nama file]”, dengan format nama file sebagai berikut

  ```
  [Nama Pengguna Komputer]_[Tanggal Dipilih (MMDDYYY)]_[Jam saat ini (HHMMSS)].log
  ```

- Format isi log

  ```
  [dd/mm/yyyy:hh:mm:ss]: Method - Endpoint - Status Code
  ```
#### Solusi
```bash
#!/bin/bash

convert_date_format() {
    temp_date=$1
    month=${temp_date:0:2}
    day=${temp_date:3:2}
    year=${temp_date:6:4}

    months=("Jan")
    month_name=${months[$((month-1))]}
    formatted_date="$day/$month_name/$year"

    echo "$formatted_date"
}

echo -e "Masukkan tanggal (MM/DD/YYYY)"
read tanggal
echo -e "Masukkan IP Address (192.168.1.X)"
read ip_address

number_pc=$(echo "$ip_address" | awk -F'.' '{print $4}')

echo "Nomor komputer berdasarkan IP adalah: $number_pc"

nama_pengguna=$(grep "$tanggal,$number_pc" peminjaman_computer.csv | awk -F',' '{print $3}')

if [ -z "$nama_pengguna" ]; then
    echo "Nama pengguna tidak ditemukan"
else
    echo "Pengguna saat itu adalah $nama_pengguna"
    echo "Log Aktivitas $nama_pengguna"
    formatted_date=$(convert_date_format "$tanggal")
    waktu=$(date | awk '{print $4}' | awk -F':' '{print $1$2$3}')
    dir="${nama_pengguna}_$(echo $tanggal | awk -F'/' '{print $1$2$3}')_$waktu.log"
    mkdir -p "backup"
    awk -v tgl=$formatted_date -v ip=$ip_address '
        $1 == ip && $4 ~ tgl {
        date = substr($4, 2, 20)
        metohod = substr($6, 2)
        print "[" date "]:", metohod, "-", $7, "-", $9}' access.log > backup/$dir
    echo "$dir berhasil dibuat"
fi
```
#### Penjelasan
```bash
convert_date_format() {
    temp_date=$1
    month=${temp_date:0:2}
    day=${temp_date:3:2}
    year=${temp_date:6:4}

    months=("Jan")
    month_name=${months[$((month-1))]}
    formatted_date="$day/$month_name/$year"

    echo "$formatted_date"
}
```
Inisialisasi fungsi ``convert_date_format()`` untuk mengganti format tanggal dari ``MM/DD/YYYY`` menjadi ``DD/nama bulan/YYYY``.
- ``month=${temp_date:0:2}``, assign variabel ``month`` dengan cara mengambil 2 huruf mulai index 0 berupa bulan dari sebuah input tanggal fungsi tersebut.
- ``day=${temp_date:3:2}``, assign variabel ``day`` dengan cara mengambil 2 huruf mulai index 3 berupa hari dari sebuah input tanggal fungsi tersebut.
- ``year=${temp_date:6:4}``, assign variabel ``year`` dengan cara mengambil 4 huruf mulai index 6 berupa tahun dari sebuah input tanggal fungsi tersebut.
- ``months=("Jan")`` variabel array untuk convert bulan dari angka menjadi nama bulan.
- ``month_name=${months[$((month-1))]}``, operasi untuk convert angka bulan dengan cara memasukkan angka bulan yang sudah kita simpan di variabel ``month`` ke dalam index lalu kita kurangi satu karena array dimulai dari index 0 sedangkan angka bulan Januari adalah "1".
- ``formatted_date="$day/$month_name/$year"`` assign ke sebuah variabel dengan format yang sesuai yaitu ``DD/nama_bulab/YYYY``
- ``echo "$formatted_date"``, fungsi untuk mencetak hasil dari convert tanggal.
```bash
echo -e "Masukkan tanggal (MM/DD/YYYY)"
read tanggal
echo -e "Masukkan IP Address (192.168.1.X)"
read ip_address
```
- ``echo`` bagian tersebut merupakan bagian untuk menerima input diawal dengan tambahan ``-e`` agar ketika di dalam terminal inputan kita dapat berada di samping cetakan ``echo``.
- ``read`` disini berfungsi untuk menyimpan hasil input dari user kedalam suatu variabel.
- Input yang diterima disini terdapat tanggal dengan format ``(MM/DD/YYYY)`` dan ip address dengan format ``(192.168.1.X)``
```bash
number_pc=$(echo "$ip_address" | awk -F'.' '{print $4}')
```
- Inisialisasi variabel ``number_pc`` yang menyimpan nomor komputer berdasarkan ip address.
- ``echo`` mencetak isi variabel ip address.
- ``awk`` dengan pembatas tanda titik mengambil kolom ke empat yang merupakan angka ip terakhir sebagai nomor komputer.
```bash
nama_pengguna=$(grep "$tanggal,$number_pc" peminjaman_computer.csv | awk -F',' '{print $3}')
```
- ``nama_pengguna`` sebuah variabel yang digunakan untuk menyimpan nama pengguna komputer berdasarkan ip address dan tanggal peminjaman.
- ``grep`` disini akan mengambil baris data dari ``peminjaman_computer.csv`` yang memiliki pola ``"$tanggal,$number_pc"`` sehingga kita dapat mencetak nama pengguna yang berada di kolom ke 3 file csv tersebut.
```bash
if [ -z "$nama_pengguna" ]; then
    echo "Nama pengguna tidak ditemukan"
else
    echo "Pengguna saat itu adalah $nama_pengguna"
    echo "Log Aktivitas $nama_pengguna"
```
``if [ -z .... ]`` berfungsi untuk melihat apakah variabel tersebut kosong atau tidak. Sesuai ketentuan, kita akan mencetak "Nama pengguna tidak ditemukan" karena variabel ``nama_pengguna`` kosong yang artinya tidak ada kecocokan di file ``peminjaman_computer.csv`` begitu juga sebaliknya akan mengeluarkan output sesuai ketentuan soal diatas.
```bash
waktu=$(date | awk '{print $4}' | awk -F':' '{print $1$2$3}')
```
Variabel waktu ini akan diisi waktu saat script ini dijalankan dengan menggunakan command ``date`` yang kemudian akan kita ambil bagian waktu dan memisahkan tanda titik dua nya juga dengan ``awk``
```bash
dir="${nama_pengguna}_$(echo $tanggal | awk -F'/' '{print $1$2$3}')_$waktu.log"
```
Dengan format file yang diminta adalah ``[Nama Pengguna Komputer]_[Tanggal Dipilih (MMDDYYY)]_[Jam saat ini (HHMMSS)].log`` maka saya masukkan bagian nama penggunakan komputer dengan variabel ``nama_pengguna``, tanggal dengan variabel ``$tanggal`` yang ditambahkan ``awk`` untuk menghilangkan tanda garis miring, dan jam saat ini dengan variabel ``waktu``.
```bash
mkdir -p "backup"
```
- ``mkdir`` disini untuk membuat folder bernama "backup" yang akan digunakan untuk menyimpan file log yang dicari dan disini ditambah ``-p`` yang mana ini untuk cek apakah directory tersebut ada dan jika ada dia tidak akan membuat folder lagi dan sebaliknya juga.
```bash
awk -v tgl=$formatted_date -v ip=$ip_address '
  $1 == ip && $4 ~ tgl {
  date = substr($4, 2, 20)
  metohod = substr($6, 2)
  print "[" date "]:", metohod, "-", $7, "-", $9}' access.log > backup/$dir
```
- ``awk -v [variabel]`` disini untuk membuat file log berdasarkan format dan kita juga menambahkan suatu variabel ke dalam ``awk``
- ``$1 == ip && $4 ~ tgl`` kondisi ini akan cek apakah kolom pertama dan kolom keempat dari tiap baris data ``access.log`` sudah sesuai dengan ip address dan tanggal yang ditentukan maka akan dilanjut proses yang ada di dalam tanda kurung kurawal setelahnya.
- ``date = substr($4, 2, 20)`` variabel date ini akan diisi dengan tanggal beserta hari sesuai format isi log yang diharapkan yaitu ``[dd/mm/yyyy:hh:mm:ss]``
- ``metohod = substr($6, 2)`` variabel method ini akan diisi oleh metode log akses web seperti ``DELETE``.
- ``print "[" date "]:", metohod, "-", $7, "-", $9}' access.log > backup/$dir`` disini akan mencetak data dengan format yang diharapkan yaitu ``[dd/mm/yyyy:hh:mm:ss]: Method - Endpoint - Status Code`` lalu dimasukkan ke dalam file log sesuai format juga.
#### Input & Output
![image](https://github.com/rdtzaa/assets/blob/e603d07387e822ee2f74d0e51ffaf069a5e84929/Sistem%20Operasi/rudi-b.png)
##### ``Caca_01262025_203527.log``
```
[26/Jan/2025:00:02:06]: GET - /index.html - 200
[26/Jan/2025:00:02:11]: GET - /login - 500
[26/Jan/2025:00:02:13]: PUT - /contact - 200
[26/Jan/2025:00:06:38]: PUT - /about.html - 200
[26/Jan/2025:00:09:24]: DELETE - /about.html - 200
[26/Jan/2025:00:11:13]: PUT - /contact - 200
[26/Jan/2025:00:11:56]: GET - /login - 200
[26/Jan/2025:00:12:48]: POST - /index.html - 200
[26/Jan/2025:00:16:05]: POST - /about.html - 302
[26/Jan/2025:00:17:47]: PUT - /login - 302
[26/Jan/2025:00:20:23]: PUT - /about.html - 404
...
```
### Poin C
Rudi ingin memberikan hadiah kepada temannya yang sudah membantu. Namun karena dana yang terbatas, Rudi hanya akan memberikan hadiah kepada teman yang berhasil menemukan server error dengan ``Status Code 500`` terbanyak. Bantu Rudi untuk menemukan siapa dari ketiga temannya yang berhak mendapat hadiah dan tampilkan jumlah ``Status Code 500`` yang ditemukan
#### Solusi
```bash
#!/bin/bash
declare -A count

str1=$(awk -F' ' 'BEGIN {months["Jan"]="01"}
$9 ~ 500 {
    tgl = substr($4, 2, 11)
    split(tgl, splitted, "/")
    peminjam = months[splitted[2]] "/" splitted[1] "/" splitted[3] " " substr($1, 11, 1)
    count[peminjam]++
}
END {for (i in count) print i " " count[i]}' access.log)

while read tanggal komputer aktivitas; do
    index=$(awk -v tgl=$tanggal -v komp=$komputer -F',' '{if ($1 ~ tgl && $2 ~ komp) print $3}' peminjaman_computer.csv)
    ((count[$index]+=$aktivitas))
    # echo "${count[$index]}"
done <<< "$str1"

for peminjam in "${!count[@]}"; do
    echo "$peminjam mendapatkan Status Code 500 sebanyak ${count[$peminjam]} kali"
    ((total+=count[$peminjam]))
    if [[ $max_count -lt ${count[$peminjam]} ]]; then
        max_count=${count[$peminjam]}
        max_name=$peminjam
    fi
done

echo "Total Status Code yang ditemukan adalah $total kali"
echo "Selamatt!!! $max_name mendapatkan hadiah dari Rudi"
```
#### Penjelasan
```bash
str1=$(awk -F' ' 'BEGIN {months["Jan"]="01"}
$9 ~ 500 {
    tgl = substr($4, 2, 11)
    split(tgl, splitted, "/")
    peminjam = months[splitted[2]] "/" splitted[1] "/" splitted[3] " " substr($1, 11, 1)
    count[peminjam]++
}
END {for (i in count) print i " " count[i]}' access.log)
```
- Inisialisasi variabel ``str1`` yang akan diisi log dengan ``Status Code 500`` dikelompokkan berdasarkan tanggal dan nomor komputer.
- ``$9 ~ 500`` mencari baris yang memenuhi kondisi untuk melanjutkan fungsi.
- ``tgl = substr($4, 2, 11)`` dan ``split(tgl, splitted, "/")`` untuk memisahkan perbagian tanggal dari ``access.log`` dengan menghilangkan tanda garis miring.
- ``peminjam = months[splitted[2]] "/" splitted[1] "/" splitted[3] " " substr($1, 11, 1)`` membuat variabel peminjam sebagai index dengan format ``MM/DD/YYYY [Nomor Komputer]``
- ``count[peminjam]++`` sebuah counter dengan index peminjam.
- ``for (i in count) print i " " count[i]}' access.log`` mencetak tanggal, nomor komputer, dan jumlah aktivitas.
```
while read tanggal komputer aktivitas; do
    index=$(awk -v tgl=$tanggal -v komp=$komputer -F',' '{if ($1 ~ tgl && $2 ~ komp) print $3}' peminjaman_computer.csv)
    ((count[$index]+=$aktivitas))
    # echo "${count[$index]}"
done <<< "$str1"
```
- ``while read tanggal komputer aktivitas; do ... <<< "$str1"`` membaca tiap baris dari variabel ``str1`` dengan pemisah default nya adalah tanda spasi yang kemudian dimasukkan ke variabel tanggal, komputer, dan aktivitas.
- ``index=$(awk -v tgl=$tanggal -v komp=$komputer -F',' '{if ($1 ~ tgl && $2 ~ komp) print $3}' peminjaman_computer.csv)`` membuat index berupa nama peminjam yang dicocokkan dengan tanggal dan nomor komputer yang ada di dalam data ``peminjaman_computer.csv``
- ``((count[$index]+=$aktivitas))`` counter untuk menambah aktivitas berdasarkan nama peminjam.
```bash
for peminjam in "${!count[@]}"; do
    echo "$peminjam mendapatkan Status Code 500 sebanyak ${count[$peminjam]} kali"
    ((total+=count[$peminjam]))
    if [[ $max_count -lt ${count[$peminjam]} ]]; then
        max_count=${count[$peminjam]}
        max_name=$peminjam
    fi
done
```
- Disini kita melakukan looping terhadap index count dengan cara ``${count[@]}``
- ``echo`` disini untuk mencetak banyaknya ``Status Code 500`` yang ditemukan setiap orang melalui variabel peminjam
- ``((total+=count[$peminjam]`` untuk menjumlah semua ``Status Code 500`` yang ditemukan.
- ``if``, kondisi ini untuk menentukan siapa yang paling banyak menemukan sehingga berhak mendapat hadiah.
```bash
echo "Total Status Code yang ditemukan adalah $total kali"
echo "Selamatt!!! $max_name mendapatkan hadiah dari Rudi"
```
Menampilkan total dan nama teman Rudi yang berhak mendapat hadiah.
#### Output
![image](https://github.com/rdtzaa/assets/blob/e603d07387e822ee2f74d0e51ffaf069a5e84929/Sistem%20Operasi/rudi-c.png)
### Kendala
Pada poin C, saya sempat kesulitan untuk menentukan counter setiap ip karena ternyata di data ``peminjaman_komputer.csv`` setiap teman Rudi bisa berpindah komputer. Oleh karena itu, saya mengelompokkan terlebih dahulu berdasarkan tanggal dan IP lalu tanggal dan IP dicocokkan dengan data ``peminjaman_komputer.csv`` dan index counter adalah nama teman-teman nya.
# Task 3 _(Ignatius si Cloud Engineer)_
### A. Register & Login
- Login dan Register: Sistem harus memastikan hanya pengguna terdaftar yang dapat mengakses layanan. Proses registrasi dilakukan melalui script register.sh dan data pengguna yang berhasil didaftarkan disimpan di /home/$user/cloud_storage/users.txt. Proses login dilakukan melalui script login.sh dan semua aktivitas login atau register dicatat dalam file cloud_log.txt.
- Data pengguna disimpan dalam: /home/$user/cloud_storage/users.txt
  * Tidak boleh terdapat username yang sama
  * Kriteria Password:
  * Minimal 8 karakter.
  * Mengandung setidaknya satu huruf kapital, satu angka, dan satu karakter spesial (misalnya: @, #, $, dll.).
  * Password tidak boleh sama dengan username.
  * Tidak boleh mengandung kata "cloud" atau "storage".
- Pencatatan Log: Semua aktivitas dicatat dalam cloud_log.txt dengan format: YY/MM/DD hh:mm:ss MESSAGE
  Contoh pesan log:
  * Jika pengguna mencoba register dengan username yang sudah ada:
    YY/MM/DD hh:mm:ss REGISTER: ERROR User already exists
  * Jika pengguna mencoba register tetapi tidak sesuai dengan kriteria password:
    YY/MM/DD hh:mm:ss REGISTER: ERROR {Penyebab Error}
  * Jika register berhasil:
    YY/MM/DD hh:mm:ss REGISTER: INFO User registered successfully
  * Jika login gagal:
    YY/MM/DD hh:mm:ss LOGIN: ERROR Failed login attempt on user {USERNAME}
  * Jika login berhasil: 
    YY/MM/DD hh:mm:ss LOGIN: INFO User {USERNAME}logged in
  * Jika logout berhasil: 
    YY/MM/DD hh:mm:ss LOGOUT: INFO User {USERNAME} logged out

Catatan: Sistem hanya mengizinkan satu pengguna login pada satu waktu. Jika sudah ada pengguna aktif berdasarkan log, login dari pengguna lain tidak diproses sampai sesi sebelumnya berakhir (User yang sedang login melakukan logout).

### Solusi
0. Variabel yang digunakan untuk path : 
```sh
usr="/home/freddskiii/cloud_storage/users.txt"
log="/home/freddskiii/cloud_storage/cloud_log.txt"
```


1. Untuk memastikan hanya pengguna terdaftar yang dapat melakukan login
```sh
usrEntry=$(grep "^$usn;" "$usr")
if [ -z "$usrEntry" ]; then
    log_message "LOGIN: Gagal login untuk user $usn (username tidak ada)"
    echo "Error: Username tidak ada!"
    exit 1
fi
```
Penjelasan : 
- ``grep`` berguna untuk mencari akun (mencakup username dan password) dari file users.txt. Dalam ``^$usr;``, ``^`` berarti awal baris, ``$usn`` adalah username yang akan dicari, dan ``;`` adalah separator yang saya gunakan untuk memisahkan username dengan password. Dengan ketentuan tersebut, ``grep`` akan mengambil line dimana terdapat username yang sesuai dengan ``$usn``
- Dalam if statement ``if [ -z "$usrEntry" ]``, ``-z`` berfungsi untuk mengecek apakah sebuah variabel/string kosong atau tidak, bila string kosong maka dengan ``-z`` akan mengembailan value 1 (true). Dalam code tersebut, apabila usrEntry tidak menemukan username yang sesuai, if statement akan dijalankan dan memberikan error berupa 'username tidak ada'.

Contoh output log : 
```
25/03/28 18:06:58 LOGIN: Gagal login untuk user freddskiii (username tidak ada)
```


2. Proses registrasi melalui ``register.sh`` dan data pengguna yang berhasil mendaftar disimpan di ``users.txt``
``` sh
#Segmen 1 : input username dan password
echo -n "Username : "
read usn
echo -n "Password : "
read pw

#Segmen 2 : mengecek apakah username sudah terdaftar pada users.txt
if grep -q "^$usn;" "$usr"; then
    log_message "REGISTER: ERROR Username tersebut sudah terdaftar!"
    echo "Error: Username tersebut sudah terdaftar!"
    exit 1
fi

#Segmen 3 : pengecekan password (kriteria minimal password)
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

#Segmen 3,5 : akan dijalankan apabila password tidak sesuai ketentuan
if [ "$error" ]; then
    log_message "REGISTER: ERROR $error"
    echo "$error"
    exit 1
fi

#Segmen 4 : bila pengguna berhasil mendaftar
echo "$usn;$pw" >> "$usr"
log_message "REGISTER: User baru berhasil didaftarkan"
echo "Registrasi berhasil!"
```
Penjelasan : 
- ``Segmen 1`` digunakan untuk user melakukan input username dan password ke variabel ``usn`` dan ``pw``.
- ``grep`` pada ``Segmen 2`` mirip seperti pada nomor 1, dimana ``grep`` akan mencari username di ``users.txt`` yang sesuai dengan ``usn``, apabila ada maka akan ada error bahwa username tersebut sudah ada di ``users.txt``.
- ``Segmen 3`` dan ``Segmen 3,5`` digunakan untuk mengecek password ketika user melakukan register, dimana password harus sesuai dengan kriteria (minimal 8 karakter, memiliki huruf kapital, memiliki digit, dst). Variabel ``error`` di deklarasikan kosong sebelum if statement, kemudian pengecekan password dimulai di if statement. Apabila terdapat error (tidak sesuai kriteria), variabel ``error`` akan di-assign value sesuai dengan error/ketidaksesuaian password dengan kriteria. Di ``Segmen 3,5``, apabila ``error`` kosong (tidak ada error), maka if statement tersebut tidak akan dijalankan, namun apabila ``error`` tidak kosong, maka if statement akan dijalankan mengeluarkan error sesuai dengan kekurangan password yang diinput oleh user.
- ``Segmen 4`` akan dijalankan apabila syarat username tidak boleh ada yang sama dan password sudah sesuai dengan kriteria terpenuhi. Username dan password yang diinput oleh user akan disimpan ke ``users.txt`` dengan separator ``;``.

Contoh log user berhasil untuk register : 
```
25/03/28 18:09:43 REGISTER: User baru berhasil didaftarkan
```

Contoh log user gagal register : 
```
25/03/25 21:52:47 REGISTER: ERROR Password terlalu pendek
25/03/25 21:57:29 REGISTER: ERROR Username tersebut sudah terdaftar!
25/03/25 21:57:52 REGISTER: ERROR Password harus memiliki karakter special {ex : !@#$%^&*}
```


3. Proses login dilakukan melalui script login.sh dan semua aktivitas login atau register dicatat dalam file cloud_log.txt
```sh
#Segmen 1
echo -n "Username: "
read usn
echo -n "Password: "
read pw

#Segmen 2
usrEntry=$(grep "^$usn;" "$usr")
if [ -z "$usrEntry" ]; then
    log_message "LOGIN: Gagal login untuk user $usn (username tidak ada)"
    echo "Error: Username tidak ada!"
    exit 1
fi

#Segmen 3
pwUsr=$(echo "$usrEntry" | awk -F';' '{print $2}')
if [ "$pw" != "$pwUsr" ]; then
    log_message "LOGIN: ERROR Gagal login untuk user $usn (password salah)"
    echo "Error: Password salah!"
    exit 1
fi

#Segmen 4
log_message "LOGIN: INFO User $usn berhasil log in"
echo "Login berhasil!"
echo "userLogged;$usn" >> "$log" 

#Segmen 5
while true; do
    echo -n "Input 1 untuk logout: "
    read choice
    if [ "$choice" == "1" ]; then
        sed -i '$d' "$log"
        log_message "LOGOUT: INFO User $usn berhasil log out"
        echo "Logout berhasil!"
        exit 0
    else
        echo "Input tidak valid"
    fi
done
```
Penjelasan : 
- ``Segmen 1`` digunakan untuk user melakukan input username dan password ke variabel ``usn`` dan ``pw``.
- ``Segmen 2`` digunakan untuk mengecek apakah ``usn`` yang diinput oleh user ada pada ``users.txt``.
- Pada ``Segmen 3``, pwUser akan berisi password dari akun yang diinput sesuai dengan input dari user. Variabel ``userEntry`` sendiri pada segmen 2 akan mengambil 1 line dari file ``users.txt`` dalam format ``username;password``, oleh karena itu ``usrEntry`` akan di pipe ke ``awk``, kemudian dengan separator ``;`` di print index kedua (yang mana dalam format tersebut akan mengambil password) kemudian meng-assign variabel ``pwUsr`` dengan password dari user. Kemudian password berdasarkan input user akan di cek apakah sama dengan password user dari file ``users.txt``.
- ``Segmen 4`` akan dijalankan apabila username user ada dan password nya sesuai, log message akan di print ke cloud_log.txt dan pada bagian akhir segmen 4 yaitu ``echo "userLogged;$usn" >> "$log"`` berguna agar hanya ada 1 user yang login di 1 waktu, dan tidak ada yang bisa melakukan register ketika ada user yang sedang login.
- ``Segmen 5`` berjalan ketika sudah berhasil login, dalam loop while tersebut berguna apabila user ingin melakukan logout. Logout dapat dilakukan dengan meng-input angka 1, apabila angka yang diinput bukang angka 1 maka akan memunculkan pesan error dan meminta user untuk menginput 1 apabila ingin melakukan logout. Bila user menginput angka 1, maka ``sed -i '$d' "$log"`` akan menghapus line terakhir dari ``cloud_log.txt``, yang berisi ``userLogged;username`` yang mengindikasikan ada user yang sedang login. Apabila line tersebut dihapus, maka user lain dapat melakukan register/login.

Contoh log username tidak ada : 
```
25/03/28 18:06:58 LOGIN: Gagal login untuk user freddskiii (username tidak ada)
```

Contoh log password salah : 
```
25/03/28 18:12:18 LOGIN: ERROR Gagal login untuk user freddskiii (password salah)
```

Contoh log user berhasil login dan logout : 
```
25/03/25 19:39:39 LOGIN: INFO User freddskiii berhasil log in
25/03/25 19:39:54 LOGOUT: INFO User freddskiii berhasil log out
```


4. Semua proses login/register dicatat dalam file cloud_log.txt dengan format sesuai ketentuan.
```sh
log_message(){
    echo "$(date +'%y/%m/%d %T') $1" >> "$log"
}
```
Penjelasan : 

Untuk mencatat kedalam file ``cloud_log.txt``, aku membuat function ``log_message`` untuk mempermudah pencatata log. Karena format yang diminta adalah ``tahun:bulan:tanggal waktu pesan log``. Contoh pemanggilan fungsi log adalah ``log_message "LOGOUT: INFO User $usn berhasil log out"``, karena dalam function log sudah ada tanggal dengan format yang diminta ``(%y/%m/%d %T) dengan y = tahun, m = bulan, d = tanggal, dan T = waktu``, dan $1 adalah input dari pemanggilan function log, dalam contoh ini $1 (inputnya) adalah ``"LOGOUT: INFO User $usn berhasil log out"``, kemudian di-append dengan ``>>`` ke ``cloud_log.txt``. Contoh outputnya adalah ``25/03/27 13:38:50 LOGOUT: INFO User freddskiii berhasil log out``

contoh log : 
```
25/03/25 20:01:48 LOGIN: INFO User user berhasil log in
25/03/25 20:02:21 LOGOUT: INFO User user berhasil log out
25/03/25 21:52:37 REGISTER: ERROR Username tersebut sudah terdaftar!
25/03/25 21:52:47 REGISTER: ERROR Password terlalu pendek
25/03/25 21:57:13 REGISTER: User baru berhasil didaftarkan
25/03/25 21:57:29 REGISTER: ERROR Username tersebut sudah terdaftar!
25/03/25 21:57:52 REGISTER: ERROR Password harus memiliki karakter special {ex : !@#$%^&*}
```


5. Memastikan hanya ada 1 user yang dapat login/register dalam 1 waktu
```sh
#code 1
echo "userLogged;$usn" >> "$log"

#code 2
if grep -q "userLogged" "$log"; then
    echo "Error: User lain sedang login!"
    exit 1
fi
```
Penjelasan : 

Dalam ``code 1`` berguna untuk mengindikasikan bahwa ada user yang sedang login. Pesan yang akan keluar di ``cloud_log.txt`` adalah seperti berikut ``userLogged;freddskiii``. Dalam ``register.sh`` dan ``login.sh``, ``code 2`` berada di paling atas program, sehingga ketika program dijalankan, hal yang pertama dilakukan adalah mengecek apakah terdapat user yang sedang login. Cara kerja dari ``code 2`` kurang lebihnya mirip dengan cara kerja pencarian username. Dengan menggunakan ``grep`` untuk mencari line dalam sebuah file dengan ``-q`` agar grep berjalan secara silent, dengan mencari string ``userLogged`` dalam file ``cloud_log.txt``. Apabila terdapat string ``userLogged``  (yang hanya akan ada apabila seorang user login), maka akan langsung menampilkan error bahwa ada user yang sedang login, dan kemudian langsung melakukan terminate terhadap program yang baru dijalankan, sehingga tetap membiarkan user yang sedang login pada kondisi login.

Contoh output percobaan login ketika ada user yang sedang login : 
```
Error: User lain sedang login!
```

### B. Automation & Download
- Pengunduhan Gambar Otomatis: Sistem akan secara berkala memeriksa file log secara berkala untuk mendeteksi apakah ada satu pengguna yang sedang login memanfaatkan script automation.sh. Jika terdeteksi, sistem akan menjalankan proses pengunduhan gambar secara otomatis pada script download.sh.
- Pengecekan Status Login:
 * Sistem mengecek file log (cloud_log.txt) setiap 2 menit untuk memastikan ada satu pengguna yang login.
 * Jika kondisi terpenuhi, proses download gambar dimulai.
 * Jika pengguna pengguna logout, proses download dihentikan.

Catatan: Memanfaatkan penggunaan cronjob untuk melakukan otomasi pengecekan status login.
- Penyimpanan Gambar:
 * Download gambar dari Google Images setiap 10 menit bertema alam.
 * Setiap pengguna memiliki folder penyimpanan gambar sendiri di:
   /home/$user/cloud_storage/downloads/{USERNAME}/
 * Gambar yang diunduh harus dinamai dengan format: HH-MM_DD-MM-YYYY.{ekstensi file} (Contoh: 14-20_12-03-2025.{ekstensi file} menunjukkan file di download pada pukul 14:20 tanggal 12 Maret 2025.

Catatan: Pastikan setiap kali melakukan download image selalu berhasil dengan beberapa variasi gambar yang di download (di perbolehkan download gambar yang sama, akan tetapi tidak terus menerus melakukan download gambar yang sama). contoh: dari total 20 gambar yang bisa di download, lakukanlah randomize download gambar setiap rentang waktu tersebut.

### Solusi
0. Variabel yang digunakan untuk path : 
```sh
usr="/home/freddskiii/cloud_storage/users.txt"
log="/home/freddskiii/cloud_storage/cloud_log.txt"
```


1. Pengecekan otomatis setiap 2 menit menggunakan automation.sh
``` sh
#Wegmen 1
usrEntry=$(awk -F ";" '/userLogged/ {print $2}' $log)

#Segmen 2
if ! [ -z "$usrEntry" ]; then
    if ! crontab -l | grep -q "download.sh"; then
        (crontab -l ; echo "*/10 * * * * /home/freddskiii/download.sh") | crontab -
        (crontab -l ; echo "* */2 * * * /home/freddskiii/archive.sh") | crontab -
    fi
else
    crontab -l | grep -v "download.sh" | grep -v "archive.sh" | crontab -
fi

#Segmen 3
if ! crontab -l | grep -q "automation.sh"; then
    (crontab -l ; echo "*/2 * * * * /home/freddskiii/automation.sh") | crontab -
fi
```
Penjelasan : 
- ``Segmen 1`` berguna untuk mengecek serta mengambil username dari user yang sedang login dengan menggunakan ``awk`` pada file ``cloud_log.txt``. ``awk`` di set dengan separator ``;``, kemudian mencari line yang terdapat string ``userLogged``, kemudian mengambil index kedua (yakni username) sehingga variabel ``usrEntry`` berisi username dari user yang sedang login.
- ``Segmen 2`` akan berjalan apabila usrEntry tidak kosong. If statement pertama berguna unuk mengecek apakah cronjob untuk ``download.sh`` dan ``archive.sh`` ada atau tidak. Dalam nested if statement tersebut, aku hanya mengecek cronjob untuk ``download.sh`` karena cronojob untuk ``download.sh`` dan ``archive.sh`` akan selalu berjalan dan berhenti bersama. Apabila belum ada, maka dapat menambahkan cronjob untuk ``download.sh`` dan ``archive.sh``. Bila ada user yang sedang login dan ``automation.sh`` mendetect user tersebut, maka cronjob untuk ``download.sh`` dan ``archive.sh`` akan dijalankan. Begitu pula sebaliknya, apabila ada user yang logout dah terdetect, maka cronjob akan dihentikan, menyisakan cronjob untuk ``automation.sh``.
- Penjelasan syntax ``(crontab -l ; echo "* */2 * * * /home/freddskiii/Programming/sisop/Praktikum/Praktikum1/3_ignatius/program") | crontab -``. Crontab -l berguna untuk memanggil cronjob lain yang sedang berjalan di waktu itu, kemudian menambahkan cronjob untuk program baru. Hal tersebut harus dilakukan karena sifat dari cronjob yang akan meng-overwrite cronjob lama apabila menambahkan cronjob baru. Setelah memastikan cronjob yang lain aman dan dapat menambahkan cronjob untuk program yang ingin dijalankan kemudian di pipe ke command ``crontab -`` yang berguna untuk menambahkan cronjob baru. Crontab - akan meng-overwrite cronjob yang sedang berjalan, namun karena sudah diantisipasi sebelumnya, maka cronjob sebelum cronjob baru untuk program yang ingin dijalankan juga ikut kedalam cronjob baru.
- ``Segmen 3`` berguna apabila ingin memulai cronjob untuk ``automation.sh``. Alih-alih memulai secara manual, cronjob ``automation.sh`` dapat dimulai dari ``automation.sh`` itu sendiri.
- Konfigurasi cronjob terdiri dari 5 bintang * * * * * yang masing2 bintang (dari kiri ke kanan) mewakili menit, jam, hari, bulan, dan tahun. ``/`` berguna untuk menjalankan cronjob "setiap" selang waktu tertentu. Dalam kasus ini, ``download.sh`` ingin menjalankan setiap 10 menit, sehingga konfigurasi cronjob nya adalah */10 * * * *, yang artinya setiap 10 menit. Untuk ``archive/sh`` yakni * */2 * * *, yang artinya setiap 2 jam. Untuk ``automation.sh`` yakni */2 * * * *, yang artinya setiap 2 menit.

Cronjob ketika tidak ada user yang login : 
```
*/2 * * * * /home/freddskiii/automation.sh
```

Cronjob ketika ada user yang login : 
```
*/10 * * * * /home/freddskiii/download.sh
* */2 * * * /home/freddskiii/archive.sh
*/2 * * * * /home/freddskiii/automation.sh
```


2. Penyimpanan gambar menggunakan download.sh dan randomisasi gambar
``` sh
#Segmen 1
user=$(awk -F";" '/userLogged/ {print $2}' $usr)

#Segmen 2
if [[ $user ]]; then
    if [[ ! -d "$picDir/$user" ]]; then
        mkdir -p "$picDir/$user"
    fi

    pic=(
        "https://onetreeplanted.org/cdn/shop/articles/nature_facts_1600x.jpg?v=1705008496"
        "https://natureinvestmenthub.ca/wp-content/uploads/2023/10/hendrik-cornelissen-qrcOR33ErA-unsplash-scaled.jpg"
        "https://i0.wp.com/picjumbo.com/wp-content/uploads/beautiful-fall-nature-scenery-free-image.jpeg?w=600&quality=80"
        "https://natureconservancy-h.assetsadobe.com/is/image/content/dam/tnc/nature/en/photos/w/o/WOPA160517_D056-resized.jpg?crop=864%2C0%2C1728%2C2304&wid=600&hei=800&scl=2.88"
        "https://deframedia.blog.gov.uk/wp-content/uploads/sites/208/2023/09/State-of-nature.jpg"
        "https://img-4.linternaute.com/0efC7-fKD0qAHCUcChtSVXT9Ego=/1500x/smart/43f787f117784c3fa9a928194bd91d5e/ccmcms-linternaute/12325430.jpg"
        "https://scx2.b-cdn.net/gfx/news/hires/2020/nature.jpg"
        "https://img.freepik.com/premium-photo/wide-angle-shot-single-tree-growing-clouded-sky-sunset-surrounded-by-grass_181624-22807.jpg"
        "https://www.aaronreedphotography.com/images/xl/Sweet-Dreams-2022.jpg"
        "https://upload.wikimedia.org/wikipedia/commons/thumb/8/83/Waterfall_at_Arthur%27s_Pass_National_Park.jpg/800px-Waterfall_at_Arthur%27s_Pass_National_Park.jpg"
        "https://www.dereknielsen.com/wp-content/uploads/2023/11/edge-of-autumn.jpg"
        "https://images.newscientist.com/wp-content/uploads/2023/02/07104439/SEI_142739270.jpg"
        "https://tnfd.global/wp-content/uploads/2023/09/Food-and-Agricultulre-cover-1.jpg"
        "https://www.andrewshoemaker.com/images/xl/revive-waianapanapa-state-park.jpg"
    )

    filename="$(date +'%H-%M_%d-%m-%Y').jpg"
    random_index=$((RANDOM % ${#pic[@]}))
    wget -q "${pic[$random_index]}" -O "$picDir/$user/$filename"
fi 
```
Penjelasan : 
- ``Segmen 1`` berguna untuk mengambil username dari user yang sedang login menggunakan ``awk`` (mekanisme yang sama pada program sebelumnya).
- ``Segmen 2`` apabila terdapat user yang sedang login, maka akan menjalankan if statement tersebut. Nested if statement berguna untuk mengecek apakah directory untuk user yang sedang login ada atau tidak. Apabila ada maka akan dibuat, namun jika sudah ada maka dibiarkan. 
- Agar gambar yang didownload bersifat random, dalam program digunakan variabel ``random_index``, degan menggunakan ``RANDOM`` dengan range angka sejumlah size of ``pic``.
- Untuk format penamaan menggunakan format ``jam-menit_tanggal-bulan-tahun.extension``, sehingga untuk mempermudah penamaan, digunakanlah variabel filename sesuai dengan kriteria tersebut.
- Proses mendownload gambar menggunakan `wget`, kemudian `"${pic[$random_index]}"` untuk link yang sudah di randomize, kemudian `"$picDir/$user/$filename"` untuk lokasi dimana file tersebut ingin di download. `-q` digunakan agar wget bekerja secara silent, dan -O untuk menentukan lokasi gambar yang ingin di download.

### C. Archive
Pengarsipan Gambar: Untuk menjaga kerapihan penyimpanan, setiap gambar yang telah diunduh akan dikumpulkan dan diarsipkan ke dalam file ZIP secara otomatis setiap 2 jam menggunakan script archive.sh. Setiap pengguna memiliki folder arsip sendiri.
- Frekuensi Pengarsipan: Sistem mengarsipkan gambar setiap 2 jam.
- Setiap pengguna memiliki folder arsip masing-masing di:
  `/home/$user/cloud_storage/archives/{USERNAME}/`
- Format nama file zip:
  `archive_HH-DD-MM-YYYY.zip` (Contoh: archive_12-03-2025.zip menunjukkan arsip dibuat pada tanggal 12 Maret 2025.)
- Setiap kali setelah melakukan arsip, hapus semua file di folder /home/$user/cloud_storage/downloads/{USERNAME}

### Solusi
0. Variabel yang akan digunakan
```sh
downDir="/home/freddskiii/cloud_storage/downloads"
arcDir="/home/freddskiii/cloud_storage/archives"
```

1. Pengarsipan gambar setiap 2 jam dan ketentuan-ketentuan nya
```sh
for usr in "$downDir"/*; do
    user=$(echo "$usr" | awk -F'/' '{print $NF}')

    #Mengecek apakah directory ada
    mkdir -p "$arcDir/$user"

    #Pengarsipan
    time=$(date +'%H-%d-%m-%Y')
    zip -r "$arcDir/$user/archive_$time.zip" "$usr"/*
    rm -f "$usr"/*
done
```
Penjelasan : 
- Dalam `archive.sh` ini, program akan langsung melakukan archive untuk semua user, tidak hanya untuk user yang sedang login. Variabel usr yang ada di for loop akan mengambil path mulai hari /home/ sampai ke /username/, sehingga pada variabel user digunakan untuk mengambil username dari path tersebut. Penggunaan `awk` dan separator `/`, kemudian `{print $NF}` yang bertujuan untuk mengambil index terakhir dari path, yang mana adalah username.
- Karena format penamaan archive harus `archive_jam-tanggal-bulan-tahun`, maka dibuatlah variabel time agar mempermudah penamaan. Untuk melakukan zip, menggunakan -r agar semua gambar yang didownload dapat di compress menjadi zip, kemudian `"$arcDir/$user/archive_$time.zip"` untuk menentukan lokasi zip, yaitu pada `/home/freddskiii/cloud_storage/archives/username`. `"$usr"/*` berguna untuk memastikan bahwa semua file dalam folder user mendapatkan perlakuan yang sama (zip).
- Karena diminta untuk menghapus semua gambar dalam folder download user setelah melakukan zip, maka dapat menggunakan `rm -f "$usr"/*` untuk menghapus semua gambar dalam folder download user.

### Kendala
1. Dalam program `register.sh` dan `login/sh`, untuk mekanisme hanya ada 1 user yang dapat login awalnya ingin menggunakan log yang sama dengan ketika user berhasil register/login, namun karena separator yang tidak konsisten dan banyak error dalam pembacaan log, maka digunakan `userLogged;username`.
2. Ketika program ingin dijalankan secara otomatis menggunakan cronjob, program `automation.sh` tidak berjalan, namun bila program dijalankan secara manual hasilnya sesuai dengan soal. Hal ini terjadi karena cron yang belum menyala di sistem, sehingga harus dinyalakan terlebih dahulu. Setelah dinyalakan, cronjob dari masing-masing aplikasi dapat bekerja secara otomatis.
3. Program `archive.sh` awalnya tidak dapat membuat sub directory untuk setiap user, dan malah membuat directory `*` yang juga tidak meng-archive gambarnya kedalam directory tersebut. Hal ini disebabkan oleh `if [[ !(-e /$arcDir) ]]` yang mana hal tersebut justru mengecek apakah root untuk directory archive ada atau tidak, bukan mengecek sub directory untuk user. Oleh karena itu, if statement tersebut kemudian dihapus dan hanya menyisakan `mkdir -p "$arcDir/$user"` yang memiliki tujuan sama, yakni mengecek apakah sub directory user ada atau tidak. 

# Task 4 - Proxy Terbaik di New Eridu

Buatlah program untuk memantau sumber daya pada setiap server. Program ini hanya perlu memantau:

- Penggunaan RAM menggunakan perintah ```free -m```.
- Ukuran suatu direktori menggunakan perintah ```du -sh <target_path>```.
- Uptime menggunakan perintah ```uptime``` dan ambil bagian yang menunjukkan waktu berjalan.
- Load average menggunakan perintah ```cat /proc/loadavg``` dan ambil tiga nilai pertama (1, 5, dan 15 menit).

| No  | Nama File Script      | Fungsi                                               |
| --- | --------------------- | ---------------------------------------------------- |
| 1   | `minute5_log.sh`      | Script pencatatan metrics setiap 5 menit             |
| 2   | `agg_5min_to_hour.sh` | Script agregasi log per jam                          |
| 3   | `uptime_monitor.sh`   | Script monitoring uptime dan load average setiap jam |
| 4   | `cleanup_log.sh`      | Script penghapusan log lama setiap hari              |


# Script ```minute5_log.sh```

Format file log yang dihasilkan dari script adalah ```metrics_{YmdHMS}.log```. Karena itu, pada bagian awal script digunakan command ```date``` untuk mencatat waktu script dijalankan.
```
timestamp=$(date +"%Y%m%d%H%M%S")
filename="metrics_$timestamp.log"
filename="/home/riyan/metrics/$filename"
```
```/home/riyan/metrics``` ditambahkan karena setiap log file yang dihasilkan akan disimpan pada directory tersebut.

```
metrics=$(free -m | awk 'NR==2 {OFS=","; print $2,$3,$4,$5,$6,$7}') 
metrics2=$(free -m | awk 'NR==3 {OFS=","; print $2,$3,$4}')
metrics3=$(du -sh "/home/riyan/" | awk '{print $2}')
metrics4=$(du -sh "/home/riyan/" | awk '{print $1}')
```
Variabel ```metrics``` mencatat hasil dari command ```free -m ``` pada baris kedua. Hal ini karena pada command ```free -m```, data mengenai memory akan ditampilkan pada baris kedua. Sementara itu, ```metrics2``` menyimpan data ```swap``` yang ditampilkan pada baris ketiga.  ```metrics3``` dan ```metrics4``` juga menggunakan prinsip yang sama, namun untuk command yang berbeda, yaitu ```du -sh```.
```
touch $filename
echo "mem_total,mem_used,mem_free,mem_shared,mem_buff,mem_available,swap_total,swap_used,swap_free,path,path_size" >> $filename
echo ${metrics},${metrics2},${metrics3},${metrics4} >> $filename
```
Command ```touch``` digunakan untuk membuat file sesuai dengan directory dan nama file yang didefinisikan sebelumnya. Sementara itu, command ```echo``` mencatat isi variabel ```metrics``` hingga ```metrics4``` ke dalam file yang dibuat.
Terakhir, ```chmod 400``` digunakan untuk memastikan file hanya dapat dibuka oleh owner.

### Output
```
mem_total,mem_used,mem_free,mem_shared,mem_buff,mem_available,swap_total,swap_used,swap_free,path,path_size
13713,2856,9058,147,2266,10857,4095,0,4095,/home/riyan/,7.7G
```

# Script ```agg_5min_to_hour.sh```

Seperti pada script ```minute5_log.sh```, digunakan command ```date``` untuk mendapatkan waktu script dijalankan.
```
current=$(date +%s)
current_time=$(date +"%Y%m%d%H")
output=/home/riyan/metrics/metrics_agg_"$current_time".log
log_files=()
```
Array ```log_files``` digunakan untuk menyimpan file-file yang akan diagregasi, sedangkan variable ```current``` merupakan ```unix epoch``` yang mencatat banyak detik yang telah berjalan sejak 1 Januari 1970. Variabel ini digunakan untuk memastikan file mana saja yang memenuhi kriteria untuk diagregasi.
```
for file in /home/$USER/metrics/metrics_*; do
    base=$(basename "$file")
    if [[ "$base" == metrics_agg* ]]; then
        continue
    fi
    timestamp=${file#/home/$USER/metrics/metrics_}
    timestamp=${timestamp%.log}
    formatted_timestamp=$(echo "$timestamp" | sed 's/\(....\)\(..\)\(..\)\(..\)\(..\)\(..\)/\1-\2-\3 \4:\5:\6/')
    epoch=$(date -d "$formatted_timestamp" +%s)

    if((current - epoch < 3600)); then
        log_files+=("$file")
    fi 

done
```
Perulangan di atas akan memeriksa semua file yang diawali dengan ```metrics_```. Variabel ```timestamp``` dan ```formatted_timestamp``` akan mengambil waktu yang terdapat pada nama file dan mengubahnya menjadi format waktu yang valid untuk dimasukkan pada command ```date -d``` yang mengecek banyak detik sejak 1 Januari 1970 hingga file tersebut dibuat. Kemudian, apabila ```current - epoch < 3600``` bernilai ```true```, hal ini menandakan bahwa file tersebut dibuat dalam kurun waktu 1 jam terakhir dan memenuhi kriteria untuk diagregasi.

Kemudian, dilakukan agregasi dengan awk
```
awk -F',' 'BEGIN { OFS="," }
FNR==2 {
    fieldCount = NF;
    for(i = 1; i<=NF; i++){
        if(i==11){
            if($i ~ /G$/){
                num = substr($i, 1, length($i)-1) + 0;
                value = num * 1024;
            }
            else{
                value = substr($i, 1, length($i)-1) + 0;
            }
            sum[i] += value;
            count[i]++;
            if(!(i in min) || value < min[i])
                min[i] = value;
            if(!(i in max) || value > max[i]) 
                max[i] = value;
        }
        else if (i!=10){
            sum[i] += $i;
            count[i]++;
            if(!(i in min) || $i < min[i])
                min[i] = $i;
            if(!(i in max) || $i > max[i]) 
                max[i] = $i;
        }
    }
}
```
- ```NF``` mencatat banyak _field_ pada masing-masing file.
- ```i==11```  menunjukkan file sedang memproses _field_ ```path_size``` yang berada pada format gigabyte. Karena itu variabel ini akan dikali 1024 untuk mengubahnya menjadi megabyte.
- ```i==10``` menunjukkan directory ```/home/riyan/```, karena itu tidak ada proses yang perlu dilakukan.
- Array ```sum``` mencatat total masing-masing _field_ yang akan digunakan pada penghitungan rata-rata.
- Array ```count``` mencatat banyak _field_.
```
  END {
    print "type,mem_total,mem_used,mem_free,mem_shared,mem_buff,mem_available,swap_total,swap_used,swap_free,path,path_size";
    type[1] = "minimum"; type[2] = "maximum"; type[3] = "average";

    for(i = 1; i<=3; i++){
        printf "%s", type[i];
        for(j = 1; j<=fieldCount; j++){
            if(j!=10){
                if(i==1)
                    printf ",%s", min[j];            
                else if(i==2)
                    printf ",%s", max[j];
                else
                    printf ",%s", sum[j]/count[j];
            }
            else
                printf ",/home/riyan/"
        }
        print ""
    }
}' "${log_files[@]}" > "$output"
chmod 400 "$output"
```
- Hasil proses agregasi dicatat dalam file dengan nama dan directory yang telah didefinisikan sebelumnya.
- ```chmod``` untuk mengubah hak akses file yang dihasilkan.
### Output
```
type,mem_total,mem_used,mem_free,mem_shared,mem_buff,mem_available,swap_total,swap_used,swap_free,path,path_size
minimum,13713,1387,11095,33,838,12204,4095,0,4095,/home/riyan/,7577.6
maximum,13713,1508,11783,35,1463,12325,4095,0,4095,/home/riyan/,7577.6
average,13713,1447.5,11439,34,1150.5,12264.5,4095,0,4095,/home/riyan/,7577.6
```

# Script ```uptime_monitor.sh```

Seperti pada file-file sebelumnya, langkah pertama adalah mendapatkan waktu script dijalankan sesuai dengan format yang ada pada soal.
```
timestamp=$(date +"%Y%m%d%H%M%S")
filename="uptime_$timestamp.log"
filename="/home/riyan/metrics/$filename"
```
Setelah itu, dilakukan pengambilan data dengan command yang sesuai.
```
touch $filename
echo "uptime,load_avg_1min,load_avg_5min,load_avg_15min" >> $filename
uptime_=$(uptime | awk '{print $1,$2,$3}')
average_=$(cat /proc/loadavg | awk '{OFS=",";print $1,$2,$3}')
```
- Variabel ```uptime_``` menyimpan 3 data pertama yang dihasilkan dari command ```uptime```
- Variabel ```average``` menyimpan 3 data pertama yang dihasilkan dari command ```cat /proc/loadavg```
```
echo ${uptime_},${average_} >> $filename
```
Command ```echo``` digunakan untuk memasukkan data dari ```uptime_``` dan ```average_``` pada file yang telah dibuat.
### Output
```
uptime,load_avg_1min,load_avg_5min,load_avg_15min
13:00:01 up 22,0.30,0.48,0.45
```
# Script ```cleanup_log.sh```
```
current_date=$(date -d "yesterday" +%Y%m%d)

for file in /home/riyan/metrics/metrics_agg_"$current_date"*; do
    base=$(basename "$file")
    hour=${base#metrics_agg_${current_date}}
    hour=${hour:0:2}
    if [[ "$hour" -lt 13 ]]; then
        rm "$file"
    fi
done
```
- Variable ```current_date``` mendapatkan tanggal 1 hari sebelum script dijalankan. Hal ini karena file yang akan dihapus merupakan file yang dibuat pada 12 jam pertama hari sebelumnya.
- Perulangan ```for``` memeriksa semua file yang ```metrics_agg``` yang dibuat pada hari sebelumnya
- Variabel ```base``` dan ```hour``` digunakan untuk mengambil informasi pada jam berapa file ```metrics_agg``` dibuat
- Apabila ```hour``` kurang dari 13, hal ini menunjukkan file tersebut dibuat antara pukul 00:00 hingga 12:00 pada hari sebelumnya, yang berarti memenuhi kriteria untuk dihapus.

# Crontab

```
*/5 * * * * /home/riyan/sisop_ws/praktikum-modul-1-d03/task-4/minute5_log.sh
0 * * * * /home/riyan/sisop_ws/praktikum-modul-1-d03/task-4/agg_5min_to_hour.sh
0 * * * * /home/riyan/sisop_ws/praktikum-modul-1-d03/task-4/uptime_monitor.sh
0 0 * * * /home/riyan/sisop_ws/praktikum-modul-1-d03/task-4/cleanup_log.sh
```
- `minute5_log.sh` dijalankan setiap 5 menit sekali
- `agg_5min_to_hour.sh` dan `uptime_monitor.sh` dijalankan setiap 1 jam sekali
- `cleanup_log.sh` dijalankan setiap hari pada pukul 00:00

# Kendala

Pada proses pengerjaan, terdapat kesalahan pada penggunaan ```chmod``` untuk mengubah hak akses file. ```chmod 400``` yang digunakan menyebabkan script ```agg_5min_to_hour.sh``` tidak memiliki hak akses terhadap file log yang dihasilkan setiap 5 menit. Kesalahan ini dapat diatasi dengan mengubah ```chmod 400``` menjadi ```chmod 600```



