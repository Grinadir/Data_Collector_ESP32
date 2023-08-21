#include "data_temperature_and_humidity.h"
#include "string.h"
#include "stdio.h"


void save_humidity_temperature_in_struct(float hum, float temp, char time[], struct data_temperature_and_humidity *data_t_h)
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