void reset_all_nodes(void);
//=================================================================================
void reset_comunications(int node);
//=================================================================================
void active_ipm_all(void);
//=================================================================================
void clear_fifo(int node);
//=================================================================================

void add_to_fifo( long int position, long int velocity, int time);
//=================================================================================
void start_ipm_trajectory(void);
//=================================================================================
char epos_get_mode(int id,int *mode,int cs_address);
//=================================================================================

char   epos_get_pos(int id,int*pos,int cs_address);
char epos_sdo_write_command_all( int id, int input,u16 index,unsigned char sub_index,unsigned char len);
//=================================================================================
void epos_set_profilepos(int id,char cs_addr);

