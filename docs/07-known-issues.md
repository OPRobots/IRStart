# Problemas Conocidos

> **Fecha de análisis**: 2025-06-25
> **Última actualización**: 2025-06-25
> **Commit**: `e274009`
> **Total**: 10 issues — 3 críticos, 4 moderados, 3 leves

---

## Correcciones Recientes ✅

| ID | Fecha | Descripción |
|----|-------|-------------|
| — | — | — |

---

## 🔴 Críticos

#### SW-01 — `delayMicroseconds(100)` en contexto de ISR

- **Archivo**: [`rc5.cpp:102`](../source_code/Module/src/rc5.cpp#L102)
- **Descripción**: La función `rc5_register()` contiene una llamada a
  `delayMicroseconds(100)` en el flujo de la ISR `rc5_isr()`. En un ATtiny
  a 1.2 MHz, esto bloquea las interrupciones durante 100 µs, equivalente a
  ~120 ciclos de CPU desperdiciados. Cualquier otra interrupción (incluyendo
  más flancos del receptor IR) se pierde durante este tiempo.
- **Impacto**: **Alto** — Puede causar pérdida de pulsos IR y decodificación
  incorrecta de tramas RC5, especialmente en entornos con ruido IR.
- **Mitigación actual**: Ninguna. La llamada está incondicionalmente en la
  ruta de la ISR.
- **Solución propuesta**: Eliminar el `delayMicroseconds(100)` o mover la
  lógica de decodificación a un buffer procesado en `loop()`.

#### SW-02 — `EEPROM.write()` en contexto de ISR

- **Archivo**: [`rc5.cpp:30-31`](../source_code/Module/src/rc5.cpp#L30-L31)
- **Descripción**: `rc5_manage_command()` (llamada desde la ISR
  `rc5_isr()` → `rc5_register()`) ejecuta `EEPROM.write()` cuando recibe un
  comando de programación (ADDRESS_PROG). La escritura en EEPROM del ATtiny
  tarda ~3.3 ms y **deshabilita todas las interrupciones** durante la
  operación. Además, `module_feedback()` añade ~2 s adicionales de delays
  dentro del contexto de ISR.
- **Impacto**: **Alto** — El sistema queda completamente ciego durante la
  programación (~2 s). Si llegan comandos IR durante este tiempo, se pierden.
- **Mitigación actual**: Ninguna. La programación ocurre en línea dentro de la ISR.
- **Solución propuesta**: Marcar un flag en la ISR (`volatile bool
  pending_program`) y procesar la escritura en EEPROM + feedback en `loop()`.

#### SW-03 — Variable `started` sin calificador `volatile`

- **Archivo**: [`control.cpp:3`](../source_code/Module/src/control.cpp#L3)
- **Descripción**: La variable `static bool started` es modificada por
  `module_start()` y `module_stop()`, que se llaman desde la ISR a través de
  `rc5_manage_command()`. También es leída por `module_is_started()` desde
  `loop()`. Sin `volatile`, el compilador puede optimizar la lectura en
  `loop()` cacheando el valor en un registro y no detectando cambios.
- **Impacto**: **Alto** — El módulo podría no reflejar cambios de estado
  start/stop correctamente si el compilador optimiza el acceso. En AVR con
  optimización baja (-Os) el bug puede no manifestarse, pero es frágil.
- **Mitigación actual**: Ninguna. Depende del nivel de optimización del compilador.
- **Solución propuesta**: Declarar `static volatile bool started = false;`.

---

## 🟡 Moderados

#### SW-04 — Gap de histéresis insuficiente en detección de pulsos RC5

- **Archivo**: [`rc5.h:21-24`](../source_code/Module/include/rc5.h#L21-L24)
- **Descripción**: Las constantes de detección de pulsos RC5 tienen solo 1 µs
  de separación entre corto y largo: `MAX_SHORT = 1333`, `MIN_LONG = 1334`.
  El estándar RC5 usa 889 µs para medio bit corto y 1778 µs para largo. Con
  tolerancia de reloj del RC interno del ATtiny (±10% a 1.2 MHz), un pulso
  nominal de 889 µs podría medirse como 978 µs (dentro del rango corto) o
  1334 µs (clasificado incorrectamente como largo).
- **Impacto**: **Medio** — Puede causar errores de decodificación con
  variaciones de temperatura o voltaje que afecten al oscilador interno.
- **Mitigación actual**: Ninguna.
- **Solución propuesta**: Ajustar los umbrales con un gap de al menos 100-200 µs:
  `MAX_SHORT = 1200`, `MIN_LONG = 1400`, o usar el punto medio entre 889 y
  1778 (1333 µs) como umbral único.

#### HW-01 — `BAT_ANALOG` definido pero sin implementar

- **Archivo**: [`pinout.h:6`](../source_code/Remote/include/pinout.h#L6)
- **Descripción**: El pin GPIO 2 está definido como `BAT_ANALOG` para lectura
  del voltaje de batería, pero no existe ningún código que lea el ADC ni
  muestre el estado de la batería. Además, GPIO 2 se usa simultáneamente como
  `DIP_SW_2`, creando un conflicto de funciones si se implementara la lectura.
- **Impacto**: **Medio** — La funcionalidad de monitorización de batería está
  ausente. El usuario no recibe aviso de batería baja. El conflicto de pines
  requeriría un rediseño del PCB o usar un pin diferente para el ADC.
- **Mitigación actual**: Ninguna.
- **Solución propuesta**: Elegir un pin libre para BAT_ANALOG (ej. GPIO 3,
  actualmente DIP_SW_3, o reasignar DIP switches) e implementar la lectura
  periódica del ADC con un divisor de tensión.

#### PR-01 — Include circular en `ir.h`

- **Archivo**: [`ir.h:4`](../source_code/Remote/include/ir.h#L4)
- **Descripción**: El archivo `ir.h` contiene `#include <ir.h>` — se incluye
  a sí mismo. Aunque los include guards (`#ifndef IR_H`) evitan la recursión
  infinita, esto es un error que indica posiblemente una intención de incluir
  otro archivo (quizás `io.h` o `config.h`).
- **Impacto**: **Medio** — No causa fallos funcionales gracias a los include
  guards, pero es un code smell y podría confundir a desarrolladores o
  herramientas de análisis estático.
- **Mitigación actual**: Los include guards evitan el fallo.
- **Solución propuesta**: Eliminar `#include <ir.h>` de `ir.h`.

#### SW-05 — Bucles busy-wait bloquean actualización de LEDs

- **Archivo**: [`main.cpp:34-35`](../source_code/Remote/src/main.cpp#L34-L35)
- **Descripción**: En `loop()`, tras enviar un comando IR, hay bucles
  `while (get_btn_ready()) {}` que esperan a que el usuario suelte el botón.
  Durante esta espera, el código de actualización de NeoPixels al final del
  loop no se ejecuta, por lo que los LEDs quedan congelados.
- **Impacto**: **Medio** — Mala experiencia de usuario (LEDs congelados al
  mantener pulsado un botón). No es crítico para la funcionalidad IR.
- **Mitigación actual**: Ninguna.
- **Solución propuesta**: Usar una máquina de estados con `static bool
  button_was_pressed` y `millis()` para anti-rebote sin bloquear el loop.
  Así los NeoPixels se actualizan incluso con botones pulsados.

---

## 🟢 Leves

#### MN-01 — Comando de programación inconsistente para `id=0`

- **Archivo**: [`rc5.cpp:87-91`](../source_code/Remote/src/rc5.cpp#L87-L91)
- **Descripción**: `rc5_send_prog()` envía el comando `0x01` cuando `id == 0`,
  pero para `id != 0` envía `id << 1`. Con `id = 0`, `id << 1 = 0`, así que
  los patrones no coinciden. El receptor interpreta `command` como STOP y
  `command + 1` como START. Con `id = 0`, `command = 0x01`, por lo que
  START = 0x02 y STOP = 0x01. Esto funciona pero es un caso especial
  inconsistente con el resto de IDs.
- **Impacto**: **Bajo** — Funciona correctamente pero añade complejidad
  innecesaria y dificulta el mantenimiento.
- **Mitigación actual**: Ninguna necesaria.
- **Solución propuesta**: Unificar la lógica para que `id = 0` siga el mismo
  patrón `id << 1`, o documentar explícitamente que `id = 0` es un caso
  especial.

#### PR-02 — Inconsistencia en lógica de repetición NEC vs SIRC

- **Archivo**: [`nec.cpp:53-68`](../source_code/Remote/src/nec.cpp#L53-L68)
  vs [`sirc.cpp:46-68`](../source_code/Remote/src/sirc.cpp#L46-L68)
- **Descripción**: NEC implementa un sistema de repetición de comando: si el
  mismo comando se reenvía dentro de la ventana `CMD_WAIT_MS + 50`, se usa
  `send_repeat()` (código de repetición NEC). SIRC no tiene esta lógica y
  reenvía la trama completa cada `CMD_WAIT_MS` (45 ms). La diferencia refleja
  las especificaciones de cada protocolo, pero el código de repetición de NEC
  es más complejo sin documentar el razonamiento.
- **Impacto**: **Bajo** — Ambos protocolos funcionan. La complejidad adicional
  en NEC podría ocultar bugs sutiles en el timing.
- **Mitigación actual**: Ninguna necesaria.
- **Solución propuesta**: Documentar la lógica de repetición en NEC o
  simplificarla.

#### DB-01 — Código comentado sin limpiar

- **Archivos**: [`main.cpp:23-25`](../source_code/Module/src/main.cpp#L23-L25),
  [`rc5.cpp:90-96`](../source_code/Module/src/rc5.cpp#L90-L96),
  [`rc5.cpp:115-121`](../source_code/Module/src/rc5.cpp#L115-L121)
- **Descripción**: Hay bloques de código comentado que implementan
  funcionalidad de reset de EEPROM y un método alternativo de lectura RC5
  por polling (`rc5_update()`). El código comentado sugiere funcionalidad
  planificada pero no implementada, lo que puede confundir.
- **Impacto**: **Bajo** — No afecta al funcionamiento.
- **Mitigación actual**: Ninguna.
- **Solución propuesta**: Eliminar el código comentado o implementar las
  funcionalidades pendientes (reset de EEPROM con protección).

---

*Documento generado el 2025-06-25. Issues detectados mediante auditoría manual del código fuente durante `doc-init`. Para actualizar tras cambios de código, usar `/doc-review`.*
