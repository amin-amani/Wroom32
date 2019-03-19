#include "stm32f4xx.h"
#include "mcp2515.h"
#include "HAL.h"
#include "epos.h"

int gear_valu[12]={460,320,160,320,460,320,460,320,160,320,460,320};

void reset_node(int node){

u8 buff[8];
buff[0]=0x81;
buff[1]=0;
buff[2]=0;
buff[3]=0;
buff[4]=0;
buff[5]=0;
buff[6]=0;
buff[7]=0;
MCP2515_CANSendMsg_check_data(0, buff,8,node);
MCP2515_start_transmittion();
   
}
//==================================================================================

void reset_all_nodes(void){
u8 i;
u8 buff[8];
buff[0]=0x81;
buff[1]=0;
buff[2]=0;
buff[3]=0;
buff[4]=0;
buff[5]=0;
buff[6]=0;
buff[7]=0;
for(i=0;i<14;i++){MCP2515_CANSendMsg_check_data(0, buff,8,i);	}
MCP2515_start_transmittion();
   
}
//==================================================================================
void reset_comunications_all(void){
u8 i;
u8 buff[8];
buff[0]=0x82;
buff[1]=0;
buff[2]=0;
buff[3]=0;
buff[4]=0;
buff[5]=0;
buff[6]=0;
buff[7]=0;
for(i=0;i<14;i++){MCP2515_CANSendMsg_check_data(0, buff,8,i);	}
MCP2515_start_transmittion();
   
}
//==================================================================================
void reset_comunications(int node){

u8 buff[8];
buff[0]=0x82;
buff[1]=0;
buff[2]=0;
buff[3]=0;
buff[4]=0;
buff[5]=0;
buff[6]=0;
buff[7]=0;
MCP2515_CANSendMsg_check_data(0, buff,8,node);
MCP2515_start_transmittion();
   
}
//==================================================================================
void start_node_all(void){
u8 i;
u8 buff[8];
buff[0]=0x1;
buff[1]=0;
buff[2]=0;
buff[3]=0;
buff[4]=0;
buff[5]=0;
buff[6]=0;
buff[7]=0;
for(i=0;i<14;i++){MCP2515_CANSendMsg_check_data(0, buff,8,i);	}
MCP2515_start_transmittion();
   
}
//==================================================================================
void start_node(int node){

u8 buff[8];
buff[0]=0x1;
buff[1]=0;
buff[2]=0;
buff[3]=0;
buff[4]=0;
buff[5]=0;
buff[6]=0;
buff[7]=0;
MCP2515_CANSendMsg_check_data(0, buff,8,node);	
MCP2515_start_transmittion();
   
}
//==================================================================================
void stop_node_all(void){
u8 i;
u8 buff[8];
buff[0]=0x2;
buff[1]=0;
buff[2]=0;
buff[3]=0;
buff[4]=0;
buff[5]=0;
buff[6]=0;
buff[7]=0;
for(i=0;i<14;i++){MCP2515_CANSendMsg_check_data(0, buff,8,i);	}
MCP2515_start_transmittion();
   
}
//==================================================================================
void stop_node(int node){

u8 buff[8];
buff[0]=0x2;
buff[1]=0;
buff[2]=0;
buff[3]=0;
buff[4]=0;
buff[5]=0;
buff[6]=0;
buff[7]=0;
MCP2515_CANSendMsg_check_data(0, buff,8,node);
MCP2515_start_transmittion();
   
}
//==================================================================================
void set_preoperational_mode_all(void){
u8 i;
u8 buff[8];
buff[0]=0x80;
buff[1]=0;
buff[2]=0;
buff[3]=0;
buff[4]=0;
buff[5]=0;
buff[6]=0;
buff[7]=0;
for(i=0;i<14;i++){MCP2515_CANSendMsg_check_data(0, buff,8,i);	}
MCP2515_start_transmittion();
   
}
//==================================================================================
void set_preoperational_mode(int node){

u8 buff[8];
buff[0]=0x80;
buff[1]=0;
buff[2]=0;
buff[3]=0;
buff[4]=0;
buff[5]=0;
buff[6]=0;
buff[7]=0;
MCP2515_CANSendMsg_check_data(0, buff,8,node);
MCP2515_start_transmittion();
   
}
//==================================================================================
void epos_set_mode(int mode){ //interpolation 7
u8 i;
u8 buff[8];
buff[0]=0x7;
buff[1]=0;
buff[2]=0;
buff[3]=0;
buff[4]=0;
buff[5]=0;
buff[6]=0;
buff[7]=0;
for(i=0;i<14;i++){MCP2515_CANSendMsg_check_data(0x301, buff,8,i);	}
MCP2515_start_transmittion();
   
}
//==================================================================================
void epos_switch_on_all(void){
u8 i;
u8 buff[8];
buff[0]=0x0f;
buff[1]=0;
buff[2]=0;
buff[3]=0;
buff[4]=0;
buff[5]=0;
buff[6]=0;
buff[7]=0;
for(i=0;i<14;i++){MCP2515_CANSendMsg_check_data(0x201, buff,8,i);	}
MCP2515_start_transmittion();
   
}
//==================================================================================
void epos_switch_on(int node){

u8 buff[8];
buff[0]=0x0f;
buff[1]=0;
buff[2]=0;
buff[3]=0;
buff[4]=0;
buff[5]=0;
buff[6]=0;
buff[7]=0;
MCP2515_CANSendMsg_check_data(0x201, buff,8,node);
MCP2515_start_transmittion();
   
} 
//==================================================================================
void epos_switch_off_all(void){
u8 i;
u8 buff[8];
buff[0]=0x06;
buff[1]=0;
buff[2]=0;
buff[3]=0;
buff[4]=0;
buff[5]=0;
buff[6]=0;
buff[7]=0;
for(i=0;i<14;i++){MCP2515_CANSendMsg_check_data(0x201, buff,8,i);	}
MCP2515_start_transmittion();
   
}
//==================================================================================
void epos_switch_off(int node){

u8 buff[8];
buff[0]=0x06;
buff[1]=0;
buff[2]=0;
buff[3]=0;
buff[4]=0;
buff[5]=0;
buff[6]=0;
buff[7]=0;
MCP2515_CANSendMsg_check_data(0x201, buff,8,node);
MCP2515_start_transmittion();
   
}
//==================================================================================
void clear_fifo_all(void){
u8 i;
u8 buff[8];
buff[0]=0x01;
buff[1]=0;
buff[2]=0;
buff[3]=0;
buff[4]=0;
buff[5]=0;
buff[6]=0;
buff[7]=0;
for(i=0;i<14;i++){MCP2515_CANSendMsg_check_data(0x401, buff,8,i);	}
MCP2515_start_transmittion();
   
}
//==================================================================================
void clear_fifo(int node){

u8 buff[8];
buff[0]=0x01;
buff[1]=0;
buff[2]=0;
buff[3]=0;
buff[4]=0;
buff[5]=0;
buff[6]=0;
buff[7]=0;
MCP2515_CANSendMsg_check_data(0x401, buff,8,node);
MCP2515_start_transmittion();
   
}
//==================================================================================
void start_ipm_trajectory(void){
u8 i;
u8 buff[8];
buff[0]=0x1f;
buff[1]=0;
buff[2]=0;
buff[3]=0;
buff[4]=0;
buff[5]=0;
buff[6]=0;
buff[7]=0;
for(i=0;i<14;i++){MCP2515_CANSendMsg_check_data(0x201, buff,8,i);	}
MCP2515_start_transmittion();
   
}
//==================================================================================
void active_ipm(int node){
 reset_node(node);
  delay_ms(300);
 set_preoperational_mode(node);
 delay_ms(300);
 start_node(node);
  delay_ms(300);
 epos_set_mode(7);
  delay_ms(300);
epos_switch_off(node);
 delay_ms(300);
 epos_switch_on(node);
  delay_ms(300);
clear_fifo(node);
}
//==================================================================================
void active_ipm_all(void){
 reset_all_nodes();
  delay_ms(300);
 set_preoperational_mode_all();
 delay_ms(300);
 start_node_all();
  delay_ms(300);
 epos_set_mode(7);
  delay_ms(300);
epos_switch_off_all();
 delay_ms(300);
 epos_switch_on_all();
  delay_ms(300);
clear_fifo_all();
}
//==================================================================================
char active_ppm_all(void){
char success;
char retry=0;


try:
success=0;
if(epos_sdo_write_command_all(1+0x600,50000,0x6065,0,4)==1)success++;//mode set profile
delay_ms(300);
if(epos_sdo_write_command_all(1+0x600,500,0x607f,0,4)==1)success++;//mode set profile
delay_ms(300);
if(epos_sdo_write_command_all(1+0x600,1,0x6060,0,1)==1)success++;//mode set profile
delay_ms(300);
if(epos_sdo_write_command_all(1+0x600,0x06,0x6040,0, 2)==1)success++;
delay_ms(300);
if(epos_sdo_write_command_all(1+0x600,0x0f,0x6040,0, 2)==1)success++;
delay_ms(300);
if(epos_sdo_write_command_all(1+0x600,0x010f,0x6040,0, 2)==1)success++;
delay_ms(300);
 //epos_sdo_write_command_all( int id, int input,u16 index,unsigned char sub_index,unsigned char len)


if(success==6)return 1;
if(success!=6 && retry==0){retry=1;goto try;}
return 0;
}
//==================================================================================
void add_to_fifo( long int position, long int velocity, int time){
 			  u8 buff[10],i;
           
			buff[0] = (position & 0xff);
            buff[1] = ((position >> 8) & 0xff);
            buff[2] = ((position >> 16) & 0xff);
            buff[3] = ((position >> 24) & 0xff);
            buff[4] = (velocity & 0xff);
            buff[5] = ((velocity >> 8) & 0xff);
            buff[6] = ((velocity >> 24) & 0xff); 
            buff[7] = (time & 0xff);
	for(i=0;i<14;i++){MCP2515_CANSendMsg_check_data(0x501, buff,8,i);	}
MCP2515_start_transmittion();

}
void add_to_fifo4( long int position, long int velocity, int time){
 			  u8 buff[10],i;
           
			buff[0] = (position & 0xff);
            buff[1] = ((position >> 8) & 0xff);
            buff[2] = ((position >> 16) & 0xff);
            buff[3] = ((position >> 24) & 0xff);
            buff[4] = (velocity & 0xff);
            buff[5] = ((velocity >> 8) & 0xff);
            buff[6] = ((velocity >> 16) & 0xff); 
            buff[7] = (time & 0xff);
	for(i=0;i<14;i++){MCP2515_CANSendMsg_check_data(0x501, buff,8,i);	}


}
//==================================================================================
void add_to_fifo2(char record[], int time){
 			  u8 buff[10],i;  
			  	int offset=0; 
			    buff[7] = (time & 0xff);
			
          for(i=0;i<12;i++){ 
			buff[0] = record[offset+0];
            buff[1] = record[offset+1];
            buff[2] = record[offset+2];
            buff[3] = record[offset+3];
            buff[4] = record[offset+4];
            buff[5] = record[offset+5];
            buff[6] = record[offset+6];
			offset+=8;
        	MCP2515_CANSendMsg_check_data(0x501, buff,8,i);	
			if(i==10 || i==11)MCP2515_CANSendMsg_check_data(0x501, buff,8,i+2);	
						}
MCP2515_start_transmittion();

}
 //==================================================================================
