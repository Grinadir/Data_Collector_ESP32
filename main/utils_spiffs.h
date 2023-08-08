#ifndef UTILS_SPIFFS_H
#define UTILS_SPIFFS_H


void initilization_file_system_spiffs();
//void test_ssprif(char index[1024], char json[200]);
void read_file_from_spiffs(const char *path, char * text);
void write_file_in_spiffs(const char * path, char * text);
void read_file_from_spiffs_with_output(const char * path, char * text);

#endif