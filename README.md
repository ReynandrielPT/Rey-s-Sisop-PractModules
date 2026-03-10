# Praktikum Sistem Operasi 2024–2025

Repositori ini berisi rangkuman tugas, laporan, dan kode sumber praktikum mata kuliah **Sistem Operasi** untuk dua angkatan/tahun ajaran dan beberapa kelas:

- **Praktikum 2024** – kelas B24
- **Praktikum 2025** – kelas D03 dan E12

Sebagian besar isi repositori berupa skrip Bash, program C, konfigurasi Docker, serta implementasi sistem berkas dan kernel sederhana untuk tugas akhir praktikum.

---

## Struktur Direktori Utama

- `Praktikum 2024/`
  - `praktikum-modul-1-b24-2024/` – Shell scripting, pengolahan data (AWK), cron, dan manajemen log.
  - `praktikum-modul-2-b24-2024/` – Pemrograman C terkait proses, thread, sinkronisasi, dan pemrosesan file.
  - `praktikum-modul-3-b24-2024/` – IPC, socket (client–server, chat), serta Docker dan docker-compose.
  - `praktikum-modul-4-b24-2024/` – Implementasi FUSE, manajemen file, dan mini OS (lanjutan ke praktikum final).
  - `praktikum-final-b24-2024/` – Tugas akhir: membuat **filesystem sederhana**, melengkapi **kernel** dan **shell** pada OS mini (lihat README di dalam folder ini untuk detail teknis).

- `Praktikum 2025/`
  - `D03/`
    - `praktikum-modul-1-d03-2025/` – Materi dan tugas praktikum Sistem Operasi kelas D03 (lihat README di dalam folder untuk detail).
    - `praktikum-modul-2-d03-2025/` – Struktur modul 2 kelas D03.
    - `praktikum-modul-3-d03-2025/` – Struktur modul 3 kelas D03.
    - `praktikum-modul-4-d03-2025/` – Struktur modul 4 kelas D03.
  - `E12/`
    - `README.md` – Ringkasan praktikum Sistem Operasi kelas E12.
    - `praktikum-modul-1-e12-main/` – Repositori modul 1 E12 (shell scripting, pengolahan data, dsb.; lihat README di dalamnya).
    - `sisop-modul-2-Sakuya133-main/` – Repositori modul 2 E12.
    - `sisop-modul-3-Sakuya133-main/` – Repositori modul 3 E12.
    - `sisop-modul-4-Sakuya133-main/` – Repositori modul 4 E12.
    - `sisop-final-praktikum-Sakuya133-main/` – Repositori final praktikum kelas E12.

Selain itu, beberapa repositori untuk kelas E12 juga tersedia di level teratas repositori ini (misalnya `praktikum-modul-1-e12-main/`, `sisop-final-praktikum-Sakuya133-main/`, dan `sisop-modul-*-Sakuya133-main/`) sebagai salinan/arsip terpisah.

Setiap folder modul umumnya memiliki:

- `README.md` – Penjelasan soal dan laporan resmi praktikum.
- `task-*/` atau `task-1/`, `task-2/`, dst. – Folder untuk setiap soal/tugas.
- Berkas sumber seperti `.sh`, `.c`, `Dockerfile`, `docker-compose.yml`, dan lain-lain.

---

## Cara Menjelajahi Repositori

1. **Pilih tahun praktikum**
   - Untuk arsip 2024: buka folder `Praktikum 2024/`.
   - Untuk arsip 2025: buka folder `Praktikum 2025/`.

2. **Pilih modul**
   - Contoh: `Praktikum 2024/praktikum-modul-1-b24-2024/` untuk modul 1 tahun 2024.
   - Baca `README.md` di dalam setiap modul untuk konteks soal dan laporan.

3. **Pilih task/soal**
   - Masuk ke subfolder `task-1/`, `task-2/`, dan seterusnya.
   - Di dalamnya terdapat skrip atau program beserta penjelasannya di README modul.

4. **Tugas akhir (Final Praktikum)**
   - Buka `Praktikum 2024/praktikum-final-b24-2024/`.
   - Baca `README.md` / `README-EN.md` di dalam folder tersebut untuk petunjuk menjalankan Bochs, struktur disk, filesystem, kernel, dan shell.

---

## Menjalankan Kode

> Catatan: Sebagian besar kode ditujukan untuk dijalankan di lingkungan **Linux** (atau WSL) dan memerlukan alat seperti `bash`, `gcc`, `make`, `docker`, hingga Bochs.

- **Skrip Bash (`*.sh`)**
  1. Pastikan file dapat dieksekusi: `chmod +x nama_script.sh`.
  2. Jalankan: `./nama_script.sh` atau `bash nama_script.sh`.

- **Program C (`*.c`)**
  - Kompilasi manual dengan `gcc`:
    - `gcc -o program program.c` lalu jalankan `./program`.
  - Beberapa tugas (terutama OS/Bochs) sudah memiliki `makefile`:
    - Cukup jalankan `make` di folder tersebut, lalu ikuti instruksi di README lokal.

- **Docker & docker-compose**
  - Untuk folder dengan `Dockerfile` atau `docker-compose.yml` (misalnya di modul 3):
    - Bangun image: `docker build -t nama-image .`
    - Jalankan dengan docker-compose: `docker-compose up`.

- **Mini OS / Bochs (Final Praktikum & Modul 4)**
  - Ikuti langkah di `README.md`/`README-EN.md` pada folder terkait (biasanya melibatkan `make`, `bochs`, dan file `bochsrc.txt`).

---

## Konvensi & Tujuan

