#include "utils_spiffs.h"

#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_spiffs.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "string.h"
#include "cJSON.h"
#include <stdio.h>

static const char *TAG = "SPIFFS";

void initilization_file_system_spiffs(){
        ESP_LOGI(TAG, "Initializing SPIFFS");

    esp_vfs_spiffs_conf_t conf = {
        .base_path = "/spiffs_data",
        .partition_label = NULL,
        .max_files = 15,
        .format_if_mount_failed = true};

    esp_err_t ret = esp_vfs_spiffs_register(&conf);

    if (ret != ESP_OK)
    {
        if (ret == ESP_FAIL)
        {
            ESP_LOGE(TAG, "Failed to mount or format filesystem");
        }
        else if (ret == ESP_ERR_NOT_FOUND)
        {
            ESP_LOGE(TAG, "Failed to find SPIFFS partition");
        }
        else
        {
            ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
        }
        return;
    }

    size_t total = 0, used = 0;
    ret = esp_spiffs_info(conf.partition_label, &total, &used);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to get SPIFFS partition information (%s)", esp_err_to_name(ret));
    }
    else
    {
        ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);
    }



}

void read_file_from_spiffs(const char *path, char * text){
    FILE *file = fopen(path, "r");
    if (file == NULL)
    {
        ESP_LOGE(TAG, "File %s does not exist!", path);
    }
    else
    {
        char line[60] = "";
        while (fgets(line, sizeof(line), file) != NULL)
        {
            strncat(text, line, sizeof(line) - 1);
        }
        memset(line,'\0',sizeof(line));
        fclose(file);

    }
}

void read_file_from_spiffs_with_output(const char *path, char *text){
    ESP_LOGI(TAG, "read_file_from_spiffs_with_output");
    FILE *file = fopen(path, "r");
    if (file == NULL)
    {
        ESP_LOGE(TAG, "File %s does not exist!", path);
    }
    else
    {
        char line[60] = "";
        while (fgets(line, sizeof(line), file) != NULL)
        {
            strncat(text, line, sizeof(line) - 1);
            
        }
        memset(line,'\0',sizeof(line));
        fclose(file);

    }
}

void write_file_in_spiffs(const char * path, char * text){
    ESP_LOGI(TAG, "Writting in %s", path);
    FILE *file = fopen(path, "w");
    if (file == NULL)
    {
        ESP_LOGE(TAG, "Failed to open file for writing");
        return;
    }

    ESP_LOGI(TAG, "Writing data to file: %s", path);
    fprintf(file, text); 
    fclose(file);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    ESP_LOGI(TAG, "File written");

}