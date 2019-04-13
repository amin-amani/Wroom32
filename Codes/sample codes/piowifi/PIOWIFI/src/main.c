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
#include "TCPServer.h"



static void TCPServerTask(void *pvParameters)
{
    ESP_LOGE("example:", "Create task"); 
    TCPServerStartListen();
    ESP_LOGE("example:", "Delete task");  
    vTaskDelete(NULL);
}
void TCPServerNewClientConnected()
{
    xTaskCreate(TCPServerTask, "tcp_server", 4096, NULL, 5, NULL);  

}
void TCPServerDataReceived(char *data,int len)
{
        ESP_LOGI("newData", "datacallback=%s", data);
        TCPServerSendData("salam",5);


}

void app_main()
{
   // esp_log_level_set("tag",ESP_LOG_ERROR);
    char data[6]={'s','a','l','a','m','\n'};
    ESP_ERROR_CHECK( nvs_flash_init() );
    TCPNewClientConnectedCallback=TCPServerNewClientConnected;
    TCPServerNewDataReceivedCallback=TCPServerDataReceived;
    TCPServerInit();

    while (true)
    {
        // if(ConnectedSocket>0)
        // send(ConnectedSocket,data, 6, 0);

         sys_delay_ms(500);

    }
    

}
