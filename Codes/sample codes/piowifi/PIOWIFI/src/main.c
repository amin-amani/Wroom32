/* BSD Socket API Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <string.h>
#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>


/* The examples use simple WiFi configuration that you can set via
   'make menuconfig'.
   If you'd rather not, just change the below entries to strings with
   the config you want - ie #define EXAMPLE_WIFI_SSID "mywifissid"
*/
#define EXAMPLE_WIFI_SSID "espwifi"
#define EXAMPLE_WIFI_PASS ""

#define PORT 8888
bool firstTime=true;

/* FreeRTOS event group to signal when we are connected & ready to make a request */
static EventGroupHandle_t wifi_event_group;

const int IPV4_GOTIP_BIT = BIT0;
const int IPV6_GOTIP_BIT = BIT1;

static const char *TAG = "example";

static void tcp_server_task(void *pvParameters)
{
    char rx_buffer[128];
    char addr_str[128];
    int addr_family;
    int ip_protocol;

    while (1) {


        ESP_LOGE(TAG, "Start While"); 
        struct sockaddr_in destAddr;
        destAddr.sin_addr.s_addr = htonl(INADDR_ANY);
        destAddr.sin_family = AF_INET;
        destAddr.sin_port = htons(PORT);
        addr_family = AF_INET;
        ip_protocol = IPPROTO_IP;
        inet_ntoa_r(destAddr.sin_addr, addr_str, sizeof(addr_str) - 1);

        int listen_sock = socket(addr_family, SOCK_STREAM, ip_protocol);
        if (listen_sock < 0) {
            ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
            break;
        }
        ESP_LOGI(TAG, "Socket created");

        int err = bind(listen_sock, (struct sockaddr *)&destAddr, sizeof(destAddr));
        if (err != 0) {
            ESP_LOGE(TAG, "Socket unable to bind: errno %d", errno);
            shutdown(listen_sock, 0);
            close(listen_sock);
            break;
        }
        ESP_LOGI(TAG, "Socket binded");

        err = listen(listen_sock, 1);
        if (err != 0) {
            ESP_LOGE(TAG, "Error occured during listen: errno %d", errno);
            break;
        }
        ESP_LOGI(TAG, "Socket listening");

        struct sockaddr_in6 sourceAddr; // Large enough for both IPv4 or IPv6
        uint addrLen = sizeof(sourceAddr);
        int sock = accept(listen_sock, (struct sockaddr *)&sourceAddr, &addrLen);
        if (sock < 0) {
            ESP_LOGE(TAG, "Unable to accept connection: errno %d", errno);
            break;
        }
        ESP_LOGI(TAG, "Socket accepted");

        while (1) {
            int len = recv(sock, rx_buffer, sizeof(rx_buffer) - 1, 0);
            // Error occured during receiving
            if (len < 0) {
                ESP_LOGE(TAG, "recv failed: errno %d", errno);
                break;
            }
            // Connection closed
            else if (len == 0) {
                ESP_LOGI(TAG, "Connection closed");
            shutdown(sock, 0);
            close(sock);
             shutdown(listen_sock, 0);
            close(listen_sock);
                break;
            }
            // Data received
            else {
                // Get the sender's ip address as string
                if (sourceAddr.sin6_family == PF_INET) {
                    inet_ntoa_r(((struct sockaddr_in *)&sourceAddr)->sin_addr.s_addr, addr_str, sizeof(addr_str) - 1);
                } else if (sourceAddr.sin6_family == PF_INET6) {
                    inet6_ntoa_r(sourceAddr.sin6_addr, addr_str, sizeof(addr_str) - 1);
                }

                rx_buffer[len] = 0; // Null-terminate whatever we received and treat like a string
                ESP_LOGI(TAG, "Received %d bytes from %s:", len, addr_str);
                ESP_LOGI(TAG, "%s", rx_buffer);

                int err = send(sock, rx_buffer, len, 0);
                if (err < 0) {
                    ESP_LOGE(TAG, "Error occured during sending: errno %d", errno);
                    break;
                }
            }
        }

        if (sock != -1) {
            ESP_LOGE(TAG, "Shutting down socket and restarting...");
            shutdown(sock, 0);
            close(sock);
        
        }
            //break;//by me 
     
    }

     ESP_LOGE(TAG, "Delete task");    
    vTaskDelete(NULL);
}

