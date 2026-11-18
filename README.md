# Robot Pemotong Rumput Otomatis

Robot pemotong rumput berbasis Arduino dan energi surya. Robot berjalan sendiri, menghindari rintangan dengan dua sensor ultrasonik, dan menyalakan pisau pemotong hanya saat sensor warna mendeteksi rumput.

![Arduino](https://img.shields.io/badge/Arduino-C%2B%2B-00979D?logo=arduino&logoColor=white)
![Status](https://img.shields.io/badge/status-prototipe-orange)

## Cara kerja

1. **Membaca jarak.** Dua sensor ultrasonik (kanan dan kiri) mengukur jarak ke rintangan di depan robot.
2. **Membaca warna.** Sensor warna TCS3200 membaca nilai merah, hijau, dan biru dari permukaan di bawah robot, lalu menggolongkannya sebagai `Hijau`, `KuningKecoklatan`, `Coklat`, atau `TidakDikenal`.
3. **Mengatur pemotong.** Motor pemotong menyala saat warna `Hijau`, `KuningKecoklatan`, atau `Coklat`, dan mati untuk warna lain.
4. **Menghindari rintangan.** Rintangan terdeteksi pada jarak 1 sampai 5 cm.
   - Rintangan di kanan dan kiri: berhenti sebentar, lalu belok kiri.
   - Hanya di kanan: belok kiri.
   - Hanya di kiri: belok kanan.
   - Jalan kosong: maju.
5. **Melaporkan data.** Jarak, warna, dan status pemotong dicetak ke Serial Monitor (9600 baud).

Contoh keluaran:

```
JarakKanan=ND | JarakKiri=23cm | Warna=Hijau | Pemotong=ON | TinggiPemotong=1cm
```

`ND` berarti tidak ada rintangan terdeteksi.

## Komponen

- Papan Arduino
- 2 sensor ultrasonik (kanan dan kiri)
- Sensor warna TCS3200
- 2 modul driver motor L298N (satu untuk roda, satu untuk pemotong)
- 2 motor DC penggerak roda
- 1 motor DC pemotong
- Panel surya dan sumber daya

## Pemetaan pin

| Fungsi | Pin |
|---|---|
| Ultrasonik kanan (trig, echo) | A0, A1 |
| Ultrasonik kiri (trig, echo) | A3, A2 |
| Roda kanan maju, mundur (IN1, IN2) | 2, 3 |
| Roda kiri maju, mundur (IN3, IN4) | 4, 5 |
| Kecepatan roda kanan, kiri (ENA, ENB) | A4, A5 |
| Motor pemotong (IN1, IN2) | 6, 7 |
| Sensor warna (S0, S1, S2, S3, OUT) | 8, 9, 10, 11, 12 |

Pin ENA driver pemotong dijumper langsung ke 5V di modul, jadi pemotong selalu berputar penuh saat dinyalakan.

## Menjalankan

1. Pasang perangkat sesuai tabel pin di atas.
2. Buka `pemotongrumput/pemotongrumput.ino` di Arduino IDE.
3. Pilih papan dan port, lalu klik **Upload**.
4. Buka **Serial Monitor** dengan kecepatan 9600 baud untuk melihat data sensor.

## Pengaturan

Nilai berikut ada di bagian atas sketsa dan bisa disesuaikan:

| Variabel | Fungsi | Nilai awal |
|---|---|---|
| `speedMotor` | Kecepatan roda (0 sampai 255) | 150 |
| `tinggiPemotong` | Tinggi pemotong dalam cm (untuk keterangan di Serial Monitor) | 1 |

Penggolongan warna di fungsi `bacaWarna()` membandingkan frekuensi merah, hijau, dan biru. Batas nilainya perlu dikalibrasi sesuai cahaya dan jenis rumput di tempat robot dipakai.

## Status

Prototipe. Rencana pengembangan: kalibrasi warna otomatis, pembacaan jarak lebih jauh, dan pemantauan daya panel surya.

## Pembuat

**Wahyu Ardiyansah, S.Kom**
[GitHub](https://github.com/wahyuardiyan) · [Portofolio](https://portofolio-wa.is-best.net)
