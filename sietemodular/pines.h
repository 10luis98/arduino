#ifndef PINES_H
#define PINES_H

// Definir pines con sus respectivos puertos y números
#define IGN1 PORTB, PB1  // Pin 52 // IGNITION OUTPUT.C
#define IGN2 PORTD, PD7  // Pin 38 // IGNITION OUTPUT.B
#define IGN3 PORTG, PG1  // Pin 40 // IGNITION OUTPUT.A
#define IGN4 PORTB, PB5  // Pin 11 // INJECTOR OUTPUT.D
#define IGN5 PORTB, PB4  // Pin 10 // INJECTOR OUTPUT.C
#define IGN6 PORTL, PL4  // Pin 45 // FUEL PUMP RELAY OUTPUT

// Macros para apagar y encender pines
#define APAGAR_PIN(NOMBRE) APAGAR_PIN_EXPANDIDO(NOMBRE)
#define APAGAR_PIN_EXPANDIDO(PORT, PIN) ((PORT) &= ~(1 << (PIN)))

#define ENCENDER_PIN(NOMBRE) ENCENDER_PIN_EXPANDIDO(NOMBRE)
#define ENCENDER_PIN_EXPANDIDO(PORT, PIN) ((PORT) |= (1 << (PIN)))

// Macros para crear nombres dinámicos como IGN1, IGN2, etc.
#define CONCATENAR(prefijo, sufijo) prefijo##sufijo
#define OBTENER_PIN(numero) CONCATENAR(IGN, numero)

#endif  // PINES_H

