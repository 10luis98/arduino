#include <Arduino.h>

#include "pines_control.h"



void setup() {
    configurarPines(); // Configurar pines como salida

}

void loop() {
    // Encender todos los pines
    ENCENDER_PIN_52;
    ENCENDER_PIN_38;
    ENCENDER_PIN_40;
    ENCENDER_PIN_11;
    ENCENDER_PIN_10;
    ENCENDER_PIN_45;

    delay(100); // Esperar 100 ms

    // Apagar todos los pines
    APAGAR_PIN_52;
    APAGAR_PIN_38;
    APAGAR_PIN_40;
    APAGAR_PIN_11;
    APAGAR_PIN_10;
    APAGAR_PIN_45;

    delay(100); // Esperar 100 ms
}

