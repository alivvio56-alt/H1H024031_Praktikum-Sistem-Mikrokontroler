# Percobaan 6A — External Interrupt

## 1. Jelaskan proses bagaimana tombol dapat mengubah kondisi LED menggunakan interrupt!

Saat tombol ditekan, pin interrupt pada Arduino mendeteksi perubahan sinyal dari HIGH menjadi LOW karena menggunakan mode `FALLING`. Interrupt kemudian menghentikan sementara program utama dan menjalankan ISR (*Interrupt Service Routine*). Pada ISR, variabel `ledState` diubah sehingga kondisi LED berubah menjadi ON atau OFF. Setelah ISR selesai, program kembali menjalankan loop utama.

## 2. Apa fungsi `attachInterrupt()` pada program tersebut?

Fungsi `attachInterrupt()` digunakan untuk menghubungkan pin interrupt dengan ISR yang akan dijalankan ketika terjadi kondisi tertentu. Fungsi ini juga menentukan mode interrupt seperti `RISING`, `FALLING`, `CHANGE`, atau `LOW`.

Contoh:

```cpp
attachInterrupt(digitalPinToInterrupt(2), tombolInterrupt, FALLING);
```


## 3. Mengapa pada ISR tidak disarankan menggunakan `delay()` dan `Serial.print()`?

Karena selama ISR berjalan, program utama akan berhenti sementara. Jika menggunakan `delay()` atau `Serial.print()`, ISR menjadi lebih lama sehingga sistem dapat menjadi lambat, tidak responsif, atau menyebabkan interrupt lain terlewat.


## 4. Apa fungsi keyword `volatile` pada variabel `ledState`?

Keyword `volatile` digunakan agar compiler selalu membaca nilai terbaru dari variabel karena nilainya dapat berubah sewaktu-waktu di dalam ISR. Tanpa `volatile`, compiler dapat menyimpan nilai lama sehingga program tidak bekerja dengan benar.


## 5. Modifikasi Program Menggunakan Mode Interrupt Lain

### a. Mode RISING

**Source Code**

```cpp
#include <Arduino.h>

volatile bool ledState = false;

void tombolInterrupt() {
  ledState = !ledState;
}

void setup() {
  pinMode(13, OUTPUT);
  pinMode(2, INPUT_PULLUP);

  attachInterrupt(
    digitalPinToInterrupt(2),
    tombolInterrupt,
    RISING
  );
}

void loop() {
  digitalWrite(13, ledState);
}
```

**Analisis**

Mode `RISING` aktif saat sinyal berubah dari LOW ke HIGH. Pada tombol dengan `INPUT_PULLUP`, interrupt biasanya terjadi saat tombol dilepas sehingga LED berubah kondisi ketika tombol dilepas.


### b. Mode CHANGE

**Source Code**

```cpp
#include <Arduino.h>

volatile bool ledState = false;

void tombolInterrupt() {
  ledState = !ledState;
}

void setup() {
  pinMode(13, OUTPUT);
  pinMode(2, INPUT_PULLUP);

  attachInterrupt(
    digitalPinToInterrupt(2),
    tombolInterrupt,
    CHANGE
  );
}

void loop() {
  digitalWrite(13, ledState);
}
```

**Analisis**

Mode `CHANGE` aktif setiap terjadi perubahan sinyal HIGH ke LOW maupun LOW ke HIGH. Akibatnya LED dapat berubah dua kali saat tombol ditekan dan dilepas sehingga LED terlihat lebih sensitif.


### c. Mode LOW

**Source Code**

```cpp
#include <Arduino.h>

volatile bool ledState = false;

void tombolInterrupt() {
  ledState = !ledState;
}

void setup() {
  pinMode(13, OUTPUT);
  pinMode(2, INPUT_PULLUP);

  attachInterrupt(
    digitalPinToInterrupt(2),
    tombolInterrupt,
    LOW
  );
}

void loop() {
  digitalWrite(13, ledState);
}
```

**Analisis**

Mode `LOW` akan terus memanggil ISR selama pin berada pada kondisi LOW. Akibatnya LED dapat berkedip sangat cepat atau berubah terus selama tombol ditekan.


# Percobaan 6B — Timer Menggunakan millis()

## 1. Jelaskan bagaimana fungsi `millis()` bekerja pada program tersebut!

Fungsi `millis()` menghitung waktu sejak Arduino dinyalakan dalam satuan milidetik. Program membandingkan waktu sekarang dengan waktu sebelumnya untuk menentukan kapan LED harus berubah kondisi.


## 2. Apa perbedaan utama antara `delay()` dan `millis()`?

`delay()` menghentikan seluruh program selama waktu tertentu, sedangkan `millis()` memungkinkan program tetap berjalan sambil menghitung waktu sehingga lebih efisien.


## 3. Mengapa metode `millis()` disebut non-blocking?

Karena `millis()` tidak menghentikan jalannya program utama. Arduino tetap dapat menjalankan proses lain sambil menghitung interval waktu.


## 4. Modifikasi Program Dua LED Tanpa delay()

**Source Code**

```cpp
#include <Arduino.h>

unsigned long previousMillis1 = 0;
unsigned long previousMillis2 = 0;

const long interval1 = 1000;
const long interval2 = 500;

bool ledState1 = false;
bool ledState2 = false;

void setup() {
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
}

void loop() {

  unsigned long currentMillis = millis();

  // LED 1 setiap 1 detik
  if(currentMillis - previousMillis1 >= interval1) {
    previousMillis1 = currentMillis;
    ledState1 = !ledState1;
    digitalWrite(13, ledState1);
  }

  // LED 2 setiap 500 ms
  if(currentMillis - previousMillis2 >= interval2) {
    previousMillis2 = currentMillis;
    ledState2 = !ledState2;
    digitalWrite(12, ledState2);
  }
}
```

**Analisis**

Program menggunakan dua timer berbeda dengan `millis()` sehingga kedua LED dapat berkedip dengan interval berbeda tanpa menggunakan `delay()`. LED pertama berkedip setiap 1 detik dan LED kedua setiap 500 ms.
