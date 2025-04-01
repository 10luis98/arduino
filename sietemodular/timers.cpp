#include "timers.h"
#include "deteccion.h"
#include "pines.h"  // O el archivo adecuado donde están definidas esas funciones y variables

// Variable para saber si el timer está activo
volatile bool timerActivo = false;

void iniciarTimer1(unsigned long tiempoMicrosegundos) {
  cli();                     // Deshabilitar interrupciones
  TIMSK1 &= ~(1 << OCIE1A);  // Deshabilitar interrupción Timer1
  TCCR1A = 0;                // Limpiar configuraciones anteriores
  TCCR1B = 0;
  TCNT1 = 0;  // Resetear el contador

  unsigned long ticks = tiempoMicrosegundos * 16;
  if (ticks < 1) ticks = 1;
  if (ticks <= 65535) {
    OCR1A = ticks;
    TCCR1B = (1 << WGM12) | (1 << CS10);  // Modo CTC, sin prescaler
  } else if (ticks / 8 <= 65535) {
    OCR1A = ticks / 8;
    TCCR1B = (1 << WGM12) | (1 << CS11);  // Prescaler 8
  } else {
    OCR1A = 65535;
    TCCR1B = (1 << WGM12) | (1 << CS12);  // Prescaler 256
  }
  TIMSK1 |= (1 << OCIE1A);  // Habilitar interrupción Timer1
  sei();                    // Habilitar interrupciones globales
}

void iniciarTimer5(unsigned long tiempoMicrosegundos) {
  cli();                     // Deshabilitar interrupciones
  TIMSK5 &= ~(1 << OCIE5A);  // Deshabilitar interrupción Timer5
  TCCR5A = 0;                // Limpiar configuraciones anteriores
  TCCR5B = 0;
  TCNT5 = 0;  // Resetear el contador

  unsigned long ticks = tiempoMicrosegundos * 16;
  if (ticks <= 65535) {
    OCR5A = ticks;
    TCCR5B = (1 << WGM52) | (1 << CS50);  // Modo CTC, sin prescaler
  } else if (ticks / 8 <= 65535) {
    OCR5A = ticks / 8;
    TCCR5B = (1 << WGM52) | (1 << CS51);  // Prescaler 8
  } else {
    OCR5A = ticks / 1024;
    TCCR5B = (1 << WGM52) | (1 << CS52) | (1 << CS50);  // Prescaler 1024
  }
  TIMSK5 |= (1 << OCIE5A);  // Habilitar interrupción Timer5
  sei();                    // Habilitar interrupciones globales
}

ISR(TIMER1_COMPA_vect) {
  TCCR1B = 0;                // Apagar Timer1
  TIMSK1 &= ~(1 << OCIE1A);  // Deshabilitar interrupción
  timerActivo = false;
  apagarPin(pines[bobina]);  // Apagar pin
                        //PORTE &= ~(1 << PE5);  // Apagar chispa
}

ISR(TIMER5_COMPA_vect) {
  TCCR5B = 0;                // Apagar Timer5
  TIMSK5 &= ~(1 << OCIE5A);  // Deshabilitar interrupción

  encenderPin(pines[bobina]);     // Encender pin
  //PORTE |= (1 << PE5);  // Encender chispa
  iniciarTimer1(1000);  // Ejemplo de tiempo de encendido
}
