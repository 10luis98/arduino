#define BOBINA1 2
#define BOBINA2 3
#define BOBINA3 5
#define BOBINA4 6
#define BOBINA5 7
#define BOBINA6 8

// Tiempos para prescaler 64 (1 tick ≈ 4 µs a 16 MHz)
#define TIEMPO_ENCENDIDO 500   // 2 ms
#define TIEMPO_ESPERA 2500     // 10 ms

volatile bool bobina1_encendida = false;
volatile bool bobina2_encendida = false;
volatile bool bobina3_encendida = false;
volatile bool bobina4_encendida = false;
volatile bool bobina5_encendida = false;
volatile bool bobina6_encendida = false;

volatile bool flancoDetectado = false;
volatile unsigned long tiempoAnterior = 0;
volatile unsigned long tiempoEntreDientes = 0;
volatile unsigned long tiempoAnteriorDiente = 0;
volatile int cilindro = 0;
volatile bool dienteReferenciaDetectado = false;

void setup() {
  Serial.begin(115200);

  pinMode(BOBINA1, OUTPUT);
  pinMode(BOBINA2, OUTPUT);
  pinMode(BOBINA3, OUTPUT);
  pinMode(BOBINA4, OUTPUT);
  pinMode(BOBINA5, OUTPUT);
  pinMode(BOBINA6, OUTPUT);

  digitalWrite(BOBINA1, LOW);
  digitalWrite(BOBINA2, LOW);
  digitalWrite(BOBINA3, LOW);
  digitalWrite(BOBINA4, LOW);
  digitalWrite(BOBINA5, LOW);
  digitalWrite(BOBINA6, LOW);

  cli(); // Deshabilitar interrupciones globales

  // ----------------------
  // Timer3 - BOBINAS 1 a 3
  // ----------------------
  TCCR3A = 0;
  TCCR3B = (1 << CS31) | (1 << CS30); // Prescaler 64, modo normal

  TCNT3 = 0;
  OCR3A = TCNT3 + TIEMPO_ESPERA;
  OCR3B = TCNT3 + TIEMPO_ESPERA;
  OCR3C = TCNT3 + TIEMPO_ESPERA;

  TIMSK3 = (1 << OCIE3A) | (1 << OCIE3B) | (1 << OCIE3C);

  // ----------------------
  // Timer4 - BOBINAS 4 a 6
  // ----------------------
  TCCR4A = 0;
  TCCR4B = (1 << CS41) | (1 << CS40); // Prescaler 64, modo normal

  TCNT4 = 0;
  OCR4A = TCNT4 + TIEMPO_ESPERA;
  OCR4B = TCNT4 + TIEMPO_ESPERA;
  OCR4C = TCNT4 + TIEMPO_ESPERA;

  TIMSK4 = (1 << OCIE4A) | (1 << OCIE4B) | (1 << OCIE4C);

  // Configuración de la interrupción para detectar el diente en el pin 18
  attachInterrupt(digitalPinToInterrupt(18), detectarDiente, RISING); // Configura para detectar el flanco ascendente

  sei(); // Habilitar interrupciones globales
}

// Función de interrupción para detectar el diente
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

      // Se activa la interrupción correspondiente para el cilindro
      switch(cilindro) {
        case 1:
          TIMSK3 |= (1 << OCIE3A); // Habilitar interrupción para bobina 1
          break;
        case 2:
          TIMSK3 |= (1 << OCIE3B); // Habilitar interrupción para bobina 2
          break;
        case 3:
          TIMSK3 |= (1 << OCIE3C); // Habilitar interrupción para bobina 3
          break;
        case 4:
          TIMSK4 |= (1 << OCIE4A); // Habilitar interrupción para bobina 4
          break;
        case 5:
          TIMSK4 |= (1 << OCIE4B); // Habilitar interrupción para bobina 5
          break;
        case 6:
          TIMSK4 |= (1 << OCIE4C); // Habilitar interrupción para bobina 6
          break;
      }
    }
  }
  tiempoAnteriorDiente = tiempoEntreDientes;
}

ISR(TIMER3_COMPA_vect) {
  if (!bobina1_encendida) {
    digitalWrite(BOBINA1, HIGH);
    bobina1_encendida = true;
    OCR3A = TCNT3 + TIEMPO_ENCENDIDO;
  } else {
    digitalWrite(BOBINA1, LOW);
    bobina1_encendida = false;
    OCR3A = TCNT3 + TIEMPO_ESPERA;
    TIMSK3 &= ~(1 << OCIE3A); // Deshabilitar la interrupción de bobina 1
  }
}

ISR(TIMER3_COMPB_vect) {
  if (!bobina2_encendida) {
    digitalWrite(BOBINA2, HIGH);
    bobina2_encendida = true;
    OCR3B = TCNT3 + TIEMPO_ENCENDIDO;
  } else {
    digitalWrite(BOBINA2, LOW);
    bobina2_encendida = false;
    OCR3B = TCNT3 + TIEMPO_ESPERA;
    TIMSK3 &= ~(1 << OCIE3B); // Deshabilitar la interrupción de bobina 2
  }
}

ISR(TIMER3_COMPC_vect) {
  if (!bobina3_encendida) {
    digitalWrite(BOBINA3, HIGH);
    bobina3_encendida = true;
    OCR3C = TCNT3 + TIEMPO_ENCENDIDO;
  } else {
    digitalWrite(BOBINA3, LOW);
    bobina3_encendida = false;
    OCR3C = TCNT3 + TIEMPO_ESPERA;
    TIMSK3 &= ~(1 << OCIE3C); // Deshabilitar la interrupción de bobina 3
  }
}

ISR(TIMER4_COMPA_vect) {
  if (!bobina4_encendida) {
    digitalWrite(BOBINA4, HIGH);
    bobina4_encendida = true;
    OCR4A = TCNT4 + TIEMPO_ENCENDIDO;
  } else {
    digitalWrite(BOBINA4, LOW);
    bobina4_encendida = false;
    OCR4A = TCNT4 + TIEMPO_ESPERA;
    TIMSK4 &= ~(1 << OCIE4A); // Deshabilitar la interrupción de bobina 4
  }
}

ISR(TIMER4_COMPB_vect) {
  if (!bobina5_encendida) {
    digitalWrite(BOBINA5, HIGH);
    bobina5_encendida = true;
    OCR4B = TCNT4 + TIEMPO_ENCENDIDO;
  } else {
    digitalWrite(BOBINA5, LOW);
    bobina5_encendida = false;
    OCR4B = TCNT4 + TIEMPO_ESPERA;
    TIMSK4 &= ~(1 << OCIE4B); // Deshabilitar la interrupción de bobina 5
  }
}

ISR(TIMER4_COMPC_vect) {
  if (!bobina6_encendida) {
    digitalWrite(BOBINA6, HIGH);
    bobina6_encendida = true;
    OCR4C = TCNT4 + TIEMPO_ENCENDIDO;
  } else {
    digitalWrite(BOBINA6, LOW);
    bobina6_encendida = false;
    OCR4C = TCNT4 + TIEMPO_ESPERA;
    TIMSK4 &= ~(1 << OCIE4C); // Deshabilitar la interrupción de bobina 6
  }
}

void loop() {
  // Todo se controla por interrupciones
}
