# Práctica 02 ESP32 y FreeRTOS

Proyectos preparados conforme a la guía de Programación y Manejo de Microcontroladores.

## Estado

Código preparado. Las mediciones, fotos y observaciones físicas están pendientes: todavía no se dispone de placa. Los commits y la compilación no acreditan ejecución en hardware.

## Cómo realizar la práctica

1. Instalar VS Code, PlatformIO IDE y Git. Clonar este repositorio.
2. Abrir Practica02-info-chip. Se usa ESP32 Dev Module (esp32dev), Arduino, espressif32@6.9.0 y monitor a 115200 baudios.
3. Conectar un ESP32 de dos núcleos mediante USB de datos. Pulsar Build, comprobar SUCCESS y pulsar Upload. Si queda en Connecting, mantener BOOT hasta iniciar la carga.
4. Abrir el monitor y pulsar EN. Capturar modelo, núcleos, frecuencia, flash, heap y MAC. Comparar con los datasheets del chip y módulo reales.
5. Con la placa desconectada: GPIO4 -> resistencia 220 ohm -> ánodo del LED externo; cátodo -> GND. El otro LED es el integrado en GPIO2, si la placa lo incluye.
6. Abrir Practica02-freertos, editar include/configuracion.h según la tabla, compilar y cargar cada cambio.

| ETAPA | EXPERIMENTO | Actividad |
|---|---|---|
| 1 | 0 | Dos tareas independientes, conmutaciones cada 500 y 1000 ms |
| 2 | 0 | LED1 en núcleo 0 y LED2 en núcleo 1 |
| 3 | 0 | Cola de cinco enteros; versión final |
| 3 | 1 | Productor con prioridad 3 |
| 3 | 2 | Pila de 256 bytes; puede reiniciarse |
| 3 | 3 | delay(1000) en lugar de vTaskDelay |
| 3 | 4 | Ambos en núcleo 1, prioridades 1/1 |
| 3 | 5 | Ambos en núcleo 1, consumidor con prioridad 3 |

Ejecutar un experimento a la vez y volver a ETAPA 3 y EXPERIMENTO 0 antes de entregar.

## Precisiones

- En Arduino ESP32, delay() llama a vTaskDelay(): ambos permiten ejecutar otras tareas.
- 500 y 1000 ms son tiempos entre cambios; los ciclos completos duran aproximadamente 1 y 2 segundos.
- Heap libre y SRAM total no son equivalentes. La flash debe verificarse para el módulo real.
- La prioridad del productor no garantiza cambiar el intervalo de un segundo; el orden de mensajes con igual prioridad puede variar.
- Se corrige la duplicación de loop() de la guía y se conservan las carpetas Practica02 aunque la guía también menciona Práctica 03.

## Evidencias e informe

Guardar capturas del chip, tabla del datasheet con secciones, foto del montaje, mensajes de núcleos, cola y cada experimento. Completar las 12 preguntas y conclusiones en el formato de informe, incorporar nombre y fechas y entregar PDF. No subir .pio.

## Verificación

GitHub Actions compila el chip y ocho configuraciones FreeRTOS: etapas 1 y 2, y etapa 3 con experimentos 0 a 5. Consultar Actions. Se verifica compilación, no funcionamiento físico.

## Referencias

- Guía y formato de la docente.
- https://documentation.espressif.com/esp32_datasheet_en.pdf
- https://docs.espressif.com/projects/esp-idf/en/v4.3.3/esp32/api-reference/system/freertos.html
- https://github.com/espressif/arduino-esp32/blob/2.0.17/cores/esp32/esp32-hal-misc.c
