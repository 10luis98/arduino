#define MAX_BUFFER 10  // Tamaño máximo del buffer circular

int bufferSize = 1;  // Comienza con 1 posición
int bufferIndex = 0;

volatile int bufferEscritura = 0;  // Índice de escritura
volatile int bufferLectura = 0;    // Índice de lectura
volatile int bufferOcupado = 0;    // Cantidad de elementos en el buffer


volatile int cantidadDatos = 0;  // Número de datos en el buffer

#define TAMANIO_BUFFER 10  // Tamaño del buffer circular

volatile unsigned long bufferDientes[48];  // Buffer circular
volatile int indiceEscritura = 0;                      // Índice donde se escribe
volatile int indiceLectura = 0;                        // Índice desde donde se lee
volatile int cantidadDientes = 15;                      // Cantidad de dientes de retraso


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
volatile unsigned long tiempoSubida = 0;
volatile unsigned long duracionDiente = 0;
//volatile unsigned long bufferDientes[10];  // Buffer para almacenar los tiempos de los dientes

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

ISR(TIMER3_COMPA_vect) {
  // Apagar el Timer 3 después de ejecutarse
  TCCR3B = 0;                // Detener el reloj del Timer 3
  TIMSK3 &= ~(1 << OCIE3A);  // Deshabilitar la interrupción
  digitalWrite(4, HIGH);     // Enciende el pin 4
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

void iniciarTimer3(unsigned long tiempoMicrosegundos) {
  cli();  // Deshabilitar interrupciones mientras configuramos

  TIMSK3 &= ~(1 << OCIE3A);  // Deshabilitar interrupción
  TCCR3A = 0;
  TCCR3B = 0;
  TCNT3 = 0;

  // Convertir microsegundos a ticks (1 tick = 0.0625 µs con 16MHz sin prescaler)
  unsigned long ticks = tiempoMicrosegundos * 16;

  // Seleccionar el mejor prescaler para asegurar precisión
  if (ticks <= 65535) {
    OCR3A = ticks;
    TCCR3B = (1 << WGM32) | (1 << CS30);  // Sin prescaler (1x)
  } else if (ticks / 8 <= 65535) {
    OCR3A = ticks / 8;
    TCCR3B = (1 << WGM32) | (1 << CS31);  // Prescaler 8
  } else if (ticks / 64 <= 65535) {
    OCR3A = ticks / 64;
    TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);  // Prescaler 64
  } else if (ticks / 256 <= 65535) {
    OCR3A = ticks / 256;
    TCCR3B = (1 << WGM32) | (1 << CS32);  // Prescaler 256
  } else {
    OCR3A = ticks / 1024;
    TCCR3B = (1 << WGM32) | (1 << CS32) | (1 << CS30);  // Prescaler 1024
  }

  TIMSK3 |= (1 << OCIE3A);  // Habilitar interrupción
  sei();                    // Habilitar interrupciones globales
}

void detectarDiente() {
  digitalWrite(4, LOW);  // Enciende el pin 4

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

      if (cilindro == 1) {  //cilindro==1 primer diente de la rueda.
        if (cilindro == 1) tiempoEntreDientes += tiempoAnteriorDiente;

        long tiempoAjustado = tiempoEntreDientes - dwell;
        if (tiempoAjustado < 0) tiempoAjustado = 0;
        iniciarTimer5(tiempoAjustado);  // Se usa Timer 5 en lugar de Timer 3
      }
    }
  }
  tiempoAnteriorDiente = tiempoEntreDientes;
}

void tiempoDiente() {
  unsigned long tiempoActual = micros();

  if (digitalRead(18) == HIGH) {  // Flanco de subida
    tiempoSubida = tiempoActual;
  } else {  // Flanco de bajada
    unsigned long duracionDiente = tiempoActual - tiempoSubida;

    // Guardar en el buffer circular
    bufferDientes[indiceEscritura] = duracionDiente;

    // Avanzar el índice de escritura de forma circular
    indiceEscritura = (indiceEscritura + 1) % TAMANIO_BUFFER;

    // Si el buffer está lleno, se pierde el dato más antiguo (como en un buffer serial)
    if (cantidadDatos < TAMANIO_BUFFER) {
      cantidadDatos++;  // Solo incrementamos si hay espacio
    } else {
      // Si el buffer se llena, avanzamos el índice de lectura para no perder datos nuevos
      indiceLectura = (indiceLectura + 1) % TAMANIO_BUFFER;
    }

    // Si hay suficientes datos en el buffer para cumplir el retraso
    if (cantidadDientes <= cantidadDatos) {
      int indiceRetrasado = (indiceLectura + cantidadDientes - 1) % TAMANIO_BUFFER;

      // Iniciar el timer con el valor almacenado en ese índice
      iniciarTimer3(bufferDientes[indiceRetrasado]);

      // Marcar el dato como leído avanzando el índice de lectura
      indiceLectura = (indiceLectura + 1) % TAMANIO_BUFFER;
      cantidadDatos--;  // Disminuir el número de datos pendientes
    }
  }
}

void tiempoDientegg() {
  unsigned long tiempoActual = micros();
  if (digitalRead(18) == HIGH) {  // Detecta flanco de subida
    tiempoSubida = tiempoActual;
  } else {  // Detecta flanco de bajada
    duracionDiente = tiempoActual - tiempoSubida;

    int cantidadDientes = 1.9;
    int indiceBuffer = 0;
    if (cantidadDientes > 1.9) {

    } else {
      bufferDientes[0] = duracionDiente;
    }
    bufferDientes[indiceBuffer] = duracionDiente;

    if (1.9 < cantidadDientes) {
      iniciarTimer3(cantidadDientes * bufferDientes[indiceBuffer]);
    }
  }
}
void setup() {
  pinMode(18, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);

  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);


  attachInterrupt(digitalPinToInterrupt(18), tiempoDiente, CHANGE);
  attachInterrupt(digitalPinToInterrupt(2), detectarDiente, RISING);
  Serial.begin(2000000);
}

void loop() {
  Serial.println(posi);
}