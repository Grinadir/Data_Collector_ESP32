#include "settings_esp.h"
#include "esp_log.h"
#include "stdio.h"

static const char *TAG = "SETTINGS ESP";

extern struct all_settings_esp all_settings;
extern struct statuses stat_e;

void output_settings()
{
    if (all_settings.wifi_settings.ssid_client != NULL)
        ESP_LOGI(TAG, "ssid client: %s", all_settings.wifi_settings.ssid_client);
    else
        ESP_LOGI(TAG, "ssid client is NULL!!");

    if (all_settings.wifi_settings.password_client != NULL)
        ESP_LOGI(TAG, "password client: %s", all_settings.wifi_settings.password_client);
    else
        ESP_LOGI(TAG, "password client is NULL!!");

    if (all_settings.wifi_settings.ssid_ap != NULL)
        ESP_LOGI(TAG, "ssid ap: %s", all_settings.wifi_settings.ssid_ap);
    else
        ESP_LOGI(TAG, "ssid ap is NULL!!");

    if (all_settings.wifi_settings.password_ap != NULL)
        ESP_LOGI(TAG, "password ap: %s", all_settings.wifi_settings.password_ap);
    else
        ESP_LOGI(TAG, "password ap is NULL!!");
}

uint8_t is_wifi_ap_status()
{
    return stat_e.statuses_esp & MASK_WIFI_AP_ON;
}

void set_wifi_ap_status(uint8_t status)
{
    if (status)
        stat_e.statuses_esp |= MASK_WIFI_AP_ON;
    else if (is_wifi_ap_status() != 0)
        stat_e.statuses_esp ^= MASK_WIFI_AP_ON;
}

uint8_t is_wifi_sta_status()
{
    return stat_e.statuses_esp & MASK_WIFI_STA_ON;
}

void set_wifi_sta_status(uint8_t status)
{
    if (status)
        stat_e.statuses_esp |= MASK_WIFI_STA_ON;
    else if (is_wifi_sta_status() != 0)
    {
        stat_e.statuses_esp ^= MASK_WIFI_STA_ON;
    }
}

uint8_t is_time_sync()
{
    return stat_e.statuses_esp & MASK_IS_TIME_SYNCH;
}

void set_time_status(uint8_t status)
{
    if (status)
        stat_e.statuses_esp |= MASK_IS_TIME_SYNCH;
    else if (is_time_sync() != 0)
    {
        stat_e.statuses_esp ^= MASK_IS_TIME_SYNCH;
    }
}