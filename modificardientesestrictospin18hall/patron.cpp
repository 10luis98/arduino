#include "patron.h"
#include <Arduino.h>
#include <stdlib.h>

const bool PATRON_ESPERADO[] = {
    1, 0, 1, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0
};

int *patronConvertido = NULL;
int tamanoConvertido = 0;

void convertirPatron() {
    tamanoConvertido = 0;
    int tamano = sizeof(PATRON_ESPERADO) / sizeof(PATRON_ESPERADO[0]);
    patronConvertido = (int *)malloc(tamano * sizeof(int));

    if (!patronConvertido) return;

    for (int i = 0; i < tamano; i++) {
        if (PATRON_ESPERADO[i] == 1) {
            patronConvertido[tamanoConvertido++] = 1;
        } else {
            int contador = 0;
            while (i < tamano && PATRON_ESPERADO[i] == 0) {
                contador++;
                i++;
            }
            i--;
            patronConvertido[tamanoConvertido++] = contador;
        }
    }
}
