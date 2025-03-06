
#include "interrupciones.h"
#include "utils.h"
#include "patron.h"  // Agregar esta línea para acceder a patronConvertido y tamanoConvertido
#include "pines_control.h"

volatile unsigned long tiempoInicio = 0;
volatile unsigned long tiempoInicioBajo = 0;
volatile unsigned long duracionPulsoAlto = 0;
volatile unsigned long duracionPulsoBajo = 0;
volatile bool datosNuevos = false;
volatile bool subida = false;
volatile bool bajada = false;
volatile int indicePatron = 1;
volatile unsigned long blk = 0;

 volatile bool ledEstado=false;
volatile bool timerActivo = false;  // Variable para controlar el estado del Timer1


volatile bool cicloCompleto = false;  // Variable para controlar si se completó el patrón
 int ledPin =45;



void detectarFlanco() {

 //  ledEstado = !ledEstado; // Cambia el estado del LED
 // digitalWrite(ledPin, ledEstado); // Actualiza el LED
  subida = true;
  unsigned long tiempoActual = micros();

 // if ((PIND & (1 << PIND2))) {  // Flanco de subida 19
    if (digitalRead(18)) {  // Flanco de subida 18

    datosNuevos = true;
    duracionPulsoBajo = tiempoActual - tiempoInicioBajo;
    tiempoInicio = tiempoActual;

    if (bajada) {
      bajada = false;

      if (indicePatron < tamanoConvertido && indicePatron % 2 == 1) {


        // Si completamos el ciclo correctamente, encender el LED y activar Timer1
        if (cicloCompleto) {




 switch (indicePatron) {
  case 3:
    PORTL |= (1 << PL4);  // Encender pin 45 (Bomba)
    iniciarTimer1();
    break;
  case 5:
    PORTD |= (1 << PD7);  // Encender pin 38 (Encendido 3)
    iniciarTimer1();
    break;
  case 7:
    PORTG |= (1 << PG1);  // Encender pin 40 (Encendido 5)
    iniciarTimer1();
    break;
  case 9:
    PORTB |= (1 << PB5);  // Encender pin 7 (BOOST)   <-- CORREGIDO
    iniciarTimer1();
    break;
  case 11:
    PORTB |= (1 << PB4);  // Encender pin 11 (INYECTOR 4)
    iniciarTimer1();
    break;
  case 13:
    PORTB |= (1 << PB1);  // Encender pin 52 (ENCENDIDO)
    iniciarTimer1();
    break;
}

        }

        if (estaEnRango(duracionPulsoBajo, patronConvertido[indicePatron] * duracionPulsoAlto, 15)) {
          indicePatron += 2;

          if (indicePatron > 13) {  // Se completó el ciclo correctamente
            cicloCompleto = true;
            indicePatron = 1;
          }

        } else {
          // Si hay un error, reiniciamos el patrón
          indicePatron = 1;
          cicloCompleto = false;
        }
      } else {
        indicePatron = 1;
        bajada = false;
        cicloCompleto = false;
      }

      // // Si completamos el ciclo correctamente, encender el LED y activar Timer1
      // if (cicloCompleto) {
      //     PORTD |= (1 << PORTD7);  // Encender LED
      //     iniciarTimer1();         // Iniciar Timer1 para apagarlo en 2 ms
      //     cicloCompleto = false;   // Evita que se siga ejecutando sin control
      // }
    }
  } else {  // Flanco de bajada
    duracionPulsoAlto = tiempoActual - tiempoInicio;
    tiempoInicioBajo = tiempoActual;
    bajada = true;
  }
}


ISR(TIMER1_COMPA_vect) {

    TCCR1B = 0;
  TIMSK1 &= ~(1 << OCIE1A);

  timerActivo = false;  // Marcar que el timer está disponible
  //PORTD &= ~(1 << PORTD7);
  //  PORTB &= ~(1 << PB2);
  PORTB &= ~(1 << PB1);  // Apagar pin 52
  PORTD &= ~(1 << PD7);  // Apagar pin 38
  PORTG &= ~(1 << PG1);  // Apagar pin 40
  PORTB &= ~(1 << PB5);  // Apagar pin 11
  PORTB &= ~(1 << PB4);  // Apagar pin 10
  PORTL &= ~(1 << PL4);  // Apagar pin 45

  APAGAR_PIN_52;
  APAGAR_PIN_38;
  APAGAR_PIN_40;
  APAGAR_PIN_11;
  APAGAR_PIN_10;
  APAGAR_PIN_45;


}


void iniciarTimer1() {

  if (timerActivo) return;  // Evitar reiniciar el timer si ya está en uso

  TIMSK1 &= ~(1 << OCIE1A);  // Deshabilita solo la interrupción del Timer1
  TCCR1A = 0;                // Modo normal
  TCCR1B = 0;                // Limpia los registros
  TCNT1 = 0;                 // Reinicia el contador

  OCR1A = 31999;  // 2 ms = 32000 ticks (contando desde 0)

  TCCR1B = (1 << WGM12) | (1 << CS10);  // Modo CTC (clear timer on compare match), prescaler 1
  TIMSK1 |= (1 << OCIE1A);              // Habilita la interrupción del Timer1
      timerActivo = true;

}