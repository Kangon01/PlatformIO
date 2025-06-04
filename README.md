# Arduino Projekt Sammlung

Dieses Repository enthält mehrere Arduino Sketche für verschiedene ESP-Module.

## Konfigurationsdatei `config.h`

Die Zugangsdaten für WLAN sowie die verwendeten IP-Adressen werden zentral in
der Datei `config.h` gepflegt. Vor der ersten Verwendung müssen die Platzhalter
in dieser Datei angepasst werden.

```cpp
#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"

#define ESP32_IP IPAddress(192, 168, 0, 2)
#define HYGROMETER_IP IPAddress(192, 168, 0, 3)
#define RELAIS_IP IPAddress(192, 168, 0, 4)
#define GATEWAY_IP IPAddress(192, 168, 0, 1)
#define SUBNET_MASK IPAddress(255, 255, 255, 0)
```

Passen Sie diese Werte an Ihr Netzwerk an und speichern Sie die Datei. Alle
Sketche binden `config.h` automatisch ein und greifen auf diese Konstanten zu.
