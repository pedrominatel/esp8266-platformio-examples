#include "sensor-shield.h"
#include <ESP8266WiFi.h>

int sleep = 10; //tempo em segundos de sleep

void setup(void) {

  Serial.begin(115200);

  Serial.println("Exemplo de sleep-mode");

  //Configura o pino digital para saida/output
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
  //Coloca todos os pinos do LED com sinal baixo
  digitalWrite(LED_R, LOW);
  digitalWrite(LED_G, LOW);

  Serial.println("Led Azul por 5 segundos");

  digitalWrite(LED_B, HIGH);
  delay(5000);
  digitalWrite(LED_B, LOW);

  Serial.println("Entrando em light sleep-mode por 10 segundos...");

  //Forca o desligamento do radio wifi
  WiFi.forceSleepBegin();

  //delay de 10 segundos
  delay(10000);

  Serial.println("Reset do ESP");
  //reset do ESP para sair do sleep mode
  ESP.reset();

}

void loop(void) {

}
