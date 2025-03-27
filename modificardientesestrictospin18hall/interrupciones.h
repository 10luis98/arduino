#ifndef INTERRUPCIONES_H
#define INTERRUPCIONES_H

#include <Arduino.h>

extern volatile unsigned long tiempoInicio;
extern volatile unsigned long tiempoInicioBajo;
extern volatile unsigned long duracionPulsoAlto;
extern volatile unsigned long duracionPulsoBajo;
extern volatile bool datosNuevos;
extern volatile bool subida;
extern volatile bool bajada;
extern volatile int indicePatron;
extern volatile unsigned long blk;

void detectarFlanco();
void iniciarTimer1();

#endif
