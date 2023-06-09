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
//#include "collect_about_ESP32.h"
#include "esp_flash.h"

#include "wifi_client.h"
#include "wifi_ap.h"
#include "web_server.h"
#include "spiffs.h"
#include "nvs_flash.h"
#include "timeESP.h"

#include "init_first_JSON.h"


void app_main(void)
{
    printf("Start working ESP\r\n");

        //Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    initilization_file_system_spiffs();
    //output_about();
    init_first_JSON();

    wifi_init_sta();

    setup_server();
    
}
