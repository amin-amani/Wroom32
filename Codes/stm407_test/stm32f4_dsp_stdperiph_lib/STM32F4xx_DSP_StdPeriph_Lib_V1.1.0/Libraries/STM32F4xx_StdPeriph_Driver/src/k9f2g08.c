#include "stm32f4xx.h"
#include <RTL.h>
#include "hal.h"
#include "lib_crc.h"
#include "usart.h"
#include <stdio.h>



#define FALSE 0
#define TRUE 1

#define INPUT	FALSE
#define OUTPUT	TRUE 

#define NAND_CLE 8
#define NAND_ALE 9
#define NAND_WE 10 
#define NAND_WP 11  
#define NAND_CE 12
#define NAND_RE 13
#define NAND_RB 14

#define CONTROL_PORT (GPIOE->ODR)

#define NAND_CLE_LOW() {CONTROL_PORT&=~(1<<NAND_CLE);}
#define NAND_CLE_HI()  {CONTROL_PORT|= (1<<NAND_CLE);}

#define NAND_ALE_LOW() {CONTROL_PORT&=~(1<<NAND_ALE);}
#define NAND_ALE_HI()  {CONTROL_PORT|= (1<<NAND_ALE);}

#define NAND_CE_LOW() {CONTROL_PORT&=~(1<<NAND_CE);}
#define NAND_CE_HI()  {CONTROL_PORT|= (1<<NAND_CE);}

#define NAND_RE_LOW() {CONTROL_PORT&=~(1<<NAND_RE);}
#define NAND_RE_HI()  {CONTROL_PORT|= (1<<NAND_RE);}

#define NAND_WE_LOW() {CONTROL_PORT&=~(1<<NAND_WE);}
#define NAND_WE_HI()  {CONTROL_PORT|= (1<<NAND_WE);}

#define NAND_WP_LOW() {CONTROL_PORT&=~(1<<NAND_WP);}
#define NAND_WP_HI()  {CONTROL_PORT|= (1<<NAND_WP);}


#define NAND_RD_CONTROL_PORT (GPIOE->IDR)
#define NAND_RB_READ() ((NAND_RD_CONTROL_PORT&(1<<NAND_RB))>>NAND_RB)

#define NAND_READ_DATA() (GPIOE->IDR&0xff)
#define NAND_WRITE_DATA(X) {GPIOE->ODR&=~(0xff);GPIOE->ODR|=X;}

#define NAND_PD_CONTROL_PORT (GPIOE->IDR)  		
#define NAND_PD2_READ() ((NAND_PD_CONTROL_PORT&0x04)>>2)

// ---------------------------------------------------
#define NAND_iib_mapping_table_adddress		0x80000		// 524288
#define NAND_invalid_start_address			0x40000		// 262144

// ---------------------------------------------------

char NAND_read_id_device[5];
char NAND_data_output[2112];
char NAND_data_input [2112];

BOOL NAND_invalid_block_table [2048]; 
char NAND_invalid_block_count;

// array for mapping invalid block
unsigned int NAND_iib_mapping_count;
unsigned int NAND_iib_mapping_address[50];

char NAND_write_buffer [2112];
unsigned int NAND_data_count ;
unsigned int NAND_start_address ;
unsigned long int NAND_current_data_count;
unsigned int NAND_write_mode_index ;
BOOL NAND_read_mode_flag ;
BOOL NAND_write_mode_flag ;
//unsigned int NAND_crc_input1;
//unsigned int NAND_crc_input2;  



//=====================================================================================================


// =====================================================================
// ===============   LEVEL 0 FUNCTION   ================================
// NAND_io_init;
// nand_set_data_direction;
// read_data_portc;
// write_data_portc;
// =====================================================================

void NAND_io_init(void){
GPIO_InitTypeDef  GPIO_InitStructure;
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);  //enable io clock for porte nand port

//	NAND_RB=14		DATA[0~7]=PE[0~7]  AS IN	
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2 |GPIO_Pin_3|GPIO_Pin_4 |GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
GPIO_Init(GPIOE, &GPIO_InitStructure);


//	NAND_CLE=8 NAND_ALE=9 NAND_WE=10 NAND_WP=11 NAND_CE=12 NAND_RE=13   AS OUT
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
GPIO_Init(GPIOE, &GPIO_InitStructure);

//GPIOE->ODR|=1<<14;  // pull up ready/busy pin, it's importannt :)

}
 //===============================================================================================
void nand_set_data_direction (char direction)
{
GPIO_InitTypeDef  GPIO_InitStructure; 	
//set as output
if(direction==1)
	{
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2 |GPIO_Pin_3|GPIO_Pin_4 |GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	return;
	}
// set as input
GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2 |GPIO_Pin_3|GPIO_Pin_4 |GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
GPIO_Init(GPIOE, &GPIO_InitStructure);
}
//===============================================================================================
char read_data_portc() 
{
	nand_set_data_direction(INPUT);
	return NAND_READ_DATA();
}

//===============================================================================================

void write_data_portc(char d_o)
{
	nand_set_data_direction(OUTPUT);
	NAND_WRITE_DATA(d_o);
	return;
}
//===============================================================================================



// =====================================================================
// ===============   LEVEL 1 FUNCTION   ================================
// NAND_read_id_operation ;
// NAND_command_latch_cycle;
// NAND_address_latch_cycle;
// NAND_input_data_latch_cycle;
// NAND_output_data_latch_cycle;
// NAND_read_status_register_cycle;
// NAND_block_address_latch_cycle;
// =====================================================================


