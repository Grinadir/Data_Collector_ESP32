
#include "init_first_JSON.h"
#include "spiffs.c"
#include "cJSON.h"
#include "collect_about_ESP32.h"
#include "stdio.h"


void init_first_JSON()
{
    printf("Enter init first JSON\r\n");
    char *jsonString;
    char *nullTerminate='\0';
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
    cJSON_AddItemToObject(about, "Mrt", cJSON_CreateString("AA"));
    cJSON_AddItemToObject(about, "Mew", cJSON_CreateString("AA"));
    jsonString = cJSON_PrintUnformatted(about);
    //printf("%s\n", jsonString);
    //write_file_in_spiffs("/spiffs_data/about.json", "WRITE TESTING\n\r");
    int i=1;
    while (i!=0)
    {
        write_file_in_spiffs("/spiffs_data/about.json", jsonString);
        i--;

    }
    
    
    

}