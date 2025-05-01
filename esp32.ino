/*
============================================================
Welcome to the pymo Pro API.
 You can get more explanations via 
https://docs.pymo.pro/
No limits on what you can do, completely free,
 no recurring payments 
 https://pymo.pro
============================================================
*/

#include <WiFi.h>
#include <ArduinoWebsockets.h>
#include <ArduinoJson.h>



using namespace websockets;
//WIFI CONNECT
const char* ssid     = "SSID";
const char* password = "SSID_PASSWORD";
//API TOKEN PYMO PRO
const String token = "API_TOKEN_PYMO";

String ws_url = "ws://api.pymo.pro/ws?token=" + token;
const char* ws_url_cstr = ws_url.c_str();

WebsocketsClient client;


void onMessage(WebsocketsMessage message) {
  Serial.println("Received message:");
  Serial.println(message.data());

  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, message.data());
  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.f_str());
    return;
  }

  JsonObject pins = doc["pins"];
  for (JsonPair kv : pins) {
    int pinNumber = atoi(kv.key().c_str() + 3); // strip "pin"
    int pinValue = kv.value().as<int>();
    pinMode(pinNumber, OUTPUT);
    digitalWrite(pinNumber, pinValue);
  }
}

void setup() {
  Serial.begin(115200);

  // الاتصال بالشبكة
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
  }
  Serial.println("WiFi connected");

  // إعداد WebSocket
  client.onMessage(onMessage);

  // الاتصال بالرابط الكامل
  if (client.connect(ws_url)) {
    Serial.println("WebSocket Connected!");
  } else {
    Serial.println("WebSocket Failed to connect.");
  }
}

void loop() {
  client.poll();  // استقبال الرسائل الجديدة
}
