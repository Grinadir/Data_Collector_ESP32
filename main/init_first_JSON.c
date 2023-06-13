
#include "init_first_JSON.h"
#include "spiffs.c"
#include "cJSON.h"
#include "collect_about_ESP32.h"
#include "stdio.h"

static const char *TAG_INIT_FIRST_JSON = "INIT FIRST JSON";

void init_first_JSON()
{
    char *jsonString;
    collection_about_esp about_struct;
    collect_about(&about_struct);
    cJSON *about = cJSON_CreateObject();
    switch (about_struct.chip_info.model)
    {
    case CHIP_ESP32:
        cJSON_AddItemToObject(about, "model", cJSON_CreateString("ESP32"));
        break;
    case CHIP_ESP32S2:
        cJSON_AddItemToObject(about, "model", cJSON_CreateString("ESP32S2"));
        break;
    case CHIP_ESP32S3:
        cJSON_AddItemToObject(about, "model", cJSON_CreateString("ESP32S3"));
        break;
    case CHIP_ESP32C3:
        cJSON_AddItemToObject(about, "model", cJSON_CreateString("ESP32C3"));
        break;
    case CHIP_ESP32H2:
        cJSON_AddItemToObject(about, "model", cJSON_CreateString("ESP32H2"));
        break;
    case CHIP_ESP32C2:
        cJSON_AddItemToObject(about, "model", cJSON_CreateString("ESP32C2"));
        break;

    default:
        cJSON_AddItemToObject(about, "model", cJSON_CreateString("UNDERFINED"));
        break;
    }
    cJSON_AddItemToObject(about, "Count of cores:", cJSON_CreateNumber(about_struct.chip_info.cores));
    cJSON_AddItemToObject(about, "flash_size", cJSON_CreateNumber(about_struct.flash_size));
    cJSON_AddItemToObject(about, "free_heap", cJSON_CreateNumber(about_struct.free_heap));
    cJSON_AddItemToObject(about, "WIFI", cJSON_CreateString("YES"));
    jsonString = cJSON_PrintUnformatted(about);
    write_file_in_spiffs("/spiffs_data/about.json", jsonString);
    ESP_LOGI(TAG_INIT_FIRST_JSON, "Create about.json");   
}