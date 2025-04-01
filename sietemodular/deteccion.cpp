// deteccion.cpp
#include "deteccion.h"
#include "timers.h"

volatile unsigned long tiempoAnterior = 0;
volatile unsigned long tiempoEntreDientes = 1000;
volatile unsigned long tiempoAnteriorDiente = 0;
volatile bool dienteReferenciaDetectado = false;
volatile bool flancoDetectado = false;
volatile int cilindro = 0;

void detectarDiente() {
    flancoDetectado = true;
    unsigned long tiempoActual = micros();
    tiempoEntreDientes = tiempoActual - tiempoAnterior;
    tiempoAnterior = tiempoActual;
    if (tiempoAnteriorDiente > tiempoEntreDientes * 2) {
        cilindro = 0;
        dienteReferenciaDetectado = true;
    } else {
        if (dienteReferenciaDetectado) {
            cilindro++;
            if (cilindro > 6) {
                cilindro = 0;
                dienteReferenciaDetectado = false;
            }
            if (cilindro == 1) tiempoEntreDientes += tiempoAnteriorDiente;
            if (cilindro == 3) {
                int avanceFijo = 0;
                float vance = 1.0 - (avanceFijo / 120.0);
                long tiempoAjustado = (2 + vance) * tiempoEntreDientes - 2000;
                if (tiempoAjustado < 0) tiempoAjustado = 0;
                iniciarTimer5(tiempoAjustado);
            }
        }
    }
    tiempoAnteriorDiente = tiempoEntreDientes;
}
