#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

const char* ssid = "Pixel HC";
const char* password = "Harshith";
const char* mqtt_server = "192.168.35.133";

const char* mqtt_username = "";
const char* mqtt_password = "";

const int dhtPin = 14; // GPIO2
const int soilMoisturePin = 35; // Analog GPIO35

DHT dht(dhtPin, DHT11);
WiFiClient espClient;
PubSubClient client(espClient);

LiquidCrystal_I2C lcd(0x27, 16, 2); // Set the LCD I2C address

void reconnect() {
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    
    if (client.connect("ESP32Client")) {
      Serial.println("Connected to MQTT server");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" Retrying in 5 seconds...");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  client.setServer(mqtt_server, 1883);
  
  dht.begin();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  int analogMoisture = analogRead(soilMoisturePin);
  int soilMoisture = ( 100 - ( (analogMoisture/4095.00) * 100 ) );

  // Publish data to MQTT topics
  char tempStr[6];
  dtostrf(temperature, 4, 2, tempStr);
  char humStr[6];
  dtostrf(humidity, 4, 2, humStr);
  char moistureStr[6];
  itoa(soilMoisture, moistureStr, 10);

  client.publish("esp32/irrigation/temperature", tempStr);
  client.publish("esp32/irrigation/humidity", humStr);
  client.publish("esp32/irrigation/soilmoisture", moistureStr);

  lcd.clear(); // Clear the LCD display
  lcd.setCursor(0, 0); // Set cursor to the first line
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print(" C");

  lcd.setCursor(0, 1); // Set cursor to the second line
  lcd.print("Humidity: ");
  lcd.print(humidity);
  lcd.print(" %");

  delay(5000); // Adjust the delay as needed
}



