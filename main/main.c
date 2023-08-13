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

#define BUTTON_AP_WIFI GPIO_NUM_18

static const char *TAG = "MAIN";

uint8_t is_ap_active = 0;

struct all_settings_esp all_settings;

struct data_temperature_and_humidity data_t_h[288];

static QueueHandle_t button_queue = NULL;

void task_wifi_server(void *pvParameters)
{
  bool but_pressed;
  while (1)
  {
    ESP_LOGI(TAG, "Start task wifi server");
    // Ждем события нажатия кнопки в очереди
    if (xQueueReceive(button_queue, &but_pressed, portMAX_DELAY) && is_ap_active == 0)
    {
      ESP_LOGI("ISR", "Button is pressed");
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
        ESP_LOGI("ISR", "BUTTON 3 sec pressed!");
        if (esp_delete_wifi_sta() == ESP_OK)
        {
          wifi_init_softap();
          update_for_server();
          is_ap_active = 1;
        }
        else
          ESP_LOGE("ISR", "FAILLED TO CREATE AP");
      }
      else
      {
        ESP_LOGI("ISR", "BUTTON not 3 sec pressed");
      }
    }
    else if (is_ap_active)
    {
      ESP_LOGE("ISR", "AP is already active!");
    }
  };
  vTaskDelete(NULL);
}

void DHT_task(void *pvParameter)
{
  setDHTgpio(GPIO_NUM_4);
  ESP_LOGI(TAG, "Starting DHT Task\n\n");
  char time[64];
  uint8_t is_log = 0;

  while (1)
  {
    // strncpy(time, getTime(), 64);
    get_time(time);
    printf("%lu\n", (unsigned long)esp_get_free_heap_size);
    printf("%lu\n", (unsigned long)esp_get_free_internal_heap_size);

    vTaskDelay(100 / portTICK_PERIOD_MS);
    if ((long)get_timestamp() % 60 == 0 && is_log == 0)
    {
      // strncpy(time, getTime(), 64);
      get_time(time);
      ESP_LOGI(TAG, "=== Reading DHT ===\n");
      int ret = readDHT();

      errorHandler(ret);

      ESP_LOGI(TAG, "Hum: %.1f Tmp: %.1f\n", getHumidity(), getTemperature());
      // char timeQ[64];
      //  timeQ=getTime();
      // strncpy(time, getTime(), 64);
      get_time(time);
      printf("TEST time %s\n", time);
      save_humidity_temperature_in_struct(getHumidity(), getTemperature(), &time, data_t_h);
      // free(timeQ);
      printf("%lu\n", (unsigned long)esp_get_free_heap_size);
      printf("%lu\n", (unsigned long)esp_get_free_internal_heap_size);
      // -- wait at least 2 sec before reading again ------------
      // The interval of whole process must be beyond 2 seconds !!
      is_log = 1;
    }
    else if ((long)get_timestamp() % 60 != 0 && is_log == 1)
    {
      is_log = 0;
    }
  }
  // ESP_LOGE("DHT_TASK FAILED")
  vTaskDelete(NULL);
}

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
  ESP_LOGI(TAG, "-=START MAIN APLICATION=-");

  button_queue = xQueueCreate(32, sizeof(bool));

  xTaskCreatePinnedToCore(task_wifi_server, "wifi server", 4096, NULL, 5, NULL, 0);

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
  initialize_sntp();
  obtain_time();
  // loop_show_tine();

  xTaskCreate(&DHT_task, "DHT_task", 60000, NULL, 5, NULL);
}
