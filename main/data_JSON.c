
#include "data_JSON.h"
#include "utils_spiffs.c"
#include "cJSON.h"
#include "collect_about_ESP32.h"
#include "stdio.h"
#include "settings_esp.h"
#define getName(var) #var

static const char *TAG_DATA_JSON = "WORK WITH JSON DATA";
extern struct all_settings_esp all_settings;

void _set_parametr_from_cJSON(cJSON *cjson, char **parametr)
{
    if (cjson != NULL)
    {
        *parametr = cjson->valuestring;
        if (parametr != NULL)
        {
            ESP_LOGI(TAG_DATA_JSON, "Load wifi parametr from /spiffs_data/wifi.json->");
            ESP_LOGI(TAG_DATA_JSON, "     JSON key:           %s", cjson->string);
            ESP_LOGI(TAG_DATA_JSON, "     value of parametr:  %s", *parametr);
        }
        else
            ESP_LOGE(TAG, "Parametr with name %s is NULL!", getName(parametr));
    }
    else
        ESP_LOGE(TAG, " cJSON Object (%s) is NULL!", cjson->string);
}

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
    ESP_LOGI(TAG_DATA_JSON, "Create about.json");
}

void set_wifi_JSON_Info(char *SSID_Client, char *password_client, char *SSID_AP, char *password_ap, uint8_t is_ap_active)
{
    char *jsonString;

    cJSON *wifi = cJSON_CreateObject();
    cJSON_AddItemToObject(wifi, "ssid_client", cJSON_CreateString(SSID_Client));
    cJSON_AddItemToObject(wifi, "password_client", cJSON_CreateString(password_client));
    if (is_ap_active)
        cJSON_AddItemToObject(wifi, "ap_active", cJSON_CreateString("yes"));
    else
        cJSON_AddItemToObject(wifi, "ap_active", cJSON_CreateString("no"));
    jsonString = cJSON_PrintUnformatted(wifi);
    write_file_in_spiffs("/spiffs_data/wifi.json", jsonString);
    ESP_LOGI(TAG_DATA_JSON, "Create wifi.json");
    char text[100];
    read_file_from_spiffs_with_output("/spiffs_data/wifi.json", text);
    ESP_LOGI(TAG, "text=%s", text);
}

void get_wifi_settings(char *SSID_Client, char *password_client, char *SSID_AP, char *password_ap, uint8_t is_ap_active)
{
    char jsonString[300] = "";
    read_file_from_spiffs("/spiffs_data/wifi.json", jsonString);
    cJSON *wifi_settings = cJSON_Parse(jsonString);
    cJSON *json_ssid_client = cJSON_GetObjectItem(wifi_settings, "ssid_client");
    cJSON *json_password_client = cJSON_GetObjectItem(wifi_settings, "password_client");
    cJSON *json_ssid_ap = cJSON_GetObjectItem(wifi_settings, "ssid_ap");
    cJSON *json_password_ap = cJSON_GetObjectItem(wifi_settings, "password_ap");
    cJSON *json_is_ap_active = cJSON_GetObjectItem(wifi_settings, "is_ap_active");
    if (json_ssid_client != NULL) // && json_password_client != NULL)
    {
        SSID_Client = json_ssid_client->valuestring;
        // password_client = json_password_client->valuestring;
        if (SSID_Client != NULL) // && password_client != NULL)
            ESP_LOGI(TAG, "CHECK wifi settings SSID client: %s", SSID_Client);
        else
            ESP_LOGE(TAG, "SSID or password is NULL!");
    }
    else
        ESP_LOGE(TAG, " cJSON Object is NULL!");
}
void init_wifi_settings_from_json_file()
{

    char jsonString[300] = "";
    read_file_from_spiffs("/spiffs_data/wifi.json", jsonString);
    printf("s: %s", jsonString);
    cJSON *wifi_settings = cJSON_Parse(jsonString);
    cJSON *json_ssid_client = cJSON_GetObjectItem(wifi_settings, "ssid_client");
    cJSON *json_password_client = cJSON_GetObjectItem(wifi_settings, "password_client");
    cJSON *json_ssid_ap = cJSON_GetObjectItem(wifi_settings, "ssid_ap");
    cJSON *json_password_ap = cJSON_GetObjectItem(wifi_settings, "password_ap");
    cJSON *json_is_ap_active = cJSON_GetObjectItem(wifi_settings, "is_ap_active");
    _set_parametr_from_cJSON(json_ssid_client, &all_settings.wifi_settings.ssid_client);
    _set_parametr_from_cJSON(json_password_client, &all_settings.wifi_settings.password_client);
    output_settings();
}

