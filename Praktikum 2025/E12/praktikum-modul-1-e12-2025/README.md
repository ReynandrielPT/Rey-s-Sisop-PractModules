[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/Ph837wyE)
|    NRP     |      Name      |
| :--------: | :------------: |
| 5025241178 | Christian Mikaxelo |
| 5025241218 | Ida Bagus Gde Dimas Sutha Maha |
| 5025241227| Safa Maulana Efendi|

# Praktikum Modul 1 _(Module 1 Lab Work)_

</div>

### Daftar Soal _(Task List)_

- [Task 1 - New Jeans Global Teratas _(Top Global New Jeans)_](/task-1/)

- [Task 2 - Liburan Bersama Rudi _(Holiday with Rudi)_](/task-2/)

- [Task 3 - Ignatius Si Cloud Engineer _(Ignatius The Cloud Engineer)_](/task-3/)

- [Task 4 - Proxy Terbaik di New Eridu _(Best Proxy in New Eridu)_](/task-4/)

### Laporan Resmi Praktikum Modul 1 _(Module 1 Lab Work Report)_

Tulis laporan resmi di sini!

_Write your lab work report here!_

Task 1 (Top Global New Jeans) 

Cara Pengerjaan : 
Problem A : Karena harus mengambil data dari file DataStreamer.csv maka menggunakan awk untuk filter data dengan pattern skip header, lalu ambil kolom nama namun yang mengandung angka 2 dan tanpa “_”. Lalu output pertama masuk ke pipe untuk di sort berdasarkan ASCII. Lalu untuk membuat list urutan setiap nama, output diteruskan lagi ke pipe “awk” untuk di tampilkan ke layar. Dan terakhir menampilkan value dari NR yang terakhir.