unsigned int NAND_read_id_operation(char* NAND_read_id_device)
{
	unsigned int i;
	// first stage: READ ID command

	NAND_WP_HI(); // Disable Write Protection 
	NAND_RE_HI(); // Disable Read 
	NAND_ALE_LOW();
	NAND_CE_LOW();
	NAND_CLE_HI();
	write_data_portc(0x90); 
	NAND_WE_LOW();
	// delay_us(1);  //   ok
	NAND_WE_HI();
	NAND_CLE_LOW(); 

	// second stage: address(1 cycle)
	NAND_ALE_HI();
	write_data_portc(0x00); // address=00
	NAND_WE_LOW();
	// delay_us(1);  //   ok
	NAND_WE_HI();
	NAND_ALE_LOW();
	//delay=tAR

	// third stage: read data 
	NAND_RE_LOW();
	for( i=0; i<4; i++)
	{
		NAND_read_id_device[i]= read_data_portc() ;
		NAND_RE_HI();
		NAND_RE_LOW();
   	}
	NAND_read_id_device[i]= read_data_portc() ;
	NAND_RE_HI();
  
if (i==4)
	return 1;
else
	return 0; 
}

//=====================================================================================================

unsigned int NAND_command_latch_cycle(unsigned int cmd)
{
	NAND_WP_HI(); // Disable Write Protection 
	NAND_RE_HI(); // Disable Read 
	NAND_CE_LOW();
	NAND_CLE_HI();
	NAND_ALE_LOW();
	write_data_portc(cmd); 
	NAND_WE_LOW();
	// delay_us(1);  //   ok
	NAND_WE_HI();
	NAND_CLE_LOW();
	return 1;
}

// =====================================================================
unsigned int NAND_address_latch_cycle(unsigned int adr)
{
	NAND_WP_HI(); // Disable Write Protection 
	NAND_ALE_LOW();
	NAND_WE_HI();
	NAND_CLE_LOW();
	NAND_CE_LOW();
	// delay_us(1);
	NAND_ALE_HI();
	//delay_us(1);
	write_data_portc(adr&0xff);
	// delay_us(1);
	NAND_WE_LOW();
	// delay_us(1);
	NAND_WE_HI();
	// delay_us(1);
	NAND_ALE_LOW();
	// delay_us(1);
	NAND_ALE_HI();
	// delay_us(1);
	adr=adr>>8;
	// delay_us(1);
	write_data_portc(adr&0x0f); 
	// delay_us(1);
	NAND_WE_LOW();
	// delay_us(1);
	NAND_WE_HI();
	// delay_us(1);
	NAND_ALE_LOW();
	// delay_us(1);
	NAND_ALE_HI();
	// delay_us(1);
	adr=adr>>4;
	// delay_us(1);
	write_data_portc(adr&0xff);
	// delay_us(1);
	NAND_WE_LOW();
	// delay_us(1);
	NAND_WE_HI();
	// delay_us(1);
	NAND_ALE_LOW();
	// delay_us(1);
	NAND_ALE_HI();
	// delay_us(1);
	adr=adr>>8;
	// delay_us(1);
	write_data_portc(adr&0xff);
	// delay_us(1);
	NAND_WE_LOW();
	// delay_us(1);
	NAND_WE_HI();
	// delay_us(1);
	NAND_ALE_LOW();
	// delay_us(1);
	NAND_ALE_HI();
	// delay_us(1);
	adr=adr>>8;
	// delay_us(1);
	write_data_portc(adr&0x01);
	// delay_us(1);
	NAND_WE_LOW();
	// delay_us(1);
	NAND_WE_HI();
	// delay_us(1); 
	NAND_ALE_LOW();
	
	return 1;
}

// =====================================================================


unsigned int NAND_input_data_latch_cycle(char* NAND_data_input, unsigned int data_cnt)
{
	unsigned int i=0;
	NAND_WP_HI(); // Disable Write Protection 
	NAND_WE_HI();
	NAND_ALE_LOW();
	NAND_CLE_LOW();
	NAND_CE_LOW();
	
	for (i=0; i<data_cnt; i++)
	{
		NAND_WE_LOW();
		write_data_portc(NAND_data_input[i]); 
		NAND_WE_HI();
	}
	NAND_CE_HI();

	return 1;

}
// =====================================================================

unsigned int NAND_output_data_latch_cycle(char* NAND_data_output, unsigned int data_cnt)
{
	int i=0;

	NAND_RE_HI();
	NAND_CE_LOW();
	NAND_CLE_LOW();
	NAND_WE_HI();
	NAND_ALE_LOW();
	
	if (NAND_RB_READ()==0)
		return 0;

	nand_set_data_direction(INPUT);
	NAND_RE_LOW();
	for( i=0; i<data_cnt; i++)
	{
		// delay_us(1);
		NAND_data_output[i] = NAND_READ_DATA(); // read_data_portc() ;
		NAND_RE_HI();
		// delay_us(1);
		NAND_RE_LOW();
	}
	// delay_us(1);
	NAND_RE_HI();
	NAND_CE_HI();
	return i;	
}

// =====================================================================
unsigned int NAND_read_status_register_cycle(unsigned int cmd) // cmd just is 0x70h or 0xf1h
{
	unsigned int status_register=0;
	NAND_WP_HI(); // Disable Write Protection 
	NAND_CLE_LOW();
	NAND_CE_HI();
	NAND_WE_HI();
	NAND_RE_HI();

	NAND_CLE_HI();
	NAND_CE_LOW();
	write_data_portc(cmd);
	NAND_WE_LOW();
	// delay_us(1);		//ok
	NAND_WE_HI();
	NAND_CLE_LOW();
	// HIGH-Z OUTPUT  ??????????????????
	// DELAY CHECK
	NAND_RE_LOW();
	// DELAY CHECK
	status_register = read_data_portc();// READ INPUT DATA
	NAND_RE_HI();
	return status_register;

}

