#ifndef DATA_JSON_H
#define DATA_JSON_H

#include "stdint.h"

extern uint8_t is_ap_active;


void init_first_JSON();

void set_wifi_JSON_Info(char *SSID_Client, char *password_client, char *SSID_AP, char *password_ap, uint8_t is_ap_active);
void get_wifi_settings(char *SSID_Client, char *password_client, char *SSID_AP, char *password_ap, uint8_t is_ap_active);
void init_wifi_settings_from_json_file();




#endif