![image](https://github.com/user-attachments/assets/73e1a5ca-37a2-41bc-9cee-3edbc146a583)

Problem B : Menggunakan cara yang sama dengan Problem A, namun ada perubahan pattern dimana mengambil kolom nama yang mengandung semua angka. Lalu actionnya yaitu membuat variable untuk menghitung jumlah user dan jumlah lagu. Untuk algoritma sorting, menggunakan maxcount yang meng-iterasikan setiap index (judul lagu). Terakhir menggunakan if statement untuk output ke Minji.

![image](https://github.com/user-attachments/assets/06c0ab81-943d-4974-8363-849b1928e605)

Problem C : Menggunakan Algoritma yang sama dengan Problem B, namun ada penambahan algoritman menggunakan awk, dimana ada variabel yang diinisiasikan “title” yang mengambil isi dari variabel max_title dari END , sehingga dapat dibandingkan dengan data Judul lagu di AlbumDetails.csv.

![image](https://github.com/user-attachments/assets/f7dcc01e-d9aa-4944-960b-b4f14d9445ce)

Problem D : Diawali penggunaan awk untuk mengambil data dari DataStreamer.csv dengan action inisialisasi tiap kolom untuk memudahkan dalam proses scripting. Untuk sub-soal D yang pertama saya menghitung jumlah pendengar lagu “New Jeans” dengan device sebagai index agar setiap device terhitung jumlahnya. Setelah itu menggunakan variabel seen_all untuk mengecek apakah mungkin ada nama yang sama untuk device yang sama agar menghindari duplikasi. Setelah itu untuk menghitung total durasi tiap device tinggal meng-increment duration_sum dengan index device. Lalu untuk menghitung device terpopuler, total streaming tertinggi,dan terloyal, menggunakan algoritma max_count untuk tiap kategorinya.

![image](https://github.com/user-attachments/assets/fcf74bd7-67c9-404f-a2ec-5b4dcd66f737)

Kendala : saat Problem D running di VMWare dengan running di git Bash windows, hasilnya berbeda, yaitu di VMware tidak berjalan dengan baik, namun di git Bash, script menghasilkan output yang baik.
VMware :

![image](https://github.com/user-attachments/assets/c3fc774f-cc7d-48c0-8088-71c2a0f09051)

Git Bash : 

![image](https://github.com/user-attachments/assets/b73cccfa-5532-403a-bfa8-72b135d595af)



Task 2 (Liburan Bersama Rudi)

Cara Pengerjaan : 
Problem A : Di sini Rudi memintak kita menampilkan total request yang di buat setiap IP dan menampilkan jumlah daro setiap status code dari file access.log

![image](https://github.com/user-attachments/assets/801fe6c1-9182-4570-b0b4-62124ffd3b76)

Di sini pada Line 3 membuat variable “log_file = access.log”, kemudian di line 5 – 8 saya memastikan jika file access.log ada, jika tidak ada maka akan keluar “File access.log tidak ditemuka!”. Di line 10 – 11 menghitung request per IP dengan menggunakan awk dan saya print di kolom ke 1 yang merupakan IP Address, selanjutnya saya sort untuk mengurutkan IP, lalu saya gunakan juka uniq -c untuk menghitung jumlah kemunculan setiap IP, dan terakhir saya sort -nr untuk mengurutkan hasil berdasarkan jumlah. Untuk line ke 13-14 saya hanya mengubah di print $9 untuk menggambil kolom ke 9 di access.log yang berisi status HTTP.

Problem B: Di sini Rudi meminta kita membuat pencarian menggunakan tanggal dan IP Address dan menyimpannya ke dalam ditektori backup 

![image](https://github.com/user-attachments/assets/169265be-1136-47ad-9599-e6d4ce64361f)

Di sini untuk Line 3-4 meminta User untuk memasukkan tanggal dan Ip Addres. Lalu untuk Line 5-7 saya membuat variable untuk file access.log, peminjaman_computer.csv. dan backup. Di line 9 saya membuat program agar membuat folder backup. Di Line 12 saya mengambil bagian terakhir dari IP Address. Di line 13 saya mencari nama pengguna dari file csvnya dengan $1 = tanggal, $2 = computer, dan $3 sebagai nama pengguna tersebut. Kemudian di Line 16 – 19 saya cek lagi juka nama pengguna di temukan, jika tidak maka akan keluar “Data yang kamu cari tidak ada”. Di Line 21-22 untuk menampilkan nama pengguna. Line 24-25 membuat folder backup untuk pengguna. Di Line 27-29 menentukan nama file backup dengan timestamp menumpan waktu, dan tanggal_format mengubah tanggal seperti 12/03/2025 menjadi 12032024. Di line 31 saya mengambil Log aktivitas dari access.log dengan menggunakan grep dan awk di mana untuk mencari baris yang mengandung IP user dan filter berdasarkan tanggal. Di Line 33 menyimpan log ke file backup. Dan terakhir di Line 35 memberitahu kita bahwa backup berhasil.

Problem C : Di sini kita di suruh mencari teman Rudi yang menemukan Status Code 500 terbanyak, dengan nama dan berapa banyak yang dia temukan 

![image](https://github.com/user-attachments/assets/c4467515-e4fa-4270-8ec3-d016768426c0)

Di sini pada line 3-4 membuat variable untuk acces.log dan peminjaman_compute.csv. Di line ke 6-12 di sini saya memetakan IP ke Nama dengan fungsi While untuk membaca gile CSV per baris dan mengabaikan baris pertama dan memetakan IP dengan nama dalam array ip_to_name. Untuk di Line 14 sata menggunakan fungsi awk untuk mengambil IP Addres dari baris log di kolom ke 9 yang memiliki status 500 di sini saya  menggunakan head -1 untuk mengambil IP dengan jumlah error terbanyak. Di Line 16-25 saya cocockan lagi IP dengan nama pengguna dengan fungsi while untuk membaca output adri variable errors yang berisi jumlah error dan ip, dan mengecek apakah ip ada di dalam ip_to_name, jika tidak ditemukan maka di hunakan

OUTPUT :
a.

![image](https://github.com/user-attachments/assets/550535a3-c5f7-4709-a478-57799c2acc18)

b.

![image](https://github.com/user-attachments/assets/2085cf07-3032-405d-9a5f-15c219d5bb60)

c.

![image](https://github.com/user-attachments/assets/06d6aa99-b2ca-4fcc-9b32-0d50f6a8b678)


Task 3 (Ignatius Si Cloud Engineer)

Problem A: ada 2 script yang pertama adalah register.sh, user memasukan nama dan password, namun password memiliki persyaratan agar user dapat membuat akun.

![image](https://github.com/user-attachments/assets/9e601d5f-e782-488b-b648-50b1c60d2e68)

Setelah user berhasil membuat akun, datanya akan disimpan dalam user.txt, dengan template user:pass:offline. Setelahnya user bisa melakukan login, menggunakan script login.sh jika user sudah login, user akan ditanya apakah mau log out apa tidak. Jika menekan ‘y’ berarti yes, dan ‘n’ berarti no. Jika user sudah login, namun mencoba login kembali maka sistem akan menolaknya, dan akan menanyakan apakah mau log out apa tidak.

 ![image](https://github.com/user-attachments/assets/39efa074-7a38-4b8c-b463-f3152d2c702b)

Problem B: Dengan automation.sh, log pengguna akan dipantau setiap 2 menit, jika pengguna sedang online, maka download.sh dijalankan, yaitu script akan menciptakan direktori khusus 
bagi setiap pengguna pada direktori download, dan mendownload gambar bertema alam secara random, untuk mengakses google image, disini saya menggunakan API key, dan CX dari web google CSE. Jika pengguna sedang offline maka proses download akan dihentikan

![image](https://github.com/user-attachments/assets/74916ccb-1c0f-4876-8664-ffbd49ca1b6f)

Problem C: Selama 2 jam sekali, semua gambar pada direktori user yang sedang online, akan di archive pada direktori archive user masing-masing, perintah ini dijalankan dengan script archive.sh Peng

![image](https://github.com/user-attachments/assets/fb21b655-9bdf-4c39-a89d-7ea89dde9a04)

Semua hal yang terjadi pada sirkulasi program, di catat oleh cloud_storage.txt

![image](https://github.com/user-attachments/assets/5742f729-b1e9-4425-a58f-a6eae8c93050)

Dan data pengguna disimpan dalam users.txt

![image](https://github.com/user-attachments/assets/84c3d527-f94d-4b2d-a7e7-d5eddb50ad30)


Task 4 (Proxy Terbaik di New Eridu)

1. minute5_log.sh - Pencatatan Metrics Setiap 5 Menit
Skrip ini dijalankan setiap 5 menit untuk memantau penggunaan RAM, ukuran direktori, uptime, dan load average server. Data yang terkumpul disimpan dalam file log dengan format metrics_{YmdHms}.log. Saya menggunakan berbagai jenis variabel memory dan fungsi awk, untuk menyimpan pecahan pecahan dari free -m, dan diakhir di print secara berbaris

![image](https://github.com/user-attachments/assets/25b3b531-81c1-4984-b122-6890718e4bb9)


2. agg_5min_to_hour.sh - Agregasi Log per Jam
Skrip ini mengumpulkan semua file log yang tercatat selama satu jam dan menghitung minimum, maksimum, dan rata-rata untuk masing-masing metrik. Hasilnya disimpan dalam file log dengan format metrics_agg_{YmdH}.log. Saya menggunakan pendekatan file yang memiliki jam yang sama, akan disimpan dalam LOG_FILES, dan diseleksi untuk ditentukan max, min, dan avg, lalu di print sesuai format.

![image](https://github.com/user-attachments/assets/edaeba6e-9b86-4463-a0c2-856807c81067)


3. uptime_monitor.sh - Monitoring Uptime dan Load Average per Jam
Skrip ini dijalankan setiap jam untuk memantau uptime dan load average server. Hasilnya disimpan dalam file log dengan format uptime_{YmdH}.log.

![image](https://github.com/user-attachments/assets/2d0d8099-0337-49fc-b181-c8ad90df937d)


4. cleanup_log.sh - Penghapusan Log Lama
Skrip ini dijalankan setiap hari pada pukul 00:00 untuk menghapus file log yang lebih lama dari 12 jam. Saya menggunakan pendekatan menggunakan find untuk mencari file yang sudah berumur 720 menit atau setara dengan 12 jam, dihapus menggunakan rm, dan -type f, yang berarti hanya menghapus jenis file

![image](https://github.com/user-attachments/assets/3b2b176a-227b-4186-889f-835b6e419758)


