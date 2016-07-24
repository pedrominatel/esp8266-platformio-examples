#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include <WebSocketsClient.h>
#include <Hash.h>

#define STATUS_PIN 13
#define BTN 12
#define LED 4

WebSocketsClient webSocket;

uint8_t btn_state = 0;
uint8_t btn_last = 0;
uint8_t *socket_payload;

void webSocketEvent(WStype_t type, uint8_t * payload, size_t lenght) {


    switch(type) {
        case WStype_DISCONNECTED:
            Serial.printf("[WSc] Disconnected!\n");
            break;
        case WStype_CONNECTED:
            {
                Serial.printf("[WSc] Connected to url: %s\n",  payload);
				webSocket.sendTXT("Connected");
            }
            break;
        case WStype_TEXT:
            Serial.printf("[WSc] get text: %s\n", payload);
            break;
        case WStype_BIN:
            Serial.printf("[WSc] get binary lenght: %u\n", lenght);
            hexdump(payload, lenght);
            break;
    }
}

uint8_t * build_payload(uint8_t *payload, uint8_t gpio, uint8_t mode, uint8_t state){
  payload[0] = 0xaa;
  payload[1] = gpio;
  payload[2] = mode;
  payload[3] = state;
  payload[4] = 0x55;
  return payload;
}

void setup() {

  Serial.begin(115200);

  pinMode(LED, OUTPUT);
  pinMode(BTN, INPUT);
  digitalWrite(LED, LOW);

  pinMode(STATUS_PIN, OUTPUT);
  digitalWrite(STATUS_PIN, LOW);

  socket_payload = (uint8_t*) calloc(5, sizeof(uint8_t));

  WiFiManager wifiManager;
  wifiManager.autoConnect("MyRemoteControl");
  //wifiManager.resetSettings();

  Serial.println("Connected to the remote control server!");
  digitalWrite(STATUS_PIN, HIGH);

  webSocket.begin("192.168.4.1", 81);
  webSocket.onEvent(webSocketEvent);
  webSocket.disconnect();

}

void read_button(void) {
  btn_state = digitalRead(BTN);

  if(btn_state != btn_last){
    if(btn_state == HIGH){
      digitalWrite(LED, HIGH);
      btn_last = HIGH;
      Serial.printf("High\n");
      webSocket.sendTXT(build_payload(socket_payload, 5, 0, 1),5);
    } else {
      digitalWrite(LED, LOW);
      btn_last = LOW;
      Serial.printf("Low\n");
      webSocket.sendTXT(build_payload(socket_payload, 5, 0, 0),5);
    }
  }
}

void loop() {
  webSocket.loop();
  read_button();
}
