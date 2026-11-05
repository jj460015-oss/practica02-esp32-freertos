#include <Arduino.h>
#include <WiFi.h>

void setup() {
  Serial.begin(115200);
  delay(1000);
  WiFi.mode(WIFI_STA);
  Serial.println("=== Informacion del ESP32 ===");
  Serial.printf("Modelo de chip: %s (rev %d)\n", ESP.getChipModel(), ESP.getChipRevision());
  Serial.printf("Nucleos: %d\n", ESP.getChipCores());
  Serial.printf("Frecuencia CPU: %d MHz\n", ESP.getCpuFreqMHz());
  Serial.printf("Flash: %u KB\n", ESP.getFlashChipSize() / 1024);
  Serial.printf("Heap libre: %u bytes\n", ESP.getFreeHeap());
  Serial.printf("MAC: %s\n", WiFi.macAddress().c_str());
}

void loop() { delay(1000); }
