#include <WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

#define WIFI_SSID "Pixel HC"
#define WIFI_PASS "Harshith"

#define MQTT_SERVER "192.168.107.133"
#define MQTT_PORT 1883
#define MQTT_USER ""
#define MQTT_PASS ""

#define DHT_PIN 4 // GPIO pin connected to DHT11 data pin
#define DHT_TYPE DHT11

WiFiClient espClient;
PubSubClient client(espClient);
DHT dht(DHT_PIN, DHT_TYPE);


void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  client.setServer(MQTT_SERVER, MQTT_PORT);
  
  dht.begin();
}

void reconnectMQTT() {
  while (!client.connect("ESP32Client", MQTT_USER, MQTT_PASS)) {
    Serial.print("MQTT connection failed, rc=");
    Serial.print(client.state());
    Serial.println(" Retrying in 5 seconds...");
    delay(5000);
  }
  Serial.println("Connected to MQTT broker");
}

void loop() {
  if (!client.connected()) {
    reconnectMQTT();
  }
  
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (!isnan(temperature) && !isnan(humidity)) {
    char tempStr[10];
    char humStr[10];
    dtostrf(temperature, 4, 2, tempStr);
    dtostrf(humidity, 4, 2, humStr);

    Serial.print("Temperature: ");
    Serial.println(temperature);
    Serial.print("Humidity: ");
    Serial.println(humidity);


    client.publish("harshithesp32/temperature", tempStr);
    client.publish("harshithesp32/humidity", humStr);
  }

  client.loop();
  delay(2000);
}

