# Praktikum FreeRTOS - Arduino

## Pertanyaan 5A

### 1. Apakah ketiga task berjalan secara bersamaan atau bergantian?

Secara fisik, pada mikrokontroler dengan satu core seperti Arduino Uno (ATMega328P), ketiga task berjalan secara **bergantian** namun dalam waktu yang sangat cepat sehingga terlihat seolah-olah bersamaan (*pseudo-parallelism*).

**Mekanisme:**

- **Context Switching** - FreeRTOS menggunakan Kernel Scheduler untuk membagi waktu CPU ke setiap task. Scheduler menghentikan satu task, menyimpan statusnya, lalu menjalankan task lain.
- **Time Slicing** - Setiap task mendapat jatah waktu (tick). Jika suatu task memanggil `vTaskDelay()`, task tersebut masuk ke kondisi *Blocked* dan memberikan kesempatan bagi task lain dengan prioritas sama atau lebih tinggi untuk berjalan.
- **Preemptive Scheduling** - Task dengan prioritas lebih tinggi dapat menyela (*preempt*) task dengan prioritas lebih rendah agar kebutuhan *real-time* terpenuhi.

---

### 2. Bagaimana cara menambahkan task keempat?

**Langkah-langkah:**

1. **Definisikan fungsi task** - Buat fungsi baru dengan struktur berikut:
   ```cpp
   void NamaTask(void *pvParameters) {
     for (;;) {
       // logika task
     }
   }
   ```

2. **Daftarkan di `setup()`** - Tambahkan `xTaskCreate()` di dalam `void setup()`:
   ```cpp
   xTaskCreate(TaskKeempat, "Task4", 128, NULL, 1, NULL);
   ```

3. **Tentukan parameter** - Atur nama fungsi, nama teks task, ukuran stack (contoh: 128 words), parameter (`NULL`), prioritas (contoh: `1`), dan handle task (`NULL`).

4. **Implementasi logika** - Tulis kode instruksi di dalam fungsi tersebut dan pastikan menyertakan `vTaskDelay()` agar tidak memonopoli CPU.

---

### 3. Modifikasi Program dengan Potensiometer

Program ini mendemonstrasikan penggunaan dua task FreeRTOS: satu task membaca nilai potensiometer dan satu task mengontrol kedipan LED berdasarkan nilai tersebut.

```cpp
#include <Arduino_FreeRTOS.h>

// ===== DEKLARASI TASK =====
void TaskBlink(void *pvParameters);
void TaskSensor(void *pvParameters);

// ===== VARIABEL GLOBAL =====
int delayLed = 200;

void setup() {
  Serial.begin(9600);
  pinMode(8, OUTPUT);

  xTaskCreate(TaskSensor, "Sensor", 128, NULL, 1, NULL);
  xTaskCreate(TaskBlink,  "Blink",  128, NULL, 1, NULL);

  vTaskStartScheduler();
}

void loop() {
  // Kosong - RTOS menggunakan scheduler
}

void TaskSensor(void *pvParameters) {
  while (1) {
    int sensorValue = analogRead(A0);
    delayLed = map(sensorValue, 0, 1023, 50, 1000);

    Serial.print("Nilai Potensio = ");
    Serial.print(sensorValue);
    Serial.print(" | Delay LED = ");
    Serial.println(delayLed);

    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void TaskBlink(void *pvParameters) {
  while (1) {
    digitalWrite(8, HIGH);
    vTaskDelay(delayLed / portTICK_PERIOD_MS);
    digitalWrite(8, LOW);
    vTaskDelay(delayLed / portTICK_PERIOD_MS);
  }
}
```

**Penjelasan Program:**

| Komponen | Keterangan |
|---|---|
| Input Hardware | Potensiometer dihubungkan ke pin `A0` |
| Variabel Global | Nilai ADC disimpan di `delayLed` dan diakses oleh task LED |
| `TaskSensor` | Membaca `analogRead(A0)` secara berkala dan memperbarui `delayLed` |
| `TaskBlink` | Menggunakan nilai `delayLed` sebagai argumen `vTaskDelay()` |

