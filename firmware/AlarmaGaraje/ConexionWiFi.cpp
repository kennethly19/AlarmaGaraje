#include "ConexionWiFi.h"
#include <WiFi.h>
#include <HTTPClient.h>

static const unsigned long INTERVALO_WIFI      = 10000;  // reintentar Wi-Fi
static const unsigned long INTERVALO_REINTENTO = 5000;   // reintentar envío

ConexionWiFi::ConexionWiFi(const char* ssid, const char* clave, const char* url)
  : _ssid(ssid), _clave(clave), _url(url),
    _pendiente(false), _estadoPendiente(false),
    _tWifi(0), _tReintento(0) {
}

void ConexionWiFi::iniciar() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(_ssid, _clave);   // no espera: conecta en segundo plano
}

// Se llama en cada vuelta del loop(); no bloquea
void ConexionWiFi::actualizar() {
  unsigned long ahora = millis();

  // Reconectar el Wi-Fi si se cayó
  if (WiFi.status() != WL_CONNECTED && ahora - _tWifi >= INTERVALO_WIFI) {
    _tWifi = ahora;
    Serial.println("Conectando al Wi-Fi...");
    WiFi.disconnect();
    WiFi.begin(_ssid, _clave);
  }

  // Reintentar un envío que falló
  if (_pendiente && ahora - _tReintento >= INTERVALO_REINTENTO) {
    _intentarEnvio();
  }
}

void ConexionWiFi::enviarEstado(bool abierta) {
  _estadoPendiente = abierta;
  _pendiente = true;
  _intentarEnvio();
}

// Devuelve el código HTTP (200 = bien) o -100 si no hay Wi-Fi
int ConexionWiFi::_publicar(bool abierta) {
  if (WiFi.status() != WL_CONNECTED) return -100;

  HTTPClient http;
  http.setTimeout(3000);
  http.begin(_url);
  http.addHeader("Content-Type", "application/json");
  String datos = String("{\"estado\":\"") + (abierta ? "ABIERTA" : "CERRADA") + "\"}";
  int codigo = http.POST(datos);
  http.end();
  return codigo;
}

void ConexionWiFi::_intentarEnvio() {
  _tReintento = millis();
  int codigo = _publicar(_estadoPendiente);

  if (codigo == 200) {
    _pendiente = false;
    Serial.println("Evento enviado a Node-RED (200).");
  } else if (codigo == -100) {
    Serial.println("ERROR E1: sin conexion Wi-Fi.");
  } else if (codigo < 0) {
    Serial.print("ERROR E2: no se pudo contactar a Node-RED (codigo ");
    Serial.print(codigo);
    Serial.println(").");
  } else {
    Serial.print("ERROR E3: Node-RED respondio con codigo ");
    Serial.println(codigo);
  }
}
