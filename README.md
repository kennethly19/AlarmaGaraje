# AlarmaGaraje

Sistema de alarma para la puerta de un garaje con **ESP32**, sensor magnético **MC-38**, **Node-RED** y **PostgreSQL**.

```
Sensor MC-38 → ESP32 → Wi-Fi → Node-RED → PostgreSQL
```

## Estructura

| Carpeta | Contenido |
|---|---|
| `firmware/AlarmaGaraje/` | Código de la ESP32 (`.ino`, `.h`, `.cpp`) |
| `nodered/` | Flujo de Node-RED exportado (JSON) |
| `base_de_datos/` | Script SQL para crear la base y la tabla |

## Librerías propias (programación orientada a objetos)

- **SensorPuerta**: lee el sensor MC-38. Métodos: `iniciar()` y `estaAbierta()`.
- **ConexionWiFi**: conecta y reconecta el Wi-Fi y envía el estado en JSON a Node-RED. Métodos: `iniciar()`, `actualizar()` y `enviarEstado()`. Usa `millis()` (sin `delay()`).
- **AlarmaGaraje.ino**: programa principal; crea los objetos y envía el estado solo cuando la puerta cambia.

## Conexión del sensor

El MC-38 tiene dos cables sin polaridad: uno a **GPIO13** y el otro a **GND**. El pin usa la resistencia pull-up interna (`INPUT_PULLUP`).

- Puerta cerrada (imán cerca): el pin lee LOW.
- Puerta abierta (imán lejos): el pin lee HIGH.

## Puesta en marcha

1. **Base de datos:** ejecutar `base_de_datos/crear_tabla.sql` en PostgreSQL.
2. **Node-RED:** instalar `node-red-contrib-postgresql`, importar el JSON de `nodered/`, configurar la conexión a la base `garaje` y presionar Deploy.
3. **ESP32:** abrir `firmware/AlarmaGaraje/AlarmaGaraje.ino` en Arduino IDE y cambiar al inicio:
   - `ssid` y `clave`: red Wi-Fi (2.4 GHz).
   - `url`: `http://IP_DEL_COMPUTADOR:1880/puerta`.
4. Subir el programa y abrir el monitor serie a 9600 baudios.

## Códigos de error en el monitor serie

| Código | Significado |
|---|---|
| E1 | Sin conexión Wi-Fi |
| E2 | No se pudo contactar a Node-RED |
| E3 | Node-RED respondió con un código distinto de 200 |

## Autor

Kenneth Santiago Castillo Gaona
