#include "utils.h"

// bool estaEnRango(unsigned long valor, unsigned long esperado, unsigned char porcentajeTolerancia) {
//     unsigned long tol = ((esperado * porcentajeTolerancia) + (1 << 6)) >> 7;
//     unsigned long diff = (valor >= esperado) ? (valor - esperado) : (esperado - valor);
//     return diff <= tol;
// }

bool estaEnRango(unsigned long valor, unsigned long esperado, unsigned char porcentajeTolerancia) {
    unsigned long tol = (esperado * porcentajeTolerancia) / 100;
    unsigned long minVal = (esperado > tol) ? (esperado - tol) : 0; // Evita valores negativos
    unsigned long maxVal = esperado + tol;

    return (valor >= minVal && valor <= maxVal);
}
