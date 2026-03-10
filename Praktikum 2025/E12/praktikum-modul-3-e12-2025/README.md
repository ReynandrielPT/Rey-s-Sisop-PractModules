[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/Eu-CByJh)
|    NRP     |      Name      |
| :--------: | :------------: |
| 5025241178 | Christian Mikaxelo |
| 5025241218 | Ida Bagus Gde Dimas Sutha Maha |
| 5025241227 | Safa Maulana Efendi |

# Praktikum Modul 3 _(Module 3 Lab Work)_

### Laporan Resmi Praktikum Modul 3 _(Module 3 Lab Work Report)_

Di suatu pagi hari yang cerah, Budiman salah satu mahasiswa Informatika ditugaskan oleh dosennya untuk membuat suatu sistem operasi sederhana. Akan tetapi karena Budiman memiliki keterbatasan, Ia meminta tolong kepadamu untuk membantunya dalam mengerjakan tugasnya. Bantulah Budiman untuk membuat sistem operasi sederhana!

_One sunny morning, Budiman, an Informatics student, was assigned by his lecturer to create a simple operating system. However, due to Budiman's limitations, he asks for your help to assist him in completing his assignment. Help Budiman create a simple operating system!_

### Soal 1

> Sebelum membuat sistem operasi, Budiman diberitahu dosennya bahwa Ia harus melakukan beberapa tahap terlebih dahulu. Tahap-tahapan yang dimaksud adalah untuk **mempersiapkan seluruh prasyarat** dan **melakukan instalasi-instalasi** sebelum membuat sistem operasi. Lakukan seluruh tahapan prasyarat hingga [perintah ini](https://github.com/arsitektur-jaringan-komputer/Modul-Sisop/blob/master/Modul3/README-ID.md#:~:text=sudo%20apt%20install%20%2Dy%20busybox%2Dstatic) pada modul!

> _Before creating the OS, Budiman was informed by his lecturer that he must complete several steps first. The steps include **preparing all prerequisites** and **installing** before creating the OS. Complete all the prerequisite steps up to [this command](https://github.com/arsitektur-jaringan-komputer/Modul-Sisop/blob/master/Modul3/README-ID.md#:~:text=sudo%20apt%20install%20%2Dy%20busybox%2Dstatic) in the module!_

**Answer:**

- **Code:**

1. Update dan Install Software
```bash
  sudo apt -y update
  sudo apt -y install qemu-system build-essential bison flex libelf-dev libssl-dev bc grub-common grub-pc libncurses-dev libssl-dev mtools grub-pc-bin xorriso tmux
```
2. Buat Direktori osboot
```bash
  mkdir -p osboot
  cd osboot
```
3. Download dan Ekstrak Kernel Linux
```bash
  wget https://cdn.kernel.org/pub/linux/kernel/v6.x/linux-6.1.1.tar.xz
  tar -xvf linux-6.1.1.tar.xz
  cd linux-6.1.1
```
4. Setting config kernel
```bash
  make tinyconfig
  make menuconfig
```
5. Compile kernel
```bash
  make -j$(nproc)
```
6. Generate the bzImage File
```bash
  cp arch/x86/boot/bzImage ..
```

- **Explanation:**
  
  - Menggunakan VMware Player, dengan  mengaktifkan nested virtualization.
  - Install Linux Ubuntu 22.04 dengan minimal 4GB memory dan 4 vCPU.

1. Sebelum mulai kita memastikan sistem yang kita pakai sudah update dan menginstal beberapa tools yang kita perlukan seperti `qemu`, `build-essential`, `bison`, `flex`, dan lainnya
2. Membuat direktori untuk file os budiman nanti
3. Selanjutnya kita mendownload kernel Linux versi 6.1.1, dan masuk ke direktori linux-6.1.1 setelah download sudah selesai
4. Mengkonfigurasi file config untuk os kita dan ini beberapa yang harus di aktifkan 
   ```
   64-Bit Kernel
   General Setup > Configure standard kernel features > Enable support for printk
   General Setup > Configure standard kernel features > Enable futex support
   General Setup > Initial RAM filesystem and RAM disk (initramfs/initrd) support
   General Setup > Control Group Support
   Enable the block layer > Legacy autoloading support
   Enable the block layer > Partition type > Advanced Partition Selection
   Device Drivers > Character devices > Enable TTY
   Device Drivers > Character devices > Virtio console
   Device Drivers > Character devices > /dev/mem virtual device support
   Device Drivers > Network device support > Virtio Network Driver
   Device Drivers > Serial Ata / Paralel ATA
   Device Drivers > Block Devices > Virtio block driver
   Device Drivers > Block Devices > loopback device support
   Device Drivers > Block Devices > RAM block device support
   Device Drivers > Virtio drivers
   Device Drivers > Virtualization Drivers
   Device Drivers > Generic  Driver Options > Maintain a devtmpfs at filesystems
   Device Drivers > Generic Driver Options > Automount devtmpfs at /dev
   Executable file formats > Kernel Support for ELF binaries
   Executable file formats > Kernel Support scripts starting with #!
   File Systems > FUSE support
   File Systems > The extended 3 filesystem
   File Systems > The extended 4 filesystem
   File Systems > Second extended fs support
   File Systems > Virtio Filesystem
   File Systems > Kernel automounter support
   File Systems > Pseudo File Systems > /proc file system support
   File Systems > Pseudo File Systems > sysctl support
   File Systems > Pseudo File Systems > sysfs file system support
   Networking Support > Networking options > Unix domain sockets
   Networking Support > Networking options > TCP/IP Networking
   ```
5. Mengcompile Kernel kita dan nanti menghasilkan file `bzImage`
6. memindahkan file `bzImage` ke direktori `osboot`

- **Screenshot:**
  ![Gambar1](https://github.com/user-attachments/assets/3f82b45d-43da-4c1d-9537-94d2c4a3c8df)
  ![Gambar1](https://github.com/user-attachments/assets/c6f43dc4-34f9-43a5-bea1-d1a98bf662a0)
  ![Gambar1](https://github.com/user-attachments/assets/f0866442-86ed-4843-a3df-cf1497f55891)
  ![Gambar1](https://github.com/user-attachments/assets/21378659-0524-4048-a496-ff04e5dc0216)
  ![Gambar1](https://github.com/user-attachments/assets/d80b9611-99c2-4c8a-99f2-7abd9fd91244)
  ![Gambar1](https://github.com/user-attachments/assets/862f65c2-0ab9-4dcc-ad4e-3ca29558503b)

  
### Soal 2

> Setelah seluruh prasyarat siap, Budiman siap untuk membuat sistem operasinya. Dosen meminta untuk sistem operasi Budiman harus memiliki directory **bin, dev, proc, sys, tmp,** dan **sisop**. Lagi-lagi Budiman meminta bantuanmu. Bantulah Ia dalam membuat directory tersebut!

> _Once all prerequisites are ready, Budiman is ready to create his OS. The lecturer asks that the OS should contain the directories **bin, dev, proc, sys, tmp,** and **sisop**. Help Budiman create these directories!_

**Answer:**

- **Code:**

1. Superuser
 ```bash
  sudo bash
 ```
  2. Buat Direktori untuk Initramfs
 ```bash
  mkdir -p myramdisk/{bin,dev,proc,sys,etc,root,home/user1}
 ```
  3. Salin File Device ke Direktori dev
 ```bash
  cp -a /dev/null myramdisk/dev
  cp -a /dev/tty* myramdisk/dev
  cp -a /dev/zero myramdisk/dev
  cp -a /dev/console myramdisk/dev
 ```
  4. Salin BusyBox ke Direktori bin
 ```bash
  cp /usr/bin/busybox myramdisk/bin
  cd myramdisk/bin
  ./busybox --install .
 ```

- **Explanation:**

1. Masuk ke dalam Superuser
2. Bueat direktori dengan nama myramdisk dan beberapa sub-direktori yang diperlukan oleh sistem dan soal
3. Menyalin direktori `/dev` berisi perangkat penting seperti `null`, `tty`, `zero`, dan `console` dari sistem host ke dalam direktori `dev` di `myramdisk` yang barusan kita buat.
4. dari sistem host ke dalam direktori `dev` di `myramdisk`


- **Screenshot:**

  ![Gambar1](https://github.com/user-attachments/assets/6fa7ac08-cbb4-4631-9dcc-3f6231739f0b)



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

1. Buat Password Root
 ```bash
  openssl passwd -1 Iniroot
  openssl passwd -1 PassBudi
  openssl passwd -1 guest
  openssl passwd -1 Praktikan1
  openssl passwd -1 Praktikan2
 ```
  2. Buat File passwd di Direktori etc
 ```
  root:$1$148xkffB$Qhn5r60aHWJbY0BWP6tfl.:0:0:root:/root:/bin/sh
  Budiman:$1$57uWxV/P$5oNcLg8Vbpxiw7vEzoiUj0:1001:100:Budiman:/home/Budiman:/bin/sh
  guest:$1$ElLLSMbB$rTUFjVPqLFeHWeZnv/LwV/:1002:100:guest:/home/guest:/bin/sh
  praktikan1:$1$qvWfOiIm$6jmT0WmqBRHZvLzkrGn0e/:1003:100:praktikan1:/home/praktikan1:/bin/sh
  praktikan2:$1$jDaE7PaG$My01qAbUbmSxgPU2CCQKv/:1004:100:praktikan2:/home/praktikan2:/bin/sh
 ```
  3. Buat File group di Direktori etc
 ```bash
  root:x:0:
  bin:x:1:root
  sys:x:2:root
  tty:x:5:root,Budiman,guest,praktikan1,praktikan2
  disk:x:6:root
  wheel:x:10:root,Budiman,guest,praktikan1,praktikan2
  users:x:100:uBudiman,guest,praktikan1,praktikan2
 ```
  4. Buat File init
 ```bash
  #!/bin/sh
  /bin/mount -t proc none /proc
  /bin/mount -t sysfs none /sys

  while true
  do
      /bin/getty -L tty1 115200 vt100
      sleep 1
  done
  ./busybox --install .
 ```
 5. Berikan Izin Eksekusi pada File init
 ```bash
  chmod +x init
 ```
  5. Kompres dan Buat File initramfs
 ```bash
  find . | cpio -oHnewc | gzip > ../myramdisk.gz
 ```

- **Explanation:**

1. Gunakan perintah openssl untuk membuat password root yang dienkripsi
2. Buat file passwd di direktori etc untuk menyimpan informasi tentang akun root dan user yang akan kita buat (misalnya Budiman)
3. Buat file group di direktori etc yang berisi pengaturan grup untuk user yang baru dibuat
4. Kembali ke direktori myramdisk, buat file init yang akan dipanggil oleh bootloader saat sistem booting,lalu akan melakukan mounting untuk proc dan sysfs, yang memungkinkan komunikasi antara user space dan kernel. Kemudian, perintah getty akan menunggu input login dari pengguna melalui konsol.
5. Agar file init bisa dieksekusi dan memberikan izin eksekusi
6. Setelah menyiapkan semua file yang diperlukan, kita akan mengumpulkan file-file tersebut menggunakan utilitas cpio dan kemudian mengompresnya dengan gzip untuk membuat image initramfs dan menghasilak file myramdisk.gz yang berisi file minimal untuk booting

- **Screenshot:**

![Gambar1](https://github.com/user-attachments/assets/53f3267c-275f-4f90-b11f-4da33cd1a983)
![Gambar1](https://github.com/user-attachments/assets/72aaf7c5-570f-4b44-9c94-a58a31ddb2d5)
![Gambar1](https://github.com/user-attachments/assets/2f6c8381-42b5-4a2d-bfdf-2ee3d0b68480)
![Gambar1](https://github.com/user-attachments/assets/f30ce74f-a86c-4bf2-9791-5eecbbbd0fe6)
![Gambar1](https://github.com/user-attachments/assets/459a9f88-b52a-4d8f-90e8-962b6dc3dc88)

### Soal 4

> Dosen meminta Budiman membuat sistem operasi ini memilki **superuser** layaknya sistem operasi pada umumnya. User root yang sudah kamu buat sebelumnya akan digunakan sebagai superuser dalam sistem operasi milik Budiman. Superuser yang dimaksud adalah user dengan otoritas penuh yang dapat mengakses seluruhnya. Akan tetapi user lain tidak boleh memiliki otoritas yang sama. Dengan begitu user-user selain root tidak boleh mengakses `./root`. Buatlah sehingga tiap user selain superuser tidak dapat mengakses `./root`!

> _The lecturer requests that the OS must have a **superuser** just like other operating systems. The root user created earlier will serve as the superuser in Budiman's OS. The superuser should have full authority to access everything. However, other users should not have the same authority. Therefore, users other than root should not be able to access `./root`. Implement this so that non-superuser accounts cannot access `./root`!_

**Answer:**

- **Code:**

  `chown root:root /home/root`

  `chmod 700 /home/root`

- **Explanation:**

  Jalankan perintah `chmod` dan `chown` pada terminal sebelum booting menggunakan QEMU, guna untuk memberi access read/write/execute kepada root (chmod) dan memastikan ownership direktori `/root` hanya kepada root (chown)

- **Screenshot:**

  ![Image](https://github.com/user-attachments/assets/e2ab902f-6a37-4dc1-873c-0cd6204f3714)
  
  ![Image](https://github.com/user-attachments/assets/7f5c1e71-27f9-48e4-8fbf-ef6e5b99c26c)

  ![Image](https://github.com/user-attachments/assets/52a66ac6-8878-46d7-a9f5-4281ed1f83ed)
### Soal 5

> Setiap user rencananya akan digunakan oleh satu orang tertentu. **Privasi dan otoritas tiap user** merupakan hal penting. Oleh karena itu, Budiman ingin membuat setiap user hanya bisa mengakses dirinya sendiri dan tidak bisa mengakses user lain. Buatlah sehingga sistem operasi Budiman dalam melakukan hal tersebut!

> _Each user is intended for an individual. **Privacy and authority** for each user are important. Therefore, Budiman wants to ensure that each user can only access their own files and not those of others. Implement this in Budiman's OS!_

**Answer:**

- **Code:**

  `chmod 700 /home/Budiman`

  `chmod 700 /home/guest`

  `chmod 700 /home/praktikan1`

  `chmod 700 /home/praktikan2`

  `chown 1001:100 /home/Budiman`

  `chown 1002:100 /home/guest`

  `chown 1003:100 /home/praktikan1`

  `chown 1004:100 /home/praktikan2`

- **Explanation:**

  Jalankan perintah `chmod` dan `chown` di terminal sebelum booting di QEMU, guna untuk memberi access read/write/execute kepada user-user (chmod) dan memastikan ownership direktori yang dimiliki setiap user (chown)

- **Screenshot:**

  ![Image](https://github.com/user-attachments/assets/7409866d-54db-498c-ae8b-d7626049eed1)

  ![Image](https://github.com/user-attachments/assets/38bc067f-5999-4d19-9367-6f8c3ce351f9)

### Soal 6

> Dosen Budiman menginginkan sistem operasi yang **stylish**. Budiman memiliki ide untuk membuat sistem operasinya menjadi stylish. Ia meminta kamu untuk menambahkan tampilan sebuah banner yang ditampilkan setelah suatu user login ke dalam sistem operasi Budiman. Banner yang diinginkan Budiman adalah tulisan `"Welcome to OS'25"` dalam bentuk **ASCII Art**. Buatkanlah banner tersebut supaya Budiman senang! (Hint: gunakan text to ASCII Art Generator)

> _Budiman wants a **stylish** operating system. Budiman has an idea to make his OS stylish. He asks you to add a banner that appears after a user logs in. The banner should say `"Welcome to OS'25"` in **ASCII Art**. Use a text to ASCII Art generator to make Budiman happy!_ (Hint: use a text to ASCII Art generator)

**Answer:**

- **Code:**
  - Membuat file /etc/motd untuk menyimpan banner
    
  ![Image](https://github.com/user-attachments/assets/4aadce1d-697b-4955-8dac-2f75b58f1174)

  - Membuat file /etc/profile untuk menampilkan banner

  ![Image](https://github.com/user-attachments/assets/260c0bdd-d4af-4146-9d46-32b2ee8d2b67)

  

- **Explanation:**

  - File `motd` berguna untuk menyimpan banner `Welcome to OS'25`
  - File `profile` berguna untuk menampilkan banner setelah login dengan perintah `cat`

- **Screenshot:**

  ![image](https://github.com/user-attachments/assets/bd88af35-e735-4791-8300-a51a55df938b)


### Soal 7

> Melihat perkembangan sistem operasi milik Budiman, Dosen kagum dengan adanya banner yang telah kamu buat sebelumnya. Kemudian Dosen juga menginginkan sistem operasi Budiman untuk dapat menampilkan **kata sambutan** dengan menyebut nama user yang login. Sambutan yang dimaksud berupa kalimat `"Helloo %USER"` dengan `%USER` merupakan nama user yang sedang menggunakan sistem operasi. Kalimat sambutan ini ditampilkan setelah user login dan setelah banner. Budiman kembali lagi meminta bantuanmu dalam menambahkan fitur ini.

> _Seeing the progress of Budiman's OS, the lecturer is impressed with the banner you created. The lecturer also wants the OS to display a **greeting message** that includes the name of the user who logs in. The greeting should say `"Helloo %USER"` where `%USER` is the name of the user currently using the OS. This greeting should be displayed after user login and after the banner. Budiman asks for your help again to add this feature._

**Answer:**

- **Code:**

  - File `/etc/profile`
 
    ![image](https://github.com/user-attachments/assets/dc3f88ea-2aea-4576-9fb9-a25d55e33ad5)


- **Explanation:**

  Setelah perintah `cat` untuk banner, buat variabel untuk mengambil hasil dari `$(whoami)` yang dimana akan menampilkan user yang sedang login/ yang sedang memberi perintah

- **Screenshot:**

  ![image](https://github.com/user-attachments/assets/09b1c27d-a5b1-40d2-94af-771a0555646d)


### Soal 8

> Dosen Budiman sudah tua sekali, sehingga beliau memiliki kesulitan untuk melihat tampilan terminal default. Budiman menginisiatif untuk membuat tampilan sistem operasi menjadi seperti terminal milikmu. Modifikasilah sistem operasi Budiman menjadi menggunakan tampilan terminal kalian.

> _Budiman's lecturer is quite old and has difficulty seeing the default terminal display. Budiman takes the initiative to make the OS look like your terminal. Modify Budiman's OS to use your terminal appearance!_

**Answer:**

- **Code:**

  ![image](https://github.com/user-attachments/assets/6d7c6f45-668f-498b-a872-45afb19d2ac3)


- **Explanation:**

  - `echo -ne "\033[48;2;178;23;140m"` untuk mengubah warna backgorund magenta
  - `export PROMPT_COMMAND='echo -ne "\033[48;2;178;23;140m"'` warna backgorund tetap
  - `export PS1='\[\033[1;32m\]\u@\h:\w \$ \[\033[0m\]'` prompt shell berwarna hijau


- **Screenshot:**

  ![image](https://github.com/user-attachments/assets/0af92824-5da9-4038-a0d9-2c03e881b0b7)


### Soal 9

> Ketika mencoba sistem operasi buatanmu, Budiman tidak bisa mengubah text file menggunakan text editor. Budiman pun menyadari bahwa dalam sistem operasi yang kamu buat tidak memiliki text editor. Budimanpun menyuruhmu untuk menambahkan **binary** yang telah disiapkan sebelumnya ke dalam sistem operasinya. Buatlah sehingga sistem operasi Budiman memiliki **binary text editor** yang telah disiapkan!

> _When trying your OS, Budiman cannot edit text files using a text editor. He realizes that the OS you created does not have a text editor. Budiman asks you to add the prepared **binary** into his OS. Make sure Budiman's OS has the prepared **text editor binary**!_

**Answer:**

- **Code:**

```git clone https://github.com/morisab/budiman-text-editor.git```
```cd budiman-text-editor```
```g++ main.cpp -o budiman```
```cp budiman myramdisk/bin/```
```./budiman```



- **Explanation:**

  Kita buat text editor dengan referensi yang diambil dari repository github
  `https://github.com/morisab/budiman-text-editor`
  setelahnya kita copy dan taruh di folder `myramdisk/bin/budiman`, yang dimana directory ini menyimpan file file binary,
  setelahnya kita masuk ke OS kita dan login ke salah satu user, dan jalankan budiman, tara text editor berhasil dibuat.

  `git clone https://github.com/morisab/budiman-text-editor.git`
  `cd budiman-text-editor`
  Penjelasan: kita clone file dari repository referensi, dan pindah ke directory yang sudah kita clone
  `g++ main.cpp -o budiman`
  Penjelasan: kita compile file yang sudah kita clone tersebut
  `cp budiman myramdisk/bin/`
  Penjelasan: Kita copy file budiman, ke tujuan myramdisk/bin/
  `./budiman`
  Penjelasan: kita login ke os yang kita buat, login ke salah satu user, dan ketik command budiman, text editor sudah muncul.
  
  

- **Screenshot:**
  ### Gambar Text Editornya
 ![Image](https://github.com/user-attachments/assets/f1f5df86-d95f-46be-8815-0ec93b6108be)

### Soal 10

> Setelah seluruh fitur yang diminta Dosen dipenuhi dalam sistem operasi Budiman, sudah waktunya Budiman mengumpulkan tugasnya ini ke Dosen. Akan tetapi, Dosen Budiman tidak mau menerima pengumpulan selain dalam bentuk **.iso**. Untuk terakhir kalinya, Budiman meminta tolong kepadamu untuk mengubah seluruh konfigurasi sistem operasi yang telah kamu buat menjadi sebuah **file .iso**.

> After all the features requested by the lecturer have been implemented in Budiman's OS, it's time for Budiman to submit his assignment. However, Budiman's lecturer only accepts submissions in the form of **.iso** files. For the last time, Budiman asks for your help to convert the entire configuration of the OS you created into a **.iso file**.

**Answer:**

- **Code:**

1. ```bash
   mkdir -p mylinuxiso/boot/grub
   ```

2. ```bash
   cp bzImage mylinuxiso/boot
   cp myramdisk.gz mylinuxiso/boot
   ```

3. ```cfg
   set timeout=5
   set default=0

   menuentry "MyLinux" {
     linux /boot/bzImage
     initrd /boot/myramdisk.gz
   }
   ```

4. ```bash
   grub-mkrescue -o mylinux.iso mylinuxiso
   ```

- **Explanation:**

1. Membuat struktur direktori ISO
2. Menyalin file kernel dan root filesystem
3. Membuat file `grub.cfg` di `mylinuxiso/boot/grub`
4. Jalankan perintah berikut dari direktori `osboot`

- **Screenshot:**
  ### Gambar osboot
![Image](https://github.com/user-attachments/assets/8966108c-8c2d-4add-95d2-5ef8079e28a9)

### Gambar saat ISO dijalankan
 ![Image](https://github.com/user-attachments/assets/2acbb141-4d8c-4af7-b7b6-c6b21f006239)


---

Pada akhirnya sistem operasi Budiman yang telah kamu buat dengan susah payah dikumpulkan ke Dosen mengatasnamakan Budiman. Kamu tidak diberikan credit apapun. Budiman pun tidak memberikan kata terimakasih kepadamu. Kamupun kecewa tetapi setidaknya kamu telah belajar untuk menjadi pembuat sistem operasi sederhana yang andal. Selamat!

_At last, the OS you painstakingly created was submitted to the lecturer under Budiman's name. You received no credit. Budiman didn't even thank you. You feel disappointed, but at least you've learned to become a reliable creator of simple operating systems. Congratulations!_
