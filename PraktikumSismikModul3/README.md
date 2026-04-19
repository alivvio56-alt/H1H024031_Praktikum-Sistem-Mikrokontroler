# Laporan Praktikum — Komunikasi Serial UART & I2C

---

## IV. Pertanyaan Praktikum

---

## Bagian A: Komunikasi Serial UART

### 1. Jelaskan proses dari input keyboard hingga LED menyala/mati!

Proses tersebut melibatkan alur komunikasi serial (UART) sebagai berikut:

| Tahap | Deskripsi |
|-------|-----------|
| **Input** | Pengguna mengetik karakter (misal `'1'`) di Serial Monitor dan menekan Enter. |
| **Transmisi** | Karakter tersebut dikirimkan bit-per-bit melalui kabel USB ke pin RX Arduino. |
| **Penyimpanan** | Data yang masuk disimpan sementara di dalam Serial Buffer Arduino. |
| **Processing** | Fungsi `Serial.read()` mengambil karakter dari buffer. Program masuk ke logika `if-else`. Jika karakter adalah `'1'`, Arduino memberikan tegangan tinggi (`HIGH`) ke pin LED. |
| **Output** | Arus mengalir dari pin digital ke LED, sehingga LED menyala. Proses yang sama berlaku untuk input `'0'` dengan memberikan tegangan rendah (`LOW`). |

---

### 2. Mengapa digunakan `Serial.available()` sebelum membaca data?

Fungsi `Serial.available()` digunakan untuk memastikan bahwa ada data yang benar-benar telah diterima di dalam buffer sebelum Arduino mencoba membacanya.

**Jika baris ini dihilangkan:**

- Program akan terus-menerus mencoba menjalankan `Serial.read()` di setiap siklus `loop()`.
- Karena `Serial.read()` mengembalikan nilai `-1` jika tidak ada data, variabel `input` akan terisi oleh nilai kosong/sampah secara terus-menerus.
- Hal ini dapat menyebabkan logika program (seperti pesan *"Perintah tidak dikenal"*) muncul berulang-ulang di Serial Monitor meskipun kita tidak mengetik apa pun.

---

### 3. Modifikasi Program — Input `'2'` untuk Mode Blink

Untuk fitur ini, kita memerlukan variabel `state` agar Arduino "ingat" bahwa ia sedang dalam mode berkedip.

#### Kode Program

```cpp
char data;
bool isBlinking = false;           // Status apakah sedang mode blink
unsigned long previousMillis = 0;
const long interval = 500;         // Interval kedip 500ms
int ledState = LOW;

void loop() {
  if (Serial.available() > 0) {
    data = Serial.read();

    if (data == '1') {
      isBlinking = false;
      digitalWrite(8, HIGH);
    }
    else if (data == '0') {
      isBlinking = false;
      digitalWrite(8, LOW);
    }
    else if (data == '2') {
      isBlinking = true;           // Aktifkan mode blink
    }
  }

  if (isBlinking) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      ledState = (ledState == LOW) ? HIGH : LOW;
      digitalWrite(8, ledState);
    }
  }
}
```

#### Penjelasan Komponen

| Komponen | Penjelasan |
|----------|------------|
| `bool isBlinking` | Variabel penanda untuk mengunci status apakah LED harus berkedip atau tidak. |
| `if (data == '2')` | Saat input `'2'` diterima, status `isBlinking` menjadi `true`, mengaktifkan mode blink. |
| `millis()` logic | Digunakan untuk membolak-balik status LED (`HIGH`/`LOW`) setiap 500ms tanpa menghentikan pembacaan serial. |

---

### 4. Penggunaan `delay()` vs `millis()`

Dalam kasus modifikasi tugas nomor 3, penggunaan `millis()` sangat disarankan dibandingkan `delay()`.

####  Pengaruh `delay()`

> Bersifat **blocking**. Jika menggunakan `delay(500)`, Arduino akan "berhenti" selama setengah detik. Jika pengguna mengirim perintah baru (misal mematikan LED) saat `delay` berlangsung, Arduino **tidak akan merespons** hingga waktu delay selesai. Ini membuat sistem terasa tidak responsif.

#### Pengaruh `millis()`