void int_to_arr(unsigned char*arr,long int val,int index){

arr[index+0]=val&0xff;
arr[index+1]=(val>>8)&0xff;
arr[index+2]=(val>>16)&0xff;
arr[index+3]=(val>>24)&0xff;
}
//========================================================================
void arr_to_int_p(unsigned char*arr,long int *val,int index,float coef){
long int result=0;
float fresult=0;
result<<=8;
result+=arr[index+3];
result<<=8;
result+=arr[index+2];
result<<=8;
result+=arr[index+1];
result<<=8;
result+=arr[index+0];
fresult=result*coef;
result=fresult;
 *val=result;
}

  int mline=0;
    char str[40];
	u8  buff2[10];
//==================================================================================
void add_to_fifo3(char record[], int time){
 			  u8 buff[10],i;
			  
			  	int offset=0; 
				long int pos,velocity;
			    buff[7] = (time & 0xff);
				 
			
          for(i=0;i<12;i++){ 
			buff[0] = record[offset+0];
            buff[1] = record[offset+1];
            buff[2] = record[offset+2];
            buff[3] = record[offset+3];

            buff[4] = record[offset+4];
            buff[5] = record[offset+5];
            buff[6] = record[offset+6];
		
        	MCP2515_CANSendMsg_check_data(0x501, buff,8,i);	
			//.........................................................
			 

		
			//if(i==3){
			   //	sprintf(str,"p=%d v=%d :n=%d\n",pos,velocity,mline);
			//send_string3(str);
			//sprintf(str,"0-6:%x %x %x %x %x %x\n",buff[0],buff[1],buff[2],buff[3],buff[4],buff[5],buff[6]);
			//send_string3(str);
			//arr_to_int_p(record,&pos,offset,0.4883);
			//arr_to_int_p(record,&velocity,offset+4,0.32);

			//sprintf(str,"%d	%d\n",pos,velocity);
		//	send_string3(str);
			//int_to_arr(buff2, pos,0);
			//int_to_arr(buff2, velocity,4);
			 // buff2[7] = (time & 0xff);
			//MCP2515_CANSendMsg_check_data(0x501, buff2,8,12);

			//sprintf(str,"0-6_r:%x %x %x %x %x %x\n",buff2[0],buff2[1],buff2[2],buff2[3],buff2[4],buff2[5],buff2[6]);
			//send_string3(str);
		//	}
			//if(i==9){
			//arr_to_int_p(record,&pos,offset,0.4883);
			//arr_to_int_p(record,&velocity,offset+4,0.32);

			//sprintf(str,"p=%d v=%d\n",pos,velocity);
		//send_string3(str);
			//int_to_arr(buff2, pos,0);
			//int_to_arr(buff2, velocity,4);
			 //buff2[7] = (time & 0xff);
			//MCP2515_CANSendMsg_check_data(0x501, buff2,8,13);
			//} 	
			offset+=8;
				//.........................................................
			if(i==3)MCP2515_CANSendMsg_check_data(0x501, buff,8,12);
			if(i==9)MCP2515_CANSendMsg_check_data(0x501, buff,8,13);	
						}


}
//====================================================================================
unsigned char get_code(int len)
{
if (len == 1) { return 0x2f; }
if (len == 2) { return 0x2b; }
if (len == 4) { return 0x23; }
if (len == -1) { return 0x22; }
if (len == 0) { return 0x40; }
return 0;
}
//=================================================================================
char epos_sdo_write_command( int id,unsigned long int input,u16 index,unsigned char sub_index,unsigned char len,char cs_address)
        {
unsigned char buff[10];
unsigned char msg[8];
u16 inpid,inp_index;
u8 inp_len;

buff[0] = get_code(len);
buff[1] = (unsigned char)(index & 0xff);
buff[2] = (unsigned char)(index >> 8);
buff[3] = (unsigned char)(sub_index);
buff[4] = (unsigned char)(input & 0xff);
buff[5] = (unsigned char)((input >> 8) & 0xff);
buff[6] = (unsigned char)((input >> 16) & 0xff);
buff[7] = (unsigned char)((input >> 24) & 0xff);

MCP2515_CANSendMsg_check_data(id, buff,8,cs_address);
MCP2515_start_transmittion();
delay_ms(10);
MCP2515_CANReadMsg(&inpid,msg,&inp_len,cs_address);
inp_index=msg[1]+(msg[2]<<8);
if(inp_len>0 && inp_index==index && msg[3]==sub_index && msg[0]==0x60){
return 1;

}
return 0;

        }
