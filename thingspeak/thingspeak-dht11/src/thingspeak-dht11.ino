/* Pedro Minatel - Sistemas Embarcados
*
* Este programa envia os dados coletados do DHT11 para a plataforma de IoT
* thingspeak.com
* Data: 16 de Setembro de 2015
* Author: Pedro Minatel
* Website: http://pedrominatel.com.br
*/

//Include da lib de Wifi do ESP8266
#include <ESP8266WiFi.h>
//Include da lib do sensor DHT11 e DHT22
#include "DHT.h"
#include "sensor-shield.h"

//Define do pino a ser utilizado no ESP para o sensor = GPIO4
#define DHTTYPE DHT11

//Definir o SSID da rede WiFi
const char* ssid = "SMART";
//Definir a senha da rede WiFi
const char* password = "redeiot123";

//Colocar a API Key para escrita neste campo
//Ela é fornecida no canal que foi criado na aba API Keys
String apiKey = "ATW74GD7XFITBSVX";
const char* server = "api.thingspeak.com";

DHT dht(SENSOR_DHT, DHTTYPE);
WiFiClient client;

long previousMillis = 0;        // Variável de controle do tempo
long redLedInterval = 10000;     // Tempo em ms do intervalo a ser executado
unsigned long currentMillis = 0;

void setup() {
  //Configuração da UART
  Serial.begin(115200);

  //Inicia a configuracao do sensor DHT
  dht.begin();

  Serial.print("Conectando na rede ");
  Serial.println(ssid);

  //Inicia o WiFi
  WiFi.begin(ssid, password);

  //Loop até conectar no WiFi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //Logs na porta serial
  Serial.println("");
  Serial.println("WiFi conectado!");
  Serial.print("Conectado na rede ");
  Serial.println(ssid);
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void envia_dados(void){

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

  //Inicia um client TCP para o envio dos dados
  if (client.connect(server,80)) {

    String postStr = apiKey;
           postStr +="&field1=";
           postStr += String(temperatura);
           postStr +="&field2=";
           postStr += String(umidade);
           postStr += "\r\n\r\n";

     client.print("POST /update HTTP/1.1\n");
     client.print("Host: api.thingspeak.com\n");
     client.print("Connection: close\n");
     client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
     client.print("Content-Type: application/x-www-form-urlencoded\n");
     client.print("Content-Length: ");
     client.print(postStr.length());
     client.print("\n\n");
     client.print(postStr);

  }
  client.stop();

}

void loop() {

  currentMillis = millis();    //Tempo atual em ms

  //Lógica de verificação do tempo
  if (currentMillis - previousMillis > redLedInterval) {
    previousMillis = currentMillis;    // Salva o tempo atual
    //Faz a leitura do sensor
    envia_dados();
  }
}
