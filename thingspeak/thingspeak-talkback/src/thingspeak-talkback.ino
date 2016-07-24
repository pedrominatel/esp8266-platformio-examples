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
#include "sensor-shield.h"

//Definir o SSID da rede WiFi
const char* ssid = "SMART";
//Definir a senha da rede WiFi
const char* password = "redeiot123";

//Colocar a API Key para escrita neste campo
//Ela é fornecida no canal que foi criado na aba API Keys
String apiKey = "L72UYKRKK69O0AXR";
const char* server = "api.thingspeak.com";
String talkbackId = "2388";

WiFiClient client;

long previousMillis = 0;        // Variável de controle do tempo
long redLedInterval = 2000;     // Tempo em ms do intervalo a ser executado
unsigned long currentMillis = 0;

void setup() {

  //Configura a GPIO como saida
  pinMode(LED_R, OUTPUT);
  //Coloca a GPIO em sinal logico baixo
  digitalWrite(LED_R, LOW);

  //Configura a GPIO como saida
  pinMode(RELE, OUTPUT);
  //Coloca a GPIO em sinal logico baixo
  digitalWrite(RELE, LOW);

  //Configuração da UART
  Serial.begin(115200);

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

void le_dados(void){

  //Inicia um client TCP para a leitura dos dados
  if (client.connect(server,80)) {

     client.print("GET /talkbacks/"+talkbackId+"/commands/execute?api_key="+apiKey+" HTTP/1.1\r\n");
     client.print("Host: api.thingspeak.com\n");
     client.print("Accept: */*\r\n");
     client.print("User-Agent: Mozilla/4.0 (compatible; esp8266; Windows NT 5.1)\r\n");
     client.print("\n\n");

  }

  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }

String resp = "";
  // Read all the lines of the reply from server and print them to Serial
while(client.available()){
  resp = client.readStringUntil('\r');
  Serial.print(resp);
}

//Trata a string do cliente em busca de comandos
if (resp.indexOf("rele_on") != -1)
  digitalWrite(RELE, HIGH);
else if (resp.indexOf("rele_off") != -1)
  digitalWrite(RELE, LOW);
else if (resp.indexOf("led_on") != -1)
  digitalWrite(LED_R, HIGH);
else if (resp.indexOf("led_off") != -1)
  digitalWrite(LED_R, LOW);
else {
  Serial.println("Requisicao invalida");
  client.stop();
}

  client.stop();

}

void loop() {

  currentMillis = millis();    //Tempo atual em ms

  //Lógica de verificação do tempo
  if (currentMillis - previousMillis > redLedInterval) {
    previousMillis = currentMillis;    // Salva o tempo atual
    //Faz a leitura do sensor
    le_dados();
  }
}
