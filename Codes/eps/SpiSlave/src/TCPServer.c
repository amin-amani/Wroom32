#include "TCPServer.h"
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
//=================================================================================================================
int ConnectedSocket=-1;
/* FreeRTOS event group to signal when we are connected & ready to make a request */
static EventGroupHandle_t wifi_event_group;
const int IPV4_GOTIP_BIT = BIT0;
const int IPV6_GOTIP_BIT = BIT1;
static const char *TAG = "example";
//=================================================================================================================
bool TCPClientHandler(struct sockaddr_in6 sourceAddr ,int listen_sock,int socket,char *rxBuffer,int buffersize)
{ 
            char addrstr[16];
            int len = recv(socket, rxBuffer, buffersize - 1, 0);
            // Error occured during receiving
            if (len < 0) {
                ESP_LOGE(TAG, "recv failed: errno %d", errno);
                return 0;
            }
            // Connection closed
             if (len == 0) {
                ESP_LOGI(TAG, "Connection closed");
            shutdown(socket, 0);
            close(socket);
             shutdown(listen_sock, 0);
            close(listen_sock);
                return 0;
            }
            // Data received
       
                // Get the sender's ip address as string
                if (sourceAddr.sin6_family == PF_INET) {
                    inet_ntoa_r(((struct sockaddr_in *)&sourceAddr)->sin_addr.s_addr, addrstr, sizeof(addrstr) - 1);
                } else if (sourceAddr.sin6_family == PF_INET6) {
                    inet6_ntoa_r(sourceAddr.sin6_addr, addrstr, sizeof(addrstr) - 1);
                }

                rxBuffer[len] = 0; // Null-terminate whatever we received and treat like a string
                ESP_LOGI(TAG, "Received %d bytes from %s:", len, addrstr);
                ESP_LOGI(TAG, "%s", rxBuffer);
              TCPServerNewDataReceivedCallback(rxBuffer,len);
                // int err = send(socket,rxBuffer, len, 0);
                // if (err < 0) {
                //     ESP_LOGE(TAG, "Error occured during sending: errno %d", errno);
                //     return 0;
                // }
            
            return 1;

}


//=================================================================================================================

int TCPServerSendData(char *data,int len)
{
    if(ConnectedSocket<0)return 0;
int err = send(ConnectedSocket,data, len, 0);
if (err < 0) {
    ESP_LOGE(TAG, "Error occured during sending: errno %d", errno);
    return 0;
}
return err;
}
//=================================================================================================================
 void TCPServerStartListen()
{
    char rx_buffer[128];
    char addr_str[128];


    while (1) {


        ESP_LOGE(TAG, "Start While"); 
        struct sockaddr_in destAddr;
        destAddr.sin_addr.s_addr = htonl(INADDR_ANY);
        destAddr.sin_family = AF_INET;
        destAddr.sin_port = htons(PORT);

        inet_ntoa_r(destAddr.sin_addr, addr_str, sizeof(addr_str) - 1);

        int listen_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
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

ConnectedSocket=sock;
while (TCPClientHandler(sourceAddr,listen_sock,sock,rx_buffer,sizeof(rx_buffer)));



        if (sock != -1) {
            ESP_LOGE(TAG, "Shutting down socket and restarting...");
            shutdown(sock, 0);
            close(sock);
        ConnectedSocket=1;
        }
    
     
    }

}
//=================================================================================================================
static esp_err_t event_handler(void *ctx, system_event_t *event)
{
    switch(event->event_id) {
    case SYSTEM_EVENT_AP_STACONNECTED:
        ESP_LOGI(TAG, "station:"MACSTR" join, AID=%d",
                 MAC2STR(event->event_info.sta_connected.mac),
                 event->event_info.sta_connected.aid);

 
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
                 TCPNewClientConnectedCallback();
              //   xTaskCreate(tcp_server_task, "tcp_server", 4096, NULL, 5, NULL);                 
        
        break;
    default:
        break;
    }
    return ESP_OK;
}
//=================================================================================================================

//  void TCPServerInit(void)
// {
//     tcpip_adapter_init();
//     wifi_event_group = xEventGroupCreate();
//     ESP_ERROR_CHECK( esp_event_loop_init(event_handler, NULL) );

//     wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
//     ESP_ERROR_CHECK(esp_wifi_init(&cfg));
//     ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
//     wifi_config_t wifi_config = {
//         .ap = {
//             .ssid = EXAMPLE_WIFI_SSID,
//             .ssid_len = strlen(EXAMPLE_WIFI_SSID),
//             .password = EXAMPLE_WIFI_PASS,
//             .max_connection = 2,
//             .authmode = WIFI_AUTH_OPEN// WIFI_AUTH_WPA_WPA2_PSK
//         },
//     };

//     ESP_LOGI(TAG, "Setting WiFi configuration SSID %s...", wifi_config.sta.ssid);
//     ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_AP) );
//     ESP_ERROR_CHECK( esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_config) );
//     ESP_ERROR_CHECK( esp_wifi_start() );
// }
 void TCPServerInit(char *ssid,char*password)
{
    tcpip_adapter_init();
    wifi_event_group = xEventGroupCreate();
    ESP_ERROR_CHECK( esp_event_loop_init(event_handler, NULL) );

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
    wifi_config_t wifi_config = {
        .ap = {
            .ssid =EXAMPLE_WIFI_SSID,
            .ssid_len = strlen(EXAMPLE_WIFI_SSID),
            .password =EXAMPLE_WIFI_PASS,
            .max_connection = 2,
            .authmode =  WIFI_AUTH_WPA_WPA2_PSK
        },
    };
    
   for(int i=0;i<strlen(password);i++)
    {
        wifi_config.ap.password[i]=password[i];
        wifi_config.ap.password[i+1]=0;
    }
   for(int i=0;i<strlen(ssid);i++)
    {
        wifi_config.ap.ssid[i]=ssid[i];
        wifi_config.ap.ssid[i+1]=0;
    }
  wifi_config.ap.ssid_len=strlen(ssid);

    if(strlen(password)<=0)
    {
    wifi_config.ap.authmode=WIFI_AUTH_OPEN;
    ESP_LOGI(TAG,"Password empty open wifi..");
    }
    ESP_LOGI(TAG, "Setting WiFi configuration SSID %s and pass %s...", wifi_config.sta.ssid,wifi_config.sta.password);
    ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_AP) );
    ESP_ERROR_CHECK( esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_config) );
    ESP_ERROR_CHECK( esp_wifi_start() );
}