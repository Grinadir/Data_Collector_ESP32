#include "wifi_ap.h"
#include "string.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_mac.h"
#include "data_JSON.h"
#include "settings_esp.h"

#define AP_ESP_WIFI_SSID "ESP32-WIFI"
#define AP_ESP_WIFI_PASS "12345789"
#define AP_ESP_WIFI_CHANNEL 9
#define AP_STA_CONN 2

static const char *TAG = "wifi softAP";
static esp_netif_t *esp_netif_ap;


static void wifi_event_handler(void *arg, esp_event_base_t event_base,
                               int32_t event_id, void *event_data)
{
    if (event_id == WIFI_EVENT_AP_STACONNECTED)
    {
        wifi_event_ap_staconnected_t *event = (wifi_event_ap_staconnected_t *)event_data;
        ESP_LOGI(TAG, "station " MACSTR " join, AID=%d",
                 MAC2STR(event->mac), event->aid);
    }
    else if (event_id == WIFI_EVENT_AP_STADISCONNECTED)
    {
        wifi_event_ap_stadisconnected_t *event = (wifi_event_ap_stadisconnected_t *)event_data;
        ESP_LOGI(TAG, "station " MACSTR " leave, AID=%d",
                 MAC2STR(event->mac), event->aid);
    }
}

void wifi_init_softap(void)
{


    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_ap = esp_netif_create_default_wifi_ap();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &wifi_event_handler,
                                                        NULL,
                                                        NULL));

    wifi_config_t wifi_config = {
        .ap = {
            .ssid = AP_ESP_WIFI_SSID,
            .ssid_len = strlen(AP_ESP_WIFI_SSID),
            .channel = AP_ESP_WIFI_CHANNEL,
            .password = AP_ESP_WIFI_PASS,
            .max_connection = AP_STA_CONN,
            .authmode = WIFI_AUTH_WPA_WPA2_PSK},
    };
    if (strlen(AP_ESP_WIFI_PASS) == 0)
    {
        wifi_config.ap.authmode = WIFI_AUTH_OPEN;
    }

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "wifi_init_softap finished. SSID:%s password:%s channel:%d",
             AP_ESP_WIFI_SSID, AP_ESP_WIFI_PASS, AP_ESP_WIFI_CHANNEL);
}

esp_err_t esp_delete_wifi_ap(void)
{
    esp_err_t err = esp_wifi_stop();
    esp_netif_destroy_default_wifi(esp_netif_ap);

    if (err == ESP_OK)
    {
        err = esp_event_loop_delete_default();
        if (err != ESP_OK)
        {
            ESP_LOGE(TAG, "ERROR DEINIT WIFI (esp_event_loop_delete_default)");
            return err;
        }

        err = esp_wifi_deinit();
        if (err != ESP_OK)
        {
            ESP_LOGE(TAG, "ERROR DEINIT WIFI (esp_wifi_deinit)");
            return err;
        }
        set_wifi_ap_status(0);
        return err;
    }
    else
    {
        ESP_LOGE(TAG, "ERROR DEINIT WIFI (esp_wifi_stop)");
        return err;
    }
}
