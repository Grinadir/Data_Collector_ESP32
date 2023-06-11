#include <stdio.h>
#include "web_server.h"
#include "spiffs.h"
#include "cJSON.h"

// char data_index[] = "<!DOCTYPE html><html><head><style type=\"text/css\">html {  font-family: Arial;  display: inline-block;  margin: 0px auto;  text-align: center;}h1{  color: #070812;  padding: 2vh;}.button {  display: inline-block;  background-color: #b30000; //red color  border: none;  border-radius: 4px;  color: white;  padding: 16px 40px;  text-decoration: none;  font-size: 30px;  margin: 2px;  cursor: pointer;}.button2 {  background-color: #364cf4; //blue color}.content {   padding: 50px;}.card-grid {  max-width: 800px;  margin: 0 auto;  display: grid;  grid-gap: 2rem;  grid-template-columns: repeat(auto-fit, minmax(200px, 1fr));}.card {  background-color: white;  box-shadow: 2px 2px 12px 1px rgba(140,140,140,.5);}.card-title {  font-size: 1.2rem;  font-weight: bold;  color: #034078}</style>  <title>ESP32 WEB SERVER</title>  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">  <link rel=\"icon\" href=\"data:,\">  <link rel=\"stylesheet\" href=\"https://use.fontawesome.com/releases/v5.7.2/css/all.css\"    integrity=\"sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr\" crossorigin=\"anonymous\">  <link rel=\"stylesheet\" type=\"text/css\" ></head><body>  <h2>ESP32 WEB SERVER</h2>  <div class=\"content\">    <div class=\"card-grid\">      <div class=\"card\">        <p><i class=\"fas fa-lightbulb fa-2x\" style=\"color:#c81919;\"></i>     <strong>GPIO2</strong></p>        <p>GPIO state: <strong> ON</strong></p>        <p>          <a href=\"/led2on\"><button class=\"button\">ON</button></a>          <a href=\"/led2off\"><button class=\"button button2\">OFF</button></a>        </p>      </div>    </div>  </div></body></html>";

char data_index[4096];
char data_wifi[4096];
char data_about[4096];
char data_json[4096];
char about_json[600];
char data_log[100];
char data_css[1000];

esp_err_t send_web_page(httpd_req_t *req)
{
    {
        int response;
        response = httpd_resp_send(req, data_index, HTTPD_RESP_USE_STRLEN);

        return response;
    }
}

esp_err_t send_wifi_page(httpd_req_t *req)
{
    int response;
    response = httpd_resp_send(req, data_wifi, HTTPD_RESP_USE_STRLEN);

    return response;
}

esp_err_t send_about_page(httpd_req_t *req)
{
    int response;
    response = httpd_resp_send(req, data_about, HTTPD_RESP_USE_STRLEN);

    return response;
}

esp_err_t send_css(httpd_req_t *req)
{
    {

        int response;
        response = httpd_resp_send(req, data_css, HTTPD_RESP_USE_STRLEN);

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

esp_err_t send_about_json(httpd_req_t *req)
{
    {
        printf("send about json\r\n");
        printf(about_json);

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

esp_err_t post_json(httpd_req_t *req)
{
    char request[req->content_len];
    int ret = httpd_req_recv(req, request, req->content_len);
    printf("%.*s\n", ret, request);
    cJSON *json = cJSON_Parse(request);
    int ch = cJSON_GetObjectItem(json, "id")->valueint;
    printf("ch=%d\n", ch);

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

esp_err_t get_req_about_handler(httpd_req_t *req)
{
    return send_about_page(req);
}

esp_err_t get_css_handler(httpd_req_t *req)
{
    return send_css(req);
}

esp_err_t get_json_handler(httpd_req_t *req)
{
    return send_json(req);
}

esp_err_t get_about_json_handler(httpd_req_t *req)
{
    return send_about_json(req);
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

httpd_uri_t uri_get_about = {
    .uri = "/about",
    .method = HTTP_GET,
    .handler = get_req_about_handler,
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
    printf("Setup server\n");
    
    read_file_from_spiffs("/spiffs_data/index.html", data_index);
    read_file_from_spiffs("/spiffs_data/wifi.html", data_wifi);
    read_file_from_spiffs("/spiffs_data/about.html", data_about);
    read_file_from_spiffs("/spiffs_data/style.css", data_css);
    read_file_from_spiffs("/spiffs_data/data.json", data_json);
    read_file_from_spiffs_with_output("/spiffs_data/log.txt", data_log);
    read_file_from_spiffs_with_output("/spiffs_data/about.json", about_json);
    // test_ssprif(data_index, data_json);
    // printf(data_index);
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;

    if (httpd_start(&server, &config) == ESP_OK)
    {
        httpd_register_uri_handler(server, &uri_get);
        httpd_register_uri_handler(server, &uri_get_wifi);
        httpd_register_uri_handler(server, &uri_get_about);
        httpd_register_uri_handler(server, &uri_get_about_json);
        httpd_register_uri_handler(server, &css_get);
        httpd_register_uri_handler(server, &json_get);
        httpd_register_uri_handler(server, &json_post);
        httpd_register_uri_handler(server, &log_get);
    }

    return server;
}