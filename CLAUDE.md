# IRStart

Sistema de arranque por infrarrojos para competiciones de robótica. Compuesto
por un mando (ESP32-C3) multi-protocolo y un módulo receptor (ATtiny13/85)
compatible con RoboChallenge Romania.

## Stack tecnológico
- **Microcontrolador**: ESP32-C3 (mando) + ATtiny13/ATtiny85 (módulo)
- **Framework**: Arduino
- **Entorno**: PlatformIO
- **Lenguaje**: C++11
- **Protocolos**: RC5, NEC, SIRC
- **Sensores**: TSOP4838 (receptor IR)
- **Actuadores**: 3× LEDs IR con MOSFETs, NeoPixel RGB ×3

## Estructura del repositorio
- `source_code/Remote/` — firmware del mando (ESP32-C3)
- `source_code/Module/` — firmware del módulo (ATtiny13/85)
- `pcb_files/` — diseño electrónico (KiCad)
- `3d_model/` — piezas mecánicas (STEP, STL)
- `docs/` — documentación técnica (MkDocs Material)
- `images/` — fotos del hardware

## Convenciones de código
- Código en inglés (comentarios en español)
- Include guards: `#ifndef NOMBRE_H`
- Nombrado: `snake_case` para funciones y variables, `UPPER_CASE` para constantes
- Pinout centralizado en `pinout.h`
- Debounce de botones: doble lectura con 10 ms de intervalo

## Documentación
- Usa `/doc-init` para regenerar la documentación desde cero
- Usa `/doc-review` para revisar cambios incrementales y actualizar known issues
- La documentación sigue el estándar OPRobots (MkDocs Material + estilo ZoroBot3)
- Despliegue en GitHub Pages (docs.oprobots.org) vía monorepo OPRobots/docs

## Notas
- El módulo RC5 está basado en la implementación de https://clearwater.com.au/code/rc5
- Compatible con https://p1r.se/startmodule/ para RoboChallenge Romania
- El mando soporta 3 protocolos pero el módulo solo decodifica RC5
- Programación del ATtiny: usar PlatformIO con entorno `attiny13_via_ArduinoISP` o `attiny85_via_ArduinoISP`
- Hay 3 issues críticos conocidos en el módulo (ver `docs/07-known-issues.md`): `delayMicroseconds` en ISR, `EEPROM.write` en ISR, variable `started` sin `volatile`
