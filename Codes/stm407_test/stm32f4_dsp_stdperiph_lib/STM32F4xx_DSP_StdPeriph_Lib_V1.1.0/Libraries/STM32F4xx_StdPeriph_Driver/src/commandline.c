#include "commandline.h"
#include "usart.h"
#include "hal.h"
#include "epos.h"
#include "mcp2515.h"
#include "sensor.h"

#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>


//...............................................................................................................
char debug_index=0;
 char temp_commandline[120];
char usart2_buffer[USART2_BUFFER_SIZE],usart3_buffer[USART3_BUFFER_SIZE];
int buffer2_index=0,buffer3_index=0;
char ipm_start=0,start_init=0;
 unsigned long int start_address=0,data_size=0,nand_address_pointer=0;
 char driver_update_done=0,driver_stop=1,step=do_nothing_step;
 volatile int motor_packet_sent=0;
 char buffer_ovf=0;
 int pos=0;

//=====================================================================================================
//===================== START OF NAND FUNCTION ========================================================
//=====================================================================================================

// ===================================================
void NAND_read(char *par)
{
char arg1[20] ; 
unsigned int address	=0;
unsigned int data_cnt	=2112;
unsigned int read_flag	=0;
unsigned int i			=0;
//char temp [20];

for(i=0; i<2112; i++)
	{ NAND_data_output[i] = 0x11; } 
	
send_string3(par);
sscanf(par,"%d %d",&address, &data_cnt);

send_string3("\n Reading data from ");
sprintf(arg1,"%d to %d \n", address, (address+data_cnt-1)); 
send_string3(arg1);
read_flag= NAND_read_operation(address, data_cnt, NAND_data_output);

if (read_flag==1)
 {
	for(i=0; i<data_cnt; i++)
		{
		 sendchar3(NAND_data_output[i]); 
		 //sendchar('*'); 
		 //delay_us(1);
		}
	send_string3("\n Read Operation Complete. \n");
 }
}   

// ===================================================
void NAND_erasing_real_block(char *par)
{
//char com[20];
unsigned int address	=0;
unsigned int block_cnt	=0;
unsigned int erase_flag =0;
//unsigned int i			=0;
sscanf(par,"%d %d", &address, &block_cnt);

send_string3("\n Erasing the block ");
erase_flag = NAND_erase_operation(address);
if (erase_flag ==1)
	{send_string3("\nErasing successfully Completed.\n");}

}

// ===================================================
void NAND_erasing_N_block(char *par)
{
unsigned int address , block_cnt, erase_flag;
address =block_cnt= erase_flag = 0;

sscanf(par,"%d %d", &address, &block_cnt);

if (block_cnt == 0)
	{block_cnt = 1;
	}
erase_flag = NAND_erase_N_block(address,block_cnt);
if (erase_flag ==1)
	{send_string3("\nErasing successfully Completed.\n");}
}

// ===================================================

void NAND_erasing_all_memory(char *par)
{
unsigned int erase_flag;
send_string3(par);

erase_flag = NAND_erase_all_memory();
if (erase_flag ==1)
	{send_string3("\nErasing successfully Completed.\n");}

else 
	{send_string3("\nErasing failed. \n");}
}

// ===================================================
void NAND_start_write (char* par)
{
 
 sscanf(par,"%d %d",&NAND_start_address, &NAND_data_count);
 sprintf(temp_commandline,"%d %d \n",NAND_start_address, NAND_data_count);
 send_string3(temp_commandline);
 if ( (NAND_start_address % 2048) != 0)
 	{
	 send_string3("Error!!! invalid input address.\n");
	}
 
 else
 	{
	 NAND_current_data_count = 0;
	 NAND_write_mode_index = 0;
	 NAND_write_mode_flag = TRUE;
	}
}

// ===================================================
void NAND_start_read (char* par)
{
 sscanf(par,"%d %d",&NAND_start_address, &NAND_data_count);
 
 if ( (NAND_start_address % 2048) != 0)
 	{
	 send_string3("Error!!! invalid input address.\n");
	}
 	   
 else
 	{
	 NAND_current_data_count = 0;
	 NAND_read_mode_flag = TRUE;
	}
}


