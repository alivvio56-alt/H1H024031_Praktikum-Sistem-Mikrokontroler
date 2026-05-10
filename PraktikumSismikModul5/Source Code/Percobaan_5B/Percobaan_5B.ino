#include <Arduino_FreeRTOS.h>
#include <queue.h>

// ================= STRUCT =================
struct readings {
  int temp;
  int h;
};

// ================= QUEUE =================
QueueHandle_t my_queue;

// ================= SETUP =================
void setup() {
  Serial.begin(9600);

  // Membuat queue
  my_queue = xQueueCreate(1, sizeof(struct readings));

  // Membuat task
  xTaskCreate(read_data, "read sensors", 128, NULL, 0, NULL);
  xTaskCreate(display, "display", 128, NULL, 0, NULL);
}

// ================= LOOP =================
void loop() {
  // Kosong karena RTOS menggunakan scheduler
}

// ================= TASK READ DATA =================
void read_data(void *pvParameters) {
  struct readings x;

  for (;;) {
    x.temp = 54;
    x.h = 30;

    xQueueSend(my_queue, &x, portMAX_DELAY);

    vTaskDelay(100);
  }
}

// ================= TASK DISPLAY =================
void display(void *pvParameters) {
  struct readings x;

  for (;;) {
    if (xQueueReceive(my_queue, &x, portMAX_DELAY) == pdPASS) {

      Serial.print("temp = ");
      Serial.println(x.temp);

      Serial.print("humidity = ");
      Serial.println(x.h);
    }
  }
}
