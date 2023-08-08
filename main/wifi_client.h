#ifndef WIFI_CLIENT_H
#define WIFI_CLIENT_H

#include "esp_err.h"

void wifi_init_sta(void);
esp_err_t esp_delete_wifi_sta(void);

#endif
