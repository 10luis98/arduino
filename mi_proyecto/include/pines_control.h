#ifndef PINES_CONTROL_H
#define PINES_CONTROL_H


// Macros para encender y apagar pines
#define ENCENDER_PIN_52 (PORTB |= (1 << PB1))
#define APAGAR_PIN_52 (PORTB &= ~(1 << PB1))

#define ENCENDER_PIN_38 (PORTD |= (1 << PD7))
#define APAGAR_PIN_38 (PORTD &= ~(1 << PD7))

#define ENCENDER_PIN_40 (PORTG |= (1 << PG1))
#define APAGAR_PIN_40 (PORTG &= ~(1 << PG1))

#define ENCENDER_PIN_11 (PORTB |= (1 << PB5))
#define APAGAR_PIN_11 (PORTB &= ~(1 << PB5))

#define ENCENDER_PIN_10 (PORTB |= (1 << PB4))
#define APAGAR_PIN_10 (PORTB &= ~(1 << PB4))

#define ENCENDER_PIN_45 (PORTL |= (1 << PL4))
#define APAGAR_PIN_45 (PORTL &= ~(1 << PL4))

#define LEER_PIN_18 (PIND & (1 << PD3))


// Declaración de la función (¡Sin implementación aquí!)
void configurarPines();

#endif // PINES_CONTROL_H
