#ifndef TASKS_H
#define TASKS_H

enum {
    NEED_SEND_DATA_SERVER,
    WIFI_CONNECTED,
    DATA_ALREADY_SEND,

} event_for_send;


void measure_DHT_save_data_JSON_task(void *pvParameter);
void create_wifi_access_point(void *pvParameters);
void init_event_and_schedule_system();

#endif