/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_flash.h"
#include "esp_log.h"
#include "wifi_client.h"
#include "wifi_ap.h"
#include "web_server.h"
#include "utils_spiffs.h"
#include "nvs_flash.h"
#include "timeESP.h"

#include "settings_esp.h"
#include "data_JSON.h"
#include "driver/gpio.h"
#include "hal/gpio_types.h"
#include "data_temperature_and_humidity.h"
#include "DHT.h"
#include "tasks.h"
#include "http_esp_client.h"

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

static const char *TAG = "MAIN";

// uint8_t is_ap_active = 0;

struct all_settings_esp all_settings;
struct statuses stat_e;

struct data_temperature_and_humidity data_t_h[288];

char data[1000] = "\0";

static QueueHandle_t button_queue = NULL;

void IRAM_ATTR isrHandler(void *arg)
{
  BaseType_t xHigherPriorityTaskWoken, xResult;
  xHigherPriorityTaskWoken = pdFALSE;
  bool pressed = true;
  xResult = xQueueSendFromISR(button_queue, &pressed, &xHigherPriorityTaskWoken);
  if (xResult == pdPASS)
  {
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
  };
}
void app_main(void)
{
  LOGI(TAG, "START MAIN APLICATION");

  button_queue = xQueueCreate(32, sizeof(bool));

  gpio_install_isr_service(0);
  gpio_set_direction(GPIO_NUM_18, GPIO_MODE_INPUT);
  gpio_set_pull_mode(GPIO_NUM_18, GPIO_PULLUP_ONLY);

  gpio_isr_handler_add(GPIO_NUM_18, isrHandler, NULL);
  gpio_set_intr_type(GPIO_NUM_18, GPIO_INTR_NEGEDGE);
  gpio_intr_enable(GPIO_NUM_18);

  // Initialize NVS
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
  {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  initilization_file_system_spiffs();
  init_first_JSON();
  init_wifi_settings_from_json_file();
  vTaskDelay(500 / portTICK_PERIOD_MS);
  wifi_init_sta();
  setup_server();
  xTaskCreatePinnedToCore(&create_wifi_access_point, "AP_task", 4096, (void *)button_queue, 5, NULL, 0);

  if (is_wifi_sta_status())
  {
    LOGI(TAG, "Try to obtain time");
    obtain_time();
  }
  else
    LOGE(TAG, "WIFI STA is disconnected and the time is not updated");
  vTaskDelay(3000 / portTICK_PERIOD_MS);

  if (is_time_sync())
    xTaskCreate(&measure_DHT_save_data_JSON_task, "DHT_task", 60000, NULL, 5, NULL);
  else
    LOGE(TAG, "Task for messuare temp and humm not start");

  init_event_and_schedule_system();
}