void save_humidity_temperature_in_json(float hum, float temp, char *time)
{

    char jsonString[1000] = "";
    static int i = 0;
    read_file_from_spiffs("/spiffs_data/data.json", jsonString);
    cJSON *common_data = cJSON_Parse(jsonString);
    printf("%s\n", jsonString);
    cJSON *json_data_array = cJSON_GetObjectItem(common_data, "data_array");
    cJSON *key = cJSON_GetObjectItem(common_data, "key");
    if (json_data_array != NULL)
    {
        printf("s: %s\n", json_data_array->string);
        printf("i: %d\n", i);
        // i++;
    }
    else
        printf("array is null %d\n", i);

    char buff_temp_big[5];
    char buff_temp[5];

    int ret = snprintf(buff_temp_big, sizeof buff_temp_big, "%.2f", temp);
    strncpy(buff_temp, buff_temp_big, 5);
    printf("buff_temp=%s\n", buff_temp);

    char buff_hum_big[5];
    char buff_hum[5];

    ret = snprintf(buff_hum_big, sizeof buff_hum_big, "%.2f", hum);
    strncpy(buff_hum, buff_hum_big, 5);

    cJSON *wrapper = cJSON_CreateObject();
    // cJSON *data_array = cJSON_CreateArray();
    if (i < 21)
    {
        cJSON *messuare_in_time = cJSON_CreateObject();

        cJSON_AddItemToObject(messuare_in_time, "time", cJSON_CreateString("test_time"));
        cJSON_AddItemToObject(messuare_in_time, "t", cJSON_CreateString(buff_temp));
        cJSON_AddItemToObject(messuare_in_time, "h", cJSON_CreateString(buff_hum));
        cJSON_AddItemToArray(json_data_array, messuare_in_time);
        i++;
    }
    // cJSON *messuare_in_time2 = cJSON_CreateObject();
    // cJSON_AddItemToObject(messuare_in_time2, "time", cJSON_CreateString("22test_time"));
    // cJSON_AddItemToObject(messuare_in_time2, "t", cJSON_CreateString("22buff_temp"));
    // cJSON_AddItemToObject(messuare_in_time2, "h", cJSON_CreateString("22buff_hum"));
    //  cJSON_AddItemToObject(wrapper, "data_array", messuare-in_time);
    ESP_LOGW(TAG, "CHECK POINT");

    // cJSON_AddItemToArray(data_array, messuare_in_time2);

    cJSON_AddItemToObject(wrapper, "data_array", json_data_array);

    // cJSON_AddItemToArray(data_array, )
    // cJSON_AddArrayToObject(wrapper, data)
    // cJSON_AddObjectToObject
    ESP_LOGW(TAG, "CHECK POINT");
    if (wrapper != NULL)
        strcpy(jsonString, cJSON_PrintUnformatted(wrapper));
    else
        ESP_LOGE(TAG, "wrapper is NULL!");
    // strncopy jsonString=cJSON_PrintUnformatted(common_data);
    ESP_LOGW(TAG, "CHECK POINT");
    write_file_in_spiffs("/spiffs_data/data.json", jsonString);
    ESP_LOGI(TAG_DATA_JSON, "Create data.json");
}

void save_humidity_temperature_in_struct(float hum, float temp, char **time, struct data_temperature_and_humidity *data_t_h)
{
    static int i = 0;
    if (i <= 287)
    {
        strncpy(data_t_h[i].time, time, 64);
        data_t_h[i].temperature = temp;
        data_t_h[i].humidity = hum;
    }
    for (int ii = 0; ii <= i; ii++)
    {
        if (ii == i)
        {
            printf("ii: %d\n", ii);
            printf("time: %s\n", data_t_h[ii].time);
            printf("temp: %.2f\n", data_t_h[ii].temperature);
            printf("hum: %.2f\n", data_t_h[ii].humidity);
            printf("-------------------\n");
        }
    }
    if (i <= 287)
        i++;
}

void struct_to_array_json(struct data_temperature_and_humidity *data_t_h, char *array, uint16_t count)
{
    cJSON *wrapper = cJSON_CreateObject();
    cJSON *data_array = cJSON_CreateArray();
    

    char buff_temp[5];
    char buff_hum[5];

    for (uint16_t i = 0; i < count; i++)
    {

        cJSON *messuare_in_time = cJSON_CreateObject();
        snprintf(buff_temp, sizeof buff_temp, "%.2f", data_t_h[i].temperature);
        snprintf(buff_hum, sizeof buff_hum, "%.2f", data_t_h[i].humidity);
        cJSON_AddItemToObject(messuare_in_time, "tm", cJSON_CreateString(data_t_h[i].time));
        cJSON_AddItemToObject(messuare_in_time, "t", cJSON_CreateString(buff_temp));
        cJSON_AddItemToObject(messuare_in_time, "h", cJSON_CreateString(buff_hum));
        cJSON_AddItemToArray(data_array, messuare_in_time);
        
    }
    cJSON_AddItemToObject(wrapper, "data_array", data_array);

    if (wrapper != NULL)
    {

        strcpy(array, cJSON_PrintUnformatted(wrapper));
        printf("%s\n", array);
    }

    else
        ESP_LOGE(TAG, "ERROR with wrapper");
}