// ===================================================
void NAND_mapping_address(char* par)
{
 char temp [30]; 
 unsigned int d1, d2;
 sscanf(par,"%d",&d1);
 
 // d2 = NAND_update_mapping_address_table (d1);
 d2 = NAND_virtual2real_address(d1);
 sprintf(temp," adr:%d --> mp adr: %d\n",d1,d2);
 send_string3(temp);
}

// ===================================================
void NAND_mapping_block(char* par)
{
 char temp [30]; 
 unsigned int d1, d2;
 sscanf(par,"%d",&d1);
 
 d2 = NAND_update_mapping_address_table(d1, NAND_invalid_block_table);
 sprintf(temp," blk:%d --> mp blk: %d\n",d1,d2);
 send_string3(temp);
}

// ===================================================
void NAND_crc_check(char *par)
{
unsigned int address =0;
unsigned int data_cnt	=0;
unsigned int crc_flag =0;
//unsigned int i			=0;

sscanf(par,"%d %d",&address, &data_cnt);

send_string3("\n Checking crc of each page...");

crc_flag= NAND_checking_crc(address, data_cnt);

if (crc_flag==1)
	{send_string3("\nCRC checking Completed  Successfully.\n");}

else 
	{send_string3("\nWarning! There is Error in CRC.\n");}
}

// ===================================================
void NAND_reset(char* par)
{
unsigned int flag =0;
flag = NAND_reset_operation() ;
if (flag==0)
	send_string3("reset failed!!!\n");
else
	send_string3("Reset Complete.\n"); 
}

// ===================================================
void NAND_i_iib(char* par)
{
unsigned int mode, cnt_invalid_block =0;
char temp [50];

mode =1;
sscanf(par,"%d", &mode);
 
cnt_invalid_block = NAND_identify_initial_invalid_blocks(mode); // mode must be 1

if (cnt_invalid_block == 0)
	{ send_string3("Error! Identify initial invalid block failed!!!\n"); 
	  return;
	}
if (mode == 1)
	{
	sprintf(temp,"Identify iib Complete, there are %d", cnt_invalid_block );
	send_string3(temp);
	send_string3(" initial invalid block in flash memory.\n");
	}
return;
}

