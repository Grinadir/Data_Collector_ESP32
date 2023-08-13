#include <stdio.h>
#include "web_server.h"
#include "utils_spiffs.h"
#include "cJSON.h"
#include "esp_log.h"
#include "wifi_ap.h"
#include "wifi_client.h"
#include "data_JSON.h"

// char buf_index_html[] = "<!DOCTYPE html><html><head><style type=\"text/css\">html {  font-family: Arial;  display: inline-block;  margin: 0px auto;  text-align: center;}h1{  color: #070812;  padding: 2vh;}.button {  display: inline-block;  background-color: #b30000; //red color  border: none;  border-radius: 4px;  color: white;  padding: 16px 40px;  text-decoration: none;  font-size: 30px;  margin: 2px;  cursor: pointer;}.button2 {  background-color: #364cf4; //blue color}.content {   padding: 50px;}.card-grid {  max-width: 800px;  margin: 0 auto;  display: grid;  grid-gap: 2rem;  grid-template-columns: repeat(auto-fit, minmax(200px, 1fr));}.card {  background-color: white;  box-shadow: 2px 2px 12px 1px rgba(140,140,140,.5);}.card-title {  font-size: 1.2rem;  font-weight: bold;  color: #034078}</style>  <title>ESP32 WEB SERVER</title>  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">  <link rel=\"icon\" href=\"data:,\">  <link rel=\"stylesheet\" href=\"https://use.fontawesome.com/releases/v5.7.2/css/all.css\"    integrity=\"sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr\" crossorigin=\"anonymous\">  <link rel=\"stylesheet\" type=\"text/css\" ></head><body>  <h2>ESP32 WEB SERVER</h2>  <div class=\"content\">    <div class=\"card-grid\">      <div class=\"card\">        <p><i class=\"fas fa-lightbulb fa-2x\" style=\"color:#c81919;\"></i>     <strong>GPIO2</strong></p>        <p>GPIO state: <strong> ON</strong></p>        <p>          <a href=\"/led2on\"><button class=\"button\">ON</button></a>          <a href=\"/led2off\"><button class=\"button button2\">OFF</button></a>        </p>      </div>    </div>  </div></body></html>";
extern uint8_t is_ap_active;
static char TAG[] = "WEB SERVER";
char buf_index_html[4096];
char buf_wifi_html[4096];
char buf_data_html[4096];
char buf_about_html[4096];
char data_json[4096];
char about_json[600];
char wifi_json[300];
char data_log[100];
char buff_css[1000];

void load_spiffs_to_buffer()
{
    read_file_from_spiffs("/spiffs_data/index.html", buf_index_html);
    read_file_from_spiffs("/spiffs_data/wifi.html", buf_wifi_html);
    read_file_from_spiffs("/spiffs_data/data.html", buf_data_html);
    read_file_from_spiffs("/spiffs_data/about.html", buf_about_html);
    read_file_from_spiffs("/spiffs_data/style.css", buff_css);
    read_file_from_spiffs("/spiffs_data/data.json", data_json);
    read_file_from_spiffs("/spiffs_data/wifi.json", wifi_json);
    read_file_from_spiffs_with_output("/spiffs_data/log.txt", data_log);
    read_file_from_spiffs_with_output("/spiffs_data/about.json", about_json);
    ESP_LOGI(TAG, "Load data buffer for server.");
}

void clear_buffer()
{

    strcpy(buf_index_html, "");
    strcpy(buf_wifi_html, "");
    strcpy(buf_data_html, "");
    strcpy(buf_about_html, "");
    strcpy(data_json, "");
    strcpy(about_json, "");
    strcpy(wifi_json, "");
    strcpy(data_log, "");
    strcpy(buff_css, "");
    ESP_LOGI(TAG, "Clear data buffer for server.");
}

void update_for_server()
{
    clear_buffer();
    load_spiffs_to_buffer();
    ESP_LOGI(TAG, "Data buffer for server was updated!");
}

