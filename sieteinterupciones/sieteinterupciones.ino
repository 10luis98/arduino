#define BOBINA1 45  //diente 2
#define BOBINA2 38  //diente 3
#define BOBINA3 40  //diente 4
#define BOBINA4 11  //diente 5
#define BOBINA5 10  //diente 6
#define BOBINA6 52  //diente 1

// {&PORTL, PL4},  // Pin 45 (IGN6)1punta
// {&PORTD, PD7},  // Pin 38 (IGN2)2punta
// {&PORTG, PG1},  // Pin 40 (IGN3)3punta
// {&PORTB, PB5},  // Pin 11 (IGN4)4punta
// {&PORTB, PB4},  // Pin 10 (IGN5)5punta
// {&PORTB, PB1},  // Pin 52 (IGN1)6punta

unsigned long lastTask1 = 0;
unsigned long lastTask2 = 0;
unsigned long lastTask3 = 0;
unsigned long lastTask4 = 0;
volatile unsigned long monitor1 = 0;
volatile unsigned long monitor2 = 0;
volatile unsigned long tiempoControl = 0;
volatile unsigned long divPrescaler = 4;
volatile unsigned long avanceRpm = 0;

volatile unsigned long tiempoReferenciaAnterior = 0;
volatile unsigned long algo = 0;

volatile unsigned long tiempoCicloMotor = 0;

volatile unsigned long retardoBobina1 = 1250;


volatile bool bobina1_primera_vez = true;
volatile bool bobina2_primera_vez = true;
volatile bool bobina3_primera_vez = true;
volatile bool bobina4_primera_vez = true;
volatile bool bobina5_primera_vez = true;
volatile bool bobina6_primera_vez = true;

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
volatile bool dienteActivo = false;


volatile unsigned int tiempo_encendido = 2000;  // microsegundo

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
  pinMode(19, INPUT_PULLUP);


  TCCR3A = 0;
  TCCR3B = (1 << CS31) | (1 << CS30);  // Prescaler 64
  TCNT3 = 0;

  TCCR4A = 0;
  TCCR4B = (1 << CS41) | (1 << CS40);  // Prescaler 64
  TCNT4 = 0;

  attachInterrupt(digitalPinToInterrupt(19), detectarDiente, RISING);  // Flanco ascendente
}

void detectarDiente() {
  flancoDetectado = true;
  unsigned long tiempoActual = micros();
  tiempoEntreDientes = tiempoActual - tiempoAnterior;
  tiempoAnterior = tiempoActual;
  if (tiempoAnteriorDiente > tiempoEntreDientes * 2) {
    cilindro = 0;
    dienteReferenciaDetectado = true;
    dienteActivo = true;
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
      switch (cilindro) {
        case 1:
          // Configura comparación para Timer3, canal A (BOBINA1)
          OCR3A = TCNT3 + 1;        // Ejecutar ISR casi de inmediato
          TIFR3 |= (1 << OCF3A);    // Limpia flag por si estaba pendiente
          TIMSK3 |= (1 << OCIE3A);  // Habilita interrupción de comparación A
          break;

        case 2:
          // Timer3, canal B (BOBINA2)
          OCR3B = TCNT3 + 1;        // Retardo deseado
          TIFR3 |= (1 << OCF3B);    // Limpia flag
          TIMSK3 |= (1 << OCIE3B);  // Habilita interrupción
          break;

        case 3:
          // Timer3, canal C (BOBINA3)
          OCR3C = TCNT3 + 1;        // Ejecutar inmediatamente
          TIFR3 |= (1 << OCF3C);    // Limpia flag
          TIMSK3 |= (1 << OCIE3C);  // Habilita interrupción
          break;

        case 4:
          // Timer4, canal A (BOBINA4)
          OCR4A = TCNT4 + 1;
          TIFR4 |= (1 << OCF4A);
          TIMSK4 |= (1 << OCIE4A);
          break;

        case 5:
          // Timer4, canal B (BOBINA5)
          OCR4B = TCNT4 + 1;
          TIFR4 |= (1 << OCF4B);
          TIMSK4 |= (1 << OCIE4B);
          break;

        case 6:
          // Timer4, canal C (BOBINA6)
          OCR4C = TCNT4 + 1;
          TIFR4 |= (1 << OCF4C);
          TIMSK4 |= (1 << OCIE4C);
          break;
      }
    }
  }
  monitor2 = tiempoEntreDientes;
  tiempoAnteriorDiente = tiempoEntreDientes;
}

