#include <ESP8266WiFi.h>
#include <sensor-shield.h>

//Definir o SSID da rede WiFi
const char* ssid = "SMART";
//Definir a senha da rede WiFi
const char* password = "redeiot123";

//Inicializacao so servidor http na porta 80
WiFiServer server(80);

void setup() {

  //Inicia a comunicacao serial
  Serial.begin(115200);

  //Configura a GPIO como saida
  pinMode(LED_R, OUTPUT);
  //Coloca a GPIO em sinal logico baixo
  digitalWrite(LED_R, LOW);

  //Configura a GPIO como saida
  pinMode(RELE, OUTPUT);
  //Coloca a GPIO em sinal logico baixo
  digitalWrite(RELE, LOW);

  //
  Serial.print("Conectando...");

  //Configura o WiFi
  WiFi.begin(ssid, password);

  //Espera até conexao
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado!");

  //Inicia o webserver
  server.begin();
  Serial.println("Server iniciado");
  //Mostra o IP
  Serial.println(WiFi.localIP());
}

void loop() {

  //Aguarda uma nova conexao
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  Serial.println("new client");

  while(!client.available()){
    delay(1);
  }

  //Le a string enviada pelo cliente
  String req = client.readStringUntil('\r');
  //Mostra a string enviada
  Serial.println(req);
  //Limpa dados/buffer
  client.flush();

  //Prepara a resposta para o cliente
  String buf = "";
  buf += "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n";
  buf += "<head><title>WebServer ESP8266</title></head>";
  buf += "<h3> ESP8266 Web Server</h3>";
  buf += "<p>Rele <a href=\"?function=rele_on\"><button>ON</button></a> <a href=\"?function=rele_off\"><button>OFF</button></a></p>";
  buf += "<p>LED Vermelho <a href=\"?function=led_on\"><button>ON</button></a> <a href=\"?function=led_off\"><button>OFF</button></a></p>";
  buf += "<h4>Criado por Pedro Minatel</h4>";
  buf += "</html>\n";

  //Envia a resposta para o cliente
  client.print(buf);
  //Limpa dados/buffer
  client.flush();

  //Trata a string do cliente em busca de comandos
  if (req.indexOf("rele_on") != -1)
    digitalWrite(RELE, HIGH);
  else if (req.indexOf("rele_off") != -1)
    digitalWrite(RELE, LOW);
  else if (req.indexOf("led_on") != -1)
    digitalWrite(LED_R, HIGH);
  else if (req.indexOf("led_off") != -1)
    digitalWrite(LED_R, LOW);
  else {
    Serial.println("Requisicao invalida");
    client.stop();
  }
  Serial.println("Client disconectado!");
}