// ===================================================
void NAND_write_mode_function_with_crc(char k)
{
char first_page = 0 ;
unsigned int flag=0;
//unsigned int i=0;
//char temp_main [40];
char* crc_p1;
char* crc_p2;
//char crc_buffer1 [4];
//char crc_buffer2 [4];

unsigned int crc_result1=0;
unsigned int crc_result2=0;
unsigned int crc_input1=0;
unsigned int crc_input2=0;

//sprintf(temp_commandline,"%d %d %d \n",NAND_start_address, NAND_data_count, NAND_current_data_count);
//send_string3(temp_commandline);
 	
if(NAND_write_mode_index>2055)
	{
	   NAND_write_mode_index =0;
	   send_string3("e");
	}
if (NAND_current_data_count < 2056)	// ok
	{first_page= 1;}
else{first_page= 0;}

NAND_write_buffer[NAND_write_mode_index]=k;
NAND_write_mode_index++;
NAND_current_data_count++;

if(NAND_write_mode_index==2056)
	{

	 NAND_current_data_count = NAND_current_data_count -8;

	 crc_result1 = NAND_crc32_array(NAND_write_buffer,0,1023);
	 crc_result2 = NAND_crc32_array(NAND_write_buffer,1024,2047);

	 // calc input crc 
	 crc_input1 = NAND_write_buffer[2051] ;
	 crc_input1 = (crc_input1 << 8) + NAND_write_buffer[2050] ;
	 crc_input1 = (crc_input1 << 8) + NAND_write_buffer[2049] ;
	 crc_input1 = (crc_input1 << 8) + NAND_write_buffer[2048] ;

	 crc_input2 = NAND_write_buffer[2055] ;
	 crc_input2 = (crc_input2 << 8) + NAND_write_buffer[2054] ;
	 crc_input2 = (crc_input2 << 8) + NAND_write_buffer[2053] ;
	 crc_input2 = (crc_input2 << 8) + NAND_write_buffer[2052] ;
	

	//  check if error occure or not
	if (crc_result1 != crc_input1  || crc_result2 != crc_input2)
		{
		 send_string3("r");
		 NAND_write_mode_index =0;
		 NAND_current_data_count = NAND_current_data_count - 2048; 
		 //write_mode_flag = FALSE;
		 return;
		}
	
	// send_string3("data input 0-1023 transfer successfully.\n");
	
	// write to Nanad 	 write with parameter( start_address, current_data_count, data_count, write_buffer)
	flag = NAND_write_all( (NAND_start_address+ NAND_current_data_count - 2048) , 2048, NAND_write_buffer, first_page);
	
	if (flag != 1)
		{send_string3("e");} 


	crc_p1= &NAND_write_buffer[2048];
	crc_p2= &NAND_write_buffer[2052];
// ------------ use crc_buffer or use crc_p ------------


	flag = NAND_write_crc( (NAND_start_address+ NAND_current_data_count -2048 +8 ), 4, crc_p1);
	if (flag != 1)
		{send_string3("e1");} 
																
	flag = NAND_write_crc( (NAND_start_address+ NAND_current_data_count -2048 +24), 4, crc_p2);
	if (flag != 1)
		{send_string3("e2");} 

 	  //sprintf(temp_main ,"\n s(%d) + c(%d) - 2048 \n", start_address, current_data_count );
	  //send_string3(temp_main);
	
	NAND_write_mode_index =0;
	

	send_string3("o");

	if (NAND_data_count == NAND_current_data_count )
		{
		 NAND_write_mode_flag = FALSE; 
		 //write_mode_index = 0; 
		 send_string3("1");   //   END
		}
	
	}  // End if(write_mode_index==2056)

else if ((NAND_current_data_count == NAND_data_count ) && (NAND_write_mode_index < 2048) )
	{// write to nand 	 write with parameter(start_address, write_mode_index, data_count,write_buffer )
	  //sprintf(temp_main ,"\n s %d + c %d - i %d \n", start_address, current_data_count , write_mode_index);
	  //send_string3(temp_main);
	 flag = NAND_write_all( (NAND_start_address+ NAND_current_data_count - NAND_write_mode_index) , NAND_write_mode_index, NAND_write_buffer, first_page);
	 NAND_write_mode_flag = FALSE; 
	 NAND_write_mode_index = 0; 
	 send_string3("o");
	 send_string3("2");		// END
	}
}


// ==============================================================

void NAND_read_mode_function(void)
{
volatile unsigned int flag, page_offset;
 unsigned int page_counter; 
 unsigned int i=0;
 //char temp [50];
	
  //send_string3("\n read mode flag is true. \n");
  //sprintf(temp," s_adr= %d , d_cnt= %d ",start_address, data_count);
  //send_string3(temp);

 page_counter = NAND_data_count >> 11 ;
 page_offset = 	NAND_start_address & 0x7ff; 

 // ????????????????   must be work on page_offset 


 for(i=0; i < page_counter; i++)
	{
	flag = NAND_read_all( (NAND_start_address+ (i*2048) ) , 2048, NAND_write_buffer);
	//send_string3("OK");
	}
 
 if (  NAND_data_count != (i*2048) ) 	//(data_count & 0x07FF) != 0)
 	{ 
	 //send_string3("read the last page");   // only for test
	 flag = NAND_read_all( (NAND_start_address+ (i*2048) ) , NAND_data_count-(i*2048) , NAND_write_buffer);
	}
 	
send_string3("END\n");
NAND_read_mode_flag = FALSE;
}
//===============================================================================================================

void read_record (char* par)
{

unsigned int start_addr, record_number;
char data[96] ;
volatile unsigned int flag, i, start_address;
unsigned long int calc_address=0;

sscanf(par,"%d %d",&start_addr, &record_number);

calc_address=start_addr + ((record_number/21)*2048) + ((record_number%21)*96);				   //21 record can be added into each nand page
start_address = NAND_virtual2real_address(calc_address);
flag = NAND_read_operation (start_address, 96, data);
for(i=0;i<96;i++)sendchar3(data[i]);
		
}
//=====================================================================================================
//======================= END OF NAND FUNCTION ========================================================
//=====================================================================================================

