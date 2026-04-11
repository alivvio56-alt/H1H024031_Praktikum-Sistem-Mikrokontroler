## Pertanyaan Praktikum 2.54
###Gambar Skema Input
gambar skema pada Folder dokumentasi

### 2. Apa yang terjadi jika nilai num lebih dari 15?
**Jawaban:**
Apabila variabel `num` memiliki nilai melebihi 15, maka program akan mencoba mengakses indeks di luar batas *array* yang telah ditentukan (*out of bounds*). Hal ini menyebabkan sistem mengambil data dari alamat memori yang tidak relevan, sehingga Seven Segment akan menampilkan pola LED yang acak, tidak beraturan, atau tidak sesuai dengan karakter heksadesimal yang seharusnya.

### 3. Apakah program ini menggunakan common cathode atau common anode? Jelaskan alasannya!
**Jawaban:**
Program ini menggunakan jenis **Common Cathode**. 

**Alasan:**
Berdasarkan logika pada kode program, instruksi `digitalWrite` memberikan nilai **HIGH (1)** untuk menyalakan segmen LED. Pada komponen *Common Cathode*, seluruh katoda segmen terhubung ke *Ground*, sehingga diperlukan tegangan positif (HIGH) dari pin output Arduino untuk menciptakan aliran arus yang menyalakan LED. Jika menggunakan *Common Anode*, maka logika yang diperlukan adalah LOW untuk menyalakan segmen.

### 4. Modifikasi program agar tampilan berjalan dari F ke 0 dan berikan penjelasan disetiap baris kode nya dalam bentuk README.md!

**Jawaban:**

```cpp
// Array pola heksadesimal untuk tampilan 0-F (Common Cathode)
byte segmentCodes[] = {
  0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 
  0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71
};

void setup() {
  // Mengatur pin digital 2 sampai 8 sebagai OUTPUT untuk segmen a-g
  for (int i = 2; i <= 8; i++) {
    pinMode(i, OUTPUT);
  }
}

void loop() {
  // Perulangan mundur dimulai dari indeks 15 (karakter F) hingga 0
  for (int i = 15; i >= 0; i--) {
    displayDigit(segmentCodes[i]); // Memanggil fungsi untuk mengirim pola ke pin
    delay(1000);                   // Memberikan jeda waktu tampil selama 1 detik
  }
}

void displayDigit(byte code) {
  // Membaca bit-bit dari pola heksadesimal untuk mengontrol pin fisik
  for (int bit = 0; bit < 7; bit++) {
    int state = bitRead(code, bit); // Mengekstraksi nilai bit (0 atau 1) dari data pola
    digitalWrite(bit + 2, state);   // Mengirimkan logika ke pin (pin 2 untuk bit 0, dst)
  }
}
```


## Pertanyaan Praktikum 2.6.4

### 1. Gambarkan rangkaian schematic yang digunakan pada percobaan!
**Jawaban:**
Gambar Rangkaian pada folder Dokumentasi

### 2. Mengapa pada push button digunakan mode INPUT_PULLUP pada Arduino Uno? Apa keuntungannya dibandingkan rangkaian biasa?
**Jawaban:**
Mode `INPUT_PULLUP` mengaktifkan resistor internal di dalam mikrokontroler yang menarik tegangan pin ke arah 5V secara *default*.

**Keuntungan:**
* **Penyederhanaan Perangkat Keras:** Tidak memerlukan resistor eksternal tambahan pada *breadboard*, sehingga meminimalisir penggunaan kabel dan komponen.
* **Stabilitas Sinyal:** Menghilangkan kondisi *floating* (sinyal mengambang) yang dapat menyebabkan Arduino menerima input palsu akibat gangguan elektromagnetik di sekitar rangkaian.
* **Logika Sederhana:** Memungkinkan koneksi langsung tombol ke *Ground*, di mana kondisi ditekan akan terbaca sebagai `LOW`.

### 3. Jika salah satu LED segmen tidak menyala, apa saja kemungkinan penyebabnya dari sisi hardware maupun software?
**Jawaban:**
Identifikasi masalah dapat ditinjau dari dua aspek:
* **Aspek Hardware:** * Putusnya jalur kabel *jumper* atau resistor pada segmen tertentu.
    * Kerusakan fisik pada salah satu elemen LED di dalam komponen Seven Segment.
    * Pemasangan kaki komponen yang tidak rapat pada *breadboard*.
* **Aspek Software:**
    * Pin terkait belum dikonfigurasi sebagai `OUTPUT` pada fungsi `setup()`.
    * Terdapat kesalahan nilai bit pada *array* `segmentCodes` yang merepresentasikan segmen tersebut.
    * Kesalahan penulisan nomor pin dalam fungsi perulangan `digitalWrite`.

---

## Modifikasi Program: Increment & Decrement Counter

Modifikasi ini mengimplementasikan dua tombol untuk menambah (*increment*) dan mengurangi (*decrement*) angka heksadesimal yang ditampilkan.

### Kode Program (Arduino Sketch)
```cpp
/*
 * Modifikasi Tugas 2.6.4: Dual Button Counter
 */

// Pola heksadesimal 0-F (Common Cathode)
byte segmentCodes[] = {
  0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 
  0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71
};

const int pinUp = 9;    // Pin untuk tombol penambah
const int pinDown = 10;  // Pin untuk tombol pengurang
int counter = 0;        // Variabel penyimpan nilai angka

void setup() {
  // Konfigurasi pin Seven Segment sebagai output
  for (int i = 2; i <= 8; i++) {
    pinMode(i, OUTPUT);
  }
  
  // Konfigurasi pin tombol sebagai input dengan internal pull-up
  pinMode(pinUp, INPUT_PULLUP);
  pinMode(pinDown, INPUT_PULLUP);
  
  // Menampilkan angka 0 pada saat awal program dijalankan
  displayDigit(segmentCodes[counter]);
}

void loop() {
  // Logika untuk tombol tambah (Increment)
  if (digitalRead(pinUp) == LOW) {
    counter++;
    if (counter > 15) counter = 0; // Kembali ke 0 jika melewati F
    displayDigit(segmentCodes[counter]);
    delay(200); // Debouncing untuk mencegah bouncing sinyal
  }

  // Logika untuk tombol kurang (Decrement)
  if (digitalRead(pinDown) == LOW) {
    counter--;
    if (counter < 0) counter = 15; // Kembali ke F jika di bawah 0
    displayDigit(segmentCodes[counter]);
    delay(200); // Debouncing untuk mencegah bouncing sinyal
  }
}

void displayDigit(byte code) {
  // Fungsi untuk mengirimkan data bit ke pin digital 2-8
  for (int bit = 0; bit < 7; bit++) {
    int state = bitRead(code, bit);
    digitalWrite(bit + 2, state);
  }
}
