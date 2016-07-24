#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include <ESP8266mDNS.h>
#include <Hash.h>

WebSocketsServer webSocket = WebSocketsServer(81);

const char *ssid = "RemoteControl";
const char *password = "remotecontrol";

#define STS_PIN 4
#define LED_0  5

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) {

  switch(type) {
    case WStype_DISCONNECTED:
    Serial.printf("[%u] Disconnected!\n", num);
    digitalWrite(STS_PIN, LOW);
    break;
    case WStype_CONNECTED:
    {
      IPAddress ip = webSocket.remoteIP(num);
      Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
      digitalWrite(STS_PIN, HIGH);
      webSocket.sendTXT(num, "Connected");
    }
    break;
    case WStype_TEXT:
    Serial.printf("header: %d\n", payload[0]);
    Serial.printf("gpio: %d\n", payload[1]);
    Serial.printf("mode: %d\n", payload[2]);
    Serial.printf("state: %d\n", payload[3]);
    Serial.printf("footer: %d\n", payload[4]);
    command_handle(payload);
    break;
    case WStype_BIN:
    Serial.printf("[%u] get binary lenght: %u\n", num, lenght);
    hexdump(payload, lenght);
    break;
  }
}

void command_handle(uint8_t *payload) {

  if((payload[0]==0xaa) && (payload[4]==0x55)){

    switch (payload[2]) {
      case 0x00:
        digitalWrite(payload[1], payload[3]);
      break;
      case 0x01:
        //PWM
      break;
      default:
      break;
    }
  } else {
    Serial.printf("Invalid command");
  }
}

void setup() {

  Serial.begin(115200);

  pinMode(STS_PIN, OUTPUT);
  pinMode(LED_0, OUTPUT);
  digitalWrite(STS_PIN, LOW);
  digitalWrite(LED_0, LOW);

  WiFi.softAP(ssid, password);

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop() {
  webSocket.loop();
}
