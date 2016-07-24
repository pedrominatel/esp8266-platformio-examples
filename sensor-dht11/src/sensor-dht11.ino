#include "sensor-shield.h"
#include "DHT.h"

//Define o tipo do sensor DHT11 ou DHT22
#define DHTTYPE DHT11

//Configura o sensor DHT com o pino de dados e o tipo de sensor
DHT dht(SENSOR_DHT, DHTTYPE);

unsigned long currentMillis = 0;
long previousMillis = 0;        // Variável de controle do tempo
long redLedInterval = 3000;     // Tempo em ms do intervalo a ser executado

void setup(void) {

  //Configura a serial/uart0 para 115200bps
  Serial.begin(115200);

  //Inicia a configuracao do sensor DHT
  dht.begin();

}

void read_sensor(void){
  //Leitura de umidade
  float umidade = dht.readHumidity();
  //Leitura de temperatura
  float temperatura = dht.readTemperature();

  //Se não for um numero retorna erro de leitura
  if (isnan(umidade) || isnan(temperatura)) {
     Serial.println("Erro ao ler o sensor!");
     return;
  }

  //Escreve na serial os dados lidos do sensor
  Serial.print("Umidade: ");
  Serial.println(umidade);
  Serial.print("Temperatura: ");
  Serial.println(temperatura);
}

void loop(void) {

  currentMillis = millis();    //Tempo atual em ms

  //Lógica de verificação do tempo
  if (currentMillis - previousMillis > redLedInterval) {
    previousMillis = currentMillis;    // Salva o tempo atual
    //Faz a leitura do sensor
    read_sensor();
  }
}
