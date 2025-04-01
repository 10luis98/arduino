#ifndef DETECCION_H
#define DETECCION_H

#include <Arduino.h>

// Declaración de variables globales
extern volatile unsigned long tiempoAnterior;
extern volatile unsigned long tiempoEntreDientes;
extern volatile unsigned long tiempoAnteriorDiente;
extern volatile bool dienteReferenciaDetectado;
extern volatile bool flancoDetectado;
extern volatile int cilindro;
extern volatile int bobina;


void detectarDiente();

#endif // DETECCION_H