// =====================================================================
// this function use for block addressing (for erase operation)
// Only address A18 to A28 is valid while A12 to A17 is ignored
// address block is  Col1: A18-A19 | Col2: A20-A27 | Col3: A28
unsigned int NAND_block_address_latch_cycle(unsigned int adr)
{
	NAND_WP_HI(); // Disable Write Protection 
	NAND_ALE_LOW();
	NAND_WE_HI();
	NAND_CLE_LOW();
	NAND_CE_LOW();
	NAND_ALE_HI();
	write_data_portc((adr&0x03)<<6);
	NAND_WE_LOW();
	NAND_WE_HI();
	NAND_ALE_LOW();
	NAND_ALE_HI();
	adr=adr>>2;
	write_data_portc(adr&0xff); 
	NAND_WE_LOW();
	NAND_WE_HI();
	NAND_ALE_LOW();
	NAND_ALE_HI();
	adr=adr>>8;
	write_data_portc(adr&0x01);
	NAND_WE_LOW();
	NAND_WE_HI();
	NAND_ALE_LOW();
	return 1;
}

// =====================================================================
// =====================================================================
// ===============   LEVEL 2 FUNCTION   ================================
// NAND_reset_operation 
// NAND_program_operation
// NAND_read_operation;
// NAND_erase_operation;
// =====================================================================


unsigned int NAND_reset_operation(void)
{
	unsigned int reset_flag = 0 ;	
	reset_flag = NAND_command_latch_cycle(0xFF); // reset command
	delay_us(500);	// maximum time for reset (Trst)
	return reset_flag ;

}

// =====================================================================
unsigned int NAND_program_operation (unsigned int address, unsigned int data_cnt,char* NAND_data_input )  
{
unsigned int cmd_latch_flag=0;
unsigned int adr_latch_flag=0;
unsigned int in_data_latch_flag=0;
unsigned int status_register_flag =0 ;
unsigned int status_register =0 ;
unsigned long int timeout =0;
// unsigned int word_address=0 ;

if (address== 0)
	{send_string3("Error! input address is invalid (==0).\n");
	 return 0;
	}

NAND_WP_HI(); // Disable Write Protection 

// command 80h  for write data
cmd_latch_flag = NAND_command_latch_cycle(0x80);
if (cmd_latch_flag==0)
	{ return 0; }

// address
adr_latch_flag = NAND_address_latch_cycle(address);
if (adr_latch_flag==0)
	{ return 0; }

// data	
in_data_latch_flag = NAND_input_data_latch_cycle(NAND_data_input, data_cnt);
if (in_data_latch_flag==0)
	{ return 0;	}

// command 10h (for write data)
cmd_latch_flag = NAND_command_latch_cycle(0x10);
if (cmd_latch_flag==0)
	{ return 0; }

// Tprog, Program Time: (Typical =200us) , (Max=700us)
delay_us(700);   // this delay is necessary 

// check RB signal , waiting to complete the program operation
// we must set a counter here for exeptions to prevent always loop  !!!!!! ?????
while (NAND_RB_READ() ==0)
	{delay_us(1) ;
	 timeout++;
	 if (timeout >= 0xffffffff)
	 	return 0;
	}

// error check
status_register_flag = NAND_read_status_register_cycle(0x70); 
if (status_register_flag == 0)
	{ return 0; }
// send_string3("status reg cmd pass.\n");   // only for test

status_register = read_data_portc() ;
if ((status_register & 0x01) == 1)
	{ return 0; }

return 1;		

}

// =====================================================================
unsigned int NAND_read_operation (unsigned int address, unsigned int data_cnt, char* NAND_data_output) 
{
unsigned int cmd_latch_flag=0;
unsigned int adr_latch_flag=0;
unsigned int status_register_flag =0 ;
unsigned int status_register =0 ;
unsigned int data_cnt_flag=0;
unsigned long int timeout =0;
char temp [20];

// command 00h for Read data
cmd_latch_flag = NAND_command_latch_cycle(0x00);
if (cmd_latch_flag==0)
	{send_string3("Error in NAND_command_latch_cycle.\n"); 
	 return 0; 
	}

// address
adr_latch_flag = NAND_address_latch_cycle(address);
if (adr_latch_flag==0)
	{send_string3("Error in NAND_address_latch_cycle.\n"); 
	 return 0; 
	}

// command 30h for Read data
cmd_latch_flag = NAND_command_latch_cycle(0x30);
if (cmd_latch_flag==0)
	{send_string3("Error in NAND_command_latch_cycle.\n"); 
	 return 0; 
	}

// Tr (Data Transfer from Cell to Register)= 25us
delay_us(25);  // this delay is necessary 

// check RB signal , waiting to complete the Read operation
// we must set a counter here for exeptions to prevent always loop  !!!!!! ?????
while (NAND_RB_READ() ==0)
 	{delay_us(1) ; 
	 timeout++;
	 if (timeout >= 0xfffff)
	 	{send_string3("Error in waiting to RB signal set.\n");
	 	 return 0;
		}
	}

// read data	
data_cnt_flag = NAND_output_data_latch_cycle(NAND_data_output, data_cnt);
if (data_cnt_flag ==0)
 	{ send_string3("Error in NAND_output_data_latch_cycle.\n");
	  return 0;	
	}

// ECC Generation !!!!!!!!!!!
// Put your error detection code here 
// check ECC byte in each page and check error 
// for knowing where is ECC byte in "an_elnec_nand_flash.pdf" page 8


//error check
status_register_flag = NAND_read_status_register_cycle(0x70); 
if (status_register_flag == 0)
	{sprintf(temp,"%d", status_register_flag);
	 send_string3(temp);
	 send_string3(" Error in NAND_read_status_register_cycle.\n"); 
	 return 0; }

status_register = read_data_portc() ;
if ((status_register & 0x01) == 1)
	{send_string3("Error in status_register.\n");  
	 return 0; }

 return 1;				
}


