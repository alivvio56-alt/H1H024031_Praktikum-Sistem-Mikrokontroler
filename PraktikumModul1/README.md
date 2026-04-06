Jawaban Pertanyaan Praktikum (Percobaan 1A)
1. Pada kondisi apa program masuk ke blok if?
	Program akan masuk ke dalam blok if ketika variabel timeDelay bernilai kurang dari atau sama dengan 100 ($timeDelay \leq 100$).
2. Pada kondisi apa program masuk ke blok else?
	Program akan masuk ke blok else selama nilai variabel timeDelay masih berada di atas 100.
3. Apa fungsi dari perintah delay(timeDelay)?
   Fungsi delay() berguna untuk menghentikan eksekusi program sementara selama jangka waktu tertentu dalam satuan milidetik (ms).
4. Modifikasi Alur: Mati $\rightarrow$ Lambat $\rightarrow$ Cepat $\rightarrow$ Sedang $\rightarrow$ Mati
   # README.md - Modifikasi Alur LED
## Logika Program
[cite_start]Program ini memodifikasi alur kedipan LED agar transisinya lebih halus (tidak langsung reset ke mati)[cite: 84, 86].

```cpp
const int ledPin = 6;      [cite_start]// Inisialisasi pin 6 untuk LED [cite: 88]
int timeDelay = 1000;      [cite_start]// Set nilai awal delay 1000ms [cite: 89]
bool semakinCepat = true;  [cite_start]// Flag untuk menentukan arah perubahan delay [cite: 90]

void setup() {
  pinMode(ledPin, OUTPUT); [cite_start]// Mengatur pin 6 sebagai output [cite: 92, 94]
}

void loop() {
  digitalWrite(ledPin, HIGH); [cite_start]// Menyalakan LED [cite: 96]
  delay(timeDelay);           [cite_start]// Jeda sesuai nilai variabel timeDelay [cite: 97]
  digitalWrite(ledPin, LOW);  [cite_start]// Mematikan LED [cite: 98]
  delay(timeDelay);           [cite_start]// Jeda sesuai nilai variabel timeDelay [cite: 99]

  if (semakinCepat) {
    timeDelay -= 200;         [cite_start]// Mengurangi delay agar kedipan semakin cepat [cite: 100, 101]
    if (timeDelay <= 200) { 
      semakinCepat = false;   // Jika sudah sangat cepat, ubah arah ke fase melambat [cite: 102, 103]
    }
  } else {
    timeDelay += 400;         // Menambah delay menuju fase "sedang" [cite: 106, 107]
    if (timeDelay >= 1000) {
      delay(3000);            // Jeda mati sebelum memulai siklus baru [cite: 107, 111]
      semakinCepat = true;    // Reset arah agar kembali menjadi cepat [cite: 112]
    }
  }
}
```
PERTANYAAN PERCOBAAN 2A
1. Gambarkan rangkaian schematic 5 LED running yang digunakan pada percobaan!
   gambar pada file gambarJawaban.jpg
2. Jelaskan bagaimana program membuat efek LED berjalan dari kiri ke kanan!
   Efek LED berjalan dari kiri ke kanan dihasilkan melalui struktur perulangan for pertama, yaitu for (int ledPin = 2; ledPin < 8; ledPin++)
	 Program akan menginisialisasi variabel ledPin mulai dari angka 2, menyalakan LED tersebut, memberikan jeda (delay), lalu mematikannya sebelum
	 melanjutkan ke angka berikutnya (inkremen) hingga mencapai pin 7.
3. Jelaskan bagaimana program membuat LED kembali dari kanan ke kiri!
	 Setelah loop pertama selesai, program mengeksekusi struktur perulangan for kedua: for (int ledPin = 7; ledPin >= 2; ledPin--)
	 Berbeda dengan sebelumnya, variabel ledPin dimulai dari angka tertinggi (pin 7) dan berkurang satu demi satu (dekremen) hingga kembali ke pin 2.
   Dengan memproses urutan pin secara terbalik, pola nyala LED terlihat bergerak kembali dari sisi kanan ke sisi kiri secara kontinu
4. Buatkan program agar LED menyala tiga LED kanan dan tiga LED kiri secara bergantian
dan berikan penjelasan disetiap baris kode nya dalam bentuk README.md!
# README.md - Modifikasi Pola Grup LED

## Logika Program
Program ini membagi 6 LED menjadi dua kelompok (Grup Kiri: Pin 2,3,4 dan Grup Kanan: Pin 5,6,7) 
[cite_start]yang menyala secara bergantian menggunakan perulangan `for` yang efisien[cite: 115, 125].

```cpp
void setup() {
  [cite_start]// Melakukan inisialisasi pin 2 sampai 7 sebagai OUTPUT dalam satu blok perulangan [cite: 121]
  for (int i = 2; i <= 7; i++) {
    pinMode(i, OUTPUT);
  }
}

void loop() {
  // FASE 1: Menyalakan 3 LED Kiri (Pin 2, 3, 4) secara bersamaan
  for (int i = 2; i <= 4; i++) digitalWrite(i, HIGH); 
  [cite_start]// Memastikan 3 LED Kanan (Pin 5, 6, 7) dalam kondisi mati [cite: 121]
  for (int i = 5; i <= 7; i++) digitalWrite(i, LOW);
  delay(1000); [cite_start]// Memberikan jeda waktu nyala selama 1 detik [cite: 79]

  // FASE 2: Menyalakan 3 LED Kanan (Pin 5, 6, 7) secara bersamaan
  for (int i = 5; i <= 7; i++) digitalWrite(i, HIGH);
  // Memastikan 3 LED Kiri (Pin 2, 3, 4) dalam kondisi mati
  for (int i = 2; i <= 4; i++) digitalWrite(i, LOW);
  delay(1000); [cite_start]// Memberikan jeda waktu nyala selama 1 detik [cite: 79]
}