//===============================================================================================================
void test(char*par)
{
send_string3(" ->");
send_string3(par);
send_string3("\n");
}
//===============================================================================================================
void on_set_debug(char*par)
{
int val;
sscanf(par,"%d",&val);
debug_index=val;
}
//===============================================================================================================
void tim_init(char*par)
{
int val;
sscanf(par,"%d",&val);
timer_init(val);
send_string3("timer init ok\n");
}
//===============================================================================================================
void list_command(char*par);
//===============================================================================================================
void start_ipm(char *par){
u8 i;
int CCR1_Val=60000;
unsigned long int capture=0;
sscanf(par,"%d %d",&start_address, &data_size);
sprintf(temp_commandline,"addr=%d cnt=%d\n",start_address,data_size) ;
send_string3(temp_commandline);


for(i=0;i<5;i++)add_to_fifo(0,0,10);
delay_ms(2000);
pos=0;
buffer_ovf=0;
nand_address_pointer=0;
motor_packet_sent=0;
step=start_trajectory_step;
//start_ipm_trajectory(); 
driver_stop=0;





}
//===============================================================================================================6000->2ms

void say (char *par){
send_string3(par);
send_string3("\n");
}
//===============================================================================================================6000->2ms

void echo (char *par){
send_string(par);

send_string("\n");

}
//===============================================================================================================6000->2ms

void on_ipm_init (char *par){
int val;
sscanf(par,"%d",&val);
if(val==255) active_ipm_all();
else active_ipm(val);
send_string3("ok\n");
}
//===============================================================================================================6000->2ms

void set_addr (char *par){
 int val;
sscanf(par,"%d",&val);
}
//===============================================================================================================6000->2ms

void on_set_data (char *par){
 int val;
sscanf(par,"%d",&val);
}
//===============================================================================================================6000->2ms
 
void on_reset_node (char *par){
 int val;
sscanf(par,"%d",&val);
reset_node(val);
}

//===============================================================================================================6000->2ms
 
void on_reset_nodes (char *par){
reset_all_nodes();
}

//===============================================================================================================6000->2ms

void on_read_sensor (char *par){
int val; 
sscanf(par,"%d",&val);
}
void on_nand_test (char *par){	//record 200 =line 202 notepad++
int val;
 char temp[96];
 int p,v;

sscanf(par,"%d",&val);
NAND_read_record (0, val,temp);
p=temp[3];
p<<=8;
p+=temp[2];
p<<=8;
p+=temp[1];
p<<=8;
p+=temp[0];

v=temp[7];
v<<=8;
v+=temp[6];
v<<=8;
v+=temp[5];
v<<=8;
v+=temp[4];
sprintf(temp_commandline, "p1=%d v1=%d \n",p,v);
send_string3(temp_commandline);
}
//===============================================================================================================6000->2ms

void on_can_board_test(char *par){
char status[14],i,send_status[14],board_error=0,can_error=0;
char buff[]={0,0,0,0,0,0,0,0};	  

 //can setup

 for(i=0;i<14;i++) MCP2515_CAN_Setup(baud_1M_bit,i);
 //check if can message send successful
for(i=0;i<14;i++){
status[i]=MCP2515_CANSendMsg_check_data(0x501, buff,8,i);
if(status[i]==0)can_error=1;

				 }
  //check if spi read write successful
for(i=0;i<14;i++){
 MCP2515_reg_write(CANCTRL,i+1,i);
 delay_ms(2);
}  
  
for(i=0;i<14;i++){
send_status[i]=MCP2515_reg_read(CANCTRL,i);	
 delay_ms(2);	
 if(send_status[i]==0)board_error=1;	   
				  } 
				  
////display errors
if(can_error==0 && board_error==0 ){send_string3("successfull\n");return;}

if(can_error)send_string3("can error=");
for(i=0;i<14;i++){
if(status[i]==0){sprintf(temp_commandline,"%d,",i);
					  send_string3(temp_commandline);
						}

}
 if(board_error)send_string3("\nboard error=");
 for(i=0;i<14;i++){
if(send_status[i]==0){sprintf(temp_commandline,"%d,",i);
				 send_string3(temp_commandline);}
				   }
				
 for(i=0;i<14;i++) MCP2515_CAN_Setup(baud_1M_bit,i);
				  
  delay_ms(1);
}
//===============================================================================================================6000->2ms
void on_getpos(char *par){ 
 int val,pos[14];
 u8 result=0,i=0;
sscanf(par,"%d",&val);
//...........................broadcast id 
if(val==255){
if(epos_get_pos_all(1,pos)==0){send_string3("r/w error\n");return;}
//for(i=0;i<14;i++)
//{
//result=epos_get_pos(1,&pos[i],i);
//if(result==0){send_string3("read all error\n"); send_string2("read all error\n");return;}
//}

for(i=0;i<14;i++){sprintf(temp_commandline,"%d ",pos[i]);send_string3(temp_commandline);send_string2(temp_commandline);}
send_string3("ok\n");
send_string2("ok\n");
return;

}
 //.......................... single selection
result=epos_get_pos(1,&pos[0],val);
if(result){
sprintf(temp_commandline,"pos=%d\n",pos[0]);
send_string3(temp_commandline);send_string2(temp_commandline);	  }
else{
send_string3("read node error\n");send_string2("read node error\n");
}

}

