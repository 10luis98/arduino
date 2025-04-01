volatile unsigned long tiempoAnterior = 0;
volatile unsigned long tiempoEntreDientes = 1000;  // Valor inicial arbitrario
volatile unsigned long tiempoAnteriorDiente = 0;
volatile bool dienteReferenciaDetectado = false;
volatile bool flancoDetectado = false;
volatile int cilindro = 0;
volatile bool timerActivo = false;
volatile int posi = 1;
volatile unsigned long dwell = 2000;
// FACTOR DE AVANCE (Porcentaje del tiempo entre dientes)
const float factorAvance = 00.0;

ISR(TIMER1_COMPA_vect) {
  TCCR1B = 0;
  TIMSK1 &= ~(1 << OCIE1A);
  timerActivo = false;
  PORTE &= ~(1 << PE5);  // Apagar chispa
}

ISR(TIMER5_COMPA_vect) {
  TCCR5B = 0;  // Apagar Timer5
  TIMSK5 &= ~(1 << OCIE5A);
  PORTE |= (1 << PE5);  // Encender chispa
  // Activar Timer1 para el tiempo de encendido de la chispa
  iniciarTimer1(dwell);  //de 1000us a 7000us
}


void iniciarTimer1(unsigned long tiempoMicrosegundos) {
  cli();  // Deshabilitar interrupciones

  TIMSK1 &= ~(1 << OCIE1A);
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  unsigned long ticks = tiempoMicrosegundos * 16;  // Convertir µs a ticks
  if (ticks < 1) ticks = 1;                        // Evitar valores cero
  if (ticks <= 65535) {
    OCR1A = ticks;
    TCCR1B = (1 << WGM12) | (1 << CS10);  // Sin prescaler
  } else if (ticks / 8 <= 65535) {
    OCR1A = ticks / 8;
    TCCR1B = (1 << WGM12) | (1 << CS11);  // Prescaler 8
  } else {
    OCR1A = 65535;                        // Máximo permitido
    TCCR1B = (1 << WGM12) | (1 << CS12);  // Prescaler 256
  }

  TIMSK1 |= (1 << OCIE1A);  // Habilitar interrupción
  sei();                    // Habilitar interrupciones globales
}

void iniciarTimer5(unsigned long tiempoMicrosegundos) {
  cli();  // Deshabilitar interrupciones mientras configuramos
  TIMSK5 &= ~(1 << OCIE5A);  // Deshabilitar interrupción
  TCCR5A = 0;
  TCCR5B = 0;
  TCNT5 = 0;
  // Convertir microsegundos a ticks (1 tick = 0.0625 µs con 16MHz sin prescaler)
  unsigned long ticks = tiempoMicrosegundos * 16;

  // Seleccionar el mejor prescaler para asegurar precisión
  if (ticks <= 65535) {
    OCR5A = ticks;
    TCCR5B = (1 << WGM52) | (1 << CS50);  // Sin prescaler (1x)
  } else if (ticks / 8 <= 65535) {
    OCR5A = ticks / 8;
    TCCR5B = (1 << WGM52) | (1 << CS51);  // Prescaler 8
  } else if (ticks / 64 <= 65535) {
    OCR5A = ticks / 64;
    TCCR5B = (1 << WGM52) | (1 << CS51) | (1 << CS50);  // Prescaler 64
  } else if (ticks / 256 <= 65535) {
    OCR5A = ticks / 256;
    TCCR5B = (1 << WGM52) | (1 << CS52);  // Prescaler 256
  } else {
    OCR5A = ticks / 1024;
    TCCR5B = (1 << WGM52) | (1 << CS52) | (1 << CS50);  // Prescaler 1024
  }

  TIMSK5 |= (1 << OCIE5A);  // Habilitar interrupción
  sei();                    // Habilitar interrupciones globales
}

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

      if (cilindro == 3) {     //cilindro==1 primer diente de la rueda.
        long avanceRpm = 0;  // si es cero no hay avance por rpm si el valor es 500 se adelanta algo.
        int avanceFijo = 0;    //avance fijo o inicial .de 0 a 120
        float vance = 1.0 - (avanceFijo / 120.0);
        long tiempoAjustado = (2 + vance) * tiempoEntreDientes - (dwell + avanceRpm);
        if (tiempoAjustado < 0) tiempoAjustado = 0;
        iniciarTimer5(tiempoAjustado);  // Se usa Timer 5 en lugar de Timer 3
      }
    }
  }
  tiempoAnteriorDiente = tiempoEntreDientes;
}
void setup() {
  pinMode(18, INPUT_PULLUP);
  pinMode(3, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(18), detectarDiente, RISING);
  Serial.begin(2000000);
}

void loop() {
  Serial.println(posi);  // carriage return after the last label
}