ISR(TIMER3_COMPA_vect) {
  if (bobina1_primera_vez) {
    bobina1_primera_vez = false;
    OCR3A = TCNT3 + ((tiempoEntreDientes - avanceRpm) / divPrescaler) - (tiempo_encendido / divPrescaler);
    return;
  }
  if (!bobina1_encendida) {
    digitalWrite(BOBINA1, HIGH);
    bobina1_encendida = true;
    OCR3A = TCNT3 + (tiempo_encendido / divPrescaler);
  } else {
    digitalWrite(BOBINA1, LOW);
    bobina1_encendida = false;
    bobina1_primera_vez = true;
    TIMSK3 &= ~(1 << OCIE3A);  // Deshabilita para evitar nueva activación automática
  }
}
ISR(TIMER3_COMPB_vect) {

  if (bobina2_primera_vez) {
    bobina2_primera_vez = false;
    OCR3B = TCNT3 + ((tiempoEntreDientes - avanceRpm) / divPrescaler) - (tiempo_encendido / divPrescaler);
    return;
  }
  if (!bobina2_encendida) {
    digitalWrite(BOBINA2, HIGH);
    bobina2_encendida = true;
    OCR3B = TCNT3 + (tiempo_encendido / divPrescaler);
  } else {
    digitalWrite(BOBINA2, LOW);
    bobina2_encendida = false;
    bobina2_primera_vez = true;
    TIMSK3 &= ~(1 << OCIE3B);
  }
}

ISR(TIMER3_COMPC_vect) {
  if (bobina3_primera_vez) {
    bobina3_primera_vez = false;
    OCR3C = TCNT3 + ((tiempoEntreDientes - avanceRpm) / divPrescaler) - (tiempo_encendido / divPrescaler);
    return;
  }
  if (!bobina3_encendida) {
    digitalWrite(BOBINA3, HIGH);
    bobina3_encendida = true;
    OCR3C = TCNT3 + (tiempo_encendido / divPrescaler);
  } else {
    digitalWrite(BOBINA3, LOW);
    bobina3_encendida = false;
    bobina3_primera_vez = true;
    TIMSK3 &= ~(1 << OCIE3C);
  }
}

// Timer4 - ISR BOBINAS 4 a 6
ISR(TIMER4_COMPA_vect) {

  if (bobina4_primera_vez) {
    bobina4_primera_vez = false;
    OCR4A = TCNT4 + ((tiempoEntreDientes - avanceRpm) / divPrescaler) - (tiempo_encendido / divPrescaler);
    return;
  }
  if (!bobina4_encendida) {
    digitalWrite(BOBINA4, HIGH);
    bobina4_encendida = true;
    OCR4A = TCNT4 + (tiempo_encendido / divPrescaler);
  } else {
    digitalWrite(BOBINA4, LOW);
    bobina4_encendida = false;
    bobina4_primera_vez = true;
    TIMSK4 &= ~(1 << OCIE4A);
  }
}

ISR(TIMER4_COMPB_vect) {

  if (bobina5_primera_vez) {
    bobina5_primera_vez = false;
    OCR4B = TCNT4 + ((tiempoEntreDientes - avanceRpm) / divPrescaler) - (tiempo_encendido / divPrescaler);
    return;
  }
  if (!bobina5_encendida) {
    digitalWrite(BOBINA5, HIGH);
    bobina5_encendida = true;
    OCR4B = TCNT4 + (tiempo_encendido / divPrescaler);
  } else {
    digitalWrite(BOBINA5, LOW);
    bobina5_encendida = false;
    bobina5_primera_vez = true;
    TIMSK4 &= ~(1 << OCIE4B);
  }
}

ISR(TIMER4_COMPC_vect) {
  if (bobina6_primera_vez) {
    bobina6_primera_vez = false;
    OCR4C = TCNT4 + ((tiempoEntreDientes - avanceRpm) / divPrescaler) - (tiempo_encendido / divPrescaler);
    return;
  }
  if (!bobina6_encendida) {
    digitalWrite(BOBINA6, HIGH);
    bobina6_encendida = true;
    OCR4C = TCNT4 + (tiempo_encendido / divPrescaler);
  } else {
    digitalWrite(BOBINA6, LOW);
    bobina6_encendida = false;
    bobina6_primera_vez = true;
    TIMSK4 &= ~(1 << OCIE4C);
  }
}

void loop() {

//   long rpm;
//   long algo;

//   if (dienteActivo) {
//     rpm = ((60L * 1000000L) / tiempoCicloMotor) * 2;
//     algo = customMap(rpm, 500, 1000, 2000, 5500);
//     algo = constrain(algo, 2000, 5500);
//   }
// //fall esto la rpm 
//   // Si ya pasaron más de 1.5 segundos sin nuevos pulsos
//   if (millis() - tiempoCicloMotor > 1500) {
//     tiempoCicloMotor=0;
//     dienteActivo = false;
//     rpm = 0;
//     algo = 0;
//   }

//   if (millis() - lastTask4 > 100) {
//     lastTask4 = millis();
//     avanceRpm = algo;
//     monitor1 = algo;

//     Serial.print("RPM: ");
//     Serial.print(rpm);
//     Serial.print(" | Monitor1: ");
//     Serial.print(algo);
//     Serial.print(" | Monitor2: ");
//     Serial.println(tiempoCicloMotor);
//   }
}


long customMap(long x, long in_min, long in_max, long out_min, long out_max) {
  if (x < in_min) x = in_min;  // Limitar valor de entrada si está por debajo del rango
  if (x > in_max) x = in_max;  // Limitar valor de entrada si está por encima del rango
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}