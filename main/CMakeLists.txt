idf_component_register(SRCS "data_JSON.c" "utils_spiffs.c" "main.c" "collect_about_esp32.c" "timeESP.c" "wifi_client.c" "wifi_ap.c" "web_server.c" "settings_esp.c" "DHT.c"
                    INCLUDE_DIRS "")
spiffs_create_partition_image(storage ../spiffs_data FLASH_IN_PROJECT)
