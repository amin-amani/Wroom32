#include <RTL.h>
#include <stm32f4xx.h>

#ifndef __K9F2G08_H
#define __K9F2G08_H

#define FALSE 0
#define TRUE 1

extern char NAND_data_output [2112];
extern char NAND_data_input [2112];
extern char NAND_read_id_device[5];
extern char NAND_invalid_block_count;
extern BOOL NAND_invalid_block_table [2048];

extern char NAND_write_buffer [2112];
extern unsigned int NAND_data_count ;
extern unsigned int NAND_start_address ;
extern unsigned long int NAND_current_data_count;
extern unsigned int NAND_write_mode_index ;
extern BOOL NAND_read_mode_flag;
extern BOOL NAND_write_mode_flag ;
//extern unsigned int NAND_crc_input1;
//extern unsigned int NAND_crc_input2;  


// =====================================================================
// ===============   LEVEL 0 FUNCTION   ================================
// NAND_io_init;
// nand_set_data_direction;
// read_data_portc;
// write_data_portc;
// =====================================================================

void NAND_io_init(void);
void nand_set_data_direction (char direction);

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
unsigned int NAND_read_id_operation(char* NAND_read_id_device);
unsigned int NAND_command_latch_cycle(unsigned int cmd);
unsigned int NAND_address_latch_cycle(unsigned int adr);
unsigned int NAND_input_data_latch_cycle(char* NAND_data_input, unsigned int data_cnt);
unsigned int NAND_output_data_latch_cycle(char* NAND_data_output, unsigned int data_cnt);
unsigned int NAND_read_status_register_cycle(unsigned int cmd); 
unsigned int NAND_block_address_latch_cycle(unsigned int adr);

// ===============   LEVEL 2 FUNCTION   ================================
// NAND_reset_operation 
// NAND_program_operation
// NAND_read_operation;
// NAND_erase_operation;
// =====================================================================
unsigned int NAND_reset_operation(void);
unsigned int NAND_program_operation (unsigned int address, unsigned int data_cnt,char* NAND_data_input);
unsigned int NAND_read_operation (unsigned int address, unsigned int data_cnt, char* NAND_data_output);
unsigned int NAND_erase_operation (unsigned int block_address);

// =====================================================================
// ===============   LEVEL 3 FUNCTION   ================================
// NAND_erase_all_memory;
// =====================================================================
unsigned int NAND_erase_all_memory(void);
unsigned int NAND_identify_initial_invalid_blocks (unsigned int mode);
unsigned int NAND_initial_iib_mapping_table(void);
unsigned int NAND_store_iib_mapping_table(unsigned int invalid_block_id);
unsigned int NAND_update_mapping_address_table(unsigned int v_block_address, BOOL* NAND_invalid_block_table);
unsigned int NAND_virtual2real_address (unsigned int v_address);
unsigned int NAND_show_device_property (char* NAND_read_id_device);
void NAND_test_device_id(char *par);
unsigned int NAND_initial_reset_function(unsigned int mode);
unsigned int NAND_read_all (unsigned int v_address, unsigned int  data_cnt, char* NAND_data_output);
unsigned int NAND_write_all (unsigned int v_address, unsigned int  data_cnt, char* NAND_data_output, char first_page);
unsigned int NAND_write_crc (unsigned int v_address, unsigned int  data_cnt, char* NAND_data_output );
unsigned int NAND_crc32_array(char* NAND_write_buffer, unsigned int start_index,unsigned int end_index);
unsigned int NAND_checking_crc (unsigned int v_address, unsigned int  data_cnt);
unsigned int NAND_erase_block (unsigned int v_block_address);
unsigned int NAND_erase_N_block(unsigned int v_block_address, unsigned int block_cnt);
unsigned int NAND_erase_all_block(void);
unsigned int NAND_read_record (unsigned int start_addr, unsigned int  record_number,char*data);

#endif
