#ifndef CONFIG_H
#define CONFIG_H

// WLAN-Zugangsdaten eintragen
#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"

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
