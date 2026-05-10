#include <Arduino_FreeRTOS.h>
#include <queue.h>
#include <DHT.h>

// ================= DHT SETUP =================
#define DHTPIN 2
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

// ================= STRUCT =================
struct readings {
  float temp;
  float h;
};

// ================= QUEUE =================
QueueHandle_t my_queue;

// ================= SETUP =================
void setup() {

  Serial.begin(9600);

  dht.begin();

  // Membuat queue
  my_queue = xQueueCreate(1, sizeof(struct readings));

  // Membuat task
  xTaskCreate(read_data, "read sensors", 128, NULL, 1, NULL);
  xTaskCreate(display, "display", 128, NULL, 1, NULL);
}

// ================= LOOP =================
void loop() {
  // Kosong karena RTOS menggunakan scheduler
}

// ================= TASK READ DATA =================
void read_data(void *pvParameters) {

  struct readings x;

  for (;;) {

    // Membaca data dari sensor DHT22
    x.temp = dht.readTemperature();
    x.h = dht.readHumidity();

    // Mengirim data ke queue
    xQueueSend(my_queue, &x, portMAX_DELAY);

    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

// ================= TASK DISPLAY =================
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
