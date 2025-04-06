// main.ino

#include "deteccion.h"
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
  
    pinMode(18, INPUT_PULLUP);
    pinMode(3, OUTPUT);
    //attachInterrupt(digitalPinToInterrupt(18), detectarDiente, RISING);
    Serial.begin(2000000);
}

void loop() {

  apagarPin(pines[0]);  // Apagar pin
delay(1000);

  encenderPin(pines[0]);     // Encender pin
  delay(1000);

}