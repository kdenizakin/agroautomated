#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>

const char* ssid = "SUPERONLINE_WiFi_1709";
const char* password = "3cdSg3cyexbe";
const char* webSocketServerAddress = "localhost"; //  IP address
const uint16_t webSocketServerPort = 8082; //port
const String webSocketEndpoint = "/WebSocketApp/websocketendpoint"; //endpoint

WiFiClient client;
WebSocketsClient webSocket;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("Connecting...");
  }
  Serial.println("WiFi connected");

  webSocket.begin(webSocketServerAddress, webSocketServerPort, webSocketEndpoint);
  webSocket.onEvent(webSocketEvent);
}

void loop() {
  webSocket.loop();
}

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.println("Disconnected from WebSocket server");
      break;
    case WStype_CONNECTED:
      Serial.println("Connected to WebSocket server");
      break;
    case WStype_TEXT:
      Serial.printf("Received message: %s\n", payload);
      // Handle received message here
      break;
    case WStype_BIN:
      Serial.println("Received binary data");
      break;
    default:
      break;
  } 
}
