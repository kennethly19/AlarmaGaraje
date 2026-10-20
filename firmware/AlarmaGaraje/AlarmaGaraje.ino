#include "SensorPuerta.h"
#include "ConexionWiFi.h"

const char* ssid  = "A25 de KS";
const char* clave = "123456723";
// IP del computador donde corre Node-RED
const char* url = "http://10.69.73.89:1880/puerta";

// Objetos: el sensor está en el pin 13 y la red usa los datos de arriba
SensorPuerta miGaraje(13);
ConexionWiFi red(ssid, clave, url);

const unsigned long INTERVALO_LECTURA = 100;  // revisar el sensor (ms)
unsigned long tLectura = 0;
bool estadoAnterior = false;                  // último estado de la puerta

void setup() {
  Serial.begin(9600);
  miGaraje.iniciar();   // configura el sensor
  red.iniciar();        // empieza la conexión Wi-Fi

  Serial.println("Sistema de seguridad de garaje encendido.");
}

void loop() {
  red.actualizar();     // mantiene el Wi-Fi y reintenta envíos

  unsigned long ahora = millis();
  if (ahora - tLectura >= INTERVALO_LECTURA) {
    tLectura = ahora;
    bool estadoActual = miGaraje.estaAbierta();

    // Solo actuamos si la puerta cambió de estado
    if (estadoActual != estadoAnterior) {
      estadoAnterior = estadoActual;

      if (estadoActual) {
        Serial.println("¡ALERTA! La puerta se ABRIÓ.");
      } else {
        Serial.println("La puerta se CERRÓ.");
      }

      red.enviarEstado(estadoActual);   // delega el envío al objeto
    }
  }
}