// =====================================================================
unsigned int NAND_erase_operation (unsigned int block_address)  
{
unsigned int cmd_latch_flag=0;
unsigned int adr_latch_flag=0;
unsigned int status_register_flag =0 ;
unsigned int status_register =0 ;
unsigned long int timeout=0;

if (block_address >=  2048)
	{ 
	 send_string3("It's not an address of block (Valid block address is 0-2047).");  
	 return 0; 
	}  
	
// command 60h  for erase
cmd_latch_flag = NAND_command_latch_cycle(0x60);
if (cmd_latch_flag==0)
	{ return 0; }
	
adr_latch_flag = NAND_block_address_latch_cycle(block_address); 
if (adr_latch_flag==0)
	{ return 0; }

// command D0h  for erase
cmd_latch_flag = NAND_command_latch_cycle(0xD0);
if (cmd_latch_flag==0)
	{ return 0; }

// Tbesr(Block Erase Time) = (Typical =1.5ms) , (Max=10ms)
delay_ms(10);  // thie delay is necessary 

// check RB signal , waiting to complete the Erase operation
// we must set a counter here for exeptions to prevent always loop  !!!!!! ?????
while (NAND_RB_READ() ==0)
	{delay_ms(1) ; 
	 timeout++;
	 if (timeout >= 0xffffffff)
	 	return 0;
	}

// error check
status_register_flag = NAND_read_status_register_cycle(0x70); 
if (status_register_flag == 0)
	{ return 0; }

status_register = read_data_portc() ;
if ((status_register & 0x01) == 1)
	{ return 0; }

return 1;		

}


// =====================================================================
// ===============   LEVEL 3 FUNCTION   ================================
// NAND_erase_all_memory;
// =====================================================================

unsigned int NAND_erase_all_memory(void)
{
unsigned int i;
unsigned int flag;
char temp [50];

for (i=0; i<2048; i++)
	{ flag = NAND_erase_operation (i) ; 
 	if (flag !=0)
	  	{
		sprintf(temp, "Block %d erased successfully.\n", i);
		send_string3(temp);
		}
	else
		{
		sprintf(temp, "Block %d do not erased.\n", i);
		send_string3(temp);
		}	
	}
send_string3("Erase memory operation done.\n");
return 1;
}

// =====================================================================

// ======================================================================
// Check "FFh" at the column address 2048 of the 1st and 2nd page in each block
// if this byte is not "FFh" then this block is an initial invalid block and add to table
// this function do not erase any block, and for checking initial invalid block
unsigned int NAND_identify_initial_invalid_blocks (unsigned int mode)
{

unsigned int i = 0;
char j;
unsigned int sparse_address   = 0x00000800 ;
unsigned int block_difference = 0x00040000 ;
unsigned int page_difference  = 0x00001000 ;

char data_read_01 [4];
char data_read_02 [4];
char temp_string [50];
unsigned int flag = 0;
char ii_b_count [128];
char ii_b_list [128];

j=0;

for (i=0; i < 2048 ; i++)
	{
	flag = NAND_read_operation(sparse_address, 1, data_read_01);
	if (flag==0)
		{send_string3("Error in reading data01.\n");
		 return 0;
		}
	flag = NAND_read_operation ((sparse_address + page_difference ),1, data_read_02);
	if (flag==0)
		{send_string3("Error in reading data01.\n");
		 return 0;
		}
	if ((data_read_01[0] != 0xff ) | (data_read_02[0] != 0xff )) 	//  we must | or & this condition
		{
		 NAND_invalid_block_table[i]=TRUE;

		 
		 ii_b_list[(j<<2)+4] = i & 0xff;   
		 ii_b_list[(j<<2)+5] = (i>>8) & 0xff;
		 ii_b_list[(j<<2)+6] = 0;   
		 ii_b_list[(j<<2)+7] = 0;   

		 j++;
		 if (mode == 1)
		 	{
			 sprintf(temp_string, "%d", i);
		 	 send_string3("\n initial invalid block: ");
		 	 send_string3(temp_string);	
			}
		}
	sparse_address += block_difference ; 
    } 
sendchar3(j);
//sprintf(temp_string,"%d", j);
//send_string3(temp_string);

ii_b_list[0] =  j ; 
ii_b_list[1] = (j>>8) & 0xff;   // equal with zero
ii_b_list[2] = (j>>16) & 0xff;  // equal with zero 
ii_b_list[3] = (j>>24) & 0xff;  // equal with zero

NAND_read_operation(NAND_invalid_start_address, ((j+1)<<2), ii_b_count);
NAND_invalid_block_count = ii_b_count[0];

if (mode ==1)
	{
	 sprintf(temp_string,"\n invalid block count = %d , and mem[0]= %d \n.", j, NAND_invalid_block_count);
	 send_string3(temp_string);
	
	if (NAND_invalid_block_count == j)
 		{send_string3("OK! The invalid initial block count do not change (mem[0] == new invalid block count). \n");
		}
	}
if (NAND_invalid_block_count != j)
 	{
	 if (mode == 1)
	 	{send_string3("Warning!!! The invalid initial block count change (mem[0] != new invalid block count)!!! \n");
	 	 send_string3("Update memory with new invalid block list.\n");
		}
	NAND_invalid_block_count = j;

	// erase the block 01 
	flag = NAND_erase_operation(1);
	if (flag == 1)
		{
		// write invalid block list in mem[262144]
		flag = NAND_program_operation(NAND_invalid_start_address,(j+1)<<2 ,ii_b_list);
		if (flag == 1) 
			{ if (mode ==1) send_string3("Write invalid_block_list in mem 262144 successfully.\n");
			}
		else  send_string3("Error in write invalid_block_list in mem 262144.\n");
		}
	else  send_string3("Error in erase block 2.\n");

	}
return j ;
}