static esp_err_t event_handler(void *ctx, system_event_t *event)
{
    switch(event->event_id) {
    case SYSTEM_EVENT_AP_STACONNECTED:
        ESP_LOGI(TAG, "station:"MACSTR" join, AID=%d",
                 MAC2STR(event->event_info.sta_connected.mac),
                 event->event_info.sta_connected.aid);
if(firstTime)
firstTime=false;
 
        break;
    case SYSTEM_EVENT_AP_STADISCONNECTED:
        ESP_LOGI(TAG, "station:"MACSTR"leave, AID=%d",
                 MAC2STR(event->event_info.sta_disconnected.mac),
                 event->event_info.sta_disconnected.aid);

        break;

        case SYSTEM_EVENT_AP_STAIPASSIGNED:
        ESP_LOGI(TAG, "station:"MACSTR"assing IP, AID=%d",
          MAC2STR(event->event_info.sta_disconnected.mac),
                 event->event_info.sta_disconnected.aid);
                 xTaskCreate(tcp_server_task, "tcp_server", 4096, NULL, 5, NULL);                 
        
        break;
    default:
        break;
    }
    return ESP_OK;
}

// static esp_err_t event_handler(void *ctx, system_event_t *event)
// {
//     switch (event->event_id) {
//     case SYSTEM_EVENT_STA_START:
//         esp_wifi_connect();
//         ESP_LOGI(TAG, "SYSTEM_EVENT_STA_START");
//         break;
//     case SYSTEM_EVENT_STA_CONNECTED:
//         /* enable ipv6 */
//         tcpip_adapter_create_ip6_linklocal(TCPIP_ADAPTER_IF_STA);
//         break;
//     case SYSTEM_EVENT_STA_GOT_IP:
//         xEventGroupSetBits(wifi_event_group, IPV4_GOTIP_BIT);
//         ESP_LOGI(TAG, "SYSTEM_EVENT_STA_GOT_IP");
//         break;
//     case SYSTEM_EVENT_STA_DISCONNECTED:
//         /* This is a workaround as ESP32 WiFi libs don't currently auto-reassociate. */
//         esp_wifi_connect();
//         xEventGroupClearBits(wifi_event_group, IPV4_GOTIP_BIT);
//         xEventGroupClearBits(wifi_event_group, IPV6_GOTIP_BIT);
//         break;
//     case SYSTEM_EVENT_AP_STA_GOT_IP6:
//         xEventGroupSetBits(wifi_event_group, IPV6_GOTIP_BIT);
//         ESP_LOGI(TAG, "SYSTEM_EVENT_STA_GOT_IP6");

//         char *ip6 = ip6addr_ntoa(&event->event_info.got_ip6.ip6_info.ip);
//         ESP_LOGI(TAG, "IPv6: %s", ip6);
//     default:
//         break;
//     }
//     return ESP_OK;
// }

static void initialise_wifi(void)
{
    tcpip_adapter_init();
    wifi_event_group = xEventGroupCreate();
    ESP_ERROR_CHECK( esp_event_loop_init(event_handler, NULL) );
    // wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    // ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
    // ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
    // wifi_config_t wifi_config = {
    //     .sta = {
    //         .ssid = EXAMPLE_WIFI_SSID,
    //         .password = EXAMPLE_WIFI_PASS,
            
    //     },
    // };


//==============
wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
    wifi_config_t wifi_config = {
        .ap = {
            .ssid = EXAMPLE_WIFI_SSID,
            .ssid_len = strlen(EXAMPLE_WIFI_SSID),
            .password = EXAMPLE_WIFI_PASS,
            .max_connection = 2,
            .authmode = WIFI_AUTH_OPEN// WIFI_AUTH_WPA_WPA2_PSK
        },
    };
    // if (strlen(EXAMPLE_WIFI_PASS) == 0) {
    //     wifi_config.ap.authmode = WIFI_AUTH_OPEN;
    // }
//===============

    ESP_LOGI(TAG, "Setting WiFi configuration SSID %s...", wifi_config.sta.ssid);
    ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_AP) );
    ESP_ERROR_CHECK( esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_config) );
    ESP_ERROR_CHECK( esp_wifi_start() );
}

static void wait_for_ip()
{
    uint32_t bits = IPV4_GOTIP_BIT | IPV6_GOTIP_BIT ;

    ESP_LOGI(TAG, "Waiting for AP connection...");
    xEventGroupWaitBits(wifi_event_group, bits, false, true, portMAX_DELAY);
    ESP_LOGI(TAG, "Connected to AP");
}


void app_main()
{
    ESP_ERROR_CHECK( nvs_flash_init() );
    initialise_wifi();
//    wait_for_ip();

    //xTaskCreate(tcp_server_task, "tcp_server", 4096, NULL, 5, NULL);
}
