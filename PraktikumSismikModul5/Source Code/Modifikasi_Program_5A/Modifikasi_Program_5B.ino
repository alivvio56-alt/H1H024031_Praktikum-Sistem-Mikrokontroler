#include <Arduino_FreeRTOS.h>

// ================= DEKLARASI TASK =================
void TaskBlink(void *pvParameters);
void TaskSensor(void *pvParameters);

// ================= VARIABEL GLOBAL =================
int delayLed = 200;

// ================= SETUP =================
void setup() {

  Serial.begin(9600);

  pinMode(8, OUTPUT);

  // Task membaca sensor
  xTaskCreate(
    TaskSensor,
    "Sensor",
    128,
    NULL,
    1,
    NULL
  );

  // Task blink LED
  xTaskCreate(
    TaskBlink,
    "Blink",
    128,
    NULL,
    1,
    NULL
  );

  // Menjalankan scheduler
  vTaskStartScheduler();
}

// ================= LOOP =================
void loop() {
  // Kosong karena RTOS menggunakan scheduler
}

// ================= TASK SENSOR =================
void TaskSensor(void *pvParameters) {

  while (1) {

    // Membaca nilai potensiometer
    int sensorValue = analogRead(A0);

    // Mengubah nilai menjadi delay LED
    delayLed = map(sensorValue, 0, 1023, 50, 1000);

    Serial.print("Nilai Potensio = ");
    Serial.print(sensorValue);

    Serial.print(" | Delay LED = ");
    Serial.println(delayLed);

    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

// ================= TASK BLINK =================
void TaskBlink(void *pvParameters) {

  while (1) {

    digitalWrite(8, HIGH);
    vTaskDelay(delayLed / portTICK_PERIOD_MS);

    digitalWrite(8, LOW);
    vTaskDelay(delayLed / portTICK_PERIOD_MS);
  }
}