// =====================================================================
// read data of invalid block from nand memory to mapping_invaid_block_address
unsigned int NAND_initial_iib_mapping_table(void)
{
 unsigned int flag, i, j, k ;
 char mem_array [200];
// char temp [50];
 
 flag = NAND_read_operation(NAND_iib_mapping_table_adddress, 200, mem_array);
 	if (flag==0)
		{ send_string3("Error ali00");
		return 0;}
 
 NAND_iib_mapping_count = (mem_array[0] && 0xff) ;

 for(i=0; i< 50 ; i++)
 	{
	 NAND_iib_mapping_address[i]= mem_array[(i<<2)+4] + (mem_array[(i<<2)+5] << 8) ;
	}
 
 if (NAND_iib_mapping_count == NAND_invalid_block_count)
 	{
	for(i=1; i< (NAND_invalid_block_count+1) ; i++)
 	{
	 for(j=i+1; j< (NAND_invalid_block_count+1); j++) 
	 	{
		 if( (NAND_iib_mapping_address[i] != 0xffff) && 
		 	 (NAND_iib_mapping_address[i] == NAND_iib_mapping_address[j]) ) 
			 {
			  mem_array[0]= NAND_invalid_block_count & 0xff ;
			  mem_array[1]= 0 ;
			  mem_array[2]= 0 ;
			  mem_array[3]= 0 ;
			  
//			  for(k=0; k<50; k++)  
//				  {
//				   NAND_iib_mapping_address[k]= 0; 
//				  }	
			  send_string3("NAND_erase_operation(2) a\n");  
			  NAND_erase_operation(2);
			  send_string3("NAND_program_operation 4 b\n");  
			  flag = NAND_program_operation(NAND_iib_mapping_table_adddress , 4, mem_array) ;
			  if (flag==0)
				{return 0;}
			  return 3;
			 }
		}
	 }
	 // copy map mem to map array 
	 //send_string3("ali1");
     for(k=0; k< 50; k++)
	 	{
		 NAND_iib_mapping_address[k]= mem_array[(k<<2)+4] + (mem_array[(k<<2)+5] << 8); 
		}
	 return 1;
  }  // End if (NAND_iib_mapping_count == NAND_invalid_block_count)
 
 // data in nand must be updated 
 else if ( (NAND_iib_mapping_count>0) && (NAND_iib_mapping_count < NAND_invalid_block_count))  
 {
  mem_array[0]= NAND_invalid_block_count & 0xff ;
  mem_array[1]= (NAND_invalid_block_count>>8) & 0xff ;
  //send_string3("NAND_erase_operation(2)c\n");  
  NAND_erase_operation(2);
  //send_string3("NAND_program_operation 200 d\n");  
  flag = NAND_program_operation(NAND_iib_mapping_table_adddress , 200, mem_array) ;
  if (flag==0)
	{return 0;}
  return 2;
 }

 send_string3("Warning!! NAND_iib_mapping_count has invalid value.\n");  
 mem_array[0]= NAND_invalid_block_count & 0xff ;
 mem_array[1]= (NAND_invalid_block_count>>8) & 0xff ;
 //send_string3("NAND_erase_operation(2)e\n");  
 for (i=2; i< 200; i++)
  	{mem_array[i]=0;}
 NAND_erase_operation(2);
 //send_string3("NAND_program_operation 200 f\n");  
 flag = NAND_program_operation(NAND_iib_mapping_table_adddress , 200, mem_array) ;
  if (flag==0)
	{return 0;}
  return 3;
}


