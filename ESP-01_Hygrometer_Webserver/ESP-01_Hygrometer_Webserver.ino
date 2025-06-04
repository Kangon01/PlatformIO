#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include "config.h"

// WLAN Zugangsdaten siehe config.h

// Feste IP-Adresse fuer das Sensor-Modul in config.h anpassen

// HTTP-Server auf Port 80
ESP8266WebServer server(80);

// Anschluss des Hygrometers
const int SENSOR_PIN = A0;

// Helferfunktion zum Auslesen der Spannung am Sensor
float readVoltage() {
  int raw = analogRead(SENSOR_PIN);            // Rohwert 0..1023
  // Der ESP-01 besitzt einen 1V ADC, ggf. sorgt die Platine fuer eine
  // Anpassung auf 3.3V. Hier gehen wir von einer 1V Referenz aus.
  return (raw / 1023.0) * 1.0;                 // Spannung in Volt
}

void handleRoot() {
  int raw = analogRead(SENSOR_PIN);
  float voltage = readVoltage();
  int percent = map(raw, 0, 1023, 0, 100);    // einfache Naeherung fuer Prozentwert

  DynamicJsonDocument doc(128);
  doc["value"] = raw;
  doc["voltage"] = voltage;
  doc["percent"] = percent;

  String json;
  serializeJson(doc, json);
  server.send(200, "application/json", json);
}

// Gibt nur die Spannung als Klartext zurueck
void handleVoltage() {
  float voltage = readVoltage();
  DynamicJsonDocument doc(64);
  doc["voltage"] = voltage;
  String json;
  serializeJson(doc, json);
  server.send(200, "application/json", json);
}

// Gibt nur den Rohwert 0..1023 zurueck
void handleValue() {
  int raw = analogRead(SENSOR_PIN);
  DynamicJsonDocument doc(64);
  doc["value"] = raw;
  String json;
  serializeJson(doc, json);
  server.send(200, "application/json", json);
}

void setup() {
  Serial.begin(115200);
  delay(10);

  pinMode(SENSOR_PIN, INPUT);

  // Verbindung zum WLAN herstellen
  WiFi.config(HYGROMETER_IP, GATEWAY_IP, SUBNET_MASK);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Verbinde mit dem WLAN");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print('.');
  }
  Serial.println();
  Serial.print("IP-Adresse: ");
  Serial.println(WiFi.localIP());

  // HTTP Endpunkte registrieren
  server.on("/", handleRoot);
  server.on("/voltage", handleVoltage);
  server.on("/value", handleValue);
  server.begin();
  Serial.println("HTTP-Server gestartet");
}

void loop() {
  server.handleClient();
}

