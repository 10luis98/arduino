
//en altas rpm se corta algunos cilindros
// por que lña duracion sobrepasa la rueda del levas 
//es normal este comportamiento si le coloco a 1 ms la bobina trabaja bien todo 


#include <Arduino.h>
#include "interrupciones.h"
#include "patron.h"

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


  DDRB |= (1 << PB1);  // Pin 52
  DDRD |= (1 << PD7);  // Pin 38
  DDRG |= (1 << PG1);  // Pin 40
  DDRB |= (1 << PB5);  // Pin 11
  DDRB |= (1 << PB4);  // Pin 10
  DDRL |= (1 << PL4);  // Pin 45




  PORTB &= ~(1 << PB1);  // Apagar pin 52
  PORTD &= ~(1 << PD7);  // Apagar pin 38
  PORTG &= ~(1 << PG1);  // Apagar pin 40
  PORTB &= ~(1 << PB5);  // Apagar pin 11
  PORTB &= ~(1 << PB4);  // Apagar pin 10
  PORTL &= ~(1 << PL4);  // Apagar pin 45



DDRB |= (1 << DDB4);  // Pin 10 como salida
PORTB &= ~(1 << PB4);  // Apagar pin 10





DDRE &= ~(1 << DDE2);   // Configurar pin 19 (PE2) como entrada
PORTE |= (1 << PORTE2); // Habilitar pull-up interno en pin 19

pinMode(18, INPUT); // Configurar el pin 18 como entrada con pull-up interno





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

          Serial.print(",");

    if ( datosNuevos) {
        datosNuevos = false;
        Serial.print(duracionPulsoAlto);
        Serial.print(",");
        Serial.println(duracionPulsoBajo);
    }
}