> Bersifat **non-blocking**. Arduino hanya mengecek waktu saat ini dan membandingkannya dengan waktu sebelumnya. Sembari menunggu waktu kedip tiba, Arduino tetap bisa mengecek `Serial.available()`. Hasilnya, sistem tetap **responsif** dan bisa menerima perintah baru kapan saja tanpa interupsi.

---

## Bagian B: Komunikasi I2C

### 1. Cara Kerja Komunikasi I2C antara Arduino dan LCD

Komunikasi I2C pada rangkaian ini bekerja menggunakan prinsip **Master-Slave**. Arduino bertindak sebagai **Master** yang mengendalikan aliran data, sedangkan modul I2C pada LCD bertindak sebagai **Slave** dengan alamat spesifik (misalnya `0x27`).

| Jalur | Fungsi |
|-------|--------|
| **SCL** (Serial Clock) | Arduino mengirimkan sinyal detak (clock) untuk mensinkronisasi pengiriman bit data. |
| **SDA** (Serial Data) | Arduino mengirimkan paket data yang berisi alamat LCD diikuti oleh instruksi tampilan (karakter/baris). |

Modul I2C pada LCD menerima data serial tersebut dan mengubahnya menjadi data paralel yang bisa dimengerti oleh driver **HD44780** pada LCD 16x2.

---

### 2. Konfigurasi Pin Potensiometer

Konfigurasi pin potensiometer tidak harus kaku, namun pemasangan kabel menentukan arah rotasi. Potensiometer memiliki tiga kaki: dua kaki luar (**VCC** dan **GND**) dan satu kaki tengah (**Wiper/Signal**).

**Jika pin kiri dan kanan tertukar:**

Secara fungsional alat tidak akan rusak. Namun, arah pembacaan nilai akan **terbalik**. Jika awalnya diputar ke kanan (searah jarum jam) nilai ADC bertambah besar, maka setelah ditukar, diputar ke kanan justru akan membuat nilai ADC mengecil menuju nol.

---

### 3. Modifikasi Program — Kombinasi UART dan I2C

#### Kode Program

```cpp
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
const int pinPot = A0;

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
}

void loop() {
  int adcVal    = analogRead(pinPot);
  float voltage = adcVal * (5.0 / 1023.0);
  int percent   = map(adcVal, 0, 1023, 0, 100);
  int barLength = map(adcVal, 0, 1023, 0, 16);

  // --- Output Serial Monitor (UART) ---
  Serial.print("ADC: ");
  Serial.println(adcVal);
  Serial.print("Volt: ");
  Serial.print(voltage);
  Serial.println(" V");
  Serial.print("Persen: ");
  Serial.print(percent);
  Serial.println("%");
  Serial.println("-----------------------");

  // --- Output LCD (I2C) ---
  lcd.setCursor(0, 0);
  lcd.print("ADC: ");
  lcd.print(adcVal);
  lcd.print(" ");
  lcd.print(percent);
  lcd.print("%   ");

  lcd.setCursor(0, 1);
  for (int i = 0; i < 16; i++) {
    if (i < barLength) {
      lcd.print((char)255);   // Karakter blok penuh
    } else {
      lcd.print(" ");         // Spasi kosong
    }
  }

  delay(500);
}
```

#### Penjelasan Fungsi/Komponen

| Fungsi / Komponen | Penjelasan |
|-------------------|------------|
| `analogRead(pinPot)` | Mengambil data mentah dari potensiometer (rentang 0–1023). |
| `adcVal * (5.0 / 1023.0)` | Rumus konversi untuk mendapatkan nilai tegangan nyata (0–5 Volt). |
| `map(adcVal, 0, 1023, 0, 100)` | Mengubah skala ADC ke persentase untuk tampilan Serial. |
| `lcd.setCursor` & `lcd.print` | Mengatur posisi kursor dan mencetak data ke layar LCD secara spesifik per baris. |
| `for` loop + `(char)255` | Logika untuk mencetak grafik batang sesuai dengan posisi potensiometer. |

#### Tabel Pengamatan

| ADC | Volt (V) | Persen (%) |
|:---:|:--------:|:----------:|
| 1   | 0.005    | 0%         |
| 21  | 0.10     | 2%         |
| 49  | 0.24     | 4%         |
| 74  | 0.36     | 7%         |
| 96  | 0.47     | 9%         |
