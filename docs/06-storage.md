# EEPROM

El sistema IRStart utiliza la EEPROM interna del microcontrolador ATtiny en el
módulo para almacenar de forma persistente los comandos RC5 programados durante
la fase de pairing.

---

## Medio de Almacenamiento

| Característica | Detalle |
|---------------|---------|
| **Tipo** | EEPROM interna del ATtiny |
| **Capacidad** | 64 B (ATtiny13) / 512 B (ATtiny85) |
| **Tecnología** | EEPROM on-chip AVR |
| **Librería** | `EEPROM.h` (Arduino) |

## Layout de Datos

| Offset | Tamaño | Campo | Descripción |
|--------|--------|-------|-------------|
| `0x00` | 1 B | `EEPROM_ADDR_RC5_START` | Comando START programado |
| `0x10` | 1 B | `EEPROM_ADDR_RC5_STOP` | Comando STOP programado |

> **Nota**: Las direcciones están separadas por 16 bytes (`EEPROM_ADDR_RC5_START = 0`,
> `EEPROM_ADDR_RC5_STOP = 16`), aunque cada valor ocupa solo 1 byte. Esto deja
> espacio para expansión futura de datos de configuración.

## Operaciones

### Inicialización

[`rc5.cpp:85-88`](../source_code/Module/src/rc5.cpp)

```cpp
void rc5_init() {
    rc5_stored_data[DATA_START] = EEPROM.read(EEPROM_ADDR_RC5_START);
    rc5_stored_data[DATA_STOP] = EEPROM.read(EEPROM_ADDR_RC5_STOP);
}
```

Al arrancar, se cargan los comandos almacenados en un array en RAM
(`rc5_stored_data[2]`).

### Programación (Escritura)

[`rc5.cpp:22-50`](../source_code/Module/src/rc5.cpp)

Cuando el módulo recibe un comando con dirección `ADDRESS_PROG` (0x0B):

1. `command` → `rc5_stored_data[DATA_STOP]`
2. `command + 1` → `rc5_stored_data[DATA_START]`
3. Se escribe en EEPROM con `EEPROM.write()`
4. Se ejecuta `module_feedback()` para confirmar

| Condición | Comando STOP guardado | Comando START guardado |
|-----------|----------------------|------------------------|
| `id == 0` | `command` (del mando) | `command + 1` |
| `id != 0` | `(id << 1) & 0xFE` | `(id << 1) \| 1` |

### Verificación (Lectura)

En cada comando con dirección `ADDRESS_COMP` (0x07), el módulo compara el
comando recibido con los valores almacenados en RAM:

```cpp
if (command == rc5_stored_data[DATA_START]) {
    module_start();
} else if (command == rc5_stored_data[DATA_STOP]) {
    module_stop();
}
```

## Mecanismo de Integridad

| Mecanismo | Detalle |
|-----------|---------|
| **Checksum** | ❌ No implementado |
| **Validación** | ❌ No se validan los datos al cargar |
| **Valores por defecto** | ❌ No se detecta EEPROM virgen (0xFF) |
| **Copia de seguridad** | ❌ Sin redundancia |

> **⚠️ Advertencia**: No hay verificación de integridad de los datos en EEPROM.
> Si la EEPROM está sin inicializar (valores 0xFF), los comandos no
> coincidirán con ningún código RC5 válido (los comandos RC5 son de 6 bits,
> máximo 0x3F). Esto actúa como protección implícita, pero no se notifica al
> usuario. Ver [EE-01](07-known-issues.md#ee-01).

## Reset de EEPROM

Existe código comentado para restaurar la EEPROM a valores por defecto:

[`rc5.cpp:90-96`](../source_code/Module/src/rc5.cpp) (comentado)

```cpp
// void rc5_reset_eeprom() {
//     EEPROM.write(EEPROM_ADDR_RC5_START, 0);
//     EEPROM.write(EEPROM_ADDR_RC5_STOP, 0);
//     rc5_init();
//     module_feedback();
// }
```

[`main.cpp:23-25`](../source_code/Module/src/main.cpp) (comentado)

```cpp
// if (!digitalRead(PIN_RESET)) {
//     rc5_reset_eeprom();
// }
```

El pin `PIN_RESET` (PB2) y la lógica de reset están definidos en [`pinout.h`](../source_code/Module/include/pinout.h#L7) pero no implementados.

---

*Documento generado el 2025-06-25. Ver también [Protocolos IR](03-ir-protocols.md), [Arquitectura Software](02-software-architecture.md), [Problemas Conocidos](07-known-issues.md).*
