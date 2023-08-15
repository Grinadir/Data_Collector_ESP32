#ifndef DATA_JSON_H
#define DATA_JSON_H

#include "stdint.h"
#include "data_temperature_and_humidity.h"

extern uint8_t is_ap_active;


void init_first_JSON();

void set_wifi_JSON_Info(char *SSID_Client, char *password_client, char *SSID_AP, char *password_ap, uint8_t is_ap_active);
void get_wifi_settings(char *SSID_Client, char *password_client, char *SSID_AP, char *password_ap, uint8_t is_ap_active);
void init_wifi_settings_from_json_file();
void save_humidity_temperature_in_json(float hum, float temp, char *time);
void save_humidity_temperature_in_struct(float hum, float temp, char **time, struct data_temperature_and_humidity *data_t_h);
void struct_to_array_json(struct data_temperature_and_humidity *data_t_h, char *array, uint16_t count);




#endif