#ifndef DATA_T_H_H_
#define DATA_T_H_H_

#include "stdint.h"
struct data_temperature_and_humidity
{
    char time[64];
    float temperature;
    float humidity;
};
void save_humidity_temperature_in_struct(float hum, float temp, char time[], struct data_temperature_and_humidity *data_t_h, uint8_t index);
extern char data[1000];
#endif

