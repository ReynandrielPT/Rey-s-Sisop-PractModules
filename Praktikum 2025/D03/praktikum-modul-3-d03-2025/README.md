[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/Eu-CByJh)
|    NRP     |      Name      |
| :--------: | :------------: |
| 5025241068 | Riyan Fadli Amazzadin |
| 5025241100 | Liem, Alfred Haryanto |
| 5025221120 | Raditya Zhafran Pranuja |

# Praktikum Modul 3 _(Module 3 Lab Work)_

### Laporan Resmi Praktikum Modul 3 _(Module 3 Lab Work Report)_

Di suatu pagi hari yang cerah, Budiman salah satu mahasiswa Informatika ditugaskan oleh dosennya untuk membuat suatu sistem operasi sederhana. Akan tetapi karena Budiman memiliki keterbatasan, Ia meminta tolong kepadamu untuk membantunya dalam mengerjakan tugasnya. Bantulah Budiman untuk membuat sistem operasi sederhana!

_One sunny morning, Budiman, an Informatics student, was assigned by his lecturer to create a simple operating system. However, due to Budiman's limitations, he asks for your help to assist him in completing his assignment. Help Budiman create a simple operating system!_

### Soal 1

> Sebelum membuat sistem operasi, Budiman diberitahu dosennya bahwa Ia harus melakukan beberapa tahap terlebih dahulu. Tahap-tahapan yang dimaksud adalah untuk **mempersiapkan seluruh prasyarat** dan **melakukan instalasi-instalasi** sebelum membuat sistem operasi. Lakukan seluruh tahapan prasyarat hingga [perintah ini](https://github.com/arsitektur-jaringan-komputer/Modul-Sisop/blob/master/Modul3/README-ID.md#:~:text=sudo%20apt%20install%20%2Dy%20busybox%2Dstatic) pada modul!

> _Before creating the OS, Budiman was informed by his lecturer that he must complete several steps first. The steps include **preparing all prerequisites** and **installing** before creating the OS. Complete all the prerequisite steps up to [this command](https://github.com/arsitektur-jaringan-komputer/Modul-Sisop/blob/master/Modul3/README-ID.md#:~:text=sudo%20apt%20install%20%2Dy%20busybox%2Dstatic) in the module!_

**Answer:**

- **Code:**

  ```bash
  make tinyconfig
  make menuconfig
  ```
  Aktifkan menu tambahan:
  ```General Setup > Namespace support```
  Compile kernel:
  ```bash
  make -j$(nproc)
  cp arch/x86/boot/bzImage ../
  ```
  
- **Explanation:**
  - `make tinyconfig` dan `make menuconfig` disini untuk mengaktifkan fitur-fitur tertentu yang ada di kernel yang kita punya. Selain mengakitfkan fitur-fitur seperti di modul prasyarat kita perlu mengaktifkan fitur tambahan yaitu `Namespace support` yang terletak di `General Setup`
  - Setelah semua selesai diaktifkan, kita dapat compile kernel dengan `make -j$(nproc)`. Disini akan meng-compile kernel yang kita punya dengan jumlah processor yang kita punya juga.
  - `cp` disini untuk mengcopy file bzImage ke direktori sebelumnya yaitu ke direktori osboot.

- **Screenshot:**
  
  `make menuconfig & Namespace Support`
  ![image](https://github.com/rdtzaa/assets/blob/280c4f2f4b9b6d4343c961147deffa1d438e567a/Sistem%20Operasi/make%20menuconfig.png)

  `make -j$(nproc)`
  ![image](https://github.com/rdtzaa/assets/blob/280c4f2f4b9b6d4343c961147deffa1d438e567a/Sistem%20Operasi/make%20jnproc.png)

### Soal 2

> Setelah seluruh prasyarat siap, Budiman siap untuk membuat sistem operasinya. Dosen meminta untuk sistem operasi Budiman harus memiliki directory **bin, dev, proc, sys, tmp,** dan **sisop**. Lagi-lagi Budiman meminta bantuanmu. Bantulah Ia dalam membuat directory tersebut!

> _Once all prerequisites are ready, Budiman is ready to create his OS. The lecturer asks that the OS should contain the directories **bin, dev, proc, sys, tmp,** and **sisop**. Help Budiman create these directories!_

**Answer:**

- **Code:**

  ```bash
  sudo bash
  cd osboot
  mkdir -p myramdisk/{bin,dev,proc,sys,tmp,sisop,home}
  cp -a /dev/null myramdisk/dev
  cp -a /dev/tty* myramdisk/dev
  cp -a /dev/zero myramdisk/dev
  cp -a /dev/console myramdisk/dev
  ```

- **Explanation:**
  Agar OS yang kita punya dapat berjalan sederhana kita perlu meng-copy beberapa file dari device kita.
  - `mkdir` disini kita gunakan membuat beberapa folder yang bakal dibutuhkan untuk menjalankan OS kita ini.
  - `cp` disini akan meng-copy beberapa file dari direktori OS Linux kita, seperti tty, dev, zero, dan console

- **Screenshot:**

  `mkdir` & `cp`
  ![image](https://github.com/rdtzaa/assets/blob/280c4f2f4b9b6d4343c961147deffa1d438e567a/Sistem%20Operasi/mkdir_1.png)

### Soal 3

> Budiman lupa, Ia harus membuat sistem operasi ini dengan sistem **Multi User** sesuai permintaan Dosennya. Ia meminta kembali kepadamu untuk membantunya membuat beberapa user beserta directory tiap usernya dibawah directory `home`. Buat pula password tiap user-usernya dan aplikasikan dalam sistem operasi tersebut!

> _Budiman forgot that he needs to create a **Multi User** system as requested by the lecturer. He asks your help again to create several users and their corresponding home directories under the `home` directory. Also set each user's password and apply them in the OS!_

**Format:** `user:pass`

```
root:Iniroot
Budiman:PassBudi
guest:guest
praktikan1:praktikan1
praktikan2:praktikan2
```

**Answer:**

- **Code:**

  ```bash
  sudo bash
  cd osboot
  mkdir -p myramdisk/home/{Budiman,guest,praktikan1,praktikan2}
  ```

  ```bash
  openssl passwd -1 Iniroot
  openssl passwd -1 PassBudi
  openssl passwd -1 guest
  openssl passwd -1 praktikan1
  openssl passwd -1 praktikan2
  ```

  ```bash
  sudo bash
  cd osboot/home/etc
  nano passwd
  nano group
  ```
  Isi file passwd:
  ```
  root:$1$czketeUu$L5uFlfklybgB84GIi941E1:0:0:root:/root:/bin/sh
  Budiman:$1$FYO8Sa2M$CRYbz7ZVp1N65PTrBp1LZ/:1001:100:Budiman:/home/Budiman:/bin/sh
  guest:$1$xH6nWRwr$d5GPxCEpkp9D5BkNPzJXS1:1002:100:guest:/home/guest:/bin/sh
  praktikan1:$1$rsUerzkn$UyRSOfB9b/vdHIa5NrnQM.:1003:100:praktikan1:/home/praktikan1:/bin/sh
  praktikan2:$1$BJtmL9e.$mIw6tT8Ee0BhM9lgrcPKD/:1004:100:praktikan2:/home/praktikan2:/bin/sh
  ```
  Isi file group
  ```
  root:x:0:
  bin:x:1:root
  sys:x:2:root
  tty:x:5:root,Budiman,guest,praktikan1,praktikan2
  disk:x:6:root
  wheel:x:10:root,Budiman,praktikan1,praktikan2
  users:x:100:Budiman,praktikan1,praktikan2
  ```
- **Explanation:**
  OS kita ini akan berjalan secara multi user, maka ada beberapa hal yang kita perlu lakukan:
  - `mkdir` disini kita akan membuat direktori untuk tiap-tiap user yang kita punya, yaitu Budiman, guest, praktikan1, dan praktikan2.
  - Setiap user juga memiliki password untuk digunakan, disini kita perlu melakukan hash pada password yang kita miliki dengan `openssl passwd -1`
  - Hasil hash password ini akan kita simpan di file bernama `passwd` di direktori `myramdisk/etc` dengan format berikut
  ```bash
  <Username>:<Hash_Password>:<ID>:<Group>:<Device>:<Direktori_User>:/bin/sh
  - Setelah password berhasil kita set, kita perlu konfigurasi juga untuk group dari tiap-tiap user. File `group` berada di `myramdisk/etc` juga. Isi file group seperti ada di bagian Code diatas.
- **Screenshot:**

  `openssl`
  ![image](https://github.com/rdtzaa/assets/blob/280c4f2f4b9b6d4343c961147deffa1d438e567a/Sistem%20Operasi/openssl.png)

  `passwd` & `group`
  ![image](https://github.com/rdtzaa/assets/blob/280c4f2f4b9b6d4343c961147deffa1d438e567a/Sistem%20Operasi/passwd.png)
  ![image](https://github.com/rdtzaa/assets/blob/280c4f2f4b9b6d4343c961147deffa1d438e567a/Sistem%20Operasi/group.png)

  _Contoh login multi user_
  ![image](https://github.com/rdtzaa/assets/blob/280c4f2f4b9b6d4343c961147deffa1d438e567a/Sistem%20Operasi/login_multi.png)

### Soal 4

> Dosen meminta Budiman membuat sistem operasi ini memilki **superuser** layaknya sistem operasi pada umumnya. User root yang sudah kamu buat sebelumnya akan digunakan sebagai superuser dalam sistem operasi milik Budiman. Superuser yang dimaksud adalah user dengan otoritas penuh yang dapat mengakses seluruhnya. Akan tetapi user lain tidak boleh memiliki otoritas yang sama. Dengan begitu user-user selain root tidak boleh mengakses `./root`. Buatlah sehingga tiap user selain superuser tidak dapat mengakses `./root`!

> _The lecturer requests that the OS must have a **superuser** just like other operating systems. The root user created earlier will serve as the superuser in Budiman's OS. The superuser should have full authority to access everything. However, other users should not have the same authority. Therefore, users other than root should not be able to access `./root`. Implement this so that non-superuser accounts cannot access `./root`!_

**Answer:**

- **Code:**

  ```bash
  cd osboot/myramdisk
  sudo chown root:root root/
  ```

- **Explanation:**
  Disini agar konsep multi user berjalan, kita perlu menjalankan perintah `chown`. Perintah tersebut akan mengubah hak kepemilikan direktori yang dituju menjadi milik user yang dituju yang berarti folder itu hanya milik user tersebut, user-user lain tidak akan bisa mengakses direktori tersebut secara langsung.

- **Screenshot:**

  `chownd`
  ![image](https://github.com/rdtzaa/assets/blob/280c4f2f4b9b6d4343c961147deffa1d438e567a/Sistem%20Operasi/chown.png)

  `superuser`
  ![image](https://github.com/rdtzaa/assets/blob/280c4f2f4b9b6d4343c961147deffa1d438e567a/Sistem%20Operasi/superuser.png)

### Soal 5

> Setiap user rencananya akan digunakan oleh satu orang tertentu. **Privasi dan otoritas tiap user** merupakan hal penting. Oleh karena itu, Budiman ingin membuat setiap user hanya bisa mengakses dirinya sendiri dan tidak bisa mengakses user lain. Buatlah sehingga sistem operasi Budiman dalam melakukan hal tersebut!

> _Each user is intended for an individual. **Privacy and authority** for each user are important. Therefore, Budiman wants to ensure that each user can only access their own files and not those of others. Implement this in Budiman's OS!_

**Answer:**

- **Code:**

``` sh
chmod 700 {Budiman,guest,praktikan1,praktikan2}

sudo chown 1001 Budiman
sudo chown 1002 guest
sudo chown 1003 praktikan1
sudo chown 1004 praktikan2
```

- **Explanation:**
Untuk menjaga privasi dan otoritas siap user serta memastikan setiap user hanya bisa mengkases dirinya sendiri adalah dengan menggunakan `chmod` dan `chown`.
- `chmod` berguna untuk mengubah perizinan dari sebuah file atau directory, yang mencakup user, group, dan semua orang. 7 berasal dari binary 111 (1 untuk true), yang mana untuk setiap perizinan (read(4), write(2), execute(1)) untuk user tersebut diperbolehkan, dan 0 yang lain merupakan izin bagi group dan user lain, yakni tidak boleh untuk melakukan read, write, ataupun execute. Format `chmod` adalah `chmod <nnn> <directory/file>`, dengan nnn adalah perizinanya.
- `chown` berguna untuk memberikan hak kepada seorang user agar memiliki otoritas atas suatu directory atau file. Dalam kasus ini, setiap user diberikan otoritas atas directorynya masing-masing. Format `chown` adalah `chown <option(optional)> <userid>:<groupid> <directory/file>`. Dengan `chown` ini, maka setiap user memiliki otoritas atas masing-masing directory, yang juga memastikan tidak ada user lain yang dapat mengakses directory user lainnya.

- **Screenshot:**

`chmod`
![alt text](https://media.discordapp.net/attachments/1123949811020873759/1375438101807235123/E4C7BCA1-D6F4-4C2B-9FB9-44F63CCECC54.png?ex=6831b00d&is=68305e8d&hm=040e884cf8c312ae42f63b26fd691d09032431961479bdb2d4b457a3cdfe9c26&=&format=webp&quality=lossless&width=2108&height=1069)

`chown`
![alt text](https://media.discordapp.net/attachments/1123949811020873759/1375438267620528148/2A5FF0A4-9A23-43D2-8802-B7F36DC7DD89.png?ex=6831b035&is=68305eb5&hm=f4b2961f5b5b3de0435be75b039f320cdf1cba221b3475130725beba6d594efa&=&format=webp&quality=lossless&width=2009&height=1319)



### Soal 6

> Dosen Budiman menginginkan sistem operasi yang **stylish**. Budiman memiliki ide untuk membuat sistem operasinya menjadi stylish. Ia meminta kamu untuk menambahkan tampilan sebuah banner yang ditampilkan setelah suatu user login ke dalam sistem operasi Budiman. Banner yang diinginkan Budiman adalah tulisan `"Welcome to OS'25"` dalam bentuk **ASCII Art**. Buatkanlah banner tersebut supaya Budiman senang! (Hint: gunakan text to ASCII Art Generator)

> _Budiman wants a **stylish** operating system. Budiman has an idea to make his OS stylish. He asks you to add a banner that appears after a user logs in. The banner should say `"Welcome to OS'25"` in **ASCII Art**. Use a text to ASCII Art generator to make Budiman happy!_ (Hint: use a text to ASCII Art generator)

**Answer:**

- **Code:**

``` sh

#Untuk membuat file `profile`
touch profile 

#Untuk menulis/mengubah isi `profile`
nano profile

#Isi `profile`
cat << "EOF"
▒█░░▒█ █▀▀ █░░ █▀▀ █▀▀█ █▀▄▀█ █▀▀ 　 ▒█▀▀▀█ ▒█▀▀▀█ 　 █░█ █▀█ █▀▀ █░█ 
▒█▒█▒█ █▀▀ █░░ █░░ █░░█ █░▀░█ █▀▀ 　 ▒█░░▒█ ░▀▀▀▄▄ 　 ░░░ ░▄▀ ▀▀▄ ░░░ 
▒█▄▀▄█ ▀▀▀ ▀▀▀ ▀▀▀ ▀▀▀▀ ▀░░░▀ ▀▀▀ 　 ▒█▄▄▄█ ▒█▄▄▄█ 　 ░░░ █▄▄ ▄▄▀ ░░░
EOF
```

- **Explanation:**
Untuk melakukan print ascii art dengan tulisan `Welcome OS "25"`, penggunaan `echo` kurang efisien karena echo hanya dapat memprint 1 baris dan membutuhkan banyak echo untuk memprint ascii art tersebut. Penggunaan `cat` disini lebih efektif, karena cat akan memprint semua line yang berada diantara `"EOF"` hingga `EOF`. Hasil print akan terlihat bagus dan lebih rapih karena `cat << "EOF"` lebih bagus dalam menghandle print lebih dari 1 baris. 

- **Screenshot:**

![alt_text](https://media.discordapp.net/attachments/1123949811020873759/1375453069239779358/08156AF8-0902-472C-A206-0305E52B0CDC.png?ex=6831bdfe&is=68306c7e&hm=e3f728acf9c05c1b297be3009d7bc01721c5de14b3f87aa9e5c4862247b1f1f7&=&format=webp&quality=lossless&width=990&height=594)

### Soal 7

> Melihat perkembangan sistem operasi milik Budiman, Dosen kagum dengan adanya banner yang telah kamu buat sebelumnya. Kemudian Dosen juga menginginkan sistem operasi Budiman untuk dapat menampilkan **kata sambutan** dengan menyebut nama user yang login. Sambutan yang dimaksud berupa kalimat `"Helloo %USER"` dengan `%USER` merupakan nama user yang sedang menggunakan sistem operasi. Kalimat sambutan ini ditampilkan setelah user login dan setelah banner. Budiman kembali lagi meminta bantuanmu dalam menambahkan fitur ini.

> _Seeing the progress of Budiman's OS, the lecturer is impressed with the banner you created. The lecturer also wants the OS to display a **greeting message** that includes the name of the user who logs in. The greeting should say `"Helloo %USER"` where `%USER` is the name of the user currently using the OS. This greeting should be displayed after user login and after the banner. Budiman asks for your help again to add this feature._

**Answer:**

- **Code:**

``` sh
#Untuk menulis/mengubah isi `profile`
nano profile

#Code yang ditambah
echo " " 
echo "Heloo ${USER}"
echo " "

#Isi `profile`
cat << "EOF"
▒█░░▒█ █▀▀ █░░ █▀▀ █▀▀█ █▀▄▀█ █▀▀ 　 ▒█▀▀▀█ ▒█▀▀▀█ 　 █░█ █▀█ █▀▀ █░█ 
▒█▒█▒█ █▀▀ █░░ █░░ █░░█ █░▀░█ █▀▀ 　 ▒█░░▒█ ░▀▀▀▄▄ 　 ░░░ ░▄▀ ▀▀▄ ░░░ 
▒█▄▀▄█ ▀▀▀ ▀▀▀ ▀▀▀ ▀▀▀▀ ▀░░░▀ ▀▀▀ 　 ▒█▄▄▄█ ▒█▄▄▄█ 　 ░░░ █▄▄ ▄▄▀ ░░░
EOF

echo " " 
echo "Heloo ${USER}"
echo " "
```

- **Explanation:**
`echo " "` digunakan hanya untuk memberi line kosong agar tampilan terlihat lebih rapih. Untuk melakukan print user yang sedang login maka dapat menggunakan `echo "Heloo ${USER}"`, dimana `${USER}` nanti akan di assign nama dari user yang sedang login. Hal ini bisa terjadi karena `USER` adalah variabel yang sudah ditentukan oleh OS. 

- **Screenshot:**

![alt_text](https://media.discordapp.net/attachments/1123949811020873759/1375453380834496552/D4EB9195-B0AD-4D4C-9578-EDC01B12D149.png?ex=6831be48&is=68306cc8&hm=849869f29f7cc8d562a37463d222009a121555485a98adccfefb64f739964b2e&=&format=webp&quality=lossless&width=1901&height=1525)

### Soal 8

> Dosen Budiman sudah tua sekali, sehingga beliau memiliki kesulitan untuk melihat tampilan terminal default. Budiman menginisiatif untuk membuat tampilan sistem operasi menjadi seperti terminal milikmu. Modifikasilah sistem operasi Budiman menjadi menggunakan tampilan terminal kalian.

> _Budiman's lecturer is quite old and has difficulty seeing the default terminal display. Budiman takes the initiative to make the OS look like your terminal. Modify Budiman's OS to use your terminal appearance!_

**Answer:**

- **Code:**

   Pada file ```init```:

   ```
   #!/bin/sh
   /bin/mount -t proc none /proc
   /bin/mount -t sysfs none /sys

   while true
   do
       /bin/getty -L ttyS0 115200 vt100
       sleep 1
   done
   ```
   Saat menjalankan qemu:

   ```qemu-system-x86_64   -smp 2   -m 256   -nographic   -kernel bzImage   -initrd myramdisk.gz  <br>```
  
   Pada file  ```etc/profile```
   ```
   PS1='\[\e[0;32m\]\u@\h:\[\e[0;34m\]\w\[\e[0m\]\$ '
   export PS1
   ```
- **Explanation:**

 Untuk menghasilkan tampilan yang sama identik dengan terminal, kami mengubah port untuk OS dari yang awalnya ```tty1``` menjadi ```ttyS0```. Hal ini karena ```ttyS0``` memperlakukan OS seperti teks mentah, sehingga tampilan OS menyatu dengan terminal. Selain itu, pada saat menjalankan qemu, diberikan command ```-nographic``` agar qemu tidak memberi grafik pada OS. PS1 pada file ```etc/profile``` digunakan untuk mengubah warna nama user dan directory user saat ini sehingga lebih menyerupai terminal.

- **Screenshot:**

  ![image](https://github.com/user-attachments/assets/4023ba0e-07b7-4543-9fa3-c6e050674252)

### Soal 9

> Ketika mencoba sistem operasi buatanmu, Budiman tidak bisa mengubah text file menggunakan text editor. Budiman pun menyadari bahwa dalam sistem operasi yang kamu buat tidak memiliki text editor. Budimanpun menyuruhmu untuk menambahkan **binary** yang telah disiapkan sebelumnya ke dalam sistem operasinya. Buatlah sehingga sistem operasi Budiman memiliki **binary text editor** yang telah disiapkan!

> _When trying your OS, Budiman cannot edit text files using a text editor. He realizes that the OS you created does not have a text editor. Budiman asks you to add the prepared **binary** into his OS. Make sure Budiman's OS has the prepared **text editor binary**!_

**Answer:**

- **Code:**

 Untuk menambahkan file binary dari nano:
 ```
 cp /usr/bin/nano /home/riyan/osboot/myramdisk/bin
 ```

 Untuk mengetahui library yang diperlukan:

 ```
 ldd /usr/bin/nano
 ```
 Untuk menambahkan dependency yang diperlukan:
 ```
 cp /lib/x86_64-linux-gnu/libncursesw.so.6 /home/riyan/osboot/myramdisk/lib/
 cp /lib/x86_64-linux-gnu/libtinfo.so.6    /home/riyan/osboot/myramdisk/lib/
 cp /lib/x86_64-linux-gnu/libc.so.6        /home/riyan/osboot/myramdisk/lib/
 cp /lib64/ld-linux-x86-64.so.2            /home/riyan/osboot/myramdisk/lib64/
 ```

 Pada file ```etc/profile``` ditambahkan: <br>
 ```export TERM=vt100```

 Juga salin ```TERMINFO``` untuk ```vt100```
 ```
 cp /usr/share/terminfo/v/vt100 /home/riyan/osboot/myramdisk/usr/share/terminfo/v/
 ```
- **Explanation:**

 Pada OS, file program atau file binary disimpan dalam folder ```bin```. Karena itu, kami mengambil file binary untuk program ```nano``` pada ubuntu dan menduplikatnya pada ```myramdisk/bin```. Agar ```nano``` dapat berjalan, diperlukan beberapa library yang diletakkan pada folder ```lib``` dan ```lib64```, yang juga diambil dari ubuntu. Terakhir, ```vt100``` diperlukan untuk memberitahu ```nano``` pada terminal mana ia harus muncul.

- **Screenshot:**
 
  ![image](https://github.com/user-attachments/assets/448f508b-b706-4f2e-b7c1-43a147fe2967)
  
### Soal 10

> Setelah seluruh fitur yang diminta Dosen dipenuhi dalam sistem operasi Budiman, sudah waktunya Budiman mengumpulkan tugasnya ini ke Dosen. Akan tetapi, Dosen Budiman tidak mau menerima pengumpulan selain dalam bentuk **.iso**. Untuk terakhir kalinya, Budiman meminta tolong kepadamu untuk mengubah seluruh konfigurasi sistem operasi yang telah kamu buat menjadi sebuah **file .iso**.

> After all the features requested by the lecturer have been implemented in Budiman's OS, it's time for Budiman to submit his assignment. However, Budiman's lecturer only accepts submissions in the form of **.iso** files. For the last time, Budiman asks for your help to convert the entire configuration of the OS you created into a **.iso file**.

**Answer:**

- **Code:**

 Membuat directory untuk iso:
 ```
 mkdir -p os25iso/boot/grub
 ```
 Menyalin kernel dan rootfs:

 ```
 cp bzImage os25iso/boot
 cp myramdisk.gz os25iso/boot
 ```
 Membuat file ```grub.cfg```
 ```
 set timeout=5
 set default=0

 menuentry "OS25" {
 linux /boot/bzImage
 initrd /boot/myramdisk.gz
 }
 ```

 Membuat ISO Bootable:
 ```
 grub-mkrescue -o os25.iso os25iso
 ```

- **Explanation:**

 Untuk membuat bootable iso, kita perlu memindahkan ```kernel``` dan ```root filesystem``` ke dalam sebuah folder. Setelah itu, perlu juga untuk membuat konfigurasi ```grub``` sehingga OS dapat dijalankan.


- **Screenshot:**
 
  ![image](https://github.com/user-attachments/assets/0db2edef-0abf-4061-9e6d-5ac56ae1afa9)

---

Pada akhirnya sistem operasi Budiman yang telah kamu buat dengan susah payah dikumpulkan ke Dosen mengatasnamakan Budiman. Kamu tidak diberikan credit apapun. Budiman pun tidak memberikan kata terimakasih kepadamu. Kamupun kecewa tetapi setidaknya kamu telah belajar untuk menjadi pembuat sistem operasi sederhana yang andal. Selamat!

_At last, the OS you painstakingly created was submitted to the lecturer under Budiman's name. You received no credit. Budiman didn't even thank you. You feel disappointed, but at least you've learned to become a reliable creator of simple operating systems. Congratulations!_

