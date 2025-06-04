#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid     = "";
const char* password = "";

IPAddress ip(192,168,0,4);
IPAddress gateway(192,168,0,1);
IPAddress subnet(255,255,255,0);

#define RELAY_PIN 0

ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);

  // Mit WLAN verbinden
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password);

  Serial.print("Verbinde mit dem WLAN");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("IP-Adresse: ");
  Serial.println(WiFi.localIP());

  // HTTP Endpunkte
  server.on("/on", [](){
    digitalWrite(RELAY_PIN, HIGH);
    server.sendHeader("Location", "/");
    server.send(303);
    Serial.println("Relais EIN");
  });

  server.on("/off", [](){
    digitalWrite(RELAY_PIN, LOW);
    server.sendHeader("Location", "/");
    server.send(303);
    Serial.println("Relais AUS");
  });

  server.on("/", [](){
    String html = "<h1>Relais-Steuerung</h1>";
    html += "<p>State: ";
    html += (digitalRead(RELAY_PIN) ? "EIN" : "AUS");
    html += "</p>";
    html += "<a href=\"/on\"><button>EIN</button></a> ";
    html += "<a href=\"/off\"><button>AUS</button></a>";
    server.send(200, "text/html", html);
  });

  server.begin();
  Serial.println("HTTP-Server gestartet");
}

void loop() {
  server.handleClient();
}
