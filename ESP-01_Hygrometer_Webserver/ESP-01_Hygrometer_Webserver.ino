#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// WLAN Zugangsdaten hier eintragen
const char* ssid = "";
const char* password = "";

// Feste IP-Adresse fuer das Sensor-Modul
IPAddress ip(192,168,0,3);
IPAddress gateway(192,168,0,1);
IPAddress subnet(255,255,255,0);

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

  String html = "<h1>Hygrometer Modul v1.2</h1>";
  html += "<p>Feuchte: " + String(percent) + "%</p>";
  html += "<p>Spannung: " + String(voltage, 2) + " V</p>";
  html += "<p>Rohwert: " + String(raw) + "</p>";

  server.send(200, "text/html", html);
}

// Gibt nur die Spannung als Klartext zurueck
void handleVoltage() {
  float voltage = readVoltage();
  server.send(200, "text/plain", String(voltage, 2));
}

// Gibt nur den Rohwert 0..1023 zurueck
void handleValue() {
  int raw = analogRead(SENSOR_PIN);
  server.send(200, "text/plain", String(raw));
}

void setup() {
  Serial.begin(115200);
  delay(10);

  pinMode(SENSOR_PIN, INPUT);

  // Verbindung zum WLAN herstellen
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password);

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

