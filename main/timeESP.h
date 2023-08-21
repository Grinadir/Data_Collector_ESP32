#ifndef TIME_ESP_H
#define TIME_ESP_H
#include <time.h>
void obtain_time(void);
void initialize_sntp(void);
void loop_show_tine(void);
void get_time(char *time_for_update);
time_t get_timestamp();
#endif