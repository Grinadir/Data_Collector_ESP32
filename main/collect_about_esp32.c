#include "collect_about_ESP32.h"

void collect_about(collection_about_esp *about)
{
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    uint32_t flash_size;
    esp_flash_get_size(NULL, &flash_size);
    flash_size = flash_size / (uint32_t)(1024 * 1024);
    esp_chip_info(&chip_info);
    about->chip_info = chip_info;
    about->flash_size = (uint8_t)flash_size;
    about->free_heap = esp_get_minimum_free_heap_size();
}

void output_about()
{
    esp_chip_info_t chip_info;

    uint32_t flash_size;
    esp_chip_info(&chip_info);
    switch (chip_info.model)
    {
    case CHIP_ESP32:
        printf("MODEL CHIP: ESP32\r\n");
        break;
    case CHIP_ESP32S2:
        printf("MODEL CHIP: ESP32S2\r\n");
        break;
    case CHIP_ESP32S3:
        printf("MODEL CHIP: ESP32S3\r\n");
        break;
    case CHIP_ESP32C3:
        printf("MODEL CHIP: ESP32C3\r\n");
        break;
    case CHIP_ESP32H2:
        printf("MODEL CHIP: ESP32H2\r\n");
        break;
    case CHIP_ESP32C2:
        printf("MODEL CHIP: ESP32C2\r\n");
        break;

    default:
        printf("MODEL NOT DEFINED\r\n");
        break;
    }

    printf("This is %s chip with %d CPU core(s), WiFi%s%s, ",
           CONFIG_IDF_TARGET,
           chip_info.cores,
           (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
           (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");
    unsigned major_rev = chip_info.revision / 100;
    unsigned minor_rev = chip_info.revision % 100;
    printf("silicon revision v%d.%d, ", major_rev, minor_rev);
    if (esp_flash_get_size(NULL, &flash_size) != ESP_OK)
    {
        printf("Get flash size failed");
        return;
    }

    printf("%" PRIu32 "MB %s flash\n", flash_size / (uint32_t)(1024 * 1024),
           (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    printf("Minimum free heap size: %" PRIu32 " bytes\n", esp_get_minimum_free_heap_size());
}