- Repositori ini berfungsi sebagai **arsip pribadi** dan **bahan belajar publik** tentang praktikum Sistem Operasi.
- Nama folder mengikuti format: `praktikum-modul-[nomor]-[kelas]-[tahun]/` atau `praktikum-final-[kelas]-[tahun]/`.
- Penamaan tugas memakai `task-1/`, `task-2/`, dan seterusnya agar navigasi lebih mudah.

---

## Catatan Etika Akademik

- Solusi di dalam repositori ini diperuntukkan sebagai **referensi dan bahan belajar**.
- Jika Anda sedang mengambil mata kuliah yang sama, **jangan melakukan plagiasi langsung** terhadap kode atau laporan di sini.
- Gunakan sebagai contoh konsep, pola pemrograman, atau inspirasi, lalu tulis dan kerjakan solusi Anda sendiri.

---

# Operating Systems Practicum 2024–2025

This repository contains assignments, reports, and source code for the **Operating Systems** laboratory/practicum over two academic years and multiple classes:

- **Praktikum 2024** – class B24
- **Praktikum 2025** – classes D03 and E12

Most of the content consists of Bash scripts, C programs, Docker configurations, and a simple filesystem / kernel / shell implementation for the final practicum project.

---

## Top-Level Directory Structure

- `Praktikum 2024/`
  - `praktikum-modul-1-b24-2024/` – Shell scripting, data processing with AWK, cron jobs, and log management.
  - `praktikum-modul-2-b24-2024/` – C programming focusing on processes, threads, synchronization, and file processing.
  - `praktikum-modul-3-b24-2024/` – IPC, sockets (client–server, chat), plus Docker and docker-compose usage.
  - `praktikum-modul-4-b24-2024/` – FUSE implementation, file management, and a mini OS (continued in the final practicum).
  - `praktikum-final-b24-2024/` – Final project: building a **simple filesystem**, extending the **kernel**, and adding a **shell** for a toy OS (see the README inside for technical details).

- `Praktikum 2025/`
  - `D03/`
    - `praktikum-modul-1-d03-2025/` – Operating Systems practicum materials for class D03 (see the module README for details).
    - `praktikum-modul-2-d03-2025/` – Module 2 directory structure for class D03.
    - `praktikum-modul-3-d03-2025/` – Module 3 directory structure for class D03.
    - `praktikum-modul-4-d03-2025/` – Module 4 directory structure for class D03.
  - `E12/`
    - `README.md` – Overview of the Operating Systems practicum for class E12.
    - `praktikum-modul-1-e12-main/` – Module 1 repository for E12 (shell scripting, data processing, etc.; see its README).
    - `sisop-modul-2-Sakuya133-main/` – Module 2 repository for E12.
    - `sisop-modul-3-Sakuya133-main/` – Module 3 repository for E12.
    - `sisop-modul-4-Sakuya133-main/` – Module 4 repository for E12.
    - `sisop-final-praktikum-Sakuya133-main/` – Final practicum project repository for class E12.

In addition, several E12 repositories are also duplicated at the repository root level (for example, `praktikum-modul-1-e12-main/`, `sisop-final-praktikum-Sakuya133-main/`, and `sisop-modul-*-Sakuya133-main/`) as separate archived copies.

Each module folder typically contains:

- `README.md` – Problem descriptions and official practicum write-up.
- `task-*/` or `task-1/`, `task-2/`, etc. – Subfolders for each problem/task.
- Source files such as `.sh`, `.c`, `Dockerfile`, `docker-compose.yml`, and others.

---

## How to Navigate the Repository

1. **Choose the practicum year**
   - For the 2024 archive: open `Praktikum 2024/`.
   - For the 2025 archive: open `Praktikum 2025/`.

2. **Choose a module**
   - Example: `Praktikum 2024/praktikum-modul-1-b24-2024/` for module 1 in 2024.
   - Read the `README.md` inside each module for context and detailed explanations.

3. **Choose a specific task**
   - Go into `task-1/`, `task-2/`, etc.
   - Inside you will find scripts or programs along with explanations in the module’s README.

4. **Final practicum project**
   - Open `Praktikum 2024/praktikum-final-b24-2024/`.
   - Read `README.md` / `README-EN.md` inside that folder for how to run Bochs and details about disk layout, filesystem, kernel, and shell.

---

## Running the Code

> Note: Most code is intended to run on **Linux** (or WSL) and may require tools such as `bash`, `gcc`, `make`, `docker`, and Bochs.

- **Bash scripts (`*.sh`)**
  1. Ensure the script is executable: `chmod +x script_name.sh`.
  2. Run: `./script_name.sh` or `bash script_name.sh`.

- **C programs (`*.c`)**
  - Compile manually with `gcc`:
    - `gcc -o program program.c` then run `./program`.
  - Some projects (especially OS/Bochs ones) provide a `makefile`:
    - Run `make` in that folder and follow the local README instructions.

- **Docker & docker-compose**
  - For folders with `Dockerfile` or `docker-compose.yml` (e.g., in module 3):
    - Build an image: `docker build -t image-name .`
    - Run with docker-compose: `docker-compose up`.

- **Mini OS / Bochs (Final Practicum & Module 4)**
  - Follow the steps in the `README.md` / `README-EN.md` files in the corresponding folders (typically using `make`, `bochs`, and `bochsrc.txt`).

---

## Conventions & Purpose

- This repository serves both as a **personal archive** and a **public learning resource** for Operating Systems practicum materials.
- Folder names follow the pattern: `praktikum-modul-[number]-[class]-[year]/` or `praktikum-final-[class]-[year]/`.
- Tasks are grouped into `task-1/`, `task-2/`, etc. to make navigation straightforward.

---

## Academic Integrity Notice

- The solutions here are provided as **references and learning material**.
- If you are currently enrolled in a similar course, **do not copy–paste these solutions** into your own submissions.
- Use them to understand the ideas, then implement and write your own solutions.
