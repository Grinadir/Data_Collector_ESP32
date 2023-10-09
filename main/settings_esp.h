#ifndef SETTINGS_ESP_H
#define SETTINGS_ESP_H
#include "stdint.h"
#include "time.h"


#define BUTTON_AP_WIFI GPIO_NUM_18
// |0|0|0|0|0|wifi sta|time is synchronized|wifi ap|
#define MASK_WIFI_AP_ON 1
#define MASK_IS_TIME_SYNCH 2
#define MASK_WIFI_STA_ON 4

struct server_settings_esp{
    char * ip_server;
    uint16_t port;

};

struct wifi_settings_esp{
    char *ssid_client;
    char *password_client;   
    char *ssid_ap;
    char *password_ap;
};

struct all_settings_esp{
    struct server_settings_esp server_settings;
    struct wifi_settings_esp wifi_settings;
    
};

struct statuses{
    uint8_t statuses_esp;
    time_t time_last_send_to_server;

};


extern struct all_settings_esp all_settings;
extern struct statuses stat_e;
uint8_t is_wifi_ap_status();
void set_wifi_ap_status(uint8_t status);
uint8_t is_wifi_sta_status();
void set_wifi_sta_status(uint8_t status);
uint8_t is_time_sync();
void set_time_status(uint8_t status);

void output_settings();


#endif