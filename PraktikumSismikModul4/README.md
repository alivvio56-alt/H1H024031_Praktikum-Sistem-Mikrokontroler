# Jawaban Praktikum Modul 4 

## Percobaan 1: Kontrol Servo dengan Potensiometer

### Jawaban Pertanyaan

**1. Apa fungsi perintah `analogRead()` pada rangkaian praktikum ini?**

Fungsi `analogRead()` digunakan untuk membaca tegangan analog dari pin input (dalam hal ini pin `A0` yang terhubung ke potensiometer). Perintah ini mengubah tegangan fisik antara `0V` hingga `5V` menjadi nilai digital dalam rentang `0` hingga `1023`.

---

**2. Mengapa diperlukan fungsi `map()` dalam program tersebut?**

Fungsi `map()` diperlukan untuk menyelaraskan atau menskalakan rentang nilai yang berbeda antara input dan output:

- **Servo:** Mengubah rentang input ADC (`0–1023`) menjadi rentang sudut servo (`0–180` derajat).
- **LED:** Mengubah rentang input ADC (`0–1023`) menjadi rentang nilai PWM (`0–255`).

Tanpa fungsi ini, nilai ADC yang besar tidak dapat langsung digunakan karena melebihi batas maksimal parameter fungsi `myservo.write()` atau `analogWrite()`.

---

**3. Modifikasi Program — Servo Rentang 30° hingga 150°**

Untuk membatasi gerak servo, ubah parameter output fungsi `map()` di dalam file `percobaan2.ino`:

```cpp
// Ubah bagian ini pada fungsi loop
pos = map(val,
           0,    // minimum ADC tetap 0
           1023, // maksimum ADC tetap 1023
           30,   // sudut minimum diubah menjadi 30 derajat
           150); // sudut maksimum diubah menjadi 150 derajat
```

**Penjelasan:**

Program ini merupakan modifikasi dari sistem kontrol servo berbasis ADC. Meskipun potensiometer memberikan input penuh dari `0` hingga `5V` (nilai digital `0–1023`), rentang gerak mekanis servo dibatasi secara perangkat lunak melalui fungsi `map()`.

- **Logika Pemetaan:** Perintah `map(val, 0, 1023, 30, 150)` menginstruksikan Arduino untuk mengonversi nilai terendah potensiometer (`0`) menjadi posisi `30°` dan nilai tertinggi (`1023`) menjadi posisi `150°`.
- **Tujuan:** Membatasi pergerakan aktuator agar hanya bekerja pada area kerja tertentu, guna mencegah kerusakan mekanis pada sistem yang memiliki pembatas fisik.

---

## Percobaan 2: Kontrol Kecerahan LED dengan PWM

### Jawaban Pertanyaan

**1. Jelaskan mengapa LED dapat diatur kecerahannya menggunakan fungsi `analogWrite()`!**

LED dapat diatur kecerahannya karena fungsi `analogWrite()` menggunakan teknik **Pulse Width Modulation (PWM)**. PWM bekerja dengan cara menyalakan dan mematikan sinyal digital secara sangat cepat dalam satu periode waktu. Dengan mengubah rasio waktu sinyal berada pada kondisi `HIGH` dibandingkan dengan waktu total (*duty cycle*), mikrokontroler dapat menghasilkan tegangan rata-rata efektif yang berbeda-beda. Mata manusia merespons perubahan tegangan rata-rata ini sebagai perubahan intensitas cahaya.

---

**2. Apa hubungan antara nilai ADC (0–1023) dan nilai PWM (0–255)?**

Hubungan tersebut terletak pada resolusi bit masing-masing fitur di mikrokontroler Arduino:

| Fitur | Resolusi | Rentang Nilai |
|-------|----------|---------------|
| ADC   | 10-bit   | 0 – 1023      |
| PWM   | 8-bit    | 0 – 255       |

Keduanya memiliki **hubungan linear sebesar 4:1**. Oleh karena itu, diperlukan fungsi `map()` untuk menyelaraskan kedua rentang tersebut agar setiap perubahan pada potensiometer dapat diterjemahkan secara proporsional ke kecerahan LED.

---

**3. Modifikasi Program — LED Rentang Kecerahan 50–200**

Untuk membatasi intensitas cahaya LED agar hanya berada pada rentang sedang, ubah parameter output fungsi `map()` di dalam file `percobaan1_ino.ino`:

```cpp
// Ubah bagian ini pada fungsi loop
pwm = map(nilaiADC,
          0,    // minimum ADC tetap 0
          1023, // maksimum ADC tetap 1023
          50,   // PWM minimum diubah menjadi 50 (redup)
          200); // PWM maksimum diubah menjadi 200 (terang sedang)
```

**Penjelasan:**

Program ini menerapkan pembatasan intensitas daya pada LED melalui manipulasi nilai *duty cycle* PWM. Walaupun input analog dari potensiometer memberikan rentang penuh (`0V` hingga `5V`), output yang dihasilkan telah dibatasi melalui perangkat lunak.

- **Logika Scaling:** Perintah `map(nilaiADC, 0, 1023, 50, 200)` memastikan bahwa saat potensiometer diputar ke titik terendah, LED tetap menyala redup (nilai PWM `50`). Sebaliknya, saat diputar ke titik maksimal, LED hanya mencapai batas nilai `200`.
- **Tujuan:** Menjaga agar beban (LED) selalu beroperasi dalam zona aman atau rentang performa tertentu yang diinginkan, tanpa harus mengubah rangkaian fisik.
