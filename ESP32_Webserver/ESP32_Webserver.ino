#include <WebServer.h>
#include <WiFi.h>

#define HOST_1 0
#define HOST_0 1

#define LEDPIN 2

bool relaisState = false;

const char* ssid = "";
const char* pass = "";

const char* host_1 = "192.168.0.3"; // Feuchtigkeitssensor (ESP-01)
const char* host_2 = "192.168.0.4"; // Relaissteuerung (ESP-01)

IPAddress ip(192,168,0,2);
IPAddress gateway(192,168,0,1);
IPAddress subnet(255,255,255,0);

WebServer server(80);

String voltage[2];
String moist[2];

unsigned long requestPeriod = 15000;

String headAndTitle = 
"<head><style>"
  ".button {"
    "border: none;"
    "color: white;"
    "width: 350px;"
    "padding: 20px;"
    "text-align: center;"
    "margin: 20px 200px;"
  "}"
  ".greenButton {background-color: green; font-size: 64px;}"
  ".greenButton:hover {background-color: darkgreen; font-size: 64px;}"
  ".redButton {background-color: red; font-size: 64px;}"
  ".redButton:hover {background-color: darkred; font-size: 64px;}"
  ".blueBox {"
    "background-color: blue;"
    "color: white;"
    "width: 350px;" 
    "padding: 20px;"
    "text-align: center;"
    "font-size: 50px;"
    "font-family: arial;"
  "}"
"</style>"
"<meta http-equiv=\"refresh\" content=\"10\">"
"</head>"
"<h1 align=\"center\">ESP32 Controll Center</h1>"
"<div align=\"center\">Relaisschaltung und Sensorwerte</BR></BR></div>";

void setup(){
  Serial.begin(115200);
  pinMode(LEDPIN, OUTPUT);
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, pass);

  // Blink-LED beim Verbindungsaufbau
  while (WiFi.status() != WL_CONNECTED) {
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
  String msg = headAndTitle;
  msg += "<div align=\"center\";>";

  // Button Relais
   if (relaisState) {
    msg += "<a href=\"/relais_off\"><button class=\"button redButton\">Relais AUS</button></a>";
  } else {
    msg += "<a href=\"/relais_on\"><button class=\"button greenButton\">Relais EIN</button></a>";
  }

  // Sensor Werte
  msg += "</BR><div class=\"blueBox\">";
  msg += "Feuchtigkeitswerte: </BR>";
  msg += moist[HOST_1];
  msg += "</div>";

  // Voltage von Sensor
  msg += "</BR><div class=\"blueBox\">";
  msg += "Spannung [V]: </BR>";
  msg += voltage[HOST_1];
  msg += "</div></div>";

  server.send(200, "text/html", msg);
}

void requestESP8266(int hostNum) {
  WiFiClient client;
  const char* host = (hostNum == HOST_1) ? host_1 : host_2;

  if (!client.connect(host, 80)) {
    Serial.printf("Fehler: Verbindung zu ESP-01 (%s) fehlgeschlagen.\n", host);
    return;
  }

  String url = "/voltage";
  client.print(String("GET ") + url + 
              " HTTP/1.1\r\n" + 
              "Host: " + host + "\r\n" + 
              "Connection: close\r\n\r\n");
              
  unsigned long timeout = millis();
  while (!client.available() && ((millis() - timeout) < 3000)) {
    delay(1);
  }

  while (client.available()) {
    String line = client.readStringUntil('\r');
    if(line != "") {
      voltage[hostNum] = line;
    }
  }
  
  client.stop();
  Serial.println("Verbindung zu ESP-01 geschlossen");
}

void relaisOn() {
  relaisState = true;

  WiFiClient client;
  if (client.connect(host_2, 80)) {
    client.print(String("GET /on HTTP/1.1\r\nHost: ") + host_2 + "\r\nConnection: close\r\n\r\n");
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
  if (client.connect(host_2, 80)) {
    client.print(String("GET /off HTTP/1.1\r\nHost: ") + host_2 + "\r\nConnection: close\r\n\r\n");
    delay(100);
    client.stop();
    Serial.println("Relais AUS gesendet an ESP-01");
  } else {
    Serial.println("Fehler: Relais AUS konnte nicht gesendet werden.");
  }

  server.sendHeader("Location", "/");
  server.send(303);
}