// ============================================================================
// write data of invalid block from mapping_invaid_block_address to nand memory
unsigned int NAND_store_iib_mapping_table(unsigned int invalid_block_id)
{
 unsigned int flag, i, j  ;
 char mem_array [200];
 char temp [50];

 mem_array[0] = NAND_invalid_block_count & 0xff ;
 mem_array[1] = 0;
 mem_array[2] = 0;
 mem_array[3] = 0;

 flag = 0;
 for(j=0; ((j < 50) && (flag ==0 )) ;j++)
 	{
	 if ((NAND_iib_mapping_address[j]== invalid_block_id ) && (NAND_iib_mapping_address[j] != 0 ))
		 {flag = 2;	
		  return (2000+j);
		 }
	 else if ( NAND_iib_mapping_address[j]== 0xffff)     //  (NAND_iib_mapping_address[i]==0) ||
	 	{
		 NAND_iib_mapping_address[j]= invalid_block_id;	 
		 flag =1;
		}
	} 
 
 if (flag == 0)
 	{send_string3("Error! There is not any more space in memory.\n"); 
	 return 0;
	}
 if (flag == 2)
 	{send_string3("Error! There is same address with this adr block in memory.\n"); 
	 return 0;
	}

 if (invalid_block_id==0)
 	{NAND_iib_mapping_address[j-1]= 0;	 
	}

 for(i=0; i < 49 ;i++)
 	{	 
	 mem_array[(i<<2)+4]   = NAND_iib_mapping_address[i] & 0xff ;
	 mem_array[(i<<2)+5] = ((NAND_iib_mapping_address[i]>>8) & 0xff) ;
	 mem_array[(i<<2)+6] = 0;
	 mem_array[(i<<2)+7] = 0;
	}

 //send_string3("NAND_erase_operation(2) -update mapping table in store \n");
 NAND_erase_operation(2);
 //send_string3("NAND_program_operation i<<2  -update mapping table in store \n");  
 flag = NAND_program_operation(NAND_iib_mapping_table_adddress , 200, mem_array) ;
 if (flag == 0)
 	{return 0;}
 
 
 // for test ------START -----------------------
 NAND_iib_mapping_count =0; 
 flag = NAND_read_operation(NAND_iib_mapping_table_adddress, 200, mem_array);
 if (flag == 0)
 	{return 0;}
 
 NAND_iib_mapping_count = mem_array[0] + (mem_array[1]<<8);
 if (NAND_iib_mapping_count != NAND_invalid_block_count)
 	{send_string3("Error!!! \n");
	 sprintf(temp,"NAND_invalid_block_count=%d, NAND_iib_mapping_count=%d \n",NAND_invalid_block_count, NAND_iib_mapping_count); 
	 send_string3(temp);
	 return 0;
	}
 // for test ------ END -----------------------

 return (2000+j-1);  // return the number of mapped block 
}


// ============================================================================
// ============================================================================
// ============================================================================
// 	NEW  GOOOOOOD  update mapping address table and after that return the new mapped address
unsigned int NAND_update_mapping_address_table(unsigned int v_block_address, BOOL* NAND_invalid_block_table)
{
 unsigned int i, j, r_block_address = 0;
 // char temp [50];
  //send_string3(" NAND_update_mapping_address_table started\n");
 if (v_block_address > 1995)
 	{send_string3("Error!!! invalid address. \n");
	 return 0;
	}
 r_block_address = 4 + v_block_address ;

 // if this block is valid
 if(!NAND_invalid_block_table[r_block_address])
 	{ return  r_block_address;
	}													
 
 // this block is invalid
 for(i=0; ((i< 50)&& (NAND_iib_mapping_address[i]!=0xffff )) ; i++)
	{
	 if (NAND_iib_mapping_address[i] == r_block_address) 
		{ return (2000 + i);  // iib_mapping_mapped[i];
		}
	}

 // this invalid block do not map yet 
 // NAND_iib_mapping_address[i] = r_block_address ;  // (i==NAND_iib_mapping_count)   ???????????????  
 
 // ???????????????????????????????????
// j=0;
// while((!NAND_invalid_block_table[2000+i+j]) && (j < 4))
// 	{
// 	 NAND_store_iib_mapping_table(0);
//	 j++;
//	}	
//
// if (j==4)
// 	{
//	 send_string3("Error!!! in mapping address.\n");
//	 return 0;
//	}
// //iib_mapping_mapped[i]  = i + 2001;
 
 // program this address in nand memory
 return NAND_store_iib_mapping_table(r_block_address);
}


// =====================================================================
unsigned int NAND_virtual2real_address (unsigned int v_address)
{
 unsigned int v_block_address, r_block_address, r_address;
 
 v_block_address = v_address >> 17 ;
 r_block_address =  NAND_update_mapping_address_table (v_block_address, NAND_invalid_block_table);
 if (r_block_address == 0)
 	{
	 send_string3("Error in mapping read address.\n");
	 return 0;
	}
 r_address = (r_block_address << 18) + ((v_address & 0x1f800)<<1) + (v_address & 0x007ff) ;
 return r_address;
}

// =======================================================
unsigned int NAND_show_device_property (char* NAND_read_id_device)
{
 unsigned int i, temp_bits, page_size, block_size, redundant_area_size, organization, serial_access_minimum_time;
 char temp_char [20];


 send_string3("Your device ID data is: ");
 for(i=0;i<5;i++)
 {
	sprintf(temp_char, " %X ", NAND_read_id_device[i] );
	send_string3(temp_char);
 } 	
 
 send_string3("\n1- The maker code is: ");
 sprintf(temp_char, "%d",NAND_read_id_device[0]);
 send_string3(temp_char);

 send_string3("\n2- The Device code is: ");
 sprintf(temp_char, "%d",NAND_read_id_device[1]);
 send_string3(temp_char);
 

 // ------- the 4th byte encoding -----
 temp_bits = NAND_read_id_device[3] & 0x03 ;
 page_size = 1024 << temp_bits;

 temp_bits = (NAND_read_id_device[3] & 0x30) >> 4 ;
 block_size = 64* 1024 << temp_bits;

 temp_bits = (NAND_read_id_device[3] & 0x04) >> 2 ;
 redundant_area_size = 8 << temp_bits;


 temp_bits = (NAND_read_id_device[3] & 0x40) >> 6 ;
 organization = 8 << temp_bits;  					// this is word size of memory ???

 temp_bits = (NAND_read_id_device[3] & 0x80) >> 7 ;
 if (temp_bits==0 )
 	{ serial_access_minimum_time = 50;}
 else if (temp_bits==1 )
 	{ serial_access_minimum_time = 25;   }

 
 send_string3("\nThe Device properties is: ");
 send_string3("\nThe Page size: ");
 sprintf(temp_char, " %d Byte ", page_size);
 send_string3(temp_char);

 send_string3("\nThe Block size: ");
 sprintf(temp_char, " %d Byte", block_size);
 send_string3(temp_char);

 send_string3("\nThe redundant area size: ");
 sprintf(temp_char, " %d Byte ", redundant_area_size);
 send_string3(temp_char);

 send_string3("\nThe organization (word size): ");
 sprintf(temp_char, " %d bit", organization);
 send_string3(temp_char);

 send_string3("\nThe serial access minimum time: ");
 sprintf(temp_char, " %d ns \n", serial_access_minimum_time);
 send_string3(temp_char);
 return 1;

}

