#ifndef DATA_T_H_H_
#define DATA_T_H_H_
struct data_temperature_and_humidity
{
    char time[64];
    float temperature;
    float humidity;
};

extern char data[1000];
#endif

