#include "settings_esp.h"
#include "esp_log.h"
#include "stdio.h"

static const char *TAG = "SETTINGS ESP";

extern struct all_settings_esp all_settings;
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