//===============================================================================================================6000->2ms
void on_sensor_board_test(char *par){ 
  char result=0;
  float rx,ry,rz;	
  send_string3("sen");
   result+=read_sensors(&rx,&ry,&rz);
 delay_ms(80);
  result+=read_sensors(&rx,&ry,&rz);
  if(result==1)send_string3("sensor ok\n");
    else send_string3("sensor error\n");
}
//===============================================================================================================6000->2ms
void on_set_pos(char *par){ 
unsigned long int id ,p,v;
sscanf(par,"%d %d %d",&id,&p,&v);
epos_goto_pos(1,p,v,id);
//sprintf(temp_commandline,"velocity=%v ok\n",v);
//send_string3(temp_commandline);
send_string3("ok\n");
}
//===============================================================================================================6000->2ms
void on_get_abs(char *par){ 
int id,i;
long int position;
char err=0;
long int positions[12];
sscanf(par,"%d",&id);
if(id==255){
for(i=0;i<12;i++){
if(epos_get_abs_pos(1,i,&position)==1)
positions[i]=position;			  
else {err=1;}
}


if(err==1){send_string3("error\n");}
else{
for(i=0;i<12;i++) {sprintf(temp_commandline,"%d ",positions[i]);send_string3(temp_commandline);}
send_string3("ok\n");
}
return;
}//end print all

if(epos_get_abs_pos(1,id,&position)==1)
{
sprintf(temp_commandline,"pos=%d ok\n",position);
send_string3(temp_commandline);}
else
send_string3("error\n");
}
//===============================================================================================================6000->2ms

void on_ppminit(char *par){ 
int id ;
int i;
sscanf(par,"%d",&id);
if(id==255) {

if(active_ppm_all()==1){
send_string3("all ok\n");
reset_node(12);delay_ms(10);reset_node(13);delay_ms(10);
reset_node(12);delay_ms(10);reset_node(13);delay_ms(10);}
else{send_string3("erreo r/w\n");}
}
else{
   epos_set_profilepos( 1,id);
						 }
send_string3("ok\n");
}
//===============================================================================================================6000->2ms

void on_get_encoder(char *par){ 
int i=0;
u16 encoders[12];
int count=0;
int abs=0,inc=0;

for(i=0;i<12;i++){
 if(epos_read_register(1,0x2210,2,&encoders[i],i)==1)
 {count++;}
  
}

 
for(i=0;i<12;i++){
   if(encoders[i]==516 )abs++;
   if(encoders[i]==2)inc++;

}

if(count==12 && abs==10){send_string3("abs\n");return;}
if(count==12 && inc==12){send_string3("inc\n");return;}
if(count!=12){send_string3("error r/w\n"); return;}
send_string3("error d-types\n");


}
//===============================================================================================================6000->2ms

