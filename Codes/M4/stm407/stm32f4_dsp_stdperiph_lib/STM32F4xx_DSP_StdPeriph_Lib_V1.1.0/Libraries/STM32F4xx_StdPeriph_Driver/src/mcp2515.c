#include "stm32f4xx.h"
#include "hal.h"
#include "mcp2515.h"
#include <stdint.h>
//==================================================================================

u8 MCP2515_reg_write(u8 addr,u8 value ,u16 CS_ADR)
{
	cs1_config(CS_ADR);  

	platform_spi1_SendByte(CAN_WRITE);
	platform_spi1_SendByte(addr);
	platform_spi1_SendByte(value);	
	cs1_config(0x000f);

	return 1;
}
//==================================================================================

u8 MCP2515_reg_bit_modify(u8 addr,u8 mask,u8 value,u16 CS_ADR)
{
	cs1_config(CS_ADR);
	

	platform_spi1_SendByte(CAN_BIT_MODIFY);
	platform_spi1_SendByte(addr);
	platform_spi1_SendByte(mask);
	platform_spi1_SendByte(value);
	

	cs1_config(0x000f);
	return 1;
} 
//==================================================================================

 u8 MCP2515_reg_read(u8 addr,u16 CS_ADR)
{
	u8 data;
	cs1_config(CS_ADR); 


	platform_spi1_SendByte(CAN_READ);
	platform_spi1_SendByte(addr);
	data = platform_spi1_SendByte(0xff);
	

	cs1_config(0x000f);
	return data;
}
//=================================================================================
void MCP2515_CANReset(u16 CS_ADR)
{
	cs1_config(CS_ADR);
	platform_spi1_SendByte(CAN_RESET);	
	cs1_config(CS_ADR);
}
//============================================================================== 
void MCP2515_setrxb0filters(int Mask0, u16* pFlt0_1,u16 CS_ADR)
{
	MCP2515_reg_write(RXM0SIDH, Mask0 >> 3,CS_ADR);
	MCP2515_reg_write(RXM0SIDL, Mask0 << 5,CS_ADR);

	// Set two filters associated with RXB0	
	
	MCP2515_reg_write(RXF0SIDH, *pFlt0_1 >> 3,CS_ADR);
	MCP2515_reg_write(RXF0SIDL, *pFlt0_1 << 5,CS_ADR);
	
	pFlt0_1++;
	MCP2515_reg_write(RXF1SIDH, *pFlt0_1 >> 3,CS_ADR);
	MCP2515_reg_write(RXF1SIDL, *pFlt0_1 << 5,CS_ADR);
}
void MCP2515_setrxb1filters(int Mask1, u16* pFlt2_5,u16 CS_ADR)
{
	MCP2515_reg_write(RXM1SIDH, Mask1 >> 3,CS_ADR);
	MCP2515_reg_write(RXM1SIDL, Mask1 << 5,CS_ADR);

	// Set Four filters associated with RXB1	
	
	MCP2515_reg_write(RXF2SIDH, *pFlt2_5 >> 3,CS_ADR);
	MCP2515_reg_write(RXF2SIDL, *pFlt2_5 << 5,CS_ADR);
	
	pFlt2_5++;
	MCP2515_reg_write(RXF3SIDH, *pFlt2_5 >> 3,CS_ADR);
	MCP2515_reg_write(RXF3SIDL, *pFlt2_5 << 5,CS_ADR);
	
	pFlt2_5++;
	MCP2515_reg_write(RXF4SIDH, *pFlt2_5 >> 3,CS_ADR);
	MCP2515_reg_write(RXF4SIDL, *pFlt2_5 << 5,CS_ADR);

	pFlt2_5++;
	MCP2515_reg_write(RXF5SIDH, *pFlt2_5 >> 3,CS_ADR);
	MCP2515_reg_write(RXF5SIDL, *pFlt2_5 << 5,CS_ADR);
}
//=========================================================================================
void MCP2515_CANEnable(u8 baudrate,u16 CS_ADR)
{
switch ( baudrate ) 
	{
case baud_1M_bit:
  	MCP2515_reg_write(CNF1,0x00,CS_ADR);
	MCP2515_reg_write(CNF2,0xD0,CS_ADR);
	MCP2515_reg_write(CNF3,0x42,CS_ADR);
  break;
case baud_500k_bit:
  MCP2515_reg_write(CNF1,0x03,CS_ADR);
	MCP2515_reg_write(CNF2,0xF8,CS_ADR);
	MCP2515_reg_write(CNF3,0xC5,CS_ADR);
	break;
case baud_125k_bit:
  MCP2515_reg_write(CNF1,0x00,CS_ADR);
	MCP2515_reg_write(CNF2,0xF8,CS_ADR);
	MCP2515_reg_write(CNF3,0xC5,CS_ADR);
  break;
default:
  // Code
  break;
}
//		MCP2515_reg_write(TXRTSCTRL,0x3F);
	
	//Interrupt on RXB0 - CANINTE
	MCP2515_reg_write(CANINTE,0x20,CS_ADR); // Interrupts are on
	
	//Set NORMAL mode
	MCP2515_reg_write(CANCTRL,REQOP_NORMAL | CLKOUT_ENABLED,CS_ADR);

	MCP2515_reg_read(CANSTAT,CS_ADR); //dummy read to give 2515 time to switch to normal mode
	
	if( (MCP2515_reg_read(CANSTAT,CS_ADR) & 0xE0) != OPMODE_NORMAL)
	MCP2515_reg_write(CANCTRL,REQOP_NORMAL | CLKOUT_ENABLED,CS_ADR);
	MCP2515_reg_write(TXB0CTRL, 0x08,CS_ADR); //Start Transmission.

}
u8 MCP2515_CANSendMsg( u16 Identifier, u8* Msg, u8 MsgSize,u8 SPI_Type,u16 CS_ADR)
{
	int WaitCntr = 0;
	u16 XMIT_TIMEOUT = 10;
	u8 temp;
//	char temp_str[20];	
	
	
	// wait for TXB0 to get ready.  If not ready within XMIT_TIMEOUT ms,then return false
	while( MCP2515_reg_read(TXB0CTRL,CS_ADR) & 0x08 ) //TXREQ == 1
	{
		delay_us(20);
		if( WaitCntr++ >= XMIT_TIMEOUT )
		{
//		sprintf(temp_str,"-%d-",CS_ADR);
//		send_string(temp_str);
			//sendchar('*');
			return 0;
		}
	}

	MCP2515_reg_write(TXB0SIDH, (Identifier >> 3) & 0xFF,CS_ADR);    //Set TXB0 SIDH
	MCP2515_reg_write(TXB0SIDL, (Identifier << 5) & 0xE0,CS_ADR);    //Set TXB0 SIDL
	
//	if( MsgSize > 8 )
//		MsgSize = 8;
	
	MCP2515_reg_write(TXB0DLC, MsgSize,CS_ADR);  //Set DLC
	

	for( temp = 0; temp < MsgSize; temp++ )
		MCP2515_reg_write(TXB0D0+temp, Msg[temp],CS_ADR);
	
	MCP2515_reg_write(TXB0CTRL, 0x08,CS_ADR); //Start Transmission.
	
	return 1;
}
u8 MCP2515_ReadStatus(u16 CS_ADR)
{
	u8 data;
	cs1_config(CS_ADR);


		
	platform_spi1_SendByte(CAN_RD_STATUS);
	data = platform_spi1_SendByte(0XFF);		


	cs1_config(0x000f);
	return data;
}	
//=================================================================================
u8 MCP2515_CANReadMsg(u16* pIdentifier, u8* Msg, u8* pMsgSize,u16 CS_ADR)
{
	u8 temp;
	u8 loc, S1, S2;

	temp = MCP2515_ReadStatus(CS_ADR);
	
	if( (temp & 3) == 0 )	
		return 0;

	//enable CAN chip
	cs1_config(CS_ADR);
				
	loc = ( temp & 1 )?0x90:0x94;	//select start location based on the buffer available (RXB 0 or 1)


	platform_spi1_SendByte(loc);
	
	S1 = platform_spi1_SendByte(0XFF);	//RXBnSIDH
	S2 = platform_spi1_SendByte(0XFF);	//RXBnSIDL
		
	*pIdentifier = S1<<3 | S2>>5; //format the 11 bit identifier
	
	platform_spi1_SendByte(0XFF);	//EID8 - ignore
	platform_spi1_SendByte(0XFF);	//EID0 - ignore
	
	*pMsgSize = (platform_spi1_SendByte(0XFF) & 0x0F); //Data Length
	
	if(*pMsgSize>8)
		*pMsgSize = 8;
		
	for( temp = 0; temp < *pMsgSize; temp++ )
		Msg[temp] = platform_spi1_SendByte(0XFF);
		
	

	cs1_config(0x000f);
		
	
	return 1;
	
}
//================================================================================
void MCP2515_CAN_Setup(u8 baudrate,u16 CS_ADR)
{
u16 Flt[] = {0,0,0,0,0,0};

MCP2515_CANReset(CS_ADR);

//setup the filter to recieve all messages
MCP2515_setrxb0filters(0, Flt,CS_ADR);
MCP2515_setrxb1filters(0, &Flt[2],CS_ADR);
MCP2515_reg_write(TXRTSCTRL,0x01,CS_ADR); // Set TXRTS pins as transmit request PINs.
delay_ms(10);
MCP2515_reg_read(TXRTSCTRL,CS_ADR);
MCP2515_CANEnable(baudrate,CS_ADR);
}
//================================================================================
u8 MCP2515_CANSendMsg_check_data( u16 Identifier, u8* Msg, u8 MsgSize,u16 CS_ADR)
{
	int WaitCntr = 0;
	u16 XMIT_TIMEOUT = 10;
	u8 temp;

	

	// wait for TXB0 to get ready.  If not ready within XMIT_TIMEOUT ms,then return false
	while( MCP2515_reg_read(TXB0CTRL,CS_ADR) & 0x08 ) //TXREQ == 1
	{
		delay_us(20);
		if( WaitCntr++ >= XMIT_TIMEOUT )
		{
			
			return 0;
		}
	}
	MCP2515_reg_write(TXB0SIDH, (Identifier >> 3) & 0xFF,CS_ADR);    //Set TXB0 SIDH
	MCP2515_reg_write(TXB0SIDL, (Identifier << 5) & 0xE0,CS_ADR);    //Set TXB0 SIDL	
	MCP2515_reg_write(TXB0DLC, MsgSize,CS_ADR);  //Set DLC
	

	for( temp = 0; temp < MsgSize; temp++ )				 // writing data bytes on the tx buffer 
		MCP2515_reg_write(TXB0D0+temp, Msg[temp],CS_ADR);
	
	for( temp = 0; temp < MsgSize; temp++ )				 // checking correctness of writen data
	{
		if(MCP2515_reg_read(TXB0D0+temp,CS_ADR) != Msg[temp])		 // if the read bytes are not correct write again
		{
		MCP2515_reg_write(TXB0D0+temp, Msg[temp],CS_ADR);
		if(MCP2515_reg_read(TXB0D0+temp,CS_ADR) != Msg[temp])
			return 0;	
		}		
	}

	
	//MCP2515_reg_write(TXB0CTRL, 0x08,SPI_Type,CS_ADR); //Start Transmission.
	
	return 1;
}	
//================================================================================
u8 MCP2515_CANSendMsg_check_data1( u16 Identifier, u8* Msg, u8 MsgSize,u16 CS_ADR)
{
	int WaitCntr = 0;
	u16 XMIT_TIMEOUT = 10;
	u8 temp;

	

	// wait for TXB0 to get ready.  If not ready within XMIT_TIMEOUT ms,then return false
	while( MCP2515_reg_read(TXB0CTRL,CS_ADR) & 0x08 ) //TXREQ == 1
	{
		delay_us(20);
		if( WaitCntr++ >= XMIT_TIMEOUT )
		{
			
			return 0;
		}
	}
	MCP2515_reg_write(TXB0SIDH, (Identifier >> 3) & 0xFF,CS_ADR);    //Set TXB0 SIDH
	MCP2515_reg_write(TXB0SIDL, (Identifier << 5) & 0xE0,CS_ADR);    //Set TXB0 SIDL	
	MCP2515_reg_write(TXB0DLC, MsgSize,CS_ADR);  //Set DLC
	

	for( temp = 0; temp < MsgSize; temp++ )				 // writing data bytes on the tx buffer 
		MCP2515_reg_write(TXB0D0+temp, Msg[temp],CS_ADR);
	
	for( temp = 0; temp < MsgSize; temp++ )				 // checking correctness of writen data
	{
		if(MCP2515_reg_read(TXB0D0+temp,CS_ADR) != Msg[temp])		 // if the read bytes are not correct write again
		{
		MCP2515_reg_write(TXB0D0+temp, Msg[temp],CS_ADR);
		if(MCP2515_reg_read(TXB0D0+temp,CS_ADR) != Msg[temp])
			return 0;	
		}		
	}
		MCP2515_start_transmittion();
		while( MCP2515_reg_read(TXB0CTRL,CS_ADR) & 0x08 ) //TXREQ == 1
	{
		delay_us(20);
		if( WaitCntr++ >= XMIT_TIMEOUT )
		{
			
			return 0;
		}
	}
	//MCP2515_reg_write(TXB0CTRL, 0x08,SPI_Type,CS_ADR); //Start Transmission.
	
	return 1;
}	
//=================================================================================
void MCP2515_start_transmittion(void)
{
	int i;
	GPIO_ResetBits(GPIOB,0X0001<<2);
	for(i=0;i<10;i++);
	GPIO_SetBits(GPIOB,GPIO_Pin_2);
}
//==================================================================================