esp_err_t send_web_page(httpd_req_t *req)
{
    {
        int response;
        response = httpd_resp_send(req, buf_index_html, HTTPD_RESP_USE_STRLEN);

        return response;
    }
}

esp_err_t send_wifi_page(httpd_req_t *req)
{
    int response;
    response = httpd_resp_send(req, buf_wifi_html, HTTPD_RESP_USE_STRLEN);

    return response;
}

esp_err_t send_data_page(httpd_req_t *req)
{
    int response;
    response = httpd_resp_send(req, buf_data_html, HTTPD_RESP_USE_STRLEN);

    return response;
}

esp_err_t send_about_page(httpd_req_t *req)
{
    int response;
    response = httpd_resp_send(req, buf_about_html, HTTPD_RESP_USE_STRLEN);

    return response;
}

esp_err_t send_css(httpd_req_t *req)
{
    {

        int response;
        response = httpd_resp_send(req, buff_css, HTTPD_RESP_USE_STRLEN);

        return response;
    }
}

esp_err_t send_json(httpd_req_t *req)
{
    {

        int response;
        response = httpd_resp_send(req, data_json, HTTPD_RESP_USE_STRLEN);

        return response;
    }
}

esp_err_t send_wifi_json(httpd_req_t *req)
{
    {
        int response;
        response = httpd_resp_send(req, wifi_json, HTTPD_RESP_USE_STRLEN);

        return response;
    }
}

esp_err_t post_wifi_json(httpd_req_t *req)
{
    char request[req->content_len];
    int ret = httpd_req_recv(req, request, req->content_len);
    printf("%.*s\n", ret, request);
    cJSON *json = cJSON_Parse(request);
    cJSON *ssid_client = cJSON_GetObjectItem(json, "ssid_client");
    cJSON *password_client = cJSON_GetObjectItem(json, "password_client");
    if (ssid_client != NULL && password_client!=NULL){
        printf("wifi client %s\n", ssid_client->valuestring);
        printf("password client: %s\n", password_client->valuestring);
    }
    else
        ESP_LOGE(TAG, "wifiClient is NULL");
    set_wifi_JSON_Info(ssid_client->valuestring, password_client->valuestring, "22222", "3333", 1);
    update_for_server();
    return 0;
}

esp_err_t send_about_json(httpd_req_t *req)
{
    {
        int response;
        response = httpd_resp_send(req, about_json, HTTPD_RESP_USE_STRLEN);

        return response;
    }
}

esp_err_t send_log(httpd_req_t *req)
{
    {
        int response;
        response = httpd_resp_send(req, data_log, HTTPD_RESP_USE_STRLEN);

        return response;
    }
}

esp_err_t stop_ap(httpd_req_t *req)
{
    ESP_LOGE(TAG, "STOP AP");
    int response;
    if (esp_delete_wifi_ap() == ESP_OK)
    {
        is_ap_active = 0;
        wifi_init_sta();
    }

    response = httpd_resp_send(req, "OK", HTTPD_RESP_USE_STRLEN);

    return 0;
}

esp_err_t post_json(httpd_req_t *req)
{
    char request[req->content_len];
    int ret = httpd_req_recv(req, request, req->content_len);
    printf("%.*s\n", ret, request);
    cJSON *json = cJSON_Parse(request);
    char *ssid_client = cJSON_GetObjectItem(json, "ssid_client")->valuestring;
    printf("ssid client=%s\n", ssid_client);

    return 0;
}

esp_err_t get_req_handler(httpd_req_t *req)
{
    return send_web_page(req);
}

esp_err_t get_req_wifi_handler(httpd_req_t *req)
{
    return send_wifi_page(req);
}

esp_err_t get_req_data_handler(httpd_req_t *req)
{
    return send_data_page(req);
}

esp_err_t get_req_about_handler(httpd_req_t *req)
{
    return send_about_page(req);
}

esp_err_t get_exit(httpd_req_t *req)
{
    return 0; 
}

