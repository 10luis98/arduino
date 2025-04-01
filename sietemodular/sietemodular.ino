// main.ino

#include "timers.h"
#include "deteccion.h"

void setup() {


  
    pinMode(18, INPUT_PULLUP);
    pinMode(3, OUTPUT);
    attachInterrupt(digitalPinToInterrupt(18), detectarDiente, RISING);
    Serial.begin(2000000);
}

void loop() {

}