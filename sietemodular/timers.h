#ifndef TIMERS_H
#define TIMERS_H

#include <Arduino.h>

void iniciarTimer1(unsigned long tiempoMicrosegundos);
void iniciarTimer5(unsigned long tiempoMicrosegundos);

// Interrupciones
ISR(TIMER1_COMPA_vect);
ISR(TIMER5_COMPA_vect);

#endif // TIMERS_H