esp_err_t get_css_handler(httpd_req_t *req)
{
    return send_css(req);
}

esp_err_t get_json_handler(httpd_req_t *req)
{
    return send_json(req);
}

esp_err_t get_wifi_json_handler(httpd_req_t *req)
{
    return send_wifi_json(req);
}

esp_err_t post_wifi_json_handler(httpd_req_t *req)
{
    return post_wifi_json(req);
}

esp_err_t get_about_json_handler(httpd_req_t *req)
{
    return send_about_json(req);
}

esp_err_t stop_ap_handler(httpd_req_t *req)
{
    return stop_ap(req);
}

esp_err_t post_json_handler(httpd_req_t *req)
{
    return post_json(req);
}

esp_err_t log_handler(httpd_req_t *req)
{
    return send_log(req);
}

httpd_uri_t uri_get = {
    .uri = "/",
    .method = HTTP_GET,
    .handler = get_req_handler,
    .user_ctx = NULL};

httpd_uri_t uri_get_wifi = {
    .uri = "/wifi_settings",
    .method = HTTP_GET,
    .handler = get_req_wifi_handler,
    .user_ctx = NULL};

httpd_uri_t uri_get_data = {
    .uri = "/data",
    .method = HTTP_GET,
    .handler = get_req_data_handler,
    .user_ctx = NULL};

httpd_uri_t uri_get_about = {
    .uri = "/about",
    .method = HTTP_GET,
    .handler = get_req_about_handler,
    .user_ctx = NULL};

httpd_uri_t uri_exit = {
    .uri = "/exit",
    .method = HTTP_GET,
    .handler = get_exit,
    .user_ctx = NULL};

httpd_uri_t uri_get_about_json = {
    .uri = "/about_json",
    .method = HTTP_GET,
    .handler = get_about_json_handler,
    .user_ctx = NULL};

httpd_uri_t css_get = {
    .uri = "/style.css",
    .method = HTTP_GET,
    .handler = get_css_handler,
    .user_ctx = NULL};

httpd_uri_t json_get = {
    .uri = "/json",
    .method = HTTP_GET,
    .handler = get_json_handler,
    .user_ctx = NULL};

httpd_uri_t wifi_json_get = {
    .uri = "/wifi_json",
    .method = HTTP_GET,
    .handler = get_wifi_json_handler,
    .user_ctx = NULL};

httpd_uri_t wifi_json_post = {
    .uri = "/wifi_json_post",
    .method = HTTP_POST,
    .handler = post_wifi_json_handler,
    .user_ctx = NULL};

httpd_uri_t get_stop_ap = {
    .uri = "/stop_ap",
    .method = HTTP_GET,
    .handler = stop_ap_handler,
    .user_ctx = NULL};

httpd_uri_t json_post = {
    .uri = "/post/json",
    .method = HTTP_POST,
    .handler = post_json_handler,
    .user_ctx = NULL};

httpd_uri_t log_get = {
    .uri = "/log",
    .method = HTTP_GET,
    .handler = log_handler,
    .user_ctx = NULL};

httpd_handle_t setup_server(void)
{
    load_spiffs_to_buffer();
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;
    if (httpd_start(&server, &config) == ESP_OK)
    {
        httpd_register_uri_handler(server, &uri_get);
        httpd_register_uri_handler(server, &uri_get_wifi);
        httpd_register_uri_handler(server, &uri_get_data);
        httpd_register_uri_handler(server, &uri_get_about);
        httpd_register_uri_handler(server, &uri_get_about_json);
        httpd_register_uri_handler(server, &wifi_json_get);
        httpd_register_uri_handler(server, &wifi_json_post);
        httpd_register_uri_handler(server, &css_get);
        httpd_register_uri_handler(server, &json_get);
        httpd_register_uri_handler(server, &json_post);
        httpd_register_uri_handler(server, &log_get);
        httpd_register_uri_handler(server, &get_stop_ap);
    }
    ESP_LOGI(TAG, "URI addresses have been successfully registered.");
    return server;
}
