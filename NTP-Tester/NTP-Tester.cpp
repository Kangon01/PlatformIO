#include <ESP8266WiFi.h>

#include <time.h>
#include "config.h"

// WLAN-Daten siehe config.h
 
// Liste der zu testenden NTP-Server

const char* ntpServers[] = {

  "pool.ntp.org",

  "de.pool.ntp.org",

  "time.google.com",

  "time.windows.com",

  "ptbtime1.ptb.de"

};
 
const int ntpServerCount = sizeof(ntpServers) / sizeof(ntpServers[0]);
 
void setup() {

  Serial.begin(115200);

  delay(1000);

  Serial.println();

  Serial.println("Starte NTP Test...");
 
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Verbinde mit WLAN");

  while (WiFi.status() != WL_CONNECTED) {

    delay(500);

    Serial.print(".");

  }

  Serial.println(" verbunden!");

}
 
bool getNtpTime(const char* ntpServer) {

  configTime(1 * 3600, 1 * 3600, ntpServer); // MEZ/CEST Sommerzeit optional anpassen

  Serial.printf("Teste NTP Server: %s ... ", ntpServer);
 
  // Warte auf Zeit-Sync max 5 Sekunden

  int tries = 0;

  time_t now = time(nullptr);

  while (now < 8 * 3600 * 2 && tries < 10) {  // 8 Stunden als "ungültige Zeit" Grenze

    delay(500);

    Serial.print(".");

    now = time(nullptr);

    tries++;

  }
 
  if (now < 8 * 3600 * 2) {

    Serial.println(" Fehler: Keine Antwort (Timeout)");

    return false;

  } else {

    struct tm timeinfo;

    localtime_r(&now, &timeinfo);

    Serial.printf(" Erfolg: %02d:%02d:%02d\n", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);

    return true;

  }

}
 
void loop() {

  for (int i = 0; i < ntpServerCount; i++) {

    getNtpTime(ntpServers[i]);

    delay(2000); // kurze Pause zwischen den Tests

  }

  Serial.println("Alle Server getestet, nächster Test in 60 Sekunden...\n");

  delay(60000);

}

 
