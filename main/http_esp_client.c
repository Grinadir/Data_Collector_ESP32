#include "esp_http_client.h"
#include "esp_log.h"
#include "esp_netif_ip_addr.h"

static char *TAG = "HTTP ESP CLIENT";
#define MAX_HTTP_OUTPUT_BUFFER 2048

void send_m(char *ip_addr, uint32_t port, char *post_data)
{
    ESP_LOGW(TAG, "SEND TO SERVER!");

    esp_http_client_config_t config = {
        .host = ip_addr,
        .port = port,
        .path ="/",
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);

    // POST
    const char *_post_data = post_data;
    esp_http_client_set_url(client, "/post");
    esp_http_client_set_method(client, HTTP_METHOD_POST);
    esp_http_client_set_header(client, "Content-Type", "application/json");
    esp_http_client_set_post_field(client, _post_data, strlen(_post_data));
    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK)
    {
        ESP_LOGI(TAG, "HTTP POST Status = %d, content_length = %" PRId64,
                 esp_http_client_get_status_code(client),
                 esp_http_client_get_content_length(client));
    }
    else
    {
        ESP_LOGE(TAG, "HTTP POST request failed: %s", esp_err_to_name(err));
    }
    esp_http_client_cleanup(client);
}
// ESP_LOG_BUFFER_HEX(TAG, local_response_buffer, strlen(local_response_buffer));