//===============================================================================================================
char epos_sdo_write_command_all( int id, int input,u16 index,unsigned char sub_index,unsigned char len)
        {
		int i;
unsigned char buff[10];
u16  inp_index=0,inpid=0;
char success=0;
char retry=0,inp_len=0;
char msg[8];
try:
buff[0] = get_code(len);
buff[1] = (unsigned char)(index & 0xff);
buff[2] = (unsigned char)(index >> 8);
buff[3] = (unsigned char)(sub_index);
buff[4] = (unsigned char)(input & 0xff);
buff[5] = (unsigned char)((input >> 8) & 0xff);
buff[6] = (unsigned char)((input >> 16) & 0xff);
buff[7] = (unsigned char)((input >> 24) & 0xff);
 for(i=0;i<14;i++){MCP2515_CANSendMsg_check_data(id, buff,8,i);}
MCP2515_start_transmittion();
delay_ms(10);
///////////////////check message
success=0;
for(i=0;i<14;i++){
inpid=0;
msg[0]=0;
msg[1]=0;
msg[2]=0;
inp_len=0;
MCP2515_CANReadMsg(&inpid,msg,&inp_len,i);
inp_index=msg[1]+(msg[2]<<8);

if(inp_len>0 && inp_index==index && msg[3]==sub_index && msg[0]==0x60){
success++;

}	
}
//////////////////////

if(success==14){return 1;}
if(success!=14 && retry==0){retry++;goto try;}
return 0;

        }
