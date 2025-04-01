#include "pines.h"

// Definir pines con sus respectivos puertos y números
Pin pines[] = {
    {&PORTB, PB1},  // Pin 52 (IGN1)
    {&PORTD, PD7},  // Pin 38 (IGN2)
    {&PORTG, PG1},  // Pin 40 (IGN3)
    {&PORTB, PB5},  // Pin 11 (IGN4)
    {&PORTB, PB4},  // Pin 10 (IGN5)
    {&PORTL, PL4}   // Pin 45 (IGN6)
};

void configurarSalida(Pin& pin) {
    // Configurar el pin como salida
    *(pin.puerto - 1) |= (1 << pin.pin);  // DDRx registro (número de puerto - 1)
}

void encenderPin(Pin& pin) {
    // Encender el pin
    *(pin.puerto) |= (1 << pin.pin);
}

void apagarPin(Pin& pin) {
    // Apagar el pin
    *(pin.puerto) &= ~(1 << pin.pin);
}
