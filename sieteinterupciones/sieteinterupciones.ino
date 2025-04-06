#define BOBINA1 45 //diente 2
#define BOBINA2 38 //diente 3
#define BOBINA3 40//diente 4
#define BOBINA4 11//diente 5
#define BOBINA5 10//diente 6
#define BOBINA6 52//diente 1

    // {&PORTL, PL4},  // Pin 45 (IGN6)1punta
    // {&PORTD, PD7},  // Pin 38 (IGN2)2punta
    // {&PORTG, PG1},  // Pin 40 (IGN3)3punta
    // {&PORTB, PB5},  // Pin 11 (IGN4)4punta
    // {&PORTB, PB4},  // Pin 10 (IGN5)5punta
    // {&PORTB, PB1},  // Pin 52 (IGN1)6punta
volatile unsigned long monitor = 0;
volatile unsigned long tiempoReferenciaAnterior = 0;
volatile unsigned long algo = 0;

volatile unsigned long tiempoCicloMotor = 0;
volatile bool bobina1_primera_vez = true;

volatile unsigned long retardoBobina1 = 1250;

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

volatile unsigned int tiempo_encendido = 500;  // valor inicial en ticks
volatile unsigned int tiempo_espera = 2500;     // valor inicial en ticks

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

  cli();  // Deshabilitar interrupciones globales

  // Timer3 - BOBINAS 1 a 3
  TCCR3A = 0;
  TCCR3B = (1 << CS31) | (1 << CS30);  // Prescaler 64

  TCNT3 = 0;
  OCR3A = TCNT3 + tiempo_espera;
  OCR3B = TCNT3 + tiempo_espera;
  OCR3C = TCNT3 + tiempo_espera;

  TIMSK3 = (1 << OCIE3A) | (1 << OCIE3B) | (1 << OCIE3C);

  // Timer4 - BOBINAS 4 a 6
  TCCR4A = 0;
  TCCR4B = (1 << CS41) | (1 << CS40);  // Prescaler 64

  TCNT4 = 0;
  OCR4A = TCNT4 + tiempo_espera;
  OCR4B = TCNT4 + tiempo_espera;
  OCR4C = TCNT4 + tiempo_espera;

  TIMSK4 = (1 << OCIE4A) | (1 << OCIE4B) | (1 << OCIE4C);

  attachInterrupt(digitalPinToInterrupt(18), detectarDiente, RISING);  // Flanco ascendente

  sei();  // Habilitar interrupciones globales
}

