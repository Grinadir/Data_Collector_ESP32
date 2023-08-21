#include "DHT.h"
#include "data_JSON.h"
#include "timeESP.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "settings_esp.h"
#include "wifi_ap.h"
#include "wifi_client.h"
#include "web_server.h"

#define DBG_OUTPUT_ENABLED
#ifdef DBG_OUTPUT_ENABLED
#define LOGI ESP_LOGI
#define LOGE ESP_LOGE
#define LOGW ESP_LOGW
#else
#define LOGI ESP_LOGI
#define LOGE ESP_LOGE
#define LOGW ESP_LOGW
#endif

static const char *TAG_DHT = "TASK DHT measure and save data JSON.";
static const char *TAG_AP = "TASK Create AP";
#define SAMPL_MESSUARE_SEC 3
#ifdef DBG_OUTPUT_ENABLED
#define DBG_LOGI ESP_LOGI
#endif

extern struct data_temperature_and_humidity data_t_h[288];

void measure_DHT_save_data_JSON_task(void *pvParameter)

{
    setDHTgpio(GPIO_NUM_4);
    LOGI(TAG_DHT, "Starting DHT task messure.");
    char time[64];
    uint8_t is_log = 0;
    uint16_t count = 0;
    while (1)
    {
        get_time(time);

        vTaskDelay(100 / portTICK_PERIOD_MS);
        if ((long)get_timestamp() % SAMPL_MESSUARE_SEC == 0 && is_log == 0)
        {
            get_time(time);
            int ret = readDHT();

            errorHandler(ret);
            LOGI(TAG_DHT, "Measure time %s", time);
            LOGI(TAG_DHT, "Hum: %.1f Tmp: %.1f\n", getHumidity(), getTemperature());
            get_time(time);

            save_humidity_temperature_in_struct(getHumidity(), getTemperature(), time, data_t_h);
            is_log = 1;
            count++;
            if (count == 3)
            {
                struct_to_array_json(data_t_h, data, count);
                LOGI(TAG_DHT, "SAVE to array");
            }
        }
        else if ((long)get_timestamp() % SAMPL_MESSUARE_SEC != 0 && is_log == 1)
        {
            is_log = 0;
        }
    }
    LOGE(TAG_DHT, "DHT_TASK FAILED!");
    vTaskDelete(NULL);
}

void create_wifi_access_point(void *pvParameters)
{
    LOGI(TAG_AP, "Create TASK AP.");
    bool but_pressed;
    while (1)
    {
        LOGI(TAG_AP, "Wait interput for button pressed.");
        // Ждем события нажатия кнопки в очереди
        if (xQueueReceive((QueueHandle_t) pvParameters, &but_pressed, portMAX_DELAY) && is_ap_active == 0)
        {
            LOGI(TAG_AP, "Button is pressed!");
            uint8_t i = 0;
            while (i < 30)
            {
                vTaskDelay(100 / portTICK_PERIOD_MS);

                if (gpio_get_level(BUTTON_AP_WIFI) != 0)
                    i = 44;
                else
                    i++;
            }
            if (i == 30)
            {
                LOGI(TAG_AP, "BUTTON 3 sec pressed!");
                if (esp_delete_wifi_sta() == ESP_OK)
                {
                    wifi_init_softap();
                    update_for_server();
                    is_ap_active = 1;
                }
                else
                    LOGE(TAG_AP, "FAILLED TO CREATE AP");
            }
            else
            {
                LOGI(TAG_AP, "BUTTON not 3 sec pressed");
            }
        }
        else if (is_ap_active)
        {
            LOGE(TAG_AP, "AP is already active!");
        }
    };
    vTaskDelete(NULL);
}
