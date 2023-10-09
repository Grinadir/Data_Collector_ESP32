#include "data_temperature_and_humidity.h"
#include "string.h"
#include "stdio.h"

void save_humidity_temperature_in_struct(float hum, float temp, char time[], struct data_temperature_and_humidity *data_t_h, uint8_t index)
{

    strncpy(data_t_h[index].time, time, 64);
    data_t_h[index].temperature = temp;
    data_t_h[index].humidity = hum;

    printf("index: %d\n", index);
    printf("time: %s\n", data_t_h[index].time);
    printf("temp: %.2f\n", data_t_h[index].temperature);
    printf("hum: %.2f\n", data_t_h[index].humidity);
    printf("-------------------\n");
}