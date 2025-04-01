// timers.cpp
#include "timers.h"

void iniciarTimer1(unsigned long tiempoMicrosegundos) {
    cli();
    TIMSK1 &= ~(1 << OCIE1A);
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;
    unsigned long ticks = tiempoMicrosegundos * 16;
    if (ticks < 1) ticks = 1;
    if (ticks <= 65535) {
        OCR1A = ticks;
        TCCR1B = (1 << WGM12) | (1 << CS10);
    } else if (ticks / 8 <= 65535) {
        OCR1A = ticks / 8;
        TCCR1B = (1 << WGM12) | (1 << CS11);
    } else {
        OCR1A = 65535;
        TCCR1B = (1 << WGM12) | (1 << CS12);
    }
    TIMSK1 |= (1 << OCIE1A);
    sei();
}

void iniciarTimer5(unsigned long tiempoMicrosegundos) {
    cli();
    TIMSK5 &= ~(1 << OCIE5A);
    TCCR5A = 0;
    TCCR5B = 0;
    TCNT5 = 0;
    unsigned long ticks = tiempoMicrosegundos * 16;
    if (ticks <= 65535) {
        OCR5A = ticks;
        TCCR5B = (1 << WGM52) | (1 << CS50);
    } else if (ticks / 8 <= 65535) {
        OCR5A = ticks / 8;
        TCCR5B = (1 << WGM52) | (1 << CS51);
    } else {
        OCR5A = ticks / 1024;
        TCCR5B = (1 << WGM52) | (1 << CS52) | (1 << CS50);
    }
    TIMSK5 |= (1 << OCIE5A);
    sei();
}
