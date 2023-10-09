#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_attr.h"
#include "esp_sleep.h"
#include "nvs_flash.h"
#include "esp_sntp.h"
#include "esp_netif.h"
#include "settings_esp.h"

#define TIME_IS_SYNCHRONISED 1

static const char *TAG = "TIME";


void time_sync_notification_cb()
{
    ESP_LOGI(TAG, "ESP32 time is SYNC!!!");
    set_time_status(TIME_IS_SYNCHRONISED);
}

void initialize_sntp(void)
{
    ESP_LOGI(TAG, "Initializing SNTP");
    setenv("TZ", "MSK-3", 1);
    tzset();
    esp_sntp_setoperatingmode(SNTP_OPMODE_POLL);
    esp_sntp_setservername(0, "pool.ntp.org");
    esp_sntp_setservername(1, "1.ru.pool.ntp.org");

    sntp_set_time_sync_notification_cb(time_sync_notification_cb);
    esp_sntp_init();
}

void obtain_time(void)
{
    initialize_sntp();
    char strftime_buf[64];
    time_t now = 0;
    struct tm timeinfo;
    int retry = 0;
    const int retry_count = 2;
    // struct timeval tv;
    // printf("\nSEC=%lld\n", tv.tv_sec);
    // sntp_sync_time(&tv);
    // tv.tv_sec=tv.tv_sec-2208988800;
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    // tv.tv_sec=1681651407;

    // printf("\nSEC=%lld\n", (time_t)tv.tv_sec);
    // settimeofday(&tv, NULL);
    // gettimeofday(&tv, NULL);
    while (sntp_get_sync_status() == SNTP_SYNC_STATUS_RESET && ++retry < retry_count)
    {
        ESP_LOGI(TAG, "Waiting for system time to be set... (%d/%d)", retry, retry_count);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    time(&now);
    ESP_LOGI(TAG, "timestamp %lld", now);

    localtime_r(&now, &timeinfo);
    strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
    ESP_LOGI(TAG, "The current date/time in MOSCOW is: %s", strftime_buf);
}

void loop_show_tine()
{

    char strftime_buf[64];
    time_t now = 0;
    struct tm timeinfo;

    while (1)
    {
        vTaskDelay(60000 / portTICK_PERIOD_MS);
        time(&now);
        ESP_LOGI(TAG, "timestamp %lld", now);

        localtime_r(&now, &timeinfo);
        strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
        ESP_LOGI(TAG, "The current date/time in MOSCOW is: %s", strftime_buf);
    }
}

void get_time(char *time_for_update)
{
    char strftime_buf[64];
    time_t now = 0;
    struct tm timeinfo;

    // vTaskDelay(60000 / portTICK_PERIOD_MS);
    time(&now);
    //ESP_LOGI(TAG, "timestamp %lld", now);

    localtime_r(&now, &timeinfo);
    strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
    //ESP_LOGI(TAG, "The current date/time in MOSCOW is: %s", strftime_buf);

    //char *p = malloc(sizeof(strftime_buf));
    strncpy(time_for_update, strftime_buf, 64);
    //printf("hh %s\n", time_for_update);
    //return p;
}

time_t get_timestamp()
{
    time_t now = 0;
    time(&now);
    return now;
}
