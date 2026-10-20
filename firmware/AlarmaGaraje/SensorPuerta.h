#ifndef SensorPuerta_h
#define SensorPuerta_h
#include <Arduino.h>

class SensorPuerta {
  public:
    // Constructor: nos permite decirle en qué pin conectamos el sensor
    SensorPuerta(int pin);
    
    // Funciones que prometemos que el sensor hará
    void iniciar();
    bool estaAbierta(); 

  private:
    // Variable privada para guardar el número del pin
    int _pin; 
};

#endif