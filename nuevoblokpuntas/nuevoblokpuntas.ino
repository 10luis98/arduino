#define BOBINA_1 52
#define BOBINA_2 38
#define BOBINA_3 40
#define BOBINA_4 11
#define BOBINA_5 10
#define BOBINA_6 45

#define PIN_RUEDA 18  // Pin de entrada para leer los dientes de la rueda

// Variables
volatile unsigned long tiempoAnterior = 0;
volatile unsigned long tiempoEntreDientes = 1000;
volatile unsigned long tiempoAnteriorDiente = 0;
volatile bool dienteReferenciaDetectado = false;
volatile bool flancoDetectado = false;
volatile int cilindro = 0;
volatile int bobina = 0;
volatile long tiempoAjustado = 0;
volatile bool enEjecucion = false;
unsigned long tiempoDeActivacion = 0;  // Para controlar el apagado de la bobina
unsigned long tiempoDePostergacion = 2;  // 2ms de postergación

void setup() {
  // Configuración de los pines de las bobinas
  pinMode(BOBINA_1, OUTPUT);
  pinMode(BOBINA_2, OUTPUT);
  pinMode(BOBINA_3, OUTPUT);
  pinMode(BOBINA_4, OUTPUT);
  pinMode(BOBINA_5, OUTPUT);
  pinMode(BOBINA_6, OUTPUT);

  // Configurar el pin de la rueda (pin 18) como entrada
  pinMode(PIN_RUEDA, INPUT);

  // Inicialización de variables
  tiempoAnterior = micros();
  tiempoAnteriorDiente = 0;
}

void loop() {
  // Leemos el estado del pin 18 para detectar el flanco de la rueda
  bool estadoRueda = digitalRead(PIN_RUEDA);  // Detecta el estado del pin 18
  
  // Verificar si hay un cambio de estado (flanco) en el pin
  if (estadoRueda == HIGH && !flancoDetectado) {  // Flanco de subida
    flancoDetectado = true;
    detectarDiente();
  } else if (estadoRueda == LOW && flancoDetectado) {  // Flanco de bajada
    flancoDetectado = false;
  }

  // Si la ejecución está en proceso, controlar el apagado de la bobina
  if (enEjecucion && micros() >= tiempoDeActivacion) {
    apagarBobina(bobina);
    enEjecucion = false;  // Marcamos que la ejecución ha terminado, listo para el siguiente diente
  }
}

// Función para detectar el diente y encender la bobina correspondiente
void detectarDiente() {
  unsigned long tiempoActual = micros();
  tiempoEntreDientes = tiempoActual - tiempoAnterior;
  tiempoAnterior = tiempoActual;

  // Si la diferencia de tiempos es grande, se detecta el diente de referencia
  if (tiempoAnteriorDiente > tiempoEntreDientes * 2) {
    cilindro = 0;  // Reestablecer el cilindro
    dienteReferenciaDetectado = true;  // Marcamos que se detectó un diente de referencia
  } else {
    // Si se detectó un diente de referencia, avanzamos el cilindro
    if (dienteReferenciaDetectado) {
      cilindro++;
      if (cilindro > 6) {
        cilindro = 0;  // Reset del cilindro
        dienteReferenciaDetectado = false;  // Se desactiva el diente de referencia
      }

      // Si aún no está en ejecución, se activa la bobina correspondiente
      if (!enEjecucion) {
        enEjecucion = true;  // Marcamos que está en ejecución
        
        // Establecemos la bobina a activar según el cilindro
        bobina = cilindro - 1;

        // Ajuste: Restamos 2ms del tiempo entre dientes para activarlo antes de tiempo
        tiempoDeActivacion = tiempoActual + tiempoEntreDientes - 2000;  // Restamos 2ms (2000 microsegundos)

        // Activamos la bobina
        activarBobina(bobina);
      }
    }
  }

  // Actualizamos el tiempo del último diente
  tiempoAnteriorDiente = tiempoEntreDientes;
}

// Función para activar la bobina
void activarBobina(int bobina) {
  switch (bobina) {
    case 0:
      digitalWrite(BOBINA_1, HIGH);
      break;
    case 1:
      digitalWrite(BOBINA_2, HIGH);
      break;
    case 2:
      digitalWrite(BOBINA_3, HIGH);
      break;
    case 3:
      digitalWrite(BOBINA_4, HIGH);
      break;
    case 4:
      digitalWrite(BOBINA_5, HIGH);
      break;
    case 5:
      digitalWrite(BOBINA_6, HIGH);
      break;
  }
}

// Función para apagar la bobina
void apagarBobina(int bobina) {
  switch (bobina) {
    case 0:
      digitalWrite(BOBINA_1, LOW);
      break;
    case 1:
      digitalWrite(BOBINA_2, LOW);
      break;
    case 2:
      digitalWrite(BOBINA_3, LOW);
      break;
    case 3:
      digitalWrite(BOBINA_4, LOW);
      break;
    case 4:
      digitalWrite(BOBINA_5, LOW);
      break;
    case 5:
      digitalWrite(BOBINA_6, LOW);
      break;
  }
}

