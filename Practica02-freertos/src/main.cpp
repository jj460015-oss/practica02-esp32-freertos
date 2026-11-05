#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include "configuracion.h"

#if ETAPA < 1 || ETAPA > 3 || EXPERIMENTO < 0 || EXPERIMENTO > 5
#error Configuracion fuera de rango
#endif
#if ETAPA != 3 && EXPERIMENTO != 0
#error Seleccione ETAPA 3 para realizar experimentos
#endif

constexpr int LED1 = 2;
constexpr int LED2 = 4;
QueueHandle_t cola = nullptr;

void detener(const char *mensaje) {
  Serial.println(mensaje);
  while (true) { vTaskDelay(pdMS_TO_TICKS(1000)); }
}
void comprobar(BaseType_t resultado) {
  if (resultado != pdPASS) detener("ERROR: no se pudo crear una tarea");
}

void tareaLed1(void *pv) {
  pinMode(LED1, OUTPUT);
#if ETAPA >= 2
  Serial.printf("Led1 en el nucleo %d\n", xPortGetCoreID());
#endif
  bool estado = false;
  for (;;) {
    estado = !estado;
    digitalWrite(LED1, estado);
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}
void tareaLed2(void *pv) {
  pinMode(LED2, OUTPUT);
#if ETAPA >= 2
  Serial.printf("Led2 en el nucleo %d\n", xPortGetCoreID());
#endif
  bool estado = false;
  for (;;) {
    estado = !estado;
    digitalWrite(LED2, estado);
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

#if ETAPA == 3
void productor(void *pv) {
  int contador = 0;
  for (;;) {
#if EXPERIMENTO == 4 || EXPERIMENTO == 5
    Serial.printf("Enviando %d\n", contador);
#endif
    if (xQueueSend(cola, &contador, portMAX_DELAY) != pdTRUE) detener("ERROR: envio a la cola");
#if EXPERIMENTO == 4 || EXPERIMENTO == 5
    Serial.printf("Enviado %d\n", contador);
#endif
    contador = (contador == INT32_MAX) ? 0 : contador + 1;
#if EXPERIMENTO == 3
    delay(1000);
#else
    vTaskDelay(pdMS_TO_TICKS(1000));
#endif
  }
}
void consumidor(void *pv) {
  int dato;
  for (;;) {
    if (xQueueReceive(cola, &dato, portMAX_DELAY) == pdTRUE) Serial.printf("Recibido: %d\n", dato);
  }
}
#endif

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.printf("Etapa %d; experimento %d\n", ETAPA, EXPERIMENTO);
#if ETAPA == 1
  comprobar(xTaskCreate(tareaLed1, "Led1", 2048, nullptr, 1, nullptr));
  comprobar(xTaskCreate(tareaLed2, "Led2", 2048, nullptr, 1, nullptr));
#else
  if (ESP.getChipCores() < 2) detener("ERROR: se requiere un ESP32 de dos nucleos");
  comprobar(xTaskCreatePinnedToCore(tareaLed1, "Led1", 2048, nullptr, 1, nullptr, 0));
  comprobar(xTaskCreatePinnedToCore(tareaLed2, "Led2", 2048, nullptr, 1, nullptr, 1));
#endif
#if ETAPA == 3
  cola = xQueueCreate(5, sizeof(int));
  if (cola == nullptr) detener("ERROR: no se pudo crear la cola");
  constexpr UBaseType_t prioridadProd = EXPERIMENTO == 1 ? 3 : 1;
  constexpr UBaseType_t prioridadCons = EXPERIMENTO == 5 ? 3 : 1;
  constexpr uint32_t pilaProd = EXPERIMENTO == 2 ? 256 : 2048;
#if EXPERIMENTO == 4 || EXPERIMENTO == 5
  comprobar(xTaskCreatePinnedToCore(consumidor, "Cons", 2048, nullptr, prioridadCons, nullptr, 1));
  comprobar(xTaskCreatePinnedToCore(productor, "Prod", pilaProd, nullptr, prioridadProd, nullptr, 1));
#else
  comprobar(xTaskCreate(productor, "Prod", pilaProd, nullptr, prioridadProd, nullptr));
  comprobar(xTaskCreate(consumidor, "Cons", 2048, nullptr, prioridadCons, nullptr));
#endif
#endif
}
void loop() { vTaskDelay(pdMS_TO_TICKS(1000)); }