void on_encoder(char *par){ 
char sensor=0;
int i, count=0;
sscanf(par,"%c",&sensor);
if(sensor=='a') //ssi abs encoder
{
for(i=0;i<12;i++){
if(i!=2 && i!=8){
if(setup_abs_encoders(1,i)==1){
 count++;
}		       
               }//if		
			     }//for
if(count==10){send_string3("set abs ok\n");}
else send_string3("set abs error\n");


}
else if(sensor=='i'){

for(i=0;i<12;i++)
if(setup_inc_encoders(1,i)==1){
 count++;
}

if(count==12){send_string3("set inc ok\n");}
else send_string3("set inc error\n");

}
sprintf(temp_commandline,"cnt=%d \n",count);
send_string3(temp_commandline);

}
//
void on_quick_stop(char *par){ 
if(epos_quick_stop_all()==1){
send_string3("ok\n");
return;
		 }	
send_string3("error\n");
 


}
//===============================================================================================================6000->2ms

void on_get_mode(char *par){ 
int id,result ;
sscanf(par,"%d",&id);

if(epos_get_mode( 1,&result,id)==1){
 switch (result)
{
case 7:
send_string3("IPM\n");
break;
case 6:
send_string3("HOMING\n");
break;
case 3:
send_string3("PVM");
break;
case 1:
send_string3("PPM\n");
break;
case 0xFF:
send_string3("POSITIN\n");
break;

case 0XFE:
send_string3("VELOCITY\n");
break;

case 0xFD:
send_string3("CURRENT\n");
break;

case 0xFC:
send_string3("DIAGNOSTIC\n");
break;
case 0xFB:
send_string3("MASTER/ENCODER\n");
break;
case 0xFA:
send_string3("STEP/DIRECTION\n");
default:
sprintf(temp_commandline,"something else=%d\n",result);
send_string3(temp_commandline);
}
}
else {
sprintf(temp_commandline,"error data=%d\n",result);
send_string3(temp_commandline);

}

}

//===============================================================================================================6000->2ms

static const SCMD tskcmd[] = {
"test",test,
"timerinit",tim_init,
"startipm",start_ipm,
"echo",echo,
"say",say,
"ls",list_command,
"read",NAND_read,
"ipminit",on_ipm_init,
"s_read", NAND_start_read,
"s_write", NAND_start_write,
"erase_blk",NAND_erasing_N_block,
"erase_mem",NAND_erasing_all_memory,
"erase_rb", NAND_erasing_real_block,
"crc_c", NAND_crc_check,
"i_iib",NAND_i_iib,
"reset", NAND_reset,
"map_adr", NAND_mapping_address,
"map_blk", NAND_mapping_block,
"record",read_record,
"addr",set_addr,
"data",on_set_data,
"debug",on_set_debug,
"sen",on_read_sensor,
"rst",on_reset_nodes,
"nrst",on_reset_node,
"ntest",on_nand_test,
"canboardtest",on_can_board_test,
"stest",on_sensor_board_test,
"getpos",on_getpos,
"getmode",on_get_mode,
"setpos",on_set_pos,
"ppminit",on_ppminit,
"getabs",on_get_abs,
"encoder",on_encoder,
"getencoder",on_get_encoder,
"qstop",on_quick_stop
 };
 //===============================================================================================================6000->2ms
 void list_command(char*par){
int i,command_array_size;
command_array_size=(sizeof(tskcmd)/sizeof(SCMD));
send_string3("-------------\n");
for(i=0;i<command_array_size;i++){send_string3((char*)(tskcmd[i].val));send_string3("\n");}
send_string3("-------------\n");
}
 //===============================================================================================================3000->1ms
int  start_with(char*str,const char*main_str)
{
int len=0,i,n=0;
len=strlen(main_str);
for(i=0;i<len;i++)
{
if(str[i]==main_str[i]){n++;}
}
if(n==len)return 0;
return -1;
}
//===============================================================================================================
void task_handle( char* buffer){
int i,command_array_size;

char com[20];
char arg[50];
char*p=0;
//
command_array_size=(sizeof(tskcmd)/sizeof(SCMD));
sscanf(buffer,"%s %s",com,arg);
p=strstr(buffer,arg);
strcpy(arg,p);
////........................................
for(i=0;i<command_array_size;i++){
if(start_with(com,tskcmd[i].val)==0){tskcmd[i].func(arg);break;}
}
strcpy(arg,"");
strcpy(com,"");
buffer[0]=0;
}
//===============================================================================================================
