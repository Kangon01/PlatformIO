# Arduino-Projekt

Dieses Repository enthaelt verschiedene Beispielsketche rund um ESP8266- und ESP32-Boards. Die Programme demonstrieren den Betrieb eines Hygrometers, einer Relaissteuerung sowie einen zentralen Webserver und einen einfachen NTP-Test.

## Zweck
Die Sketche dienen als Ausgangsbasis fuer eigene Experimente im Bereich Heimautomatisierung und vernetzte Sensorik. Sie zeigen, wie sich mehrere ESP-Geraete ueber das lokale Netzwerk steuern und abfragen lassen.

## Benoetigte Hardware
- **ESP-01** Modul fuer das Hygrometer
- **ESP-01** Modul zur Relaissteuerung
- **ESP32** als zentrales Steuergeraet
- Relaismodul (3,3&nbsp;V) und Hygrometersensor
- Stabile 3,3&nbsp;V Spannungsversorgung und ein gemeinsames WLAN

## Verzeichnisstruktur
- `ESP-01_Hygrometer_Webserver/` – Webserver fuer den Feuchtigkeitssensor
- `ESP-01_Relaisteuerung/` – Schaltet ein Relais via HTTP
- `ESP32_Webserver/` – Kontrollzentrale, ruft Sensorwerte ab und steuert das Relais
- `NTP-Tester/` – Kleiner Test fuer verschiedene NTP-Server
- `libraries/` – Lokale Kopien benoetigter Bibliotheken (ArduinoJson, NTPClient)

## Beispielsketche
Die oben genannten Unterordner enthalten jeweils eine `.ino`-Datei. Vor dem Upload muessen SSID und WLAN-Passwort angepasst werden. Der ESP32-Webserver erwartet, dass die beiden ESP-01-Module feste IP-Adressen erhalten.

## Bibliotheken
Fuer die Projekte werden die folgenden Bibliotheken benoetigt:
- **ESP8266WiFi** bzw. **WiFi** – fuer die WLAN-Anbindung
- **ESP8266WebServer** bzw. **WebServer** – fuer die HTTP-Schnittstelle
- **ArduinoJson** (liegt im Ordner `libraries/ArduinoJson`)
- **NTPClient** (im Ordner `libraries/NTPClient`)

Die letzten beiden sind dem Repository bereits beigelegt und koennen direkt in der Arduino-IDE eingebunden werden.
