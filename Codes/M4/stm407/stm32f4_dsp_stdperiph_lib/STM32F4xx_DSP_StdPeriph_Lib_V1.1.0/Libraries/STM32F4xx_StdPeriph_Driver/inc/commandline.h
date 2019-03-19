#define USART2_BUFFER_SIZE (40)
#define USART3_BUFFER_SIZE (40)
#include "K9F2G08.h"
#define do_nothing_step 0
#define start_trajectory_step 1
#define addfifo_step 2
#define send_stop_step 3

extern char debug_index;
extern char usart2_buffer[USART2_BUFFER_SIZE],usart3_buffer[USART3_BUFFER_SIZE];
extern int buffer3_index,buffer2_index,pos;
extern char ipm_start,start_init;
extern unsigned long int start_address,data_size,nand_address_pointer;;
extern char driver_update_done,driver_stop,step;
extern volatile int motor_packet_sent;
extern char buffer_ovf;
typedef struct scmd {
 char val[30];
 void (*func)(char *par);
} SCMD;
void task_handle( char*buffer);
void NAND_write_mode_function_with_crc(char k);
void NAND_read_mode_function(void);


