#ifndef ConexionWiFi_h
#define ConexionWiFi_h
#include <Arduino.h>

class ConexionWiFi {
  public:
    // Constructor: red Wi-Fi y dirección de Node-RED
    ConexionWiFi(const char* ssid, const char* clave, const char* url);

    void iniciar();                    // Empieza la conexión Wi-Fi
    void actualizar();                 // Reconecta y reintenta envíos
    void enviarEstado(bool abierta);   // Envía el estado de la puerta

  private:
    const char* _ssid;
    const char* _clave;
    const char* _url;

    bool _pendiente;                   // Hay un envío sin completar
    bool _estadoPendiente;             // Estado que falta por enviar
    unsigned long _tWifi;              // Último intento de Wi-Fi
    unsigned long _tReintento;         // Último intento de envío

    int  _publicar(bool abierta);      // Hace el POST y devuelve el código
    void _intentarEnvio();             // Intenta enviar y muestra errores
};

#endif
