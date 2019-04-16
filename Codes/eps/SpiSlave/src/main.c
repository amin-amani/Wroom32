#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "driver/spi_slave.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "SpiHandler.h"
#include "TCPServer.h"
#include "esp_log.h"
#include "nvs_flash.h"


#define GPIO_HANDSHAKE 2
#define GPIO_MOSI 23
#define GPIO_MISO 19
#define GPIO_SCLK 18
#define GPIO_CS 5
//------------------------------------------------------------------------------------
SpiHandler *Spiparser;
char SPITxBuffer[20]="";
char SPIRxBuffer[20]="";
char WIFIRxBuffer[20]="";



//========================================================================================
static void TCPServerTask(void *pvParameters)
{
    ESP_LOGE("example:", "Create task"); 
    TCPServerStartListen();
    ESP_LOGE("example:", "Delete task");  
    vTaskDelete(NULL);
}
//========================================================================================
void TCPServerStartAP(char *data,int len)
{
ESP_LOGI("start ap","%d",0);
}
//========================================================================================
void TCPServerSetPassword(char *data,int len)
{
ESP_LOGI("set pass:","%s",data);
}
//========================================================================================
void TCPServerSetSSID(char *data,int len)
{
    ESP_LOGI("set ssid:","%s",data);
}
//========================================================================================
void TCPServerNewClientConnected()
{
    xTaskCreate(TCPServerTask, "tcp_server", 4096, NULL, 5, NULL);  
}
//========================================================================================
void TCPServerDataReceived(char *data,int len)
{
    ESP_LOGI("newData", "datacallback=%s", data);
    memset(WIFIRxBuffer, 0, sizeof(WIFIRxBuffer));
    memcpy(WIFIRxBuffer,data,len);
}
//========================================================================================
void TCPSend(char*data,int len) 
{
ESP_LOGI("tcp send:","%s -> %d",data,len);
TCPServerSendData(data,len);
}
//========================================================================================
void TCPServerReadData(char*data,int len)
{
            ESP_LOGI("read sata:","%s -> %d",data,len);
    memset(SPITxBuffer, 0, 20);
    memcpy(SPITxBuffer,WIFIRxBuffer,sizeof(WIFIRxBuffer));

}
//========================================================================================
void SPISendData(char*data,int len)
{
   ESP_LOGI("spi end:","%s -> %d",data,len);
    memset(SPITxBuffer, 0, 20);
    memcpy(SPITxBuffer,data,len);
}

//========================================================================================
void prvTaskA (void* pvParameters)
{		
    (void) pvParameters;                    // Just to stop compiler warnings.
	
    for (;;) {
       //vTaskDelay(500);
        printf("Task A\n");
        vTaskDelay(100);
    }
}
//========================================================================================
void my_post_setup_cb(spi_slave_transaction_t *trans) {
    WRITE_PERI_REG(GPIO_OUT_W1TS_REG, (1<<GPIO_HANDSHAKE));
}
//========================================================================================
void my_post_trans_cb(spi_slave_transaction_t *trans) {
    WRITE_PERI_REG(GPIO_OUT_W1TC_REG, (1<<GPIO_HANDSHAKE));
}

//========================================================================================
void SPIInit()
{

 esp_err_t ret;
    //Configuration for the SPI bus
    spi_bus_config_t buscfg={
        .mosi_io_num=GPIO_MOSI,
        .miso_io_num=GPIO_MISO,
        .sclk_io_num=GPIO_SCLK
    };

    //Configuration for the SPI slave interface
    spi_slave_interface_config_t slvcfg={
        .mode=0,
        .spics_io_num=GPIO_CS,
        .queue_size=3,
        .flags=0,
        .post_setup_cb=my_post_setup_cb,
        .post_trans_cb=my_post_trans_cb
    };


    //Configuration for the handshake line
    gpio_config_t io_conf={
        .intr_type=GPIO_INTR_DISABLE,
        .mode=GPIO_MODE_OUTPUT,
        .pin_bit_mask=(1<<GPIO_HANDSHAKE)
    };

    //Configure handshake line as output
    gpio_config(&io_conf);
    //Enable pull-ups on SPI lines so we don't detect rogue pulses when no master is connected.
    gpio_set_pull_mode(GPIO_MOSI, GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(GPIO_SCLK, GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(GPIO_CS, GPIO_PULLUP_ONLY);

    //Initialize SPI slave interface
    ret=spi_slave_initialize(HSPI_HOST, &buscfg, &slvcfg, 1);
    assert(ret==ESP_OK);

}
//========================================================================================
void app_main()
{
    esp_err_t ret; 

    spi_slave_transaction_t t;
     memset(SPIRxBuffer, 0, 20);
    memset(&t, 0, sizeof(t));

    ESP_ERROR_CHECK( nvs_flash_init() );
    Spiparser=CreateSpiHandler();

    Spiparser->WIFISendDataCallback=TCPSend;
    Spiparser->SPISendDataCallback=SPISendData;
    Spiparser->WIFIReadDataCallback=TCPServerReadData;
    Spiparser->WIFISetPasswordCallback=TCPServerSetPassword;
    Spiparser->WIFISetSSIDCallback=TCPServerSetSSID;
    Spiparser->WIFIStartApCallback=TCPServerStartAP;
    
    
    SpiHandlerInit(Spiparser);

    SPIInit();
    TCPNewClientConnectedCallback=TCPServerNewClientConnected;
    TCPServerNewDataReceivedCallback=TCPServerDataReceived;
    TCPServerInit("mainwifi","");
    memset(SPIRxBuffer, 0, 20);
    sprintf(SPITxBuffer,"%s", "init response"); 
    t.length=20*8;
    t.tx_buffer=SPITxBuffer;
    t.rx_buffer=SPIRxBuffer;
    // xTaskCreate( prvTaskA, "TaskA", configMINIMAL_STACK_SIZE, NULL,
    //                         tskIDLE_PRIORITY, ( xTaskHandle * ) NULL );
     while(1){
        ret=spi_slave_transmit(HSPI_HOST, &t, portMAX_DELAY);
        Spiparser->ProcessSPIData(Spiparser,SPIRxBuffer,20);
        printf("Received: %s\n", SPIRxBuffer);
    }
    
}
//========================================================================================