//===============================================================================================================
void epos_sdo_read_command( int id,u16 index,unsigned char sub_index,char cs_address)
{

unsigned char buff[10];


buff[0] = get_code(0);
buff[1] = (unsigned char)(index & 0xff);
buff[2] = (unsigned char)(index >> 8);
buff[3] = (unsigned char)(sub_index);
buff[4] = 0	;
buff[5] = 0	;
buff[6] = 0	;
buff[7] = 0	;

MCP2515_CANSendMsg_check_data(id, buff,8,cs_address);
MCP2515_start_transmittion();




}
//===============================================================================================================

void epos_sdo_read_command_all( int id,u16 index,unsigned char sub_index)
{

unsigned char buff[10];
int i;

buff[0] = get_code(0);
buff[1] = (unsigned char)(index & 0xff);
buff[2] = (unsigned char)(index >> 8);
buff[3] = (unsigned char)(sub_index);
buff[4] = 0	;
buff[5] = 0	;
buff[6] = 0	;
buff[7] = 0	;

 for(i=0;i<14;i++){MCP2515_CANSendMsg_check_data(id, buff,8,i);}
MCP2515_start_transmittion();	
}
//==================================================================================
u8 waite_can_recive(int timeout,u8 cs_addr){
u8 temp=0;
u8 result=0;
int retry=0;
while(++retry<timeout){
temp = MCP2515_ReadStatus(cs_addr);
if( (temp & 3) != 0 ){result=1;break;}	
				}
return result;
}

