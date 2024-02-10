#include <Arduino.h>
#include <DHT.h>

#define DHTPIN 4       // Pin where the DHT11 is connected
#define DHTTYPE DHT11  // DHT 11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  delay(2000);
  
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C\t");
  
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println("%");
}


