
#include "init_first_JSON.h"


void init_first_JSON()
{
    printf("Enter init first JSON");
    char *out;
    collection_about_esp about_struct;
    collect_about(&about_struct);
    cJSON *about = cJSON_CreateObject();
    switch (about_struct.chip_info.model)
    {
    case CHIP_ESP32:
        cJSON_AddItemToObject(about, "Model:", cJSON_CreateString("ESP32"));
        break;
    case CHIP_ESP32S2:
        cJSON_AddItemToObject(about, "Model:", cJSON_CreateString("ESP32S2"));
        break;
    case CHIP_ESP32S3:
        cJSON_AddItemToObject(about, "Model:", cJSON_CreateString("ESP32S3"));
        break;
    case CHIP_ESP32C3:
        cJSON_AddItemToObject(about, "Model:", cJSON_CreateString("ESP32C3"));
        break;
    case CHIP_ESP32H2:
        cJSON_AddItemToObject(about, "Model:", cJSON_CreateString("ESP32H2"));
        break;
    case CHIP_ESP32C2:
        cJSON_AddItemToObject(about, "Model:", cJSON_CreateString("ESP32C2"));
        break;

    default:
        cJSON_AddItemToObject(about, "Model:", cJSON_CreateString("UNDERFINED"));
        break;
    }
    cJSON_AddItemToObject(about, "Count of cores:", cJSON_CreateNumber(about_struct.chip_info.cores));
    cJSON_AddItemToObject(about, "WIFI", cJSON_CreateString("YES"));
    cJSON_AddItemToObject(about, "Model", cJSON_CreateString("AA"));
    cJSON_AddItemToObject(about, "Model", cJSON_CreateString("AA"));
    out = cJSON_Print(about);
    printf("%s\n", out);
}