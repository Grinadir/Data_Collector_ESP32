#ifndef SETTINGS_ESP_H
#define SETTINGS_ESP_H

struct server_settings_esp{
    char * ip_server;
    uint16_t port;

};

struct wifi_settings_esp{
    char *ssid_client;                         /**< SSID of target AP. */
    char *password_client;   
    char *ssid_ap;
    char *password_ap;
};

struct all_settings_esp{
    struct server_settings_esp server_settings;
    struct wifi_settings_esp wifi_settings;
};

extern struct all_settings_esp all_settings;

#endif