#include "interrupciones.h"
#include "utils.h"
#include "patron.h" // Agregar esta línea para acceder a patronConvertido y tamanoConvertido
#include "pines_control.h"
volatile unsigned long tiempoInicio = 0;
volatile unsigned long tiempoInicioBajo = 0;
volatile unsigned long duracionPulsoAlto = 0;
volatile unsigned long duracionPulsoBajo = 0;
volatile bool datosNuevos = false;
volatile bool bajada = false;
volatile int indicePatron = 1;
volatile unsigned long blk = 0;
volatile bool ledEstado = false;
volatile bool timerActivo = false;
volatile bool cicloCompleto = false;
int ledPin = 45;
volatile bool flanco = true;


void detectarFlanco()
{


  unsigned long tiempoActual = micros();
  bool estadoActual = LEER_PIN_18;
  if (estadoActual == flanco) 
  {


    datosNuevos = true;
    duracionPulsoBajo = tiempoActual - tiempoInicioBajo;
    tiempoInicio = tiempoActual;
    if (bajada)
    {

      bajada = false;
      //wau estop consume 20 microsegundos    no usar modulo
      //if (indicePatron < tamanoConvertido && indicePatron % 2 == 1)  
      if ((indicePatron & 1) && (indicePatron < tamanoConvertido))

      {

        if (cicloCompleto)
        {
          switch (indicePatron)
          {
          case 3:
            ENCENDER_PIN_45;
            break;
          case 5:
            ENCENDER_PIN_38;
            break;
          case 7:
            ENCENDER_PIN_40;
            break;
          case 9:
            ENCENDER_PIN_10;
            break;
          case 11:
            ENCENDER_PIN_11;
            break;
          case 13:
            ENCENDER_PIN_52;
            break;
          }
          iniciarTimer1();
        }

        if (estaEnRango(duracionPulsoBajo, patronConvertido[indicePatron] * duracionPulsoAlto, 15))
        {
          indicePatron += 2;

          if (indicePatron > 13)
          {
            cicloCompleto = true;
            indicePatron = 1;
          }
        }
        else
        {
          indicePatron = 1;
          cicloCompleto = false;
        }
      }
      else
      {
        indicePatron = 1;
        bajada = false;
        cicloCompleto = false;
      }
    }
  }
  else
  {
    duracionPulsoAlto = tiempoActual - tiempoInicio;
    tiempoInicioBajo = tiempoActual;
    bajada = true;
  }
}

ISR(TIMER1_COMPA_vect)
{
  TCCR1B = 0;
  TIMSK1 &= ~(1 << OCIE1A);
  timerActivo = false;
  APAGAR_PIN_52;
  APAGAR_PIN_38;
  APAGAR_PIN_40;
  APAGAR_PIN_11;
  APAGAR_PIN_10;
  APAGAR_PIN_45;
}

void iniciarTimer1()
{

  if (timerActivo)
    return; // Evitar reiniciar el timer si ya está en uso

  TIMSK1 &= ~(1 << OCIE1A); // Deshabilita solo la interrupción del Timer1
  TCCR1A = 0;               // Modo normal
  TCCR1B = 0;               // Limpia los registros
  TCNT1 = 0;                // Reinicia el contador

  OCR1A = 31999; // 2 ms = 32000 ticks (contando desde 0)

  TCCR1B = (1 << WGM12) | (1 << CS10); // Modo CTC (clear timer on compare match), prescaler 1
  TIMSK1 |= (1 << OCIE1A);             // Habilita la interrupción del Timer1
  timerActivo = true;
}