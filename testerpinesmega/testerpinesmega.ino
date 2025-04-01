#include "pines.h"

void setup() {
    // Configurar todos los pines como salida
    for (int i = 0; i < 6; i++) {
        configurarSalida(pines[i]);
    }

    // Iniciar con todos los pines apagados
    for (int i = 0; i < 6; i++) {
        apagarPin(pines[i]);
    }
}

void loop() {
    // Encender y apagar los pines de forma dinámica
    for (int i = 0; i < 6; i++) {
        encenderPin(pines[i]);  // Encender pin
        Serial.print("Encendido IGN");
        Serial.println(i + 1);
        delay(100);

        apagarPin(pines[i]);  // Apagar pin
        Serial.print("Apagado IGN");
        Serial.println(i + 1);
        delay(100);
    }
}