//==================================================================================
char  epos_read_register(int id,u16 index,unsigned char sub_index,int*val,int cs_address){	   //0x600 +node id
 u16 inpid;
u16 inpindex=0;
 u8 len;
 u8 msg[8];
 char retry=0;
try:
MCP2515_CANReadMsg(&inpid,msg,&len,cs_address);
MCP2515_CANReadMsg(&inpid,msg,&len,cs_address);
epos_sdo_read_command(id+0x600,index,sub_index,cs_address);  //send sdo command
delay_ms(3);
if(waite_can_recive(400,cs_address)==0 && retry==0){dbg("time_out\n");retry++;goto try;}
if(waite_can_recive(400,cs_address)==0 && retry==1){dbg("time_out\n");return 0;}

MCP2515_CANReadMsg(&inpid,msg,&len,cs_address);
inpindex=msg[1]+(msg[2]<<8);

if((inpid==0x580+id) && (sub_index==msg[3])&& (inpindex==index)){
 (*val)=0;

*val=msg[7];
(*val)=(*val)<<8;
(*val)+=msg[6];
(*val)=(*val)<<8;
(*val)+=msg[5];
(*val)=(*val)<<8;
(*val)+=msg[4];

return 1;
}
dbg("wrong recive message\n");									 
return 0;
}
//==================================================================================

