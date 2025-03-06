#ifndef PINES_CONTROL_H
#define PINES_CONTROL_H

// Configuración de pines (Macros para encender y apagar)
#define ENCENDER_PIN_52 (PORTB |= (1 << PB1))
#define APAGAR_PIN_52   (PORTB &= ~(1 << PB1))

#define ENCENDER_PIN_38 (PORTD |= (1 << PD7))
#define APAGAR_PIN_38   (PORTD &= ~(1 << PD7))

#define ENCENDER_PIN_40 (PORTG |= (1 << PG1))
#define APAGAR_PIN_40   (PORTG &= ~(1 << PG1))

#define ENCENDER_PIN_11 (PORTB |= (1 << PB5))
#define APAGAR_PIN_11   (PORTB &= ~(1 << PB5))

#define ENCENDER_PIN_10 (PORTB |= (1 << PB4))
#define APAGAR_PIN_10   (PORTB &= ~(1 << PB4))

#define ENCENDER_PIN_45 (PORTL |= (1 << PL4))
#define APAGAR_PIN_45   (PORTL &= ~(1 << PL4))

// Función para configurar los pines como salida
inline void configurarPines() {
    DDRB |= (1 << PB1);  // Pin 52 como salida
    DDRD |= (1 << PD7);  // Pin 38 como salida
    DDRG |= (1 << PG1);  // Pin 40 como salida
    DDRB |= (1 << PB5);  // Pin 11 como salida
    DDRB |= (1 << PB4);  // Pin 10 como salida
    DDRL |= (1 << PL4);  // Pin 45 como salida
}

#endif // PINES_CONTROL_H
