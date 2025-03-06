#include <Arduino.h> // Para usar pinMode()

#include "pines_control.h"

void configurarPines() {
    DDRB |= (1 << PB1); // Pin 52 como salida
    DDRD |= (1 << PD7); // Pin 38 como salida
    DDRG |= (1 << PG1); // Pin 40 como salida
    DDRB |= (1 << PB5); // Pin 11 como salida
    DDRB |= (1 << PB4); // Pin 10 como salida
    DDRL |= (1 << PL4); // Pin 45 como salida

    pinMode(18, INPUT_PULLUP); // Configurar el pin 18 como entrada con pull-up interno
}