// ===================================================

void NAND_test_device_id(char *par)
{
 unsigned int flag, mode;
 char com [20];
  
 sscanf(par,"%s %d",com, &mode);
 flag = NAND_read_id_operation(NAND_read_id_device);
 if (flag==0)
  	{ send_string3("Error in reading device ID.\n"); 
	  return;
	}
 // check id_device
 if  ((NAND_read_id_device[1]==0)||(NAND_read_id_device[2]==0)||(NAND_read_id_device[3]==0)||(NAND_read_id_device[4]== 0))
	{
	 send_string3("Error!!! Nand Flash device is not detected.\n"); 
	 return;
	} 
 if  ((NAND_read_id_device[1]!= 0xDA) || (NAND_read_id_device[2]!= 0x10) || (NAND_read_id_device[3]!= 0x95) || 
 	  (NAND_read_id_device[4]!= 0x44) )
	{
	 send_string3("Warning!!! This device is not Samsung Nand Flash ('K9F2G08U0B').\n"); 
	 return;
	} 
 if (mode == 1)
 	{
	 send_string3("OK! reading device ID done successfully.\n"); 
	 flag= NAND_show_device_property (NAND_read_id_device);
	}

 return;
}



// ===================================================

unsigned int NAND_initial_reset_function(unsigned int mode)
{
unsigned int flag, cnt_invalid_block =0;
char temp [20];

sprintf(temp,"testid %d", mode);
NAND_test_device_id(temp);  

cnt_invalid_block = NAND_identify_initial_invalid_blocks(mode);

if (cnt_invalid_block == 0)
	{ send_string3("Error! Identify initial invalid block failed!!!\n"); 
	  return 0;
	}
if (mode == 1)
	{
	send_string3("Identify iib Complete, there are "); 
	sprintf(temp,"%d", cnt_invalid_block );
	send_string3(temp);
	send_string3(" initial invalid block in flash memory.\n");
	}
// test id device   
// do not used serial port in case there is no error

// read data of invalid block from nand memory to mapping_invaid_block_address
flag = NAND_initial_iib_mapping_table(); 
//sprintf(temp,"%d", flag );
//send_string3(temp);
return flag;
}

// =====================================================================
// ================== READING & WRITING FUNCTION =======================
// =====================================================================

unsigned int NAND_read_all (unsigned int v_address, unsigned int  data_cnt, char* NAND_data_output)
{
unsigned int flag, i, start_address;
//char temp [20];
start_address = NAND_virtual2real_address(v_address);
flag = NAND_read_operation (start_address, data_cnt, NAND_data_output);

if (flag==1)
 {
	for(i=0; i<data_cnt; i++)
		{
		 sendchar3(NAND_data_output[i]); 
		 //sprintf(temp,"%d", NAND_data_output[i]);
		 //send_string3(temp);
		 //delay_us(100);
		}
 }
return flag;	
}

// =====================================================================
unsigned int NAND_write_all (unsigned int v_address, unsigned int  data_cnt, char* NAND_data_output, char first_page)
{
unsigned int start_address, page_number, flag;
// char temp [40];

page_number =  (v_address >> 11) & 0x3f ; 
start_address = NAND_virtual2real_address(v_address);

// !!!!!!! Erase this Block  !!!!!!!!!
if (first_page ==1 || page_number == 0 )
	{
	 flag = NAND_erase_operation(start_address>>18);
	 if(flag==0)
		{send_string3("Error! in erasing block.\n");
		 return 0;
		}
	}
flag = NAND_program_operation (start_address, data_cnt, NAND_data_output);
return flag;	
}

// =====================================================================
unsigned int NAND_write_crc (unsigned int v_address, unsigned int  data_cnt, char* NAND_data_output )
{
unsigned int flag, start_address = 0;
start_address = NAND_virtual2real_address(v_address) + 2048; // this 2048 is for crc
flag = NAND_program_operation(start_address, data_cnt, NAND_data_output);
return flag;	
}


// =====================================================================
unsigned int NAND_crc32_array(char* NAND_write_buffer, unsigned int start_index,unsigned int end_index)
{
 unsigned int crc_result= 0xffffffff;
 unsigned int i=0;
 
 for(i=start_index; i<end_index+1; i++)
 	{
	 crc_result = update_crc_32(crc_result , NAND_write_buffer[i] );	 
	}
 
 return (crc_result^0xffffffffL);
}