char   epos_get_pos(int id,int*pos,int cs_address){	   //0x600 +node id
 u16 inpid;

 u8 len;
 u8 msg[8];

MCP2515_CANReadMsg(&inpid,msg,&len,cs_address);
MCP2515_CANReadMsg(&inpid,msg,&len,cs_address);
epos_sdo_read_command(id+0x600,0x6064,0,cs_address);  //send sdo command
if(waite_can_recive(4000,cs_address)==0){return 0;}

MCP2515_CANReadMsg(&inpid,msg,&len,cs_address);

if(inpid==0x580+id){
 (*pos)=0;

*pos=msg[7];
(*pos)=(*pos)<<8;
(*pos)+=msg[6];
(*pos)=(*pos)<<8;
(*pos)+=msg[5];
(*pos)=(*pos)<<8;
(*pos)+=msg[4];
return 1;
}									 
return 0;
}
//==================================================================================
char   epos_get_pos_all(int id,int*pos){	   //0x600 +node id
 u16 inpid;
 int i;
 u8 len;
 u8 msg[8];
 int temp_pos=0;
 u16 inpindex=0;
 int success=0;
u8 retry=0;

 try:
success=0;

for(i=0;i<14;i++){
MCP2515_CANReadMsg(&inpid,msg,&len,i);
MCP2515_CANReadMsg(&inpid,msg,&len,i);}
epos_sdo_read_command_all(id+0x600,0x6064,0);  //send sdo command
//if(waite_can_recive(4000,cs_address)==0){return 0;}
 delay_ms(200);

 for(i=0;i<14;i++){
 inpindex=0;  msg[0]=0;inpindex=0;
MCP2515_CANReadMsg(&inpid,msg,&len,i);
 inpindex=msg[1]+(msg[2]<<8);
if(inpid==0x580+id && (msg[0]==0x43) && (0==msg[3]) && inpindex==0x6064){
 (temp_pos)=0;

temp_pos=msg[7];
(temp_pos)=(temp_pos)<<8;
(temp_pos)+=msg[6];
(temp_pos)=(temp_pos)<<8;
(temp_pos)+=msg[5];
(temp_pos)=(temp_pos)<<8;
(temp_pos)+=msg[4];
pos[i]=temp_pos;
success++;


}			} //end for

if(success==14)return 1;
if(success!=14 && retry==0){retry++;goto try;}						 
return 0;
}

//==================================================================================

