#ifndef WIFI_CONFIGURATION_H_GUARD
#define WIFI_CONFIGURATION_H_GUARD

#define WIFI_DEFAULT_HOSTNAME "无线DAP"

#define WIFI_DEFAULT_AP_SSID "无线DAP"
#define WIFI_DEFAULT_AP_PASS "12345678"
#define WIFI_DEFAULT_AP_IP      192, 168, 1, 1
#define WIFI_DEFAULT_AP_GATEWAY 192, 168, 1, 1
#define WIFI_DEFAULT_AP_NETMASK 255, 255, 255, 0

#define WIFI_DEFAULT_STA_SSID "example_ssid"
#define WIFI_DEFAULT_STA_PASS "12345678"

#define IP4_ADDR_EXPAND(...) IP4_ADDR(__VA_ARGS__)

#endif //WIFI_CONFIGURATION_H_GUARD