**Hasil Analisis:** Saat tuas potensiometer diputar, nilai analog (0-1023) dibaca oleh `TaskSensor` dan dikonversi ke rentang delay 50-1000 ms. Hasilnya, kecepatan kedipan LED berubah secara dinamis sesuai posisi potensiometer.

**Contoh Output Serial Monitor:**

```
Nilai Potensio = 512 | Delay LED = 525
Nilai Potensio = 300 | Delay LED = 343
```

**Hasil Percobaan:**

- Nilai delay LED berubah secara dinamis sesuai posisi potensiometer
- FreeRTOS dapat menjalankan task pembacaan sensor dan task kontrol LED secara *concurrent*
- Tidak terjadi konflik akses data karena hanya satu task yang menulis ke variabel global `delayLed`

---

## Pertanyaan 5B

### 1. Apakah kedua task berjalan bersamaan?

Kedua task berjalan secara **concurrent** (bergantian sangat cepat), bukan benar-benar bersamaan, karena Arduino Uno hanya memiliki satu inti prosesor. FreeRTOS menggunakan mekanisme scheduler untuk mengatur pergantian eksekusi task dengan *time slicing*.

- Task `read_data` mengirim data ke Queue menggunakan `xQueueSend()`
- Task `display` menerima data menggunakan `xQueueReceive()`
- Ketika suatu task sedang *blocked* (menunggu `vTaskDelay()` atau Queue), scheduler menjalankan task lain yang siap dieksekusi

### 2. Apakah program berpotensi mengalami race condition?

Program ini **tidak berpotensi** mengalami race condition karena komunikasi data antar task dilakukan melalui Queue. Queue pada FreeRTOS dirancang agar akses data berlangsung aman dan terstruktur.

- `read_data` hanya **menulis** ke Queue
- `display` hanya **membaca** dari Queue
- Keduanya tidak mengakses variabel global yang sama secara langsung

> Race condition terjadi jika dua task mengakses dan mengubah data yang sama secara bersamaan tanpa mekanisme sinkronisasi seperti Queue, Semaphore, atau Mutex.

---

### Program: Pembacaan Sensor DHT22 dengan Queue

```cpp
#include <Arduino_FreeRTOS.h>
#include <queue.h>
#include <DHT.h>

// ===== DHT SETUP =====
#define DHTPIN  2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// ===== STRUCT =====
struct readings {
  float temp;
  float h;
};

// ===== QUEUE =====
QueueHandle_t my_queue;

void setup() {
  Serial.begin(9600);
  dht.begin();

  my_queue = xQueueCreate(1, sizeof(struct readings));

  xTaskCreate(read_data, "read sensors", 128, NULL, 1, NULL);
  xTaskCreate(display,   "display",      128, NULL, 1, NULL);
}

void loop() {
  // Kosong - RTOS menggunakan scheduler
}

void read_data(void *pvParameters) {
  struct readings x;
  for (;;) {
    x.temp = dht.readTemperature();
    x.h    = dht.readHumidity();
    xQueueSend(my_queue, &x, portMAX_DELAY);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void display(void *pvParameters) {
  struct readings x;
  for (;;) {
    if (xQueueReceive(my_queue, &x, portMAX_DELAY) == pdPASS) {
      Serial.print("Temperature = ");
      Serial.print(x.temp);
      Serial.println(" C");
      Serial.print("Humidity = ");
      Serial.print(x.h);
      Serial.println(" %");
    }
  }
}
```

**Contoh Output Serial Monitor:**

```
Temperature = 29.40 C
Humidity = 71.20 %

Temperature = 29.50 C
Humidity = 70.90 %
```

**Hasil Percobaan:**

- Nilai suhu dan kelembapan berubah secara dinamis sesuai kondisi lingkungan
- Data berhasil dikirim antar task menggunakan Queue
- FreeRTOS dapat menjalankan task pembacaan sensor dan task tampilan data secara *concurrent* tanpa konflik
