#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <esp_http_server.h>

httpd_handle_t setup_server(void);
void update_for_server();

#endif