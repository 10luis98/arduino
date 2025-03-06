
//en altas rpm se corta algunos cilindros
// por que lña duracion sobrepasa la rueda del levas 
//es normal este comportamiento si le coloco a 1 ms la bobina trabaja bien todo 


#include <Arduino.h>
#include "interrupciones.h"
#include "patron.h"
#include "pines_control.h"


#define PIN_ENTRADA 18
#define PIN_SALIDA1 7


#define LED_PIN     PB4  // Pin 10 está en PB4
#define LED_ON()    (PORTB |= (1 << LED_PIN))   // Encender LED
#define LED_OFF()   (PORTB &= ~(1 << LED_PIN))  // Apagar LED
#define LED_TOGGLE() (PORTB ^= (1 << LED_PIN))  // Alternar LED
unsigned long tiempoPrevioToogle = 0;  // Variable para controlar el tiempo
const unsigned long intervaloToogle = 500000;  // Intervalo en microsegundos (500 ms)




void setup() {
    Serial.begin(2000000);

    configurarPines(); // Configurar pines como salida

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


    convertirPatron();
    Serial.println("Patrón convertido:");
    for (int i = 0; i < tamanoConvertido; i++) {
        Serial.print(patronConvertido[i]);
        Serial.print(" ");
    }
    Serial.println();

    attachInterrupt(digitalPinToInterrupt(PIN_ENTRADA), detectarFlanco, CHANGE);
}

void loop() {


    unsigned long tiempoActualToogle = micros();  // Obtener el tiempo actual

    if (tiempoActualToogle - tiempoPrevioToogle >= intervaloToogle) {
        tiempoPrevioToogle = tiempoActualToogle;  // Guardar el tiempo actual
      //  LED_TOGGLE();  // Cambiar el estado del LED
    }

 /*          Serial.print(",");

    if ( datosNuevos) {
        datosNuevos = false;
        Serial.print(duracionPulsoAlto);
        Serial.print(",");
        Serial.println(duracionPulsoBajo);
    } */
}



