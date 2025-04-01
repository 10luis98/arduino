#ifndef PINES_H
#define PINES_H

#include <Arduino.h>

// Definir estructura para almacenar información del pin
struct Pin {
    volatile uint8_t* puerto;
    uint8_t pin;
};

// Definir pines con sus respectivos puertos y números
extern Pin pines[];  // Declarar el arreglo de pines

// Funciones para manipular los pines
void configurarSalida(Pin& pin);
void encenderPin(Pin& pin);
void apagarPin(Pin& pin);

#endif  // PINES_H

