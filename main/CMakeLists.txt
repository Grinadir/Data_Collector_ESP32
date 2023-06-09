idf_component_register(SRCS "init_first_JSON.c" "collect_about_esp32.c" "timeESP.c" "spiffs.c" "wifi_client.c" "wifi_ap.c" "web_server.c" "hello_world_main.c"
                    INCLUDE_DIRS "")
spiffs_create_partition_image(storage ../spiffs_data FLASH_IN_PROJECT)
