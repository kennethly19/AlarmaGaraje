#include "SensorPuerta.h"

// 1. El Constructor: guarda el pin que elegimos
SensorPuerta::SensorPuerta(int pin) {
  _pin = pin;
}

// 2. Función iniciar: configura el pin del ESP32
void SensorPuerta::iniciar() {
  // INPUT_PULLUP activa una resistencia interna del ESP32
  pinMode(_pin, INPUT_PULLUP); 
}

// 3. Función para leer el sensor magnético MC-38
bool SensorPuerta::estaAbierta() {
  int lectura = digitalRead(_pin);
  
  // Como usamos PULLUP, si el imán se aleja (puerta abierta), 
  // el circuito se abre y el ESP32 lee un estado ALTO (HIGH o 1).
  if (lectura == HIGH) {
    return true;  // La puerta está abierta
  } else {
    return false; // La puerta está cerrada
  }
}