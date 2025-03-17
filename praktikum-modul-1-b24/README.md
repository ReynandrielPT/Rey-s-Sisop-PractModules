[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/nyzv0V02)
<div align=center>

|    NRP     |      Name                  |
| :--------: | :------------------------: |
| 5025231091 | Fernando                   |
| 5025231113 | Reynandriel Pramas Thandya |
| 5025231264 | Daffa Raihan Saputra       |

# Praktikum Modul 1 _(Module 1 Lab Work)_

</div>

## Daftar Soal _(Task List)_

- [Task 1 - Tim Kawal Bubu _(Bubu's Monitoring Team)_](/task-1/)

- [Task 2 - Gabut Yuan _(Yuan's Boredom)_](/task-2/)

- [Task 3 - Magang Bubu _(Bubu's Internship)_](/task-3/)

- [Task 4 - LDR Isabel _(Isabel Sad Relationship)_](/task-4/)


## Task 1 - Tim Kawal Bubu _(Bubu's Monitoring Team)_
### Soal
Setelah masa upload proposal PKM 2024, Bubu sebagai sebagai anggota tim kawal melakukan scraping data upload dan mendapat file dalam format comma separated value (.csv). Bubu Pun mengirimkan file tersebut ke komandan PKM, namun komandan memiliki kesulitan untuk membaca data tersebut. Beliau mendelegasikan Bubu untuk menganalisis data tersebut. Karena kamu adalah Suhu dalam perlinux-an menurut Bubu maka ia spontan membuat challenge untukmu mengerjakan task tersebut seluruhnya menggunakan shell scripting. Sebagai teman yang baik kamu pun menerima challenge itu. Adapun analisis yang diminta oleh komandan adalah sebagai berikut:
- a. Karena Belmawa menetapkan judul maksimum proposal 20 kata, maka komandan ingin mencari data siapa saja tim yang tidak memenuhi ketentuan ini. Tampilkan nama pengusul, beserta departemennya yang judulnya lebih dari 20 kata. Pisahkan spasi dan hapus underscore "_" pada nama pengusul.
- b. Komandan PKM juga tertarik ingin tahu antusiasme dan partisipasi mahasiswa sehingga meminta Bubu menampilkan bidang paling banyak diminati oleh mahasiswa. Tampilkan nama skema saja.
- c. Karena ada aturan baru dimana 1 mahasiswa hanya boleh submit 1 proposal, maka komandan juga meminta Bubu untuk memberikan list mahasiswa yang mengajukan 2 proposal. Tampilkan data pembimbingnya karena ingin di kontak komandan.
### Penyelesaian
- a. Code Lengkap:
  ```
  #!/bin/bash

  awk -F '\t' '
  {split($5, a,  " ")}
  {nw += length(a)} 
  {if(nw>20) 
    {
    gsub("_"," ",$2); 
    print $2;
  }
  }{nw=0}' ~/praktikum-modul-1-b24/resources/DataPKM.tsv
  ```
Penjelasan:
```
#!/bin/bash
```
Pada line pertama terdapat shebang/hashbang yang berguna untuk memberi tahu bagaimana program/script tersebut dijalankan, yakni dengan di _execute_ atau di _bash_

```
awk -F '\t' 
```
Penggunakan `awk -F '\t'` untuk mengambil data dari file .tsv. Bagian -F '\t' digunakan untuk membuat tab atau '\t' sebagai _field_separator_, sehingga awk dapat mengambil data dari .tsv.

```
{split($5, a,  " ")}
```
Split digunakan untuk mengambil data dari kolom kelima file .tsv lalu dipisahkan dengan spasi dan hasilnya disimpan kedalam array a, fungsinya adalah supaya kalimat pada kolom kelima dapat diketahui jumlah katanya.

```
{nw += length(a)}
```
Mendeklarasikan nw dengan value `+=` panjang dari array a. Tujuannya adalah untuk mendapatkan data panjangnya array a (dengan length(a)) setiap awk dijalankan (untuk mengambil data tiap baris), lalu disimpan kedalam nw supaya nantinya dapat dijadikan suatu pengondisian.

```
{if(nw>20){
```
Merupakan pengondisian dimana ketika nw berjumlah lebih dari 20, maka _code_ dibawahnya akan dieksekusi. Value nw disini sama dengan jumlah kata pada kolom kelima. Pengondisian lebih dari 20 mengikuti dari permintaan soal.

```
gsub("_"," ",$2); 
```
Bagian ini bertujuan untuk mengganti semua underscore pada data kolom kedua dengan spasi
```
print $2;}
```
Bagian ini berguna untuk menampilkan isi dari data kolom kedua. Tampilan isi dari kolom kedua yang sebelumnya dipisahkan dengan underscore, sekarang menjadi terpisahkan dengan spasi.
```
}{nw=0}' ~/praktikum-modul-1-b24/resources/DataPKM.tsv
```
Bagian '}' guna menutup pengondisian if. Lalu `{nw=0}` adalah pemberian value 0 kepada nw supaya setiap awk dijalankan jumlah nw menjadi 0 kembali, sehingga jumlah kata dari baris sebelumnya tidak terbawa hingga baris sekarang. Bagian `'` guna menutup awk dan `~/praktikum-modul-1-b24/resources/DataPKM.tsv` adalah direktori dari data yang diambil oleh awk.

### Foto Hasil Output
![image](https://github.com/sisop-its-s24/praktikum-modul-1-b24/assets/79700258/f74cc030-3d1e-4e93-997a-fcc9c90b43df)

- b. Code Lengkap:
  ```
  #!/bin/bash

  max=0
  awk -F '\t' '
  {count[$7]++} 
  {if(count[$7] > max) 
    {
      max=count[$7]; 
      mval=$7;
    }
  } END {print mval}' ~/praktikum-modul-1-b24/resources/DataPKM.tsv
  ```
Penjelasan:
```
#!/bin/bash
```
Pada line pertama terdapat shebang/hashbang yang berguna untuk memberi tahu bagaimana program/script tersebut dijalankan, yakni dengan di _execute_ atau di _bash_
```
max=0
```
Mendeklarasikan variabel max dengan nilai 0
```
awk -F '\t' 
```
Kode `awk -F '\t'` digunakan untuk mengambil data dari file .tsv. Terdapat tambahan -F '\t' untuk membuat tab atau '\t' sebagai _field_separator_, sehingga awk dapat mengambil data dari .tsv.
```
{count[$7]++}
```
Bagian ini akan menghitung jumlah kemunculan setiap nilai yang sama dalam kolom ketujuh ketika awk sedang dijalankan (_looping_)
```
{if(count[$7] > max){
```
Merupakan suatu pengondisian dimana jika jumlah kemunculan nilai di kolom ketujuh lebih besar dari nilai maksimum,maka kerjakan code dibawahnya
```
max=count[$7]; 
```
Bagian ini untuk memberikan value kepada max yakni jumlah kemunculan dari nilai yang sama dalam kolom ketujuh. Tujuan utama dari seluruh code ini sebenarnya untuk mencari nilai pada kolom ketujuh yang memiliki kemunculan paling banyak, sehingga logika yang dipakai adalah setiap ada kemunculan yang melebihi kemunculan terbanyak sebelumnya maka akan dijadikan nilai maksimal.
```
mval=$7;}
```
Mendeklarasikan variabel mval dengan value data dari kolom ketujuh, gunanya adalah ketika nanti data kolom ketujuh terbanyak ingin ditampilkan, maka hanya tinggal menampilkan variabel mval. Nilai mval dinyatakan setiap ada jumlah kemunculan yang melebih sebelumnya, sehingga nilai mval adalah data pada kolom ketujuh yang memiliki jumlah kemunculan terbanyak.
```
END {print mval}' ~/praktikum-modul-1-b24/resources/DataPKM.tsv
```
`END` adalah suatu perintah pada awk yang akan dijalankan sekali setelah awk tuntas dijalankan, pada kode tersebut `END` akan menjalankan perintah `{print mval}` untuk menampilkan mval atau data pada kolom ketujuh dengan kemunculan terbanyak. Bagian `~/praktikum-modul-1-b24/resources/DataPKM.tsv` adalah direktori dari data yang akan dipakai sebagai input oleh awk.

### Foto Hasil Output
![image](https://github.com/sisop-its-s24/praktikum-modul-1-b24/assets/79700258/b8fc1997-c08f-45c0-90a5-db93fafcfcac)

- c. Code Lengkap:
  ```
   #!/bin/bash

  awk -F '\t' '
  {count[$2]++} 
  {if(count[$2]>1)
  {print $6}}' ~/praktikum-modul-1-b24/resources/DataPKM.tsv
  ```
Penjelasan:
```
#!/bin/bash
```
Pada line pertama terdapat shebang/hashbang yang berguna untuk memberi tahu bagaimana program/script tersebut dijalankan, yakni dengan di _execute_ atau di _bash_
```
awk -F '\t' '
```
Seperti pada soal 1.a dan 1.b, `awk -F '\t'` digunakan untuk mengambil data dari file .tsv, dengan tambahan -F '\t' untuk membuat tab atau '\t' sebagai _field_separator_, sehingga awk dapat mengambil data dari .tsv.
```
{count[$2]++} 
```
Digunakan untuk menghitung jumlah kemunculan setiap nilai yang sama pada kolom kedua. Sesuai dengan permintaan soal, hal ini digunakan untuk menghitung jumlah kemunculan satu nama mahasiswa.
```
{if(count[$2]>1)
```
Merupakan suatu pengondisian dimana ketika nilai dari jumlah kemunculan data pada kolom kedua lebih dari satu maka eksekusi kode dibawahnya. Sesuai dengan permintaan soal, hal ini digunakan untuk mencari mahasiswa yang telah mengajukan laporan PKM lebih dari sekali.
```
{print $6}} ~/praktikum-modul-1-b24/resources/DataPKM.tsv
```
Merupakan perintah untuk menampilkan data kolom keenam (dosen pembimbing), sesuai dengan permintaan soal ketika ada mahasiswa yang telah mengumpulkan laporan PKM lebih dari satu kali, maka tampilkan penanggung jawab (dosen pembimbing). Kode tersebut hanya akan dijalankan bila pengondisian tersebut bernilai _true_ atau benar. Lalu seperti soal 1.a dan 1.b, `~/praktikum-modul-1-b24/resources/DataPKM.tsv` merupakan direktori yang mengacu pada file .tsv yang akan dipakai awk sebagai input.

### Foto Hasil Output
![image](https://github.com/sisop-its-s24/praktikum-modul-1-b24/assets/79700258/c30763dd-bb2b-425e-9d1d-8262c03191f1)

## Task 2 - Gabut Yuan _(Yuan's Boredom)_
### Soal
Yuan sedang gabut dan lagi makan toge. Setelah kenyang, dia ingin menambah suatu kerjaan baru. Dia sebagai panitia KSN ingin membuat suatu website yang handle login dan register untuk proposal PKM. [File](/resources/data-pkm.csv) yang digunakan sama seperti di nomor 1, hanya saja Yuan ingin memodifikasi file ini:


- a. Yuan ingin membuat file bash login bernama [yu_login.sh](./yu_login.sh) untuk memastikan bahwa peserta yang telah ada di dalam file .csv tersebut, dapat langsung melakukan login, tanpa perlu register. Apabila loginnya sukses, maka akan masuk ke log.txt. Berikut format untuk login:

  - Memasukkan username. Username didapatkan dari kata pertama pada kolom nama_pengusul

  - Memasukkan password. Password didapatkan dari fakultas+nipd_dosen_pendamping

    Misal: `FTEIC0030016403`


  - Setiap percobaan login akan tercatat pada log.txt dengan format `YY/MM/DD hh:mm:ss MESSAGE`

    Dengan MESSAGE berupa:

    `LOGIN: SUCCESS USER_NAME is logged in`

    Atau

    `LOGIN: ERROR Failed login attempt on USER_NAME`

- b. Yuan juga ingin membuat file bash register bernama [yu_register.sh](./yu_register.sh) untuk handle peserta baru yang ingin upload proposal. Data baru ini akan langsung disimpan dalam file .csv tersebut. Ketentuannya adalah:

  - Memasukkan nama_pengusul, asal departemen, fakultas, judul proposal, dosen pendamping (nidn), skema pkm. (Sesuaikan dengan file .csv)
  - Setiap percobaan register akan tercatat pada log.txt dengan format `YY/MM/DD hh:mm:ss MESSAGE`
  
  Dengan MESSAGE berupa:
  
    `REGISTER: SUCCESS USER_NAME is registered. Proposal PROPOSAL_NUMBER is added`
  
    Atau

    `REGISTER: ERROR USER_NAME is already existed`

- c. Yuan tidak ingin capek. Dia membuat automasi di file bash bernama [yu_database.sh](./yu_database.sh) untuk dapat membuat file users.txt guna menyimpan username dan password dari para peserta. Ketentuannya adalah:

  - File users.txt akan diupdate setiap 1 jam sekali

  - Simpan konfigurasi cron pada file [crontabs](./crontabs)
 
### Penyelesaian
- a. Code Lengkap:
  ```
  #!/bin/bash

  read username
  read password
  awk -v username=$username -v password=$password -F, '
  BEGIN {found=0}
  {for(i=6; i<=NF; i++) {if(match($i, "\([0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9]\)")>0) {currentpassword=$i}}}
  {split($2, arr, "_")} 
  {gsub(".*\(", "", currentpassword)}
  {gsub("\).*", "", currentpassword)}
  {if(arr[1]==username) {if(password==$4currentpassword){found=1}}}
  END {if(found==1) 
    {print strftime("%Y/%m/%d %H:%M:%S", systime()) " LOGIN: SUCCESS "username" is logged in" >> "log.txt"} 
  else 
    {print strftime("%Y/%m/%d %H:%M:%S", systime()) " LOGIN: ERROR Failed login attempt on "username >> "log.txt"}}' ~/praktikum-modul-1-b24/resources/data-pkm.csv 
  ```
Penjelasan:
```
#!/bin/bash
```
Pada line pertama terdapat shebang/hashbang yang berguna untuk memberi tahu bagaimana program/script tersebut dijalankan, yakni dengan di _execute_ atau di _bash_
```
read username
```
Merupakan perintah untuk mengambil inputan dari user lalu dimasukkan kedalam variabel username
```
read password
```
Merupakan perintah untuk mengambil inputan dari user lalu dimasukkan kedalam variabel password
```
awk -v username=$username -v password=$password -F, '
```
Merupakan perintah untuk mengambil data dengan mencari suatu nilai dengan menggunakan `awk`. Terdapat atribut tambahan berupa -v username=$username yang berguna untuk mendeklarasikan username memiliki nilai $username dalam awk, begitu juga dengan -v password=$password yakni untuk mendeklarasikan password dengan nilai $password dalam awk. Kemudian juga terdapat `-F,` yang berguna memberi tahu awk bahwa data yang diinput terpisahkan oleh koma (-F artinya _field_separator_, dan `,` artinya _field_separator_ nya berupa koma)
```
BEGIN {found=0}
```
Merupakan code yang akan dilakukan sekali sebelum _looping_ awk dijalankan dengan menggunakan `BEGIN`. Perintah yang akan dijalankan sekali adalah mendeklarasikan nilai `found` dengan nilai 0.
```
 {for(i=6; i<=NF; i++) {if(match($i, "\([0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9]\)")>0) {currentpassword=$i}}}
```
Merupakan kode untuk mencari pola yang cocok dengan sepuluh digit di dalam tanda kurung dalam kolom. Jika pola tersebut ditemukan, nilai dari kolom tersebut disimpan dalam variabel 'currentpassword'. Sebetulnya berdasarkan file .csv, data yang menyimpan id dosen pembimbing ada pada kolom keenam, hanya saja terdapat data pada kolom kelima yang dapat merusak jalannya awk (ada pada kolom kelima data yang menggunakan koma, sehingga awk mengira itu adalah kolom keenam). Untuk mengatasi hal tersebut, dibuatlah suatu perulangan dengan nilai awal i yakni 6, sesuai dengan kolom 6, dan selama i kurang dari sama dengan NF (NF adalah jumlah _field_, sama saja artinya jumlah kolom), maka kode dibawahnya akan terus dijalankan, dan pada akhir satu loop nilai i akan ditambahkan (_auto_increment_). Lalu penggunaan _if_ adalah untuk pengondisian dimana ketika telah ditemukan (`match($i,` $i adalah kolom ke-i) pola 10 angka (`"\([0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9]\)"`), maka data pada kolom tersebut akan dimasukkan kedalam currentpassword (`{currentpassword=$i}}}`).
```
{split($2, arr, "_")} 
```
Berguna untuk memisahkan underscore (`_`) pada data kolom kedua, lalu memasukkan tiap yang terpisahkan tersebut kedalam array arr. Gunanya adalah untuk mendapatkan kata-kata dari kolom kedua, lalu tiap kata dimasukkan ke array arr.
```
{gsub(".*\(", "", currentpassword)}
```
Merupakan kode untuk menghilangkan (mengganti dengan kosong atau `""`) tanda semua data sebelum tanda kurung dalam value yang disimpan currentpassword. Hal ini disebabkan nilai currentpassword merupakan seluruh data kolom ke-i yang dimana masih terdapat nama dosen, yang padahal sesuai dengan permintaan soal, password hanya menggunakan angka id dosen pembimbing.
```
{gsub("\).*", "", currentpassword)}
```
Kurang lebih sama dengan line sebelumnya, yakni untuk menghilangkan data setelah tanda kurung pada value currentpassword.
```
{if(arr[1]==username) {if(password==$4currentpassword){found=1}}}
```
Sesuai permintaan soal, username merupakan kata pertama dari nama mahasiswa pengusul, dan ketika ditemukan username dengan password yang sesuai _login_ dapat dilakukan. Untuk mendapatkan apa yang dimaksud, maka dapat menggunakan pengondisian ketika array arr pada indeks pertama (kata pertama nama mahasiswa/data dalam kolom kedua) sesuai dengan username maka masuk ke perintah selanjutnya, yang dimana perintah selanjutnya juga berupa pengondisian ketika password yang dimasukkan user sama dengan data kolom keempat (fakultas) yang diikuti nomor id dosen pembimbing, maka jadikan variabel found bernilai 1. Dua pengondisian tersebut nantinya dapat menentukan apabila user dapat melakukan _login_ atau tidak.
```
END {if(found==1)
```
Merupakan kode/perintah yang akan dijalankan sekali setelah _loop_ awk dijalankan, yakni dengan `END`. Perintah pertamanya adalah suatu pengondisian dimana nilai found adalah 1 maka jalankan perintah berikutnya.
```
{print strftime("%Y/%m/%d %H:%M:%S", systime()) " LOGIN: SUCCESS "username" is logged in" >> "log.txt"} 
```
Ketika found bernilai 1, maka lakukan perintah untuk memasukkan pesan yang berformat `Tahun/Bulan/Hari Jam:menit:detik LOGIN: SUCCESS "username" is logged in` kedalam file bernama log.txt. Pemasukkan pesan kedalam log.txt menggunakan `>>` sehingga setiap pesan yang dimasukkan akan ditambahkan diakhir dari data pada log.txt. Apabila log.txt belum terbuat, dengan `>>` file tersebut akan otomatis terbentuk. Pada kode ini untuk mengambil data tanggal dan waktu saat ini, dapat menggunakan 'strftime("%Y/%m/%d %H:%M:%S", systime())'. Kemudian `username` mengacu pada nilai variabel username yang berhasil _login_.
```
else
```
Kode tersebut untuk menjalankan perintah berikutnya bila mana pengondisian found sama dengan 1 tidak benar atau _false_.
```
{print strftime("%Y/%m/%d %H:%M:%S", systime()) " LOGIN: ERROR Failed login attempt on "username >> "log.txt"}}' ~/praktikum-modul-1-b24/resources/data-pkm.csv
```
Ketika found tidak bernilai 1, maka perintah tersebut akan jalan. Perintah tersebut akan memasukkan pesan dengan format `Tahun/Bulan/Hari Jam:menit:detik LOGIN: ERROR Failed login attempt on username` (dengan username merupakan nilai variabel username) ke dalam suatu file bernama log.txt. Sama dengan sebelumnya untuk mengambil tanggal dan waktu terkini dapat menggunakan 'strftime("%Y/%m/%d %H:%M:%S", systime())'. Pemasukkan pesan kedalam log.txt menggunakan `>>` sehingga setiap pesan yang dimasukkan akan ditambahkan diakhir dari data pada log.txt. Apabila log.txt belum terbuat, dengan `>>` file tersebut akan otomatis terbentuk.


### Foto Hasil Output
![image](https://github.com/sisop-its-s24/praktikum-modul-1-b24/assets/79700258/e354cf64-02fb-4ec3-ab19-2a642d3d38f8)
![image](https://github.com/sisop-its-s24/praktikum-modul-1-b24/assets/79700258/55fc12a9-4eef-46ec-8e0d-cf6dd6065754)

Jika diinputkan dengan password salah

![image](https://github.com/sisop-its-s24/praktikum-modul-1-b24/assets/79700258/fb885fb5-f9a1-4892-b4bd-7f139040bf31)
![image](https://github.com/sisop-its-s24/praktikum-modul-1-b24/assets/79700258/8b31d702-7e48-4621-b08b-80d29facb6f8)

- b. Code Lengkap:
```
#!/bin/bash

read np
read dep
read fak
read jud
read dp
read skema
nomor=$(awk -F, 'END{print $1+1}' ~/praktikum-modul-1-b24/resources/data-pkm.csv)
awk -v nomor="$nomor"  -v dep="$dep" -v fak="$fak" -v  jud="$jud" -v dp="$dp" -v skema="$skema" -v np="$np"  -F, 'BEGIN{f=0} {if(np==$2){f=1}}END{if(f==0){
OFS=","; print nomor,np,dep,fak,jud,dp,skema >> "temp.csv";
print "REGISTER: SUCCESS "np" is registered. Proposal "nomor" is added" >> "log.txt";
}else {print strftime("%Y/%m/%d %H:%M:%S",systime())" REGISTER : ERROR "np" is already existed">>"log.txt"}}' ~/praktikum-modul-1-b24/resources/data-pkm.csv

if [ -f "temp.csv" ]; then
cat  "temp.csv"  >> ~/praktikum-modul-1-b24/resources/data-pkm.csv
rm "temp.csv"
fi
```
Penjelasan:
```
#!/bin/bash
```
Pada line pertama terdapat shebang/hashbang yang berguna untuk memberi tahu bagaimana program/script tersebut dijalankan, yakni dengan di _execute_ atau di _bash_
```
read np
```
Merupakan perintah untuk mengambil inputan dari user lalu dimasukkan kedalam variabel np
```
read dep
```
Merupakan perintah untuk mengambil inputan dari user lalu dimasukkan kedalam variabel dep
```
read fak
```
Merupakan perintah untuk mengambil inputan dari user lalu dimasukkan kedalam variabel fak
```
read jud
```
Merupakan perintah untuk mengambil inputan dari user lalu dimasukkan kedalam variabel jud
```
read dp
```
Merupakan perintah untuk mengambil inputan dari user lalu dimasukkan kedalam variabel dp
```
read skema
```
Merupakan perintah untuk mengambil inputan dari user lalu dimasukkan kedalam variabel skema
```
nomor=$(awk -F, 'END{print $1+1}' ~/praktikum-modul-1-b24/resources/data-pkm.csv)
```
Merupakan pendeklarasian variabel nomor yang memiliki nilai hasil dari perintah awk (mengambil data dari file) dengan _field_separator_ yakni `,` dengan `END` (perintah yang dijalankan sekali setelah awk dijalankan) untuk melakukan penjumlahan pada data kolom pertama dengan 1, dari data file  `~/praktikum-modul-1-b24/resources/data-pkm.csv`. Hal ini fungsinya adalah untuk mengambil nomor terakhir tambah 1, yang nantinya menjadi berguna saat memasukkan data baru kedalam file.
```
awk -v nomor="$nomor"  -v dep="$dep" -v fak="$fak" -v  jud="$jud" -v dp="$dp" -v skema="$skema" -v np="$np"  -F,
```
Merupakan perintah untuk mengambil data dengan mencari suatu nilai dengan menggunakan `awk`. Terdapat atribut tambahan berupa `-v nomor="$nomor"  -v dep="$dep" -v fak="$fak" -v  jud="$jud" -v dp="$dp" -v skema="$skema""` yang berguna untuk mendeklarasikan nomor, dep, fak, jud, dp, skema, dan np. Kemudian juga terdapat `-F,` yang berguna memberi tahu awk bahwa data yang diinput terpisahkan oleh koma (-F artinya _field_separator_, dan `,` artinya _field_separator_ nya berupa koma) sehingga awk dapat mengambil data dari file bertipe .csv.
```
'BEGIN{f=0} {if(np==$2){f=1}}END{if(f==0){
```
Merupakan perintah yang dilakukan hanya sekali sebelum awk dijalankan (`BEGIN`). Dimana perintah yang dilakukan adalah mendeklarasikan variabel f dengan nilai 0. Kemudian masuk kedalam awk utama, setiap _looping_ awk, akan dilakukan suatu pengondisian dimana bila nilai np yang dimasukkan user sama dengan nilai pada data kolom kedua, maka jadikan variabel f bernilai 1. Kemudian pada akhir awk dilakukan suatu perintah yakni `END` dengan perintah pertama yakni pengondisian bila nilai f sama dengan 0 maka lakukan perintah-perintah berikutnya. 
```
OFS=","; print nomor,np,dep,fak,jud,dp,skema >> "temp.csv";
```
Merupakan perintah yang dilakukan ketika pengondisian f sama dengan 1 benar, yakni memasukkan data nomor,np,dep,fak,jud,dp, dan skema kedalam file temp.csv. Digunakan `OFS=","` yang berguna untuk memberi tahu perintah print bahwa `,' adalah pemisah data, jadi data yang dimasukkan nantinya berupa `nomor np dep fak jud dp skema`. Pemasukkan data juga menggunakan `>>` sehingga bila file temp.csv belum terbentuk, maka akan otomatis terbentuk, dan bila sudah ada data di dalamnya, maka data akan disisipkan setelah data yang telah ada di temp.csv.
```
print "REGISTER: SUCCESS "np" is registered. Proposal "nomor" is added" >> "log.txt";
```
Kemudian selain memasukkan data-data tersebut kedalam temp.csv, apabila pengondisian _true_ maka masukkan pula pesan "REGISTER: SUCCESS "np" is registered. Proposal "nomor" is added" kedalam file log.txt dengan np dan nomor merupakan nilai dari variabel np dan nomor. Sama seperti pemasukkan sebelumnya yakni menggunakan `>>`, sehingga bila file belum terbentuk maka akan otomatis terbentuk, dan bila sudah ada data didalamnya maka data yang dimasukkan akan disisipkan setelah data yang sudah ada.
```
}else {print strftime("%Y/%m/%d %H:%M:%S",systime())" REGISTER : ERROR "np" is already existed">>"log.txt"}}' ~/praktikum-modul-1-b24/resources/data-pkm.csv
```
Merupakan kode yang akan melakukan perintah ketika nilai f sama dengan 1 adalah _false_ atau tidak benar. Perintah yang dilakukan adalah memasukkan ke dalam file log.txt pesan berformat `Tahun/Bulan/Hari Jam:menit:detik REGISTER : ERROR np is already existed`, dengan np merupakan nilai dari variabel np. Untuk mendapatkan tanggal dan waktu terkini dapat menggunakan `strftime("%Y/%m/%d %H:%M:%S",systime())`. Terakhir `~/praktikum-modul-1-b24/resources/data-pkm.csv` digunakan untuk memberi tahu awk dimana file .csv yang akan dipakai.

Setelah awk sebelumnya dijalankan, maka untuk memenuhi permintaan soal dimana data yang dimasukkan langsung ke dalam .csv, maka diperlukan mekanisme untuk memindahkan data dari file temp.csv ke data-pkm.csv. 
```
if [ -f "temp.csv" ]; then
```
Pertama dibuatlah suatu pengondisian dimana ketika temp.csv ada/terbentuk maka lakukan perintah berikutnya.
```
cat  "temp.csv"  >> ~/praktikum-modul-1-b24/resources/data-pkm.csv
```
Bila terdapat temp.csv, masukkan data pada temp.csv ke dalam ~/praktikum-modul-1-b24/resources/data-pkm.csv. Mekanisme pemasukkan menggunakan `>>` sehingga data yang dimasukkan tidak akan menimpa data yang sudah ada, melainkan akan disisipkan setelah data yang sudah ada sebelumnya.
```
rm "temp.csv"
```
Ketika sudah dilakukan pemindahan data, hapus temp.csv karena sudah tidak terpakai.
```
fi
```
Kode untuk menutup pengondisian _if_.

### Foto Hasil Output
![image](https://github.com/sisop-its-s24/praktikum-modul-1-b24/assets/79700258/76d4dfab-bf89-4cba-b74c-e4b02315546c)
![image](https://github.com/sisop-its-s24/praktikum-modul-1-b24/assets/79700258/5d1bcdae-fbed-49c6-ae55-dc0a478e1834)

- c. Code Lengkap:
```
#!/bin/bash

awk -F, '
{if(NR==1){next}}
{for(i=6; i<=NF; i++) {if(match($i, "\([0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9]\)")>0) {currentpassword=$i}}}
{split($2, arr, "_")} 
{gsub(".*\(", "", currentpassword)}
{gsub("\).*", "", currentpassword)}
{print arr[1] " " $4 currentpassword > "users.txt"}
' ~/praktikum-modul-1-b24/resources/data-pkm.csv
```
Penjelasan:
```
#!/bin/bash
```
Pada line pertama terdapat shebang/hashbang yang berguna untuk memberi tahu bagaimana program/script tersebut dijalankan, yakni dengan di _execute_ atau di _bash_
```
awk -F, '
```
Merupakan perintah untuk mengambil data dari suatu file dengan atribut `-F,` yang berguna untuk memberitahu bahwa file yang diambil terpisahkan dengan `,`. Kemudian `'` adalah syntax untuk membuka awk.
```
{if(NR==1){next}}
```
Merupakan perintah yang merupakan suatu pengondisian dimana ketika NR bernilai 1 maka lakukan perintah `next`. Perintah tersebut pada dasarnya berfungsi untuk mengabaikan baris pertama dari data yang ada di file karena format pada data adalah nama kolom baru data-datanya pada baris berikutnya.
```
{for(i=6; i<=NF; i++) {if(match($i, "\([0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9]\)")>0) {currentpassword=$i}}}
```
Sama seperti soal 2.a, perintah tersebut adalah untuk mencari suatu pola yang terdiri dari 10 angka (id dosen pembimbing) dari kolom keenam dan seterusnya yang kemudian data pada kolom tersebut dimasukkan kedalam variabel currentpassword. Penggunaan _for_loop_ digunakan untuk mencegah apabila data pada kolom kelima dianggap kolom keenam oleh awk karena terdapat `,` pada kolom kelima. Pengulangan dilakukan terus dan tiap kolomnya dicek apakah ditemukan pola 10 angka pada kolom tersebut (`{for(i=6; i<=NF; i++) {if(match($i, "\([0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9]\)")>0) {currentpassword=$i}}}`) bila ditemukan, masukkan data pada kolom tersebut ke dalam currentpassword.
```
{split($2, arr, "_")} 
```
Kode tersebut untuk memisahkan `_` dari data dalam kolom kedua, dan hasilnya dimasukkan kedalam array arr.
```
{gsub(".*\(", "", currentpassword)}
```
Merupakan pengubahan seluruh data sebelum `(` pada variabel currentpassword ke bentuk `""` atau kosong jadi sama saja menghilangkan seluruh data sebelum tanda kurung dalam currentpassword. Hal ini dilakukan untuk memenuhi kebutuhan permintaan soal dimana password yang diminta hanya terdiri dari nama fakultas dan id dosen pembimbing.
```
{gsub("\).*", "", currentpassword)}
```
Merupakan pengubahan seluruh data setelah `)` pada variabel currentpassword ke bentuk `""` atau kosong jadi sama saja menghilangkan seluruh data setelah tanda kurung dalam currentpassword. Hal ini dilakukan untuk memenuhi kebutuhan permintaan soal dimana password yang diminta hanya terdiri dari nama fakultas dan id dosen pembimbing.
```
{print arr[1] " " $4 currentpassword > "users.txt"}
```
Merupakan kode untuk memasukkan data indeks pertama dari array arr, data pada kolom keempat, dan variabel current password ke dalam users.txt. Nilai pada indeks pertama array arr merupakan kata pertama dari nama mahasiswa. Kolom keempat berisi nama fakultas dari mahasiswa tersebut dan currentpassword menyimpan id dosen pembimbing. Memasukkan data menggunakan `>` karena supaya bila ada perubahan atau update data, data yang dimasukkan menimpa data berikutnya supaya tidak ada data yang ganda.
```
' ~/praktikum-modul-1-b24/resources/data-pkm.csv
```
Terakhir, terdapat `'` untuk menutup awk, dan ` ~/praktikum-modul-1-b24/resources/data-pkm.csv` untuk memberi tahu awk dimana file data yang dipakai sebagai input.

Selain dari kode untuk 2.c, soal juga meminta supaya file `.sh` dapat dijalankan setiap jam sekali sehingga diperlukan cronjob pada file tersebut. Maka dibuatlah cronjob yakni `0 * * * * !/bin/bash ~/praktikum-modul-1-b24/task-2/yu_database.sh` yang dituliskan pada file `crontabs`. Sehingga apabila user ingin menjalankan cronjob dari file tersebut, maka user dapat menggunakan perintah `crontab crontabs` pada terminal.

### Foto Hasil Output
![image](https://github.com/sisop-its-s24/praktikum-modul-1-b24/assets/79700258/0241066f-8638-4759-96d3-bc879d388345)
![image](https://github.com/sisop-its-s24/praktikum-modul-1-b24/assets/79700258/2e853568-fc8a-4765-b190-1d7129417f0e)

## Task 3 - Magang Bubu _(Bubu's Internship)_
### Soal
Bubu adalah seorang IT Support magang di PT. Pencari Cinta. Bubu diminta oleh atasannya untuk memonitor setiap server yang dimiliki oleh kantor. Karena Bubu sangat sibuk mengawal PKM, maka Bubu meminta kalian untuk membuatkan program monitoringnya.
Buatlah program monitoring resource pada setiap server. Cukup monitoring RAM dan monitoring size suatu directory. Untuk RAM gunakan command free -m. Untuk disk gunakan command du -sh <target_path>. Catat semua metrics yang didapatkan dari hasil free -m. Untuk hasil du -sh <target_path> catat size dari path directory tersebut. Untuk target_path yang akan dimonitor adalah /home/{user}/.
- a. Masukkan semua metrics ke dalam suatu file log bernama `metrics_{YmdHms}.log`. `{YmdHms}` adalah waktu disaat file script bash kalian dijalankan. Misal dijalankan pada 2024-03-30 15:00:00, maka file log yang akan tergenerate adalah `metrics_20240330150000.log`.
- b. Script untuk mencatat metrics diatas diharapkan dapat berjalan otomatis setiap menit.
- c. Kemudian, buat satu script untuk membuat aggregasi file log ke satuan jam. Script aggregasi akan memiliki info dari file-file yang tergenerate tiap menit. Dalam hasil file aggregasi tersebut, terdapat nilai minimum, maximum, dan rata-rata dari tiap-tiap metrics. File aggregasi akan ditrigger untuk dijalankan setiap jam secara otomatis. Berikut contoh nama file hasil aggregasi `metrics_agg_2023033015.log` dengan format `metrics_agg_{YmdH}.log`.
- d. Selanjutnya agar lebih menghemat penyimpan, buatlah script [backup_metrics.sh](./bakcup_metrics.sh). Dimana script ini akan menyimpan semua log metrics aggregasi mulai dari pukul 00:00 sampai 23:59 didalam 1 file .gz menggunakan gunzip. Contoh nama file hasil zip `backup_metrics_{date_YmdH}.gz`
- e. Karena file log bersifat sensitif pastikan semua file log hanya dapat dibaca oleh user pemilik file.

**Notes:**
- Nama file untuk script per menit adalah [minute_log.sh](./minute_log.sh)
- Nama file untuk script aggregasi per jam adalah [aggregate_minutes_to_hourly_log.sh](./aggregate_minutes_to_hourly_log.sh)
- Semua file log terletak di `/home/{user}/metrics`
- Konfigurasi cron diletakkan di file [crontabs](./crontabs)
**Berikut adalah contoh isi dari file metrics yang dijalankan tiap menit:**
```csv
mem_total,mem_used,mem_free,mem_shared,mem_buff,mem_available,swap_total,swap_used,swap_free,path,path_size
15949,10067,308,588,5573,4974,2047,43,2004,/home/$USER/test/,74M
```
**Berikut adalah contoh isi dari file aggregasi yang dijalankan tiap jam:**
```csv
type,mem_total,mem_used,mem_free,mem_shared,mem_buff,mem_available,swap_total,swap_used,swap_free,path,path_size
minimum,15949,10067,223,588,5339,4626,2047,43,1995,/home/$USER/test/,50M
maximum,15949,10387,308,622,5573,4974,2047,52,2004,/home/$SERtest/,74M
average,15949,10227,265.5,605,5456,4800,2047,47.5,1999.5,/home/$USER/test/,62M
```
### Penyelesaian
- a. Code Lengkap:

```
#!/bin/bash

read mt mu mf ms mb ma < <(free -m | awk 'NR==2{print $2,$3,$4,$5,$6,$7}')
read st su sf << (free -m | awk 'NR==3{print $2,$3,$4}')

Date=$(date +'%Y%m%d%H%M%S')

echo "mem_total,mem_used,mem_free,mem_shared,mem_buff,mem_available,swap_total,swap_used,swap_free,path,path_size" > "metrics_${Date}.log"

echo "$mt,$mu,$mf,$ms,$mb,$ma,$st,$su,$sf,~/,$(du -sh ~/ | awk '{print $1}')" >> "metrics_${Date}.log"

chmod go-rwx "metrics_${Date}.log"
chmod u+rwx "metrics_${Date}.log"
```

Penjelasan:

```
#!/bin/bash
```
Merupakan kode untuk memberi tahu sistem operasi apa yang harus dilakukan terhadap file. Sistem operasi akan tahu kalau file .sh ketika dijalankan akan langsung di _execute_ atau di _bash_
```
read mt mu mf ms mb mc ma < <(free -m | awk 'NR==2{print $2,$3,$4,$5,$6,$7,$8}')
```
Merupakan kode untuk mengambil hasil perintah `free -m` dan  `awk 'NR==2{print $2,$3,$4,$5,$6,$7,$8}')` dan dimasukkan kedalam variabel mt mu mf ms mb mc ma. Perintah `free -m` adalah perintah untuk membersihkan memori pada perangkat user, kemudian perintah `awk` akan mengambil hasil dari `free -m` dengan `NR==2` yakni pada baris kedua akan dilakukan perintah `print $2,$3,$4,$5,$6,$7,$8` yang akan memasukkan nilai data pada urutan ke 2 hingga 8 dan dimasukkan ke 7 variabel diawal. 
```
read st su sf << (free -m | awk 'NR==3{print $2,$3,$4}')
```
Sama seperti sebelumnya, bagian ini adalah untuk mengambil keluaran dari perintah `free -m` pada baris ketiga dan dimasukkan ke dalam variabel st, su, dan sf. 
```
Date=$(date +'%Y%m%d%H%M%S')
```
Kode tersebut adalah sebuah pendeklarasian variabel Date yang berisi Tahun/Bulan/HariJam:Menit:Detik sekarang yang nantinya variabel ini akan dipakai dalam penamaan file.
```
echo "mem_total,mem_used,mem_free,mem_shared,mem_buff,mem_available,swap_total,swap_used,swap_free,path,path_size" > "metrics_${Date}.log"
```
Sesuai dengan permintaan soal, dibuatlah suatu file log dengan nama metrics_Tahun/Bulan/HariJam:Menit:Detik sekarang dengan eksistensi file berupa `.log`. Untuk mengambil waktu sekarang dapat digunakan variabel Date sehingga nama file menjadi `"metrics_${Date}.log"`. Setelah dibuat maka dimasukkanlah suatu baris untuk menjadi awal mula dari isi file yakni `"mem_total,mem_used,mem_free,mem_shared,mem_buff,mem_available,swap_total,swap_used,swap_free,path,path_size"`/ Digunakanlah perintah `echo` dan `>` untuk memasukkan kata-kata tersebut ke dalam file log. Sebetulnya apabila file metrics_Date tidak ditemukan, maka serangkaian perintah tersebut akan membuat file metrics_Date dengan sendirinya.
```
echo "$mt,$mu,$mf,$ms,$mb,$ma,$st,$su,$sf,~/,$(du -sh ~/ | awk '{print $1}')" >> "metrics_${Date}.log"
```
Seperti sebelumnya serangkaian perintah `echo` diatas adalah perintah memasukkan data ke dalam metrics_Date. Hanya saja pada perintah kali ini, yang dimasukkan adalah nilai-nilai dari tujuh variabel awal. Lalu perbedaan lainnya adalah dengan adanya tambahan `$(du -sh ~/ | awk '{print $1}')`, yang sesuai permintaan soal, selain tujuh variabel dari perintah `free -m` dimasukkan pula hasil dari perintah `du -sh ~/` ke dalam metrics_Date dan dengan menggunakan perintah `awk '{print $1}')` untuk mengambil hasil perintah `du -sh ~/`. Kemudian kode ini menggunakan `>>` bukan `>` supaya data yang dimasukkan tidak menimpa kata-kata yang telah dimasukkan sebelumnya melainkan disisipkan setelah data sebelumnya. 
```
chmod go-rwx "metrics_${Date}.log"
chmod u+rwx "metrics_${Date}.log"
```
Sebetulnya dua baris kode tersebut adalah untuk jawaban soal 3.e yang dimana diminta file metrics_Date.log hanya dapat diakses oleh pemilik dan pembuat file sehingga diperlukan `chmod` untuk memberikan _permission_ khusus untuk mengakses file.

### Foto Hasil Output
![image](https://github.com/sisop-its-s24/praktikum-modul-1-b24/assets/79700258/cca76a5b-e026-4536-b233-d5d7ea7d47ad)
![image](https://github.com/sisop-its-s24/praktikum-modul-1-b24/assets/79700258/e44c7aa2-a2d5-4a4f-a210-d1af46bb3ba0)

- b. Code Lengkap:
```
* * * * * !/bin/bash ~/praktikum-modul-1-b24/task-3/minute_log.sh
```
Penjelasan:
Pada 3.b, soal meminta untuk menjalankan `.sh` soal 3.a supaya dapat dijalankan setiap 1 menit sekali, maka diperlukan cronjob yang dapat menjalankan file secara otomatis. Dengan begitu diperlukan `* * * * *` yang artinya file akan dijalankan setiap menit, kemudian ada pula kode `!/bin/bash` yan fungsinya adalah untuk memberi tahu sistem operasi bahwa file yang diberi cronjob akan di _bash_, dan `~/praktikum-modul-1-b24/task-3/minute_log.sh` adalah file `.sh` beserta direktorinya untuk memberi tahu cronjob file mana yang akan dijalankan secara otomatis.

-  c. Code Lengkap:
```
#!/bin/bash

log_dir="/home/mob/praktikum-modul-1-b24/task-3"

datetime=$(date +'%Y%m%d%H')

cat "$log_dir"/metrics_"$datetime"*.log > "$log_dir/metrics_agg_$datetime.log"

rown=0

min_total=9999999999
min_used=9999999999
min_free=9999999999
min_shared=9999999999
min_buff=9999999999
min_available=9999999999
min_swaptotal=9999999999
min_swapused=9999999999
min_swapfree=9999999999
min_pathsize=9999999999

max_total=0
max_used=0
max_free=0
max_shared=0
max_buff=0
max_available=0
max_swaptotal=0
max_swapused=0
max_swapfree=0
max_pathsize=0

avg_total=0
avg_used=0
avg_free=0
avg_shared=0
avg_buff=0
avg_available=0
avg_swaptotal=0
avg_swapused=0
avg_swapfree=0
avg_pathsize=0

min() {
  local a=$1
  local b=$2
  if ((a<b)); then
    echo "$a"
  else
    echo "$b"
  fi
}

max() {
  local a=$1
  local b=$2
  if ((a>b)); then
    echo "$a"
  else
    echo "$b"
  fi
}

while IFS= read -r line; do
  ((rown++))
  if [ $((rown % 2)) -eq 0 ]; then
    IFS=',' read -ra data <<< "$line"
    cnt=0
    for i in "${data[@]}"; do
      ((cnt++))
      if [ $((cnt)) -eq 1 ]; then
        min_total=$(min "$min_total" "$i")
        max_total=$(max "$max_total" "$i")
        ((avg_total+=$i))
      elif [ $((cnt)) -eq 2 ]; then
        min_used=$(min "$min_used" "$i")
        max_used=$(max "$max_used" "$i")
        ((avg_used+=$i))
      elif [ $((cnt)) -eq 3 ]; then
        min_free=$(min "$min_free" "$i")
        max_free=$(max "$max_free" "$i")
        ((avg_free+=$i))
      elif [ $((cnt)) -eq 4 ]; then
        min_shared=$(min "$min_shared" "$i")
        max_shared=$(max "$max_shared" "$i")
        ((avg_shared+=$i))
      elif [ $((cnt)) -eq 5 ]; then
        min_buff=$(min "$min_buff" "$i")
        max_buff=$(max "$max_buff" "$i")
        ((avg_buff+=$i))
      elif [ $((cnt)) -eq 6 ]; then
        min_available=$(min "$min_available" "$i")
        max_available=$(max "$max_available" "$i")
        ((avg_available+=$i))
      elif [ $((cnt)) -eq 7 ]; then
        min_swaptotal=$(min "$min_swaptotal" "$i")
        max_swaptotal=$(max "$max_swaptotal" "$i")
        ((avg_swaptotal+=$i))
      elif [ $((cnt)) -eq 8 ]; then
        min_swapused=$(min "$min_swapused" "$i")
        max_swapused=$(max "$max_swapused" "$i")
        ((avg_swapused+=$i))
      elif [ $((cnt)) -eq 9 ]; then
        min_swapfree=$(min "$min_swapfree" "$i")
        max_swapfree=$(max "$max_swapfree" "$i")
        ((avg_swapfree+=$i))
      elif [ $((cnt)) -eq 11 ]; then
        num=$(echo "$i" | tr -cd '[:digit:]')
        suf=$(echo "$i" | tr -cd '[:alpha:]')
        if [ "$suf" = "G" ]; then
          num=$((num * 1048576))
        elif [ "$suf" = "M"]; then
          num=$((num * 1024))
        fi
        min_pathsize=$(min "$min_pathsize" "$((num))")
        max_pathsize=$(max "$max_pathsize" "$((num))")
        ((avg_pathsize+=$((num))))
      fi
    done
  fi
done < "$log_dir/metrics_agg_$datetime.log"

avg_rn=$((rown / 2))

avg_total=$((avg_total / avg_rn))
avg_used=$((avg_used / avg_rn))
avg_free=$((avg_free / avg_rn))
avg_shared=$((avg_shared / avg_rn))
avg_buff=$((avg_buff / avg_rn))
avg_available=$((avg_available / avg_rn)) 
avg_swaptotal=$((avg_swaptotal / avg_rn))
avg_swapused=$((avg_swapused / avg_rn))
avg_swapfree=$((avg_swapfree / avg_rn))
avg_pathsize=$((avg_pathsize / avg_rn))

echo "type,mem_total,mem_used,mem_free,mem_shared,mem_buff,mem_available,swap_total,swap_used,swap_free,swap_cache,path,path_size" > "$log_dir/metrics_agg_$datetime.log"
echo "minimum,$min_total,$min_used,$min_free,$min_shared,$min_buff,$min_available,$min_swaptotal,$min_swapused,$min_swapfree,$log_dir,$min_pathsize""K" >> "$log_dir/metrics_agg_$datetime.log"
echo "maximum,$max_total,$max_used,$max_free,$max_shared,$max_buff,$max_available,$max_swaptotal,$max_swapused,$max_swapfree,$log_dir,$max_pathsize""K">> "$log_dir/metrics_agg_$datetime.log"
echo "average,$avg_total,$avg_used,$avg_free,$avg_shared,$avg_buff,$avg_available,$avg_swaptotal,$avg_swapused,$avg_swapfree,$log_dir,$avg_pathsize""K">> "$log_dir/metrics_agg_$datetime.log"
```
Penjelasan:
```
#!/bin/bash
```
Adalah kode untuk memberi tahu sistem operasi bahwa file `.sh` baru di _bash_
```
log_dir="/home/mob/praktikum-modul-1-b24/task-3"
```
Mendeklarasikan variabel `log_dir` dengan value direktori dari task-3 supaya mempermudah kedepannya.
```
datetime=$(date +'%Y%m%d%H')
```
Mendeklarasikan variabel `datetime` dengan value yakni hasil dari perintah `date +'%Y%m%d%H'` yang dimana hasilnya adalah tahun, bulan, hari, dan jam saat ini.
```
cat "$log_dir"/metrics_"$datetime"*.log > "$log_dir/metrics_agg_$datetime.log"
```
Merupakan perintah untuk memasukkan seluruh isi (`cat`) segala file dengan direktori sesuai dengan `log_dir` dan dengan nama yang diawali dengan metrics_tahunbulanharijam saat ini (value datetime) dan dengan eksistensi `.log` ke dalam file dengan direktori `log_dir` dan dengan nama `"metrics_agg_$datetime.log"`.
```
rown=0
```
Mendeklarasikan suatu variabel bernama `rown` dengan value awal 0
```
min_total=9999999999
min_used=9999999999
min_free=9999999999
min_shared=9999999999
min_buff=9999999999
min_available=9999999999
min_swaptotal=9999999999
min_swapused=9999999999
min_swapfree=9999999999
min_pathsize=9999999999
```
Mendeklarasikan variabel minimum dari setiap variabel yang ada dalam file metrics_date.log (seperti pada soal sebelumnya) yang dimana setiap variabel minimum diberi value 9999999999 yang gunanya adalah untuk pembandingan/komparasi nantinya.
```
max_total=0
max_used=0
max_free=0
max_shared=0
max_buff=0
max_available=0
max_swaptotal=0
max_swapused=0
max_swapfree=0
max_pathsize=0
```
Sama seperti sebelumnya, hanya saja pada bagian ini yang dideklarasikan adalah maksimum dari setiap variabel dengan value tiap-tiapnya 0.
```
avg_total=0
avg_used=0
avg_free=0
avg_shared=0
avg_buff=0
avg_available=0
avg_swaptotal=0
avg_swapused=0
avg_swapfree=0
avg_pathsize=0
```
Kode tersebut adalah pendeklarasian dari rata-rata awal dari setiap varibel dengan nilai awal yakni 0.
```
min() {
  local a=$1
  local b=$2
  if ((a<b)); then
    echo "$a"
  else
    echo "$b"
  fi
}
```
Merupakan pendeklarasian fungsi bernama `min` yang gunanya sebagai fungsi pembandingan dan mencari nilai minimum dari setiap variabel dalam kumpulan file metrics_date.log. Komparasi dimulai dengan mendeklarasikan variabel `local a` dan `local b` dengan value data pertama dan kedua dari inputan yang dibandingkan, kemudian gunakan pengondisian apa bila a lebih kecil dari b maka keluarkan outputan a, begitu juga sebaliknya. 
```
while IFS= read -r line; do
  ((rown++))
  if [ $((rown % 2)) -eq 0 ]; then
    IFS=',' read -ra data <<< "$line"
    cnt=0
    for i in "${data[@]}"; do
      ((cnt++))
      if [ $((cnt)) -eq 1 ]; then
        min_total=$(min "$min_total" "$i")
        max_total=$(max "$max_total" "$i")
        ((avg_total+=$i))
      elif [ $((cnt)) -eq 2 ]; then
        min_used=$(min "$min_used" "$i")
        max_used=$(max "$max_used" "$i")
        ((avg_used+=$i))
      elif [ $((cnt)) -eq 3 ]; then
        min_free=$(min "$min_free" "$i")
        max_free=$(max "$max_free" "$i")
        ((avg_free+=$i))
      elif [ $((cnt)) -eq 4 ]; then
        min_shared=$(min "$min_shared" "$i")
        max_shared=$(max "$max_shared" "$i")
        ((avg_shared+=$i))
      elif [ $((cnt)) -eq 5 ]; then
        min_buff=$(min "$min_buff" "$i")
        max_buff=$(max "$max_buff" "$i")
        ((avg_buff+=$i))
      elif [ $((cnt)) -eq 6 ]; then
        min_available=$(min "$min_available" "$i")
        max_available=$(max "$max_available" "$i")
        ((avg_available+=$i))
      elif [ $((cnt)) -eq 7 ]; then
        min_swaptotal=$(min "$min_swaptotal" "$i")
        max_swaptotal=$(max "$max_swaptotal" "$i")
        ((avg_swaptotal+=$i))
      elif [ $((cnt)) -eq 8 ]; then
        min_swapused=$(min "$min_swapused" "$i")
        max_swapused=$(max "$max_swapused" "$i")
        ((avg_swapused+=$i))
      elif [ $((cnt)) -eq 9 ]; then
        min_swapfree=$(min "$min_swapfree" "$i")
        max_swapfree=$(max "$max_swapfree" "$i")
        ((avg_swapfree+=$i))
      elif [ $((cnt)) -eq 11 ]; then
        num=$(echo "$i" | tr -cd '[:digit:]')
        suf=$(echo "$i" | tr -cd '[:alpha:]')
        if [ "$suf" = "G" ]; then
          num=$((num * 1048576))
        elif [ "$suf" = "M"]; then
          num=$((num * 1024))
        fi
        min_pathsize=$(min "$min_pathsize" "$((num))")
        max_pathsize=$(max "$max_pathsize" "$((num))")
        ((avg_pathsize+=$((num))))
      fi
    done
  fi
done < "$log_dir/metrics_agg_$datetime.log"
```
Bagian kode diatas adalah serangkaian kode untuk menentukan data terkecil dan terbesar dari data-data file `metrics_DATE.log`. Mulanya dibuatlah suatu while loop dimana selama file yang dapat terbaca (ada isinya) maka jalankan kode dibawahnya. Kode atau perintah yang pertama dijalankan dalam loop adalah _auto_increment_ terhadap variabel `rown`. Kemudian dibuatlah suatu pengondisian apabila `rown` dimodulo 2 hasilnya 0 (genap), maka jalankan perintah didalam pengondisian _if_. Pengondisian tersebut dibuat untuk membaca setiap baris genap, karena sesuai format isian dalam `metrics_DATE` dimana terdapat dua baris yakni penamaan kolom di baris pertama dan data-datanya di baris kedua, maka untuk perbandingan data, yang diambil hanya pada baris yang genap. Lalu dalam pengondisian, untuk membaca setiap data yang terpisahkan oleh koma dibuatlah perintah `    IFS=',' read -ra data <<< "$line"`, dimana `IFS=','` menunjukkan bahwa setiap data dipisahkan oleh koma, lalu `read -ra data  <<< "$line"` untuk membaca data dari setiap baris. Terbuat juga variabel `data` dengan value setiap baris atau `line` dari file. Kemudian buat variabel `cnt` dengan value awal 0, variabel ini nantinya dipakai untuk menentukan data ke berapa, sehingga nantinya pengomparasian bisa tiap data (seperti indexing). 

Untuk pengomparasian data dibuatlah for loop dimana untuk setiap i dalam variabel data maka jalankan perintah dibawahnya. Perintah pertama dalam _for_ adalah _auto_increment_ pada variabel `cnt` untuk indexing data. Kemudian dibuatlah beberapa pengondisian untuk pengomparasian setiap variabel/data tiap barisnya. Setiap data dibandingkan dengan kode berikut:
```
 if [ $((cnt)) -eq 1 ]; then
        min_total=$(min "$min_total" "$i")
        max_total=$(max "$max_total" "$i")
        ((avg_total+=$i))
```
Setiap data pada baris genap akan dibandingkan dengan serangkaian kode tersebut. Perbedaan tiap kode pada pengondisian yakni untuk data pertama dengan `-eq 1`, data kedua `-eq 2` dan seterusnya. Maksud pengondisian tersebut adalah apabila indexing (cnt) berada pada bernilai angka pembanding maka index tersebut berada pada data ke-angka tersebut, dan lakukan perintah-perintah dibawahnya. Perintah dibawahnya yakni memberikan value kepada variabel minimum dari data/variabel dan maksimum dari data/variabel yang dibandingkan, dengan value dari minimumnya adalah hasil dari perintah pemanggilan fungsi `min` dengan inputan `min_total` dan value dari data pada indeks ke cnt pada baris i, begitu juga dengan maksimum. Artinya adalah setiap data ditemukan, maka _update_ value dari minimum dan maksimumnya dengan pemanggilan fungsi perbandingan (min dan max), sehingga apabila ditemukan data yang lebih kecil dari minimum maka nilai minimum akan diganti ke nilai tersebut, dan begitu juga untuk maksimum. Setelah didapat minimum dan maksimumnya, jumlahkan nilai `avg_data` dengan data pada `i` lalu dimasukkan ke `avg_total` (atau `avg_data` diberi value penjumlahan nilai sebelumnya dengan nilai `i`) sehingga setiap tahapan loop akan menambahkan nilai variabel `avg_data` dengan data pada `i` dan pada akhirnya didapat sum total dari seluruh data. Kode tersebut diterapkan kepada seluruh data sehingga didapat minimum, maksimum, dan sum (avg_) tiap datanya. 
```
 if [ "$suf" = "G" ]; then
          num=$((num * 1048576))
        elif [ "$suf" = "M"]; then
          num=$((num * 1024))
        fi
```
Ada pula data terakhir hasil dari perintah `du -sh ~/`, yakni angka dengan suatu karakter yang menunjukkan jumlah _disk_usage_. Data berupa angka dan karakter yang menunjukkan satuan (G berarti Gigabyte dan M berarti Megabyte). Karena soal meminta untuk diganti ke _path_size_, maka data tersebut perlu dikalikan berdasarkan satuannya. Dibuatlah suatu pengondisian, apabila data bersatuan 'G' maka variabel `num` (angka data `du`) kalikan dengan 1048576, sedangkan apabila satuannya 'M' maka kalikan dengan 1024. Hasil perkalian dimasukkan kedalam variabel `num` lagi. Lalu barulah hasil dari `num` dikomparasikan seperti data lain dan melakukan penjumlahan total ke dalam `avg_pathsize`.

Loop ditutup dengan `done < "$log_dir/metrics_agg_$datetime.log"` yang artinya adalah while loop mengambil data dari direktori `$log_dir/metrics_agg_$datetime.log` dengan `$log_dir` adalah nilai variabel dari log_dir dan `$datetime` adalah nilai variabel dari datetime.
```
avg_rn=$((rown / 2))
```
Kode tersebut adalah untuk pendeklarasian variabel `avg_rn` dengan value hasil dari `rown` yang dibagi 2, atau sama saja dengan jumlahnya data. Varibel `rown` dibagi 2 karena dari seluruh isi file, hanya setengahnya yang berisikan data (hanya pada baris dengan urutan genap). Jumlah baris data ini nantinya digunakan dalam mencari rata-rata.
```
avg_total=$((avg_total / avg_rn))
avg_used=$((avg_used / avg_rn))
avg_free=$((avg_free / avg_rn))
avg_shared=$((avg_shared / avg_rn))
avg_buff=$((avg_buff / avg_rn))
avg_available=$((avg_available / avg_rn)) 
avg_swaptotal=$((avg_swaptotal / avg_rn))
avg_swapused=$((avg_swapused / avg_rn))
avg_swapfree=$((avg_swapfree / avg_rn))
avg_pathsize=$((avg_pathsize / avg_rn))
```
Serangkaian kode tersebut bertujuan untuk menghitung rata-rata setiap datanya. Setiap `avg_` dari data diberi nilai dari hasil pembagian `avg_` dengan `avg_rn` (sum dibagi jumlah data), sehingga isi dari varibel `avg_` tiap datanya menjadi rata-rata tiap data. Sebetulnya lebih mudah dipahami apabila sum data menggunakan variabel tersendiri, hanya saja untuk mempermudah dan menghemat variabel maka gunakan variabel `avg_` untuk fungsi ganda yakni sebagai sum dan sebagai rata-rata akhir.
```
echo "type,mem_total,mem_used,mem_free,mem_shared,mem_buff,mem_available,swap_total,swap_used,swap_free,swap_cache,path,path_size" > "$log_dir/metrics_agg_$datetime.log"
echo "minimum,$min_total,$min_used,$min_free,$min_shared,$min_buff,$min_available,$min_swaptotal,$min_swapused,$min_swapfree,$log_dir,$min_pathsize""K" >> "$log_dir/metrics_agg_$datetime.log"
echo "maximum,$max_total,$max_used,$max_free,$max_shared,$max_buff,$max_available,$max_swaptotal,$max_swapused,$max_swapfree,$log_dir,$max_pathsize""K">> "$log_dir/metrics_agg_$datetime.log"
echo "average,$avg_total,$avg_used,$avg_free,$avg_shared,$avg_buff,$avg_available,$avg_swaptotal,$avg_swapused,$avg_swapfree,$log_dir,$avg_pathsize""K">> "$log_dir/metrics_agg_$datetime.log"
```
Terakhir, untuk memasukkan data-data tersebut kedalam file `metrics_agg_$datetime.log`, dipakailah perintah `echo` yang disertai serangkaian data yang ingin dimasukkan. Pemasukkan data pertama kali adalah seluruh nama kolom/data yang ada dibawahnya. Pemasukkan pertama juga menggunakan `>` karena diasumsikan tidak ada data sebelumnya dalam `metrics_agg_$datetime.log` sehingga tidak mungkin tertimpa. Pemasukkan kedua adalah untuk memasukkan data minimum dan menggunakan `>>` supaya data yang sudah ada dalam file tidak tertimpa karena `>>` akan memasukan data dengan menyisipkan ke akhir data yang sudah ada sebelumnya. Untuk perintah ketiga adalah untuk memasukkan data maksimum, juga menggunakan `>>`. Perintah keempat untuk memasukkan rata-rata atau _average_ dari data, juga menggunakan '>>' supaya data sebelumnya tidak tertimpa.

### Foto Hasil Output
![image](https://github.com/sisop-its-s24/praktikum-modul-1-b24/assets/79700258/a54e4b48-2cd8-44bf-ae18-121ec60a9942)
![image](https://github.com/sisop-its-s24/praktikum-modul-1-b24/assets/79700258/36cc0bc2-5395-48dc-af04-75fee29ff1f8)

- d. Code Lengkap:
```
#!/bin/bash

DATE=$(date '+%Y%m%d')
patt="metrics_agg_$DATE*"

files=$(find ~/praktikum-modul-1-b24/task-3/ -maxdepth 1 -name "$patt")

if [ -z "$files" ]; then
    echo "No files were found for backup."
    exit 0
fi

tar -czf ~/praktikum-modul-1-b24/task-3/backup_metrics_${DATE}.tar.gz -C ~/praktikum-modul-1-b24/task-3 $(echo $files | xargs -n 1 basename)
```
Penjelasan:
```
#!/bin/bash
```
Merupakan kode untuk memberi tahu sistem operasi jikalau file dijalankan dengan di _bash_
```
DATE=$(date '+%Y%m%d')
patt="metrics_agg_$DATE*"
```
Mendeklarasikan variabel DATE dengan value hasil dari `date '+%Y%m%d'`yang dimana akan mengambil tanggal saat ini. Kemudian mendeklarasikan pula variabel patt yang valuenya adalah `metrics_agg_DATE*` dengan DATE adalah value dari variabel DATE. Dua variabel tersebut nantinya akan mempermudah dalam pembuatan kode.
```
files=$(find ~/praktikum-modul-1-b24/task-3/ -maxdepth 1 -name "$patt")
```
Mendeklarasikan variabel files dengan value hasil dari perintah `find ~/praktikum-modul-1-b24/task-3/ -maxdepth 1 -name "$patt"` yang hasilnya adalah setiap file dengan nama sesuai dengan nilai variabel patt (diawali dengan  `metrics_agg_DATE`) pada direktori `~/praktikum-modul-1-b24/task-3/`.
```
if [ -z "$files" ]; then
    echo "No files were found for backup."
    exit 0
fi
```
Merupakan sebuah pengondisian dimana apabila nilai files tidak ada (tidak ditemukan file yang dicari) maka keluarkan pesan "No files were found for backup.".
```
tar -czf ~/praktikum-modul-1-b24/task-3/backup_metrics_${DATE}.tar.gz -C ~/praktikum-modul-1-b24/task-3 $(echo $files | xargs -n 1 basename)
```
Merupakan kode untuk membuat `zip` dengan nama `backup_metrics_${DATE}.tar.gz` dengan `${DATE}` adalah value dari DATE pada direktori `~/praktikum-modul-1-b24/task-3/`. Isi dari zip yakni seluruh value dari variabel files atau seluruh file yang diawali `"metrics_agg_$DATE"`. Perintah `tar` digunakan untuk membuat file _archive_, `-czf` memiliki fungsi untuk memberi tahu `tar` supaya membuat/_create_ (-c), membuat _archive_ dengan `gzip` (z), dan untuk memberi nama dari filenya (f). Atribut `-C` adalah untuk memberi tahu perintah `tar` supaya pindah ke direktori yang diinginkan. Kemudian `xargs -n 1 basename` adalah perintah untuk mengambil `basename` dari tiap file dalam files (`xargs` untuk execute, `-n 1` adalah untuk tiap line jalankan perintah, dan `basename` adalah perintah untuk mengambil nama file tanpa direktorinya).

Sesuai permintaan soal untuk file `aggregate_minutes_to_hourly_log.sh` dapat dijalankan 
### Foto Hasil Output
![image](https://github.com/sisop-its-s24/praktikum-modul-1-b24/assets/79700258/e519fbc8-244f-4448-a190-adc876c0e497)
![image](https://github.com/sisop-its-s24/praktikum-modul-1-b24/assets/79700258/cea8bc80-d38b-4474-8db0-4feaeb3f95f2)

## Task 4 - LDR Isabel _(Isabel Sad Relationship)_
### Soal
Isabel sedang LDR dengan pacarnya dan sangat rindu. Isabel ingin menyimpan semua foto-foto yang dikirim oleh pacarnya. Bantulah Isabel menyimpan gambar "Mingyu Seventeen”.

- a. Berikut adalah ketentuan yang diinginkan Isabel:
- Gambarnya di download setiap 5 jam sekali mulai dari saat script dijalankan dan memperhatikan waktu sekarang. Jika waktu genap, maka simpan foto sebanyak 5x. Jika waktu ganjil, maka simpan foto sebanyak 3x. (Contoh : pukul sekarang 12.38, maka pukul 12 adalah   genap sehingga simpan foto sebanyak 5x).
- Jika pukul menunjukkan 00.00, maka simpan foto sebanyak 10x.
- File yang didownload memiliki format nama foto_NOMOR.FILE dengan NOMOR.FILE adalah urutan file yang download (foto_1, foto_2, dst)File batch yang didownload akan dimasukkan ke dalam folder dengan format nama folder_NOMOR.FOLDER dengan NOMOR.FOLDER adalah urutan folder saat dibuat (folder_1, folder_2, dst)


- b. Karena memory laptop Isabel penuh, maka bantulah Isabel untuk zip foto-foto tersebut dengan ketentuan:
- Isabel harus melakukan zip setiap 1 jam dengan nama zip ayang_NOMOR.ZIP dengan NOMOR.ZIP adalah urutan folder saat dibuat (ayang_1, ayang_2, dst). Yang di ZIP hanyalah folder dari soal di atas.

- c. Ternyata laptop Isabel masih penuh, bantulah dia untuk delete semua folder dan zip setiap hari pada pukul 02.00!


- d. Untuk mengisi laptopnya kembali, Isabel ingin mendownload gambar Levi dan Eren secara bergantian setiap harinya dengan ketentuan nama file:

- Jika Levi, maka nama file nya menjadi levi_YYYYMMDD (Dengan YYYYMMDD adalah tahun, bulan, dan tanggal gambar tersebut di download)
- Jika Eren, maka nama file nya menjadi eren_YYYYMMDD (Dengan YYYYMMDD adalah tahun, bulan, dan tanggal gambar tersebut di download)

Note: Semua gambar yang didownload bisa dicari bebas dari internet

### Penyelesaian
Code Lengkap:
```
#!/bin/bash

curr_H=$(date +'%H')
curr_M=$(date +'%M')

CRON_FILE="~/praktikum-modul-1-b24/task-4/crontabs"

function update_crontab {
    JOB="$1"
    FUNC="$2"
    sed -i '/'$FUNC'/d' ~/praktikum-modul-1-b24/task-4/crontabs
    if ! grep -Fq "$JOB" ~/praktikum-modul-1-b24/task-4/crontabs; then
        echo "$JOB * * * /bin/bash ~/praktikum-modul-1-b24/task-4/isabel.sh $FUNC" >> ~/praktikum-modul-1-b24/task-4/crontabs
        crontab /bin/bash ~/praktikum-modul-1-b24/task-4/crontabs
    fi
}

if ! grep -Fq "0 2 * * * /bin/bash ~/praktikum-modul-1-b24/task-4/isabel.sh isabel-c" ~/praktikum-modul-1-b24/task-4/crontabs; then
    echo "0 2 * * * /bin/bash ~/praktikum-modul-1-b24/task-4/isabel.sh isabel-c" >> ~/praktikum-modul-1-b24/task-4/crontabs
    crontab /bin/bash ~/praktikum-modul-1-b24/task-4/crontabs
fi

if ! grep -Fq "0 0 * * * /bin/bash ~/praktikum-modul-1-b24/task-4/isabel.sh isabel-d" ~/praktikum-modul-1-b24/task-4/crontabs; then
    echo "0 0 * * * /bin/bash ~/praktikum-modul-1-b24/task-4/isabel.sh isabel-d" >> ~/praktikum-modul-1-b24/task-4/crontabs
    crontab /bin/bash ~/praktikum-modul-1-b24/task-4/crontabs
fi

function isabel_a {
    newH=$(($curr_H + 5))
    if [ $newH -ge 24 ]; then
        newH=$((newH-24))
    fi
    update_crontab "$curr_M $newH" "isabel-a"

    if [[ "$curr_H" == "00" && "$curr_M" == "00" ]]; then
        count=10
    elif ((curr_H % 2 == 0)); then
        count=5
    else
        count=3
    fi

    jumlah=$(find ~/praktikum-modul-1-b24/task-4/ -type d -name 'folder_*' | wc -l)
    num=$((jumlah + 1))
    folder="folder_${num}"

    mkdir -p ~/praktikum-modul-1-b24/task-4/"$folder"

    for ((i=1; i<=count; i++)); do
        wget -O ~/praktikum-modul-1-b24/task-4/${folder}/foto_${i}.jpg "https://media.discordapp.net/attachments/1121728188935045164/1220633219662086144/20240321_143812.jpg?ex=660fa645&is=65fd3145&hm=6e86655a8cb573d1d2ff2bf0320e840c78528d91c1b9eabe3fc42676aefc46e4&=&format=webp&width=1247&height=627"
    done
}

function isabel_b {
    newH=$(($curr_H + 10))
    if [ $newH -ge 24 ]; then
        newH=$((newH-24))
    fi
    update_crontab "$curr_M $newH" "isabel-b"

    folders=$(find ~/praktikum-modul-1-b24/task-4/ -maxdepth 1 -type d -name 'folder_*' | sort)

    cnt=1

    for folder in $folders; do
        namazip="ayang_${cnt}.zip"
        if [ ! -f "$namazip" ]; then
            zip -r "$namazip" "$folder"
        fi
        cnt=$((cnt + 1))
    done
}

function isabel_c {
    rm -rf ./folder_*
    rm -rf ./ayang_*.zip
}

function isabel_d {
    urlL="https://media.discordapp.net/attachments/1121728188935045164/1220633219662086144/20240321_143812.jpg?ex=660fa645&is=65fd3145&hm=6e86655a8cb573d1d2ff2bf0320e840c78528d91c1b9eabe3fc42676aefc46e4&=&format=webp&width=1247&height=627"
    urlE="https://api.duniagames.co.id/api/content/upload/file/8237210571695204665.jpg"

    trakir=$(ls -Art ~/praktikum-modul-1-b24/task-4 | egrep "levi_|eren_" | tail -n 1)
    waktu=$(date +'%Y%m%d')

    if [[ $trakir == "levi_"* ]]; then
        wget "$urlE" -O "eren_$waktu.jpg"
        touch "eren_$waktu.jpg"
    else
        wget "$urlL" -O "levi_$waktu.jpg"
        touch "levi_$waktu.jpg"
    fi
}

if [[ $1 == "isabel-a" ]]; then
    isabel_a
elif [[ $1 == "isabel-b" ]]; then    
    isabel_b
elif [[ $1 == "isabel-c" ]]; then
    isabel_c
elif [[ $1 == "isabel-d" ]]; then
    isabel_d
fi
```
Penjelasan:
```
#!/bin/bash
```
Merupakan kode untuk memberi tahu sistem operasi jikalau script harus di _bash_ 
```
curr_H=$(date +'%H')
curr_M=$(date +'%M')
```
Merupakan kode yang mendeklarasikan curr_H dengan value nilai jam sekarang dan curr_M dengan value nilai menit waktu sekarang.
```
CRON_FILE="~/praktikum-modul-1-b24/task-4/crontabs"
```
Merupakan kode untuk membuat variabel bernama CRON_FILE yang memiliki nilai direktori dari crontabs, sehingga nanti lebih mudah ketika ingin menggunakan alamat file direktori dari crontabs.
```
function update_crontab {
    JOB="$1"
    FUNC="$2"
    sed -i '/'$FUNC'/d' ~/praktikum-modul-1-b24/task-4/crontabs
    if ! grep -Fq "$JOB" ~/praktikum-modul-1-b24/task-4/crontabs; then
        echo "$JOB * * * /bin/bash ~/praktikum-modul-1-b24/task-4/isabel.sh $FUNC" >> ~/praktikum-modul-1-b24/task-4/crontabs
        crontab /bin/bash ~/praktikum-modul-1-b24/task-4/crontabs
    fi
}
```
Merupakan kode untuk membuat suatu fungsi bernama update_crontab yang berguna untuk memasukkan serangkaian cronjob ke dalam crontabs. Fungsi diawali dengan mendeklarasikan variabel JOB dengan nilai $1 atau yang nantinya sama dengan inputan ketika fungsi dipanggil, kemudian deklarasikan pula FUNC untuk mengambil inputan kedua saat pemanggilan fungsi. `sed -i '/'$FUNC'/d' ~/praktikum-modul-1-b24/task-4/crontabs` berguna untuk menghapus baris/line bila ditemukan $FUNC pada suatu line/baris dalam ~/praktikum-modul-1-b24/task-4/crontabs. Kemudian terdapat pengondisian dimana ketika dalam `~/praktikum-modul-1-b24/task-4/crontab` tidak ditemukan cronjob yang sesuai dengan variabel JOB maka masukkan serangkaian cronjob ke dalam file crontabs (echo "$JOB * * * /bin/bash ~/praktikum-modul-1-b24/task-4/isabel.sh $FUNC" >> ~/praktikum-modul-1-b24/task-4/crontabs). Lalu setelah dimasukkan, crontabs dijalankan sebagai crontab dengan menggunakan command `crontab /bin/bash ~/praktikum-modul-1-b24/task-4/crontabs`.
```
if ! grep -Fq "0 2 * * * /bin/bash ~/praktikum-modul-1-b24/task-4/isabel.sh isabel-c" ~/praktikum-modul-1-b24/task-4/crontabs; then
    echo "0 2 * * * /bin/bash ~/praktikum-modul-1-b24/task-4/isabel.sh isabel-c" >> ~/praktikum-modul-1-b24/task-4/crontabs
    crontab /bin/bash ~/praktikum-modul-1-b24/task-4/crontabs
fi
```
Merupakan bagian kode untuk memasukkan serangkaian cronjob kedalam `~/praktikum-modul-1-b24/task-4/crontabs` untuk fungsi isabel_c. Pertama diberi pengondisian ketika cronjob "0 2 * * * /bin/bash ~/praktikum-modul-1-b24/task-4/isabel.sh isabel-c" tidak ditemukan dalam file crontabs maka masukkan "0 2 * * * /bin/bash ~/praktikum-modul-1-b24/task-4/isabel.sh isabel-c" ke dalam file crontabs. Kemudian setelah dimasukkan, jalankan crontabs sebagai crontab dengan `crontab /bin/bash ~/praktikum-modul-1-b24/task-4/crontabs`. Sesuai dengan permintaan soal, "0 2 * * *" sebagai cronjob dari isabel_c, akan menjalankan fungsi isabel_c setiap pukul 02.00. 
```
if ! grep -Fq "0 0 * * * /bin/bash ~/praktikum-modul-1-b24/task-4/isabel.sh isabel-d" ~/praktikum-modul-1-b24/task-4/crontabs; then
    echo "0 0 * * * /bin/bash ~/praktikum-modul-1-b24/task-4/isabel.sh isabel-d" >> ~/praktikum-modul-1-b24/task-4/crontabs
    crontab /bin/bash ~/praktikum-modul-1-b24/task-4/crontabs
fi
```
Sama seperti sebelumnya, bagian ini berfungsi untuk memasukkan cronjob ke dalam crontabs hanya saja kali ini cronjob yang dimasukkan adalah untuk fungsi isabel_d. Sesuai permintaan soal yang dimana isabel_d harus dilakukan setiap hari sekali, maka cronjob yang dimasukkan adalah "0 0 * * *" yang memiliki makna bahwa fungsi akan dijalankan setiap hari sekali (ketika hari berganti).
```
function isabel_a {
    newH=$(($curr_H + 5))
    if [ $newH -ge 24 ]; then
        newH=$((newH-24))
    fi
    update_crontab "$curr_M $newH" "isabel-a"

    if [[ "$curr_H" == "00" && "$curr_M" == "00" ]]; then
        count=10
    elif ((curr_H % 2 == 0)); then
        count=5
    else
        count=3
    fi

    jumlah=$(find ~/praktikum-modul-1-b24/task-4/ -type d -name 'folder_*' | wc -l)
    num=$((jumlah + 1))
    folder="folder_${num}"

    mkdir -p ~/praktikum-modul-1-b24/task-4/"$folder"

    for ((i=1; i<=count; i++)); do
        wget -O ~/praktikum-modul-1-b24/task-4/${folder}/foto_${i}.jpg "https://media.discordapp.net/attachments/1121728188935045164/1220633219662086144/20240321_143812.jpg?ex=660fa645&is=65fd3145&hm=6e86655a8cb573d1d2ff2bf0320e840c78528d91c1b9eabe3fc42676aefc46e4&=&format=webp&width=1247&height=627"
    done
}
```
Serangkaian kode diatas merupakan suatu fungsi bernama isabel_a. Sesuai dengan permintaan soal, isabel_a akan mendownload suatu gambar sebanyak 5, 3, atau 10 sesuai dengan jam saat program dijalankan. Fungsi dimulai dengan mendeklarasikan newH sebagai nilai dari jam sekarang yang ditambah dengan 5, fungsi newH ini adalah nantinya digunakan untuk menjadi waktu cronjob berikutnya setelah program ini dijalankan, sehingga fungsi isabel_a dapat berjalan tiap 5 jam sekali (sesuai apa yang diinginkan soal). Kemudian diberikan pengondisian apabila nilai dari newH lebih dari 24 maka kurangi nilai newH dengan 24, hal ini dilakukan untuk mencegah terjadinya nilai yang melebihi 24 jam (misalkan curr_H bernilai 23, bila tanpa pengondisian maka hasil newH adalah 28 yang dimana tidak sesuai dengan waktu nyata, maka perlu digunakan pengondisian sehingga nilai newH adalah 4). Setelah didapat nilai newH, panggil fungsi update_crontab untuk membuat cronjob baru dengan menit sekarang dan newH sebagai data inputan pertama ($1) dan "isabel-a" sebagai inputan kedua ($2). Mengapa cronjob yang dimasukkan adalah menit sekarang dan newH? karena bila isabel-a ingin dijalankan tiap 5 jam, maka menitnya akan selalu sama tetapi jamnya ditambah 5, misalkan waktu sekarang 23.20 maka waktu berikutnya adalah 4.20 maka value yang dimasukkan adalah 20 dan 4. 

Pada soal dinyatakan bahwa apabila nilai dari jam waktu sekarang menunjukkan angka genap maka jumlah download yang dilakukan adalah sebanyak 5 kali, ketika angka ganjil maka 3 kali, dan ketika waktu yang ditunjukkan adalah 00.00 maka sebanyak 10 kali. Hal tersebut dapat dipenuhi dengan membuat suatu pengondisian dengan 2 parameter seperti pada kode tersebut. Apabila curr_H menunjukkan "00" dan curr_M menunjukkan "00" maka deklarasikan count dengan value 10. Apabila curr_H dibagi 2 sisanya 0 (genap) maka count bernilai 5. Lalu selain dari dua parameter tersebut (ganjil) maka count bernilai 3. 

Selanjutnya deklarasikan variabel jumlah dengan value hasil dari jumlah folder pada direktori `~/praktikum-modul-1-b24/task-4/` dengan nama yang diawali oleh "folder_" guna untuk mengetahui jumlah folder yang telah terbuat. Kemudian buat suatu variabel lagi yakni num dengan value jumlah ditambah dengan 1, nantinya num ini digunakan untuk penaman folder yang akan dibuat selanjutnya. Untuk mempermudah penaman folder, dibuatlah lagi suatu variabel bernama folder yang memiliki value "folder_${num}" dengan `${num}` adalah nilai num.

Untuk membuat folder, dapat menggunakan suatu command `mkdir` yang disertai dengan type yang akan dibuat yakni folder (-p) dan direktori dari foldernya (~/praktikum-modul-1-b24/task-4/"$folder"). Bagian akhir dari direktori, adalah value dari folder yang dimana sesuai dengan sebelumnya berisikan folder_num untuk penamaan folder yang dibuat.

Setelah folder dibuat, diperlukan suatu algoritma untuk mengunduh sebanyak n kali (sesuai paragraf kedua) gambar dan dimasukkan kedalam folder. Untuk itu dibuatlah suatu perulangan dengan for loop yang dimana nilai awal variabel i adalah 1, dan selama i kurang dari sama dengan count (jumlah gambar yang akan diunduh) jalankan perintah/code dibawahnya/didalamnya, lalu diakhiri dengan _auto_increment_ i setiap tahap loopnya. Di dalam perulangan tentunya terdapat code yang berfungsi untuk mengunduh gambar. Pada kode ini terdapat command `wget` yang berfungsi untuk mengunduh, kemudian atribut `-O` supaya hasil unduhan bisa diganti namanya, `~/praktikum-modul-1-b24/task-4/${folder}/foto_${i}.jpg` direktori beserta nama foto yang akan diunduh (sesuai soal, foto akan diunduh dengan nama foto_angka urutan didownloadnya), dan `"https://media.discordapp.net/attachments/1121728188935045164/1220633219662086144/20240321_143812.jpg?ex=660fa645&is=65fd3145&hm=6e86655a8cb573d1d2ff2bf0320e840c78528d91c1b9eabe3fc42676aefc46e4&=&format=webp&width=1247&height=627"` sebagai link URL untuk _source_ dari gambar yang diunduh.

### Foto Hasil Output
![image](https://github.com/sisop-its-s24/praktikum-modul-1-b24/assets/79700258/1474add0-e9ce-4328-b881-578aa7276ef0)
![image](https://github.com/sisop-its-s24/praktikum-modul-1-b24/assets/79700258/de3f997b-286f-41ea-b8fc-dc6b27b88e8f)

```
function isabel_b {
    newH=$(($curr_H + 10))
    if [ $newH -ge 24 ]; then
        newH=$((newH-24))
    fi
    update_crontab "$curr_M $newH" "isabel-b"

    folders=$(find ~/praktikum-modul-1-b24/task-4/ -maxdepth 1 -type d -name 'folder_*' | sort)

    cnt=1

    for folder in $folders; do
        namazip="ayang_${cnt}.zip"
        if [ ! -f "$namazip" ]; then
            zip -r "$namazip" "$folder"
        fi
        cnt=$((cnt + 1))
    done
}
```
Serangkaian kode tersebut adalah kode untuk fungsi isabel_b. Sesuai dengan soal, isabel_b akan membuat zip dari folder-folder yang telah dibuat dari fungsi isabel_a setiap 10 jam sekali. Awalnya deklarasikan newH sebagai variabel yang akan menyimpan hasil penjumlahan dari curr_H (jam sekarang) dengan 10, nantinya newH ini akan digunakan dalam penentuan waktu cronjob. Kemudian seperti pada fungsi isabel_a, yang dimana untuk mencegah angka yang melebihi 24 (tidak sesuai dengan jam dunia nyata) dibuatlah suatu pengodisian yang dimana apabila nilai dari newH lebih dari sama dengan 24, maka kurangi nilai newH dengan 24. Lalu setelah didapat nilai newH, panggil fungsi update_crontab dengan tujuan untuk memasukkan cronjob ke dalam crontabs. Inputan dari fungsi tersebut adalah menit sekarang (curr_M), jam selanjutnya yakni jam mula-mula tambah 10 (newH), dan "isabel-b" untuk keterangan fungsi yang dijalankand dalam cronjob. 

Setelah pemasukkan cronjob, dibuat suatu variabel bernama folders dengan value dari hasil pencarian seluruh folder pada direktori `~/praktikum-modul-1-b24/task-4/` dengan nama yang diawali dengan "folder_" kemudian hasil pencarian tersebut di sortir. Kemudian untuk penamaan zip, diperlukan suatu variabel untuk membuat nama zipnya (yakni sesuai dengan urutan zip dibuat), maka dibuatlah suatu variabel dengan nilai awal 1. 

Untuk pembuatan zip diperlukan suatu for guna mengumpulkan seluruh folder yang ingin dizip, yakni dapat digunakan dengan `for folder in $folders`. Kemudian buat suatu variabel namazip sesuai dengan format nama zip yakni ayang_urutan zip (cnt) untuk mempermudah dalam pembuatan zip. Lalu buat suatu pengondisian dimana bila tidak ditemukan suatu zip dengan nama yang sama, maka buat zip dengan nama namazip dan zip seluruh folder dalam folders. Pengondisian tersebut dibuat supaya tidak terjadi penggandaan dalam pembuatan zip. Lalu diakhiri dengan penambahan jumlah cnt sehingga setiap tahapan loop, jumlah cnt bertambah (untuk pengecekan apakah zip dengan nama ayang_cnt ada, bila ada, perulangan terus dilakukan hingga ayang_cnt tidak ditemukan). 

### Foto Hasil Output
![image](https://github.com/sisop-its-s24/praktikum-modul-1-b24/assets/79700258/4ff7057a-c18a-475b-a5c6-ec32b92cd425)
![image](https://github.com/sisop-its-s24/praktikum-modul-1-b24/assets/79700258/63d6ca37-9b60-42bb-85c0-a09bdda78710)

```
function isabel_c {
    rm -rf ./folder_*
    rm -rf ./ayang_*.zip
}
```
Merupakan suatu kode yang mendeklarasikan fungsi bernama isabel_c, yang sesuai dengan permintaan soal berfungsi untuk menghapus setiap folder yang namanya diawali "folder_" dan setiap zip yang namanya diawali "ayang_" yang dilakukan setiap pukul 02.00. Perintah yang dilakukan pertama adalah `rm` atau remove untuk menghapus file dengan nama yang diawali dengan "folder_". Kemuudian perintah kedua adalah `rm` juga yang digunakan untuk menghapus file zip dengan nama yang diawali dengan "ayang_".

### Foto Hasil Output
![image](https://github.com/sisop-its-s24/praktikum-modul-1-b24/assets/79700258/a991d03c-e129-40f9-b2ed-eecd3221b608)
![image](https://github.com/sisop-its-s24/praktikum-modul-1-b24/assets/79700258/7ee37f71-74c1-46bd-9770-5615c4a2390a)

```
function isabel_d {
    urlL="https://media.discordapp.net/attachments/1121728188935045164/1220633219662086144/20240321_143812.jpg?ex=660fa645&is=65fd3145&hm=6e86655a8cb573d1d2ff2bf0320e840c78528d91c1b9eabe3fc42676aefc46e4&=&format=webp&width=1247&height=627"
    urlE="https://api.duniagames.co.id/api/content/upload/file/8237210571695204665.jpg"

    trakir=$(ls -Art ~/praktikum-modul-1-b24/task-4 | egrep "levi_|eren_" | tail -n 1)
    waktu=$(date +'%Y%m%d')

    if [[ $trakir == "levi_"* ]]; then
        wget "$urlE" -O "eren_$waktu.jpg"
        touch "eren_$waktu.jpg"
    else
        wget "$urlL" -O "levi_$waktu.jpg"
        touch "levi_$waktu.jpg"
    fi
}
```
Merupakan serangkaian kode fungsi bernama isabel_d, yang sesuai dengan apa yang diminta soal mencakup seluruh perintah yang berguna untuk mendownload dua gambar yang diberi nama levin_waktu dan eren_waktu dan disimpan kedalam direktori `~/praktikum-modul-1-b24/task-4`. Mulanya deklarasikan `urlL` dengan value `"https://media.discordapp.net/attachments/1121728188935045164/1220633219662086144/20240321_143812.jpg?ex=660fa645&is=65fd3145&hm=6e86655a8cb573d1d2ff2bf0320e840c78528d91c1b9eabe3fc42676aefc46e4&=&format=webp&width=1247&height=627"` yang merupakan link gambar yang nantinya akandiunduh dan diberi nama levi, dan `urlE` dengan value `"https://api.duniagames.co.id/api/content/upload/file/8237210571695204665.jpg"` yang merupakan link gambar yang nantinya akan diunduh dan diberi nama eren. 

Kemudian dibuat suatu variabel bernama trakir dengan value hasil dari perintah `$(ls -Art ~/praktikum-modul-1-b24/task-4 | egrep "levi_|eren_" | tail -n 1)`. Perintah tersebut akan mengambil nama gambar terakhir yang dimodifikasi (didownload) dengan menggunakan atribut `-Art`( `r` berarti reverse dan `t` berarti berdasarkan waktu) dalam direktori `~/praktikum-modul-1-b24/task-` diawali dengan "levi_" dan "eren_" (menggunakan `egrep "levi_|eren_"` untuk mengambil sesuai kata "levi_" ataupun "eren_"). Data terakhir dapat diambil dengan meggunakan perintah `tail -n 1`  yang akan mengambil 1 data terakhir "tail". 

Penamaan file levi dan eren adalah levi_tahunbulanhari dan eren_tahunbulanhari dengan tahunbulanhari adalah waktu saat gambar didownload. Untuk mempermudah penamaan file nantinya, dibuatlah variabel bernama waktu yang merupakan hasil dari perintah '$(date +'%Y%m%d')' yang akan menghasilkan tahun bulan dan hari (berupa angka dan tanpa spasi) saat ini. 

Kemudian karena permintaan soal gambar yang didownload berganti-gantian (antara levi dan eren) maka buat suatu pengondisian dimana bila gambar yang terakhir didonlowad (variabel trakir) adalah levi maka download eren dengan menggunakan `wget` dengan `urlE` sebagai sumber, atribut `-O` supaya gambar yang didownload bisa diganti namanya, dan nama gambarnya yakni `"eren_$waktu.jpg"` yang dimana `$waktu` adalah nilai variabel waktu (tahun bulan hari sekarang). Terkadang waktu gambar tidak sesuai dengan waktu saat didownload, maka untuk mencegah hal tersebut lakukan perintah `touch` untuk mengakses file gambar eren sehingga waktu modifikasi menjadi waktu sekarang. Apabila tidak masuk dalam pengondisian pertama maka berarti file yang didownload terakhir adalan eren sehingga download gambar levi dengan perintah `wget "$urlL" -O "levi_$waktu.jpg"`, `wget` untuk mengunduh, `"$urlL"` adalah link dari gambar levi, atribut `-O` untuk mengubah nama file gambar, dan nama file levi yakni "levi_$waktu.jpg" dengan `$waktu` adalah nilai dari variabel waktu. Sama dengan sebelumnya untuk mencegah hal yang tidak diinginkan, `touch` gambar levi.

### Foto Hasil Output
![image](https://github.com/sisop-its-s24/praktikum-modul-1-b24/assets/79700258/d2e6cf61-3c73-471f-9f50-38dec6773893)
![image](https://github.com/sisop-its-s24/praktikum-modul-1-b24/assets/79700258/69631e97-91cd-4ab7-8ca8-64a68e5142a9)


