#include "deteccion.h"
#include "timers.h"

volatile unsigned long tiempoAnterior = 0;
volatile unsigned long tiempoEntreDientes = 1000;
volatile unsigned long tiempoAnteriorDiente = 0;
volatile bool dienteReferenciaDetectado = false;
volatile bool flancoDetectado = false;
volatile int cilindro = 0;
volatile int bobina = 0;
volatile long tiempoAjustado = 0;
volatile bool enEjecucion = false;

void detectarDiente() {
  flancoDetectado = true;
  unsigned long tiempoActual = micros();
  tiempoEntreDientes = tiempoActual - tiempoAnterior;
  tiempoAnterior = tiempoActual;

  // Si la diferencia de tiempos es grande, se detecta el diente de referencia
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

  if (!enEjecucion) {  // Si no está en ejecución
    enEjecucion = true;  // Marcar como en ejecución
    
    bobina = cilindro - 1;
    int avanceFijo = 0;
    float vance = 1.0 - (avanceFijo / 120.0);
    tiempoAjustado = (0 + vance) * tiempoEntreDientes - 4000;
    
    if (tiempoAjustado < 0) {
      tiempoAjustado = 0;
      if (true) {
        tiempoAjustado = 0;
      }
    }
    
    iniciarTimer5(tiempoAjustado);  // Ajuste de tiempo en base a detección
  }
      
    }
  }
  tiempoAnteriorDiente = tiempoEntreDientes;
}
