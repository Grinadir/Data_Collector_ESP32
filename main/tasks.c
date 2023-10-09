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

#include "http_esp_client.h"

#define DBG_OUTPUT_ENABLED
#ifdef DBG_OUTPUT_ENABLED
#define LOGI ESP_LOGI
#define LOGE ESP_LOGE
#define LOGW ESP_LOGW
#else
#define LOGI(...)
#define LOGE(...)
#define LOGW(...)
#endif

static const char *TAG_DHT = "TASK DHT measure and save data JSON.";
static const char *TAG_AP = "TASK Create AP";
static const char *TAG_WAIT_WIFI = "TASK wait wifi";
static const char *TAG_SHEDULE_AND_EVENT_SYSTEM = "TASK shedule and event";
static const char *TAG_EVENT="EVENT";
#define SAMPL_MESSUARE_SEC 2
#define WIFI_AP_ON 1

ESP_EVENT_DECLARE_BASE(EVENT_AND_SHEDULE);
ESP_EVENT_DEFINE_BASE(EVENT_AND_SHEDULE);
esp_event_loop_handle_t loop_handle;

extern struct data_temperature_and_humidity data_t_h[288];
extern struct statuses stat_e;

void wait_to_http_send(void *pvParameter)
{
    LOGI(TAG_WAIT_WIFI, "Wait when WIFI is connected.");
    while (1)
    {
        if (is_wifi_sta_status())
        {
            LOGI(TAG_WAIT_WIFI, "WIFI is apear. Try to send");
            send_m("192.168.1.10", 8080, "data");
            vTaskDelete(NULL);
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void event_need_send_data_to_server_handler(void *handler_arg, esp_event_base_t base, int32_t id, void *event_data)
{
    if (is_wifi_sta_status())
    {
        LOGI(TAG_EVENT, "Try to send data to server.");
        send_m("192.168.1.10", 8080, data);
    }
    else
    {
        LOGI(TAG_EVENT, "WIFI is not connected.");
        xTaskCreate(&wait_to_http_send, "WAIT WIFI CONNECTED TASK", 7000, NULL, 4, NULL);
    }
}
void init_event_and_schedule_system()
{
    int32_t NEED_SEND_DATA_SERVER = 1;
    esp_event_loop_args_t loop_args = {
        .queue_size = 5,
        .task_name = "event_loop",
        .task_priority = 10,
        .task_stack_size = 20048,
        .task_core_id = 0};

    esp_err_t err = esp_event_loop_create(&loop_args, &loop_handle);
    if (err == ESP_OK)
        LOGI(TAG_SHEDULE_AND_EVENT_SYSTEM, "OK. Create event loop.");
    esp_event_handler_register_with(loop_handle, EVENT_AND_SHEDULE, NEED_SEND_DATA_SERVER, event_need_send_data_to_server_handler, NULL);

    // esp_event_handler_unregister_with(loop_handle, EVENT_AND_SHEDULE, NEED_SEND_DATA_SERVER, run_on_event);
    // esp_event_loop_delete(loop_handle);
}

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

            save_humidity_temperature_in_struct(getHumidity(), getTemperature(), time, data_t_h, count);
            is_log = 1;
            count++;
            printf("count: %d\r\n", count);
            if (count % 10 == 0)
            {
                struct_to_array_json(data_t_h, data, count);
                LOGI(TAG_DHT, "SAVE to array");
                esp_event_post_to(loop_handle, EVENT_AND_SHEDULE, 1, NULL, 0, 0);
            }
            if (count == 20)
            {
                count = 0;
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
        if (xQueueReceive((QueueHandle_t)pvParameters, &but_pressed, portMAX_DELAY) && !is_wifi_ap_status())
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
                    set_wifi_ap_status(WIFI_AP_ON);
                }
                else
                    LOGE(TAG_AP, "STA is not delated. FAILLED TO CREATE AP.");
            }
            else
            {
                LOGI(TAG_AP, "BUTTON is not 3 sec pressed");
            }
        }
        else if (is_wifi_ap_status())
            LOGE(TAG_AP, "FAILLED TO CREATE AP. AP is already active.");
    };
    vTaskDelete(NULL);
}