void detectarDiente() {
  flancoDetectado = true;
  unsigned long tiempoActual = micros();
  tiempoEntreDientes = tiempoActual - tiempoAnterior;
  tiempoAnterior = tiempoActual;

  if (tiempoAnteriorDiente > tiempoEntreDientes * 2) {
    cilindro = 0;
    dienteReferenciaDetectado = true;
    // Guardamos el tiempo de referencia para calcular en loop()
    tiempoCicloMotor = tiempoActual - tiempoReferenciaAnterior;
    tiempoReferenciaAnterior = tiempoActual;
  } else {
    if (dienteReferenciaDetectado) {
      cilindro++;
      if (cilindro > 6) {
        cilindro = 0;
        dienteReferenciaDetectado = false;
      }
      if (cilindro == 1) tiempoEntreDientes += tiempoAnteriorDiente;
      //monitor = tiempoEntreDientes;

      switch (cilindro) {
        case 1:
          OCR3A = TCNT3 + (tiempoEntreDientes / 4 -algo) - (2000 / 4);
          TIMSK3 |= (1 << OCIE3A);
          break;
        case 2:

          OCR3B = TCNT3 + (tiempoEntreDientes / 4 ) - (2000 / 4);
          TIMSK3 |= (1 << OCIE3B);
          break;
        case 3:
          OCR3C = TCNT3 + (tiempoEntreDientes / 4 ) - (2000 / 4);
          TIMSK3 |= (1 << OCIE3C);
          break;
        case 4:
          OCR4A = TCNT4 + (tiempoEntreDientes / 4 ) - (2000 / 4);
          TIMSK4 |= (1 << OCIE4A);
          break;
        case 5:
          OCR4B = TCNT4 + (tiempoEntreDientes / 4 ) - (2000 / 4);
          TIMSK4 |= (1 << OCIE4B);
          break;
        case 6:
          OCR4C = TCNT4 + (tiempoEntreDientes / 4 ) - (2000 / 4);
          TIMSK4 |= (1 << OCIE4C);
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
    OCR3A = TCNT3 + tiempo_encendido;
  } else {
    digitalWrite(BOBINA1, LOW);
    bobina1_encendida = false;
    TIMSK3 &= ~(1 << OCIE3A);  // Deshabilita para evitar nueva activación automática
  }
}
ISR(TIMER3_COMPB_vect) {
  if (!bobina2_encendida) {
    digitalWrite(BOBINA2, HIGH);
    bobina2_encendida = true;
    OCR3B = TCNT3 + tiempo_encendido;
  } else {
    digitalWrite(BOBINA2, LOW);
    bobina2_encendida = false;
    TIMSK3 &= ~(1 << OCIE3B);
  }
}
ISR(TIMER3_COMPC_vect) {
  if (!bobina3_encendida) {
    digitalWrite(BOBINA3, HIGH);
    bobina3_encendida = true;
    OCR3C = TCNT3 + tiempo_encendido;
  } else {
    digitalWrite(BOBINA3, LOW);
    bobina3_encendida = false;
    TIMSK3 &= ~(1 << OCIE3C);
  }
}

// Timer4 - ISR BOBINAS 4 a 6
ISR(TIMER4_COMPA_vect) {
  if (!bobina4_encendida) {
    digitalWrite(BOBINA4, HIGH);
    bobina4_encendida = true;
    OCR4A = TCNT4 + tiempo_encendido;
  } else {
    digitalWrite(BOBINA4, LOW);
    bobina4_encendida = false;
    TIMSK4 &= ~(1 << OCIE4A);
  }
}

ISR(TIMER4_COMPB_vect) {
  if (!bobina5_encendida) {
    digitalWrite(BOBINA5, HIGH);
    bobina5_encendida = true;
    OCR4B = TCNT4 + tiempo_encendido;
  } else {
    digitalWrite(BOBINA5, LOW);
    bobina5_encendida = false;
    TIMSK4 &= ~(1 << OCIE4B);
  }
}

ISR(TIMER4_COMPC_vect) {
  if (!bobina6_encendida) {
    digitalWrite(BOBINA6, HIGH);
    bobina6_encendida = true;
    OCR4C = TCNT4 + tiempo_encendido;
  } else {
    digitalWrite(BOBINA6, LOW);
    bobina6_encendida = false;
    TIMSK4 &= ~(1 << OCIE4C);
  }
}

void loop() {
  static unsigned long ultimaActualizacion = 0;

  if (millis() - ultimaActualizacion > 3) {  // Cada 100 ms
    ultimaActualizacion = millis();
long rpm; // Usa long para evitar problemas con los float

if (tiempoCicloMotor > 0) {
  rpm = ((60L * 1000000L) / tiempoCicloMotor) * 2;  // µs a RPM con enteros largos


    // if (tiempoCicloMotor > 0) {
    //   float rpm = ((60.0 * 1000000.0) / tiempoCicloMotor)*2;  // µs a RPM
  // tiempo_encendido = constrain(4000 - (int)(rpm * 0.5), 1000, 4000);
 // algo = constrain(5000 - (int)(rpm * 3), 500, 4000);
  
  // Aumenta 'algo' con más RPM (ejemplo lineal)

algo = customMap(rpm, 500, 1000, 100, 500);
algo = constrain(algo, 100, 500);  // Limita a entre 1 y 1000
monitor=rpm;
  //  algo = map(rpm, 500, 5000, 10, 1000);
  // algo = constrain(algo, 10, 1000);  // Asegura que esté dentro del rango
                                                          // monitor=algo;

      // tiempo_espera = tiempo_encendido * 4;

      Serial.print("RPM: ");
      Serial.print(2 * rpm);
      Serial.print(" | Encendido: ");
      Serial.print(tiempo_encendido);
      Serial.print(" | Monitor: ");
      Serial.println(monitor);
    }
  }
}
long customMap(long x, long in_min, long in_max, long out_min, long out_max) {
  if (x < in_min) x = in_min;    // Limitar valor de entrada si está por debajo del rango
  if (x > in_max) x = in_max;    // Limitar valor de entrada si está por encima del rango
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}