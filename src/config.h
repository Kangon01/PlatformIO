#ifndef CONFIG_H
#define CONFIG_H

// WLAN-Zugangsdaten eintragen
#define WIFI_SSID "KabelBox-9B90"
#define WIFI_PASSWORD "13311814531144239571"

// IP-Adressen der einzelnen Module
#define ESP32_IP IPAddress(192, 168, 0, 2)
#define HYGROMETER_IP IPAddress(192, 168, 0, 3)
#define RELAIS_IP IPAddress(192, 168, 0, 4)
#define GATEWAY_IP IPAddress(192, 168, 0, 1)
#define SUBNET_MASK IPAddress(255, 255, 255, 0)

// String-Adressen fuer HTTP-Abfragen
#define HYGROMETER_HOST "192.168.0.3"
#define RELAIS_HOST "192.168.0.4"

#endif // CONFIG_H
