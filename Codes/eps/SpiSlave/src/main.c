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


//========================================================================================
static void TCPServerTask(void *pvParameters)
{
    ESP_LOGE("example:", "Create task"); 
    TCPServerStartListen();
    ESP_LOGE("example:", "Delete task");  
    vTaskDelete(NULL);
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
        TCPServerSendData("salam",5);


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

//Called after a transaction is queued and ready for pickup by master. We use this to set the handshake line high.
void my_post_setup_cb(spi_slave_transaction_t *trans) {
    WRITE_PERI_REG(GPIO_OUT_W1TS_REG, (1<<GPIO_HANDSHAKE));
}
//========================================================================================
//Called after transaction is sent/received. We use this to set the handshake line low.
void my_post_trans_cb(spi_slave_transaction_t *trans) {
    WRITE_PERI_REG(GPIO_OUT_W1TC_REG, (1<<GPIO_HANDSHAKE));
}

void WIFIDataReady(char*data,int len) {
TCPServerSendData(data,len);
}
//========================================================================================
void app_main()
{
    
    ESP_ERROR_CHECK( nvs_flash_init() );
    Spiparser=CreateSpiHandler();

    Spiparser->WIFIDataReadyCallback=WIFIDataReady;
    SpiHandlerInit(Spiparser);

    int n=0;
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
;

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


    char sendbuf[129]="";
    char recvbuf[129]="";
    memset(recvbuf, 0, 33);
    spi_slave_transaction_t t;
    memset(&t, 0, sizeof(t));
    printf("Start...\n");
    vTaskDelay(4000 / portTICK_PERIOD_MS);
    TCPNewClientConnectedCallback=TCPServerNewClientConnected;
    TCPServerNewDataReceivedCallback=TCPServerDataReceived;
    TCPServerInit("mainwifi","");

    // xTaskCreate( prvTaskA, "TaskA", configMINIMAL_STACK_SIZE, NULL,
    //                         tskIDLE_PRIORITY, ( xTaskHandle * ) NULL );
     while(1){
          //Clear receive buffer, set send buffer to something sane
        memset(recvbuf, 0xA5, 129);
        sprintf(sendbuf, "This is the receiver, sending data for transmission number %04d.", n);

        //Set up a transaction of 128 bytes to send/receive
        t.length=20*8;
        t.tx_buffer=sendbuf;
        t.rx_buffer=recvbuf;
        /* This call enables the SPI slave interface to send/receive to the sendbuf and recvbuf. The transaction is
        initialized by the SPI master, however, so it will not actually happen until the master starts a hardware transaction
        by pulling CS low and pulsing the clock etc. In this specific example, we use the handshake line, pulled up by the
        .post_setup_cb callback that is called as soon as a transaction is ready, to let the master know it is free to transfer
        data.
        */
        ret=spi_slave_transmit(HSPI_HOST, &t, portMAX_DELAY);
        recvbuf[1]=2;
        Spiparser->ProcessData(Spiparser,recvbuf,20);
        //spi_slave_transmit does not return until the master has done a transmission, so by here we have sent our data and
        //received data from the master. Print it.
        //TCPServerSendData(recvbuf,strlen(recvbuf));

        printf("Received: %s\n", recvbuf);
        n++;
    }
    
}
//========================================================================================
