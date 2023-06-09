#ifndef COLLECT_ABOUT_ESP32_H
#define COLLECT_ABOUT_ESP32_H


#include "esp_chip_info.h"
#include "stdio.h"
#include "esp_flash.h"
#include <inttypes.h>
#include "esp_system.h"

typedef struct {
    esp_chip_info_t chip_info;
    uint8_t flash_size;
    uint32_t free_heap;

}collection_about_esp;


void collect_about(collection_about_esp *about);
void output_about();

#endif