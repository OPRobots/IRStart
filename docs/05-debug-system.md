# Sistema de Debug

El sistema IRStart ofrece capacidades de debug mediante salida serie en el
mando y patrones de LEDs en ambos dispositivos.

---

## Salida Serie (Remote)

| Parámetro | Valor |
|-----------|-------|
| **Interfaz** | USB CDC Serial |
| **Baudrate** | 115200 baud |
| **Función** | `Serial` (Arduino) |

### Función `debug_all()`

[`debug.cpp:3-33`](../source_code/Remote/src/debug.cpp)

La función `debug_all()` imprime el estado de todas las entradas digitales.
Está definida pero **no se llama desde `loop()`** — requiere descomentar la
llamada en [`main.cpp:29`](../source_code/Remote/src/main.cpp#L29) para
activarla:

```cpp
// debug_all();  // Descomentar para activar
```

**Formato de salida:**

```
BTN_START: 0 | BTN_STOP: 0 | BTN_READY: 0 | DIP_SW0: 0 | DIP_SW1: 0 | DIP_SW2: 0 | DIP_SW3: 0
```

| Campo | Descripción |
|-------|-------------|
| `BTN_START` | Estado del botón Start (GPIO 21) |
| `BTN_STOP` | Estado del botón Stop (GPIO 20) |
| `BTN_READY` | Estado del botón Ready (GPIO 10) |
| `DIP_SW0` | DIP switch posición 0 (GPIO 0) |
| `DIP_SW1` | DIP switch posición 1 (GPIO 1) |
| `DIP_SW2` | DIP switch posición 2 (GPIO 2) |
| `DIP_SW3` | DIP switch posición 3 (GPIO 3) |

> **Nota**: `debug_all()` también activa momentáneamente el LED onboard y el
> PWM del IR_CMD a 1/3 de duty cycle durante cada iteración.

## LEDs de Información (Remote)

| LED | Color/Patrón | Significado |
|-----|-------------|-------------|
| **LED onboard** | Fijo | Mando encendido (se enciende en `init_components()`) |
| **LED onboard** | Parpadeo en boot | Esperando liberación de botones |
| **LED onboard** | Parpadeo rápido ×3 | Error de protocolo (`error_led()`) |
| **NeoPixel 0** | Verde | Modo IRSTART — Ready activo |
| **NeoPixel 0** | Apagado | Modo IRMENU |
| **NeoPixel 1** | Rojo | Modo IRSTART — Start activo |
| **NeoPixel 1** | Apagado | Modo IRMENU |
| **NeoPixel 2** | Magenta | Modo IRSTART — Stop activo |
| **NeoPixel 2** | Azul | Modo IRMENU |

### Patrón `error_led()`

[`io.cpp:25-34`](../source_code/Remote/src/io.cpp)

Secuencia de 3 pulsos cortos (50 ms ON/OFF) seguidos de pausa de 250 ms.
Se activa al usar un protocolo o comando no implementado en el modo actual.

## LEDs de Información (Module)

| LED | Patrón | Significado |
|-----|--------|-------------|
| **LED azul** | Fijo | Módulo started (carrera en curso) |
| **LED azul** | Parpadeo alternante | Módulo stopped (en espera) |
| **LED azul** | 5 parpadeos acelerados | Feedback de programación exitosa |

### Parpadeo en Espera

[`main.cpp:14-26`](../source_code/Module/src/main.cpp)

En estado stopped, el LED alterna con duty cycle asimétrico: ~100ms ON,
~1600ms OFF. Esto proporciona información visual de que el módulo está
alimentado y en espera sin consumir corriente excesiva.

### Feedback de Programación

[`control.cpp:21-29`](../source_code/Module/src/control.cpp)

```cpp
void module_feedback() {
    for (uint8_t i = 0; i < 5; i++) {
        digitalWrite(PIN_LED, HIGH);
        delay(80 - (15 * i));   // 80, 65, 50, 35, 20 ms
        digitalWrite(PIN_LED, LOW);
        delay(80 - (15 * i));
    }
    delay(1500);
}
```

Cinco parpadeos de frecuencia creciente (de 80 ms a 20 ms), seguidos de
una pausa de 1.5 segundos. Se ejecuta en el contexto de la ISR al recibir
un comando de programación válido.

> **⚠️ Advertencia**: Esta función se ejecuta desde la ISR del receptor IR a
> través de `rc5_manage_command()`. Contiene múltiples `delay()` que bloquean
> las interrupciones durante ~2 segundos. Ver [SW-02](07-known-issues.md#sw-02).

---

*Documento generado el 2025-06-25. Ver también [Arquitectura Software](02-software-architecture.md), [Menú](04-menu-system.md), [Problemas Conocidos](07-known-issues.md).*
