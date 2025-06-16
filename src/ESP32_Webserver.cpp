#include <WebServer.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include "config.h"

#define HOST_1 0
#define HOST_0 1

#define LEDPIN 2

bool relaisState = false;

WebServer server(80);

String voltage[2];
String moist[2];

unsigned long requestPeriod = 15000;

// Funktionsprototypen
void handleRoot();
void requestESP8266(int hostNum);
void relaisOn();
void relaisOff();

String headAndTitle =
  "<!DOCTYPE html>"
  "<html lang=\"de\">"
  "<head>"
   "<meta charset=\"utf-8\">"
   "<meta http-equiv=\"refresh\" content=\"10\">"
   "<title>ESP32 Control Center</title>"
   "<style>"
     ".button {"
       "border: none;"
       "color: white;"
       "width: 350px;"
       "padding: 20px;"
       "text-align: center;"
       "margin: 20px auto;"
       "display: block;"
     "}"
     ".greenButton { background-color: green; font-size: 64px; }"
     ".greenButton:hover { background-color: darkgreen; }"
     ".redButton { background-color: red; font-size: 64px; }"
     ".redButton:hover { background-color: darkred; }"

     ".centerBox {"
       "text-align: center;"
       "margin: 0 auto;"
     "}"

     ".blueBox {"
       "background-color: blue;"
       "color: white;"
       "display: block;"
       "width: 90%;"
       "max-width: 600px;"
       "margin: 20px auto;"
       "padding: 20px;"
       "text-align: center;"
       "font-size: 50px;"
       "font-family: Arial, sans-serif;"
       "box-sizing: border-box;"
       "word-wrap: break-word;"
       "line-height: 1.2;"
     "}"

     "h1 { text-align: center; }"
   "</style>"
 "</head>"
 "<body>"
   "<h1>ESP32 Control Center</h1>"
   "<p style=\"text-align:center;\">Relaisschaltung und Sensorwerte</p>"
 "</body>"
 "</html>";

void setup(){
  Serial.begin(115200);
  pinMode(LEDPIN, OUTPUT);
  WiFi.config(ESP32_IP, GATEWAY_IP, SUBNET_MASK);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  // Blink-LED beim Verbindungsaufbau
  while (WiFiClass::status() != WL_CONNECTED) {
    digitalWrite(LEDPIN, HIGH);
    delay(50);
    digitalWrite(LEDPIN, LOW);
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");

  server.on("/", handleRoot);
  server.on("/relais_on", relaisOn);
  server.on("/relais_off", relaisOff);
  server.begin();
}

void loop() {
  static unsigned long startTime = millis();
  server.handleClient();

  if ((millis() - startTime) > requestPeriod) {
    requestESP8266(HOST_1); // Sensor-Daten
    startTime = millis();
  }
}

void handleRoot() {
  // Basis-HTML
  String msg = headAndTitle;
  msg.replace("</body>", "");
  msg += "<div class=\"centerBox\">";

  // Relais-Button
  if (relaisState) {
    msg += R"(<a href="/relais_off"><button class="button redButton">Relais AUS</button></a>)";
  } else {
    msg += R"(<a href="/relais_on"><button class="button greenButton">Relais EIN</button></a>)";
  }

  // Feuchtigkeitswerte
  msg += "<div class=\"blueBox\">"
         "<strong>Feuchtigkeitswerte:</strong><br>" +
         moist[HOST_1] +
         "</div>";

  // Spannung
  msg += "<div class=\"blueBox\">"
         "<strong>Spannung [V]:</strong><br>" +
         voltage[HOST_1] +
         "</div>";

  msg += "</div></body></html>";

  server.send(200, "text/html; charset=utf-8", msg);
}

void requestESP8266(const int hostNum) {
  WiFiClient client;
  const char* host = (hostNum == HOST_1) ? HYGROMETER_HOST : RELAIS_HOST;

  if (!client.connect(host, 80)) {
    Serial.printf("Fehler: Verbindung zu ESP-01 (%s) fehlgeschlagen.\n", host);
    return;
  }

  String url = "/voltage";
  client.print(String("GET ") + url +
              " HTTP/1.1\r\n" +
              "Host: " + host + "\r\n" +
              "Connection: close\r\n\r\n");

  client.setTimeout(3000);
  if (!client.find("\r\n\r\n")) {
    Serial.println("Fehler: Ungueltige Antwort vom Server");
    client.stop();
    return;
  }

  String body = client.readString();
  client.stop();
  Serial.println("Verbindung zu ESP-01 geschlossen");

  DynamicJsonDocument doc(128);
  DeserializationError error = deserializeJson(doc, body);
  if (error) {
    Serial.println("Fehler: JSON konnte nicht geparst werden");
    return;
  }

  voltage[hostNum] = String(doc["voltage"].as<float>(), 2);
}

void relaisOn() {
  relaisState = true;

  WiFiClient client;
  if (client.connect(RELAIS_HOST, 80)) {
    client.print(String("GET /on HTTP/1.1\r\nHost: ") + RELAIS_HOST + "\r\nConnection: close\r\n\r\n");
    delay(100);
    client.stop();
    Serial.println("Relais EIN gesendet an ESP-01");
  } else {
    Serial.println("Fehler: Relais EIN konnte nicht gesendet werden.");
  }
  server.sendHeader("Location", "/");
  server.send(303);
}

void relaisOff() {
  relaisState = false;


  WiFiClient client;
  if (client.connect(RELAIS_HOST, 80)) {
    client.print(String("GET /off HTTP/1.1\r\nHost: ") + RELAIS_HOST + "\r\nConnection: close\r\n\r\n");
    delay(100);
    client.stop();
    Serial.println("Relais AUS gesendet an ESP-01");
  } else {
    Serial.println("Fehler: Relais AUS konnte nicht gesendet werden.");
  }

  server.sendHeader("Location", "/");
  server.send(303);
}