// =====================================================================
unsigned int NAND_checking_crc (unsigned int v_address, unsigned int  data_cnt)
{
unsigned int start_address, flag, page_cnt, i, read_flag  ;
unsigned int crc_saved1, crc_saved2, crc_result1, crc_result2;
char temp [50];

if ((v_address % 2048) != 0)
	{send_string3("Error in start address.\n");
	 return 0;
	}

start_address = NAND_virtual2real_address(v_address); 
page_cnt = (data_cnt >> 11) & (0x1ffff);

flag=1;
for(i=0; i< page_cnt+1; i++)
	{

	 // read a page
	 sprintf(temp,"page addr=%d \n",(start_address+(i<<12)) );
	 send_string3(temp);
	 read_flag = NAND_read_operation ((start_address+(i<<12)), 2100, NAND_data_output);
	 if (read_flag != 1)
	 	{
		 send_string3("Error in Reading data from nand.\n");
		 flag = 0;
		}
	 
	 //crc_result1= 0x00000000;
	 //crc_result2= 0x00000000;

	 crc_result1 = NAND_crc32_array(NAND_data_output,0,1023);
	 crc_result2 = NAND_crc32_array(NAND_data_output,1024,2047);

	 // calc input crc 
	 //crc_saved1 = 0x00000000;
	 crc_saved1 = NAND_data_output[2059] ;
	 crc_saved1 = (crc_saved1 << 8) + NAND_data_output[2058] ;
	 crc_saved1 = (crc_saved1 << 8) + NAND_data_output[2057] ;
	 crc_saved1 = (crc_saved1 << 8) + NAND_data_output[2056] ;

	 //crc_saved2 = 0x00000000;
	 crc_saved2 = NAND_data_output[2075] ;
	 crc_saved2 = (crc_saved2 << 8) + NAND_data_output[2074] ;
	 crc_saved2 = (crc_saved2 << 8) + NAND_data_output[2073] ;
	 crc_saved2 = (crc_saved2 << 8) + NAND_data_output[2072] ;

	//  check if error occure or not
	if( (crc_saved1 == crc_result1 ) && (crc_saved2 == crc_result2 ) )
		{
		 send_string3("Page CRC checked. \n");
		// sprintf(temp,"CRC for %d page is: %X, %X \n", (i+1), crc_result1, crc_result2 );
		// send_string3(temp);
		}
	else if ( (crc_saved1 == 0xFFFFFFFF) && (crc_saved1 == 0xFFFFFFFF) &&
			  (crc_result1== 0xB83AFFF4) && (crc_result2== 0xB83AFFF4) )
		{ send_string3("This page is blank. \n");
		}
	
	else if (( crc_saved1 == 0xFFFFFFFF) && ( crc_saved1 == 0xFFFFFFFF)  )
		{send_string3("This page has not any CRC bits. \n");
		 sprintf(temp,"%d--> crc_result =%X, %X \n", (i+1), crc_result1, crc_result2 );
		 send_string3(temp);
		 flag=0;
		}

	else // if (crc_result1 != crc_saved1  || crc_result2 != crc_saved2)
		{
		 send_string3("Error in CRC page \n");
		 sprintf(temp,"%d--> crc_saved =%X, %X \n", (i+1), crc_saved1, crc_saved2 );
		 send_string3(temp);
		 sprintf(temp,"%d--> crc_result=%X, %X \n", (i+1), crc_result1, crc_result2);
		 send_string3(temp);
		 flag=0;
		}
	
	}

send_string3("\n");
return flag;	
}


// =====================================================================

// this function map block address and run NAND_erase_operation
unsigned int NAND_erase_block (unsigned int v_block_address)
{
 unsigned int r_block_address, flag =0;

 r_block_address =  NAND_update_mapping_address_table (v_block_address, NAND_invalid_block_table);

 flag = NAND_erase_operation(r_block_address);
 if (!flag )
 	{
	 send_string3("Error! in erase operation.\n");
	 return 0;
	}
return 1;
}

// =====================================================================
// this function map block address and run NAND_erase_operation for N block
unsigned int NAND_erase_N_block(unsigned int v_block_address, unsigned int block_cnt)
{
 unsigned int i, r_block_address, flag =0;
 char temp [40];
 
 // i start from v_block_address 
 for (i=v_block_address; i< (v_block_address + block_cnt); i++)  
	{
	 r_block_address =  NAND_update_mapping_address_table (v_block_address, NAND_invalid_block_table);
 	 flag = NAND_erase_operation(r_block_address);
	 if (!flag )
	 	{
		 sprintf(temp, "Error! block %d did not erase.\n", i);
	 	 send_string3(temp);
	 	 //return 0;
		}
	}
 
 return 1;
}

// =====================================================================

// This function erase all block (4-2048)
unsigned int NAND_erase_all_block()
{
 unsigned int i, flag =0;
 char temp [40];
 
 // i start from 4 (the 4 firt block are reserve);   size must be 2048
 for (i=4; i< 2048; i++)  
	{
	 if (!NAND_invalid_block_table[i])
		{
		flag = NAND_erase_operation(i);
		if (!flag )
	 		{
			 sprintf(temp, "Error! block %d did not erase.\n", i);
		 	 send_string3(temp);
		 	 //return 0;
			}
	   }
	}
 
 return 1;
} 
// =====================================================================
unsigned int NAND_read_record (unsigned int start_addr, unsigned int  record_number,char*data)
{

volatile unsigned int flag, i, start_address;
unsigned long int calc_address=0;

calc_address=start_addr + ((record_number/21)*2048) + ((record_number%21)*96);				   //21 record can be added into each nand page
start_address = NAND_virtual2real_address(calc_address);
flag = NAND_read_operation (start_address, 96, data);
return 0;		
}
// =====================================================================


