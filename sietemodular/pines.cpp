#include "pines.h"

// Definir pines con sus respectivos puertos y números
Pin pines[] = {
    {&PORTL, PL4},  // Pin 45 (IGN6)1punta
    {&PORTD, PD7},  // Pin 38 (IGN2)2punta
    {&PORTG, PG1},  // Pin 40 (IGN3)3punta
    {&PORTB, PB5},  // Pin 11 (IGN4)4punta
    {&PORTB, PB4},  // Pin 10 (IGN5)5punta
    {&PORTB, PB1},  // Pin 52 (IGN1)6punta
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
