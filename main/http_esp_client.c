#include "esp_http_client.h"
#include "esp_log.h"


void send_m(){
ESP_LOGW("HTTP TEST", "SEND TO SERVER!");
esp_http_client_config_t request;
memset(&request, 0, sizeof(request));
request.host="";
request.port=3000;
request.path="/post_data";
request.user_agent="ESP32";
request.query="ddddddd";
request.transport_type=HTTP_TRANSPORT_OVER_TCP;
request.is_async=false;

request.timeout_ms=60000;
request.disable_auto_redirect=false;
request.max_redirection_count=0;

esp_http_client_handle_t client=esp_http_client_init(&request);
if(client){
    esp_http_client_perform(client);
    esp_http_client_cleanup(client);
}





}