void epos_goto_pos(int id,unsigned long int position,unsigned long int velocity,char cs_address){
epos_sdo_write_command(id+0x600,position,0x607a,0,4,cs_address);  //target pos
epos_sdo_write_command(id+0x600,velocity,0x6081,0,4,cs_address);  //target velocity
epos_sdo_write_command(id+0x600, 0x3f,0x6040,0,2,cs_address);	//START position abs

}
//==================================================================================
setup_abs_encoders(int id,char cs_address){


//set controller structure
if(epos_sdo_write_command(id+0x600,1,0x2220,0,2,cs_address)==0)
{
if(epos_sdo_write_command(id+0x600,1,0x2220,0,2,cs_address)==0)
{
return 0;
}
} 
//set sensor type to SSI
//sensor type=4 abs
if(epos_sdo_write_command(id+0x600,516,0x2210,2,2,cs_address)==0){
if(epos_sdo_write_command(id+0x600,516,0x2210,2,2,cs_address)==0){
return 0;
}

} 

//set GEAR RATIO
if(epos_sdo_write_command(id+0x600,gear_valu[cs_address],0x2230,1,4,cs_address)==0)
{
if(epos_sdo_write_command(id+0x600,gear_valu[cs_address],0x2230,1,4,cs_address)==0)
{
return 0;
}
} 
//set Gvelocity notation index
if(epos_sdo_write_command(id+0x600,-3,0x608B,0,1,cs_address)==0)
{
if(epos_sdo_write_command(id+0x600,-3,0x608B,0,1,cs_address)==0)
{
return 0;
}
} 


return 1;

}
//==================================================================================
char setup_inc_encoders(int id,char cs_address){
if (epos_sdo_write_command(id+0x600,2,0x2210,2,2,cs_address)==0){
if (epos_sdo_write_command(id+0x600,2,0x2210,2,2,cs_address)==0){

return 0;
}


}
 return 1;
}
//==================================================================================

char epos_get_abs_pos(int id,char cs_address,long int *position){

char result=0;
long int temp_pos=0;
int sensor_type=0,gear=0;
//set sensor type to SSI
//sensor type=4 abs
if(epos_sdo_write_command(id+0x600,4,0x2210,2,2,cs_address)==0){
if(epos_sdo_write_command(id+0x600,4,0x2210,2,2,cs_address)==0){
return 0;
}

} 
//set GEAR RATIO
if(epos_sdo_write_command(id+0x600,gear_valu[cs_address],0x2230,1,4,cs_address)==0)
{
if(epos_sdo_write_command(id+0x600,gear_valu[cs_address],0x2230,1,4,cs_address)==0)
{
return 0;
}
} 
delay_ms(10);
epos_read_register(1,0x2210,2,&sensor_type,cs_address);
epos_read_register(1,0x2230,1,&gear,cs_address);

if(gear!=gear_valu[cs_address] || sensor_type!=4 )return 0;



//GET POSITION
if(epos_get_pos(1,&temp_pos,cs_address)==1 )
{
*position= temp_pos;
}

//set sensor type to SSI
  //delay_ms(50);
 //epos_sdo_write_command(id+0x600,2,0x2210,2,2,cs_address);  //sensor type=2 inc
 // delay_ms(50);
 //epos_read_register(1,0x2210,2,&sensor_type,cs_address);
 //if(sensor_type!=2 )return 0;


return 1;
}
//==================================================================================

void epos_set_profilepos(int id,char cs_addr){
epos_sdo_write_command(id+0x600,1,0x6060,0,1,cs_addr);//mode set profile
delay_ms(300);
epos_sdo_write_command(id+0x600,0x06,0x6040,0, 2,cs_addr);
delay_ms(300);
epos_sdo_write_command(id+0x600,0x0f,0x6040,0, 2,cs_addr);
delay_ms(300);
epos_sdo_write_command(id+0x600,0x010f,0x6040,0, 2,cs_addr);
delay_ms(300);
}
//==================================================================================
char epos_get_mode(int id,int *mode,int cs_address){
char validity=0;

validity=  epos_read_register( id,0x6060,0,mode,cs_address);
return validity;

}


//==================================================================================
char epos_quick_stop_all(){
return epos_sdo_write_command_all(1+0x600,0x0b,0x6040,0, 2);
//char epos_sdo_write_command_all( int id, int input,u16 index,unsigned char sub_index,unsigned char len)

}
