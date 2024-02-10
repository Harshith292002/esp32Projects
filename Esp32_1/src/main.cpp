#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "V 401";
const char* password = "Jabs1234";

IPAddress local_ip(192, 168, 29, 150);
IPAddress gateway(192, 168, 29, 1);
IPAddress subnet(255, 255, 255, 0);

WebServer server(80);

uint8_t LEDPin1 = 2;
bool LED1status = LOW;

uint8_t LEDPin2 = 5;
bool LED2status = LOW;

void handle_OnConnect();
void handle_led1on();
void handle_led1off();
void handle_led2on();
void handle_led2off();
void handle_NotFound();
String SendHTML(uint8_t led1stat, uint8_t led2stat);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LEDPin1, OUTPUT);
  pinMode(LEDPin2, OUTPUT);

  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);

  server.on("/", handle_OnConnect);
  server.on("/led1on", handle_led1on);
  server.on("/led1off", handle_led1off);
  server.on("/led2on", handle_led2on);
  server.on("/led2off", handle_led2off);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP Server has Started");
}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
  if (LED1status) {
    digitalWrite(LEDPin1, HIGH);
  } else {
    digitalWrite(LEDPin1, LOW);
  }

  if (LED2status) {
    digitalWrite(LEDPin2, HIGH);
  } else {
    digitalWrite(LEDPin2, LOW);
  }
}

void handle_OnConnect() {
  LED1status = LOW;
  LED2status = LOW;
  Serial.println("GPIO2 Status: OFF and GPIO5 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status, LED2status));
}

void handle_led1on() {
  LED1status = HIGH;
  Serial.println("GPIO2 Status: ON");
  server.send(200, "text/html", SendHTML(LED1status, LED2status));
}

void handle_led1off() {
  LED1status = LOW;
  Serial.println("GPIO2 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status, LED2status));
}

void handle_led2on() {
  LED2status = HIGH;
  Serial.println("GPIO5 Status: ON");
  server.send(200, "text/html", SendHTML(LED1status, LED2status));
}

void handle_led2off() {
  LED2status = LOW;
  Serial.println("GPIO5 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status, LED2status));
}

void handle_NotFound() {
  Serial.println("Not Found");
  server.send(404, "text/html", "Not Found");
}

String SendHTML(uint8_t led1stat, uint8_t led2stat) {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<title>LED Control ESP32</title>\n";
  ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr += "h1 { color: #444444; margin: 50px auto 30px;}\n";
  ptr += "p { font-size: 20px; color: #888888; margin-bottom: 10px;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<h1>ESP32 Web Server</h1>\n";
  ptr += "<p>GPIO2 Status: ";
  if (led1stat) {
    ptr += "ON";
  } else {
    ptr += "OFF";
  }
  ptr += "</p>\n";
  ptr += "<p>GPIO5 Status: ";
  if (led2stat) {
    ptr += "ON";
  } else {
    ptr += "OFF";
  }
  ptr += "</p>\n";
  ptr += "<p><a href=\"/led1on\"><button>GPIO2 ON</button></a></p>\n";
  ptr += "<p><a href=\"/led1off\"><button>GPIO2 OFF</button></a></p>\n";
  ptr += "<p><a href=\"/led2on\"><button>GPIO5 ON</button></a></p>\n";
  ptr += "<p><a href=\"/led2off\"><button>GPIO5 OFF</button></a></p>\n";
  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}