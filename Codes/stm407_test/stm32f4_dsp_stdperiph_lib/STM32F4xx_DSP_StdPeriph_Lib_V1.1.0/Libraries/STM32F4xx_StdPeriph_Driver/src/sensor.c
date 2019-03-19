#include "stm32f4xx.h"
#include "hal.h"
#include "sensor.h"

float byte_to_float(unsigned char *data,int start){
			
char *point;
float result;

point=(char*)(&result);

*point=data[start+3];
*(point+1)=data[start+2];
*(point+2)=data[start+1];
*(point+3)=data[start+0];
return result;
}
unsigned char  check_parity(unsigned char *data,int start){
 unsigned char result=255;
 int i;
            for ( i = start+0  ; i < start+24; i++)
            {
                result^=data[i];
            }
			return result;
}
char read_sensors(float *rx,float *ry, float*rz){
char data[80];
int i;

static char space=0;

if(space==0){space=1;}
else{space=0;return 0;}
cs2_hi();
for(i=0;i<75;i++)  {data[i]=platform_spi2_SendByte(0xaa);}


cs2_low();
if(check_parity(data,0)==data[24] && check_parity(data,25)==data[49]   ){
//sprintf(temp_main,"Rx=%1.2f Ry=%1.2f Rz=%1.2f Ax=%1.2f Ay=%1.2f Az=%1.2f ",byte_to_float(data,0),byte_to_float(data,8),byte_to_float(data,4),byte_to_float(data,12),byte_to_float(data,16),byte_to_float(data,20));


*rx=byte_to_float(data,0);
*ry=byte_to_float(data,8);
*rz=byte_to_float(data,4);

byte_to_float(data,12);
byte_to_float(data,16);
byte_to_float(data,20);
//USART_puts(USART3,temp_main);	 
//sprintf(temp_main,"Rx2=%1.2f Ry2=%1.2f Rz2=%1.2f Ax2=%1.2f Ay2=%1.2f Az2=%1.2f",byte_to_float(data,25),byte_to_float(data,33),byte_to_float(data,29),byte_to_float(data,37),byte_to_float(data,41),byte_to_float(data,45));
byte_to_float(data,25);
byte_to_float(data,33);
byte_to_float(data,29);
byte_to_float(data,37);
byte_to_float(data,41);
byte_to_float(data,45);

//USART_puts(USART3,temp_main);	
//sprintf(temp_main,"Rx3=%1.2f Ry3=%1.2f Rz3=%1.2f Ax3=%1.2f Ay3=%1.2f Az3=%1.2f\n",byte_to_float(data,50),byte_to_float(data,58),byte_to_float(data,54),byte_to_float(data,62),byte_to_float(data,66),byte_to_float(data,70));
byte_to_float(data,50);
byte_to_float(data,58);
byte_to_float(data,54);
byte_to_float(data,62);
byte_to_float(data,66);
byte_to_float(data,70);

//USART_puts(USART3,temp_main);
return 1;
}
 return 0;
}

//================================================================================================================================================
double m_dResultChValue[6];
double m_dDecouplingValue [6];
double m_dResultChValue1[6];
double m_dDecouplingValue1[6];
         double m_dDecouplingCoefficient [6][6]=
{


	{1033.78406318488,0,0,0,0,0},
	{0,1037.08620260516,0,0,0,0},
	{0,0,7022.47191011235,0,0,0},
	{0,0,0,92.2339051835454,0,0},
	{0,0,0,0,92.6698174404596,0},
	{0,0,0,0,0,60.819851599562},
};
char read_sensors2(_IMU *imu,_FORCE *ft1,_FORCE *ft2 ){
char data[80];
int i;

static char space=0;

if(space==0){space=1;}
else{space=0;return 0;}
cs2_hi();
for(i=0;i<50;i++)  {data[i]=platform_spi2_SendByte(0xFF);}


cs2_low();
if(check_parity(data,0)==data[24] && check_parity(data,25)==data[49]   )
{

imu->rx=byte_to_float(data,25);
imu->ry=byte_to_float(data,29);
imu->rz=byte_to_float(data,33);
imu->ax=byte_to_float(data,37);
imu->ay=byte_to_float(data,41);
imu->az=byte_to_float(data,45);
//sprintf(temp_main,"Rx=%1.2f Ry=%1.2f Rz=%1.2f Ax=%1.2f Ay=%1.2f Az=%1.2f ",byte_to_float(data,0),byte_to_float(data,8),byte_to_float(data,4),byte_to_float(data,12),byte_to_float(data,16),byte_to_float(data,20));
m_dResultChValue[0] = (double)(data[0] << 8 | data[1]);
m_dResultChValue[1] = (double)(data[2] << 8 | data[3]);
m_dResultChValue[2] = (double)(data[4] << 8 | data[5]);
m_dResultChValue[3] = (double)(data[6] << 8 | data[7]);
m_dResultChValue[4] = (double)(data[8] << 8 | data[9]);
m_dResultChValue[5] = (double)(data[10] << 8 | data[11]);

m_dResultChValue1[0] = (double)(data[12] << 8 | data[13]);
m_dResultChValue1[1] = (double)(data[14] << 8 | data[15]);
m_dResultChValue1[2] = (double)(data[16] << 8 | data[17]);
m_dResultChValue1[3] = (double)(data[18] << 8 | data[19]);
m_dResultChValue1[4] = (double)(data[20] << 8 | data[21]);
m_dResultChValue1[5] = (double)(data[22] << 8 | data[23]);

for ( i = 0; i < 6; i++)
{
    m_dResultChValue[i] = 1000 * ((m_dResultChValue[i] - 32761.000000) / (double)65535 * (double)5) / 123.542684 / 4.986603;
    m_dResultChValue1[i] = 1000 * ((m_dResultChValue1[i] - 32761.000000) / (double)65535 * (double)5) / 123.542684 / 4.986603;

}
 for ( i = 0x00; i < 6; i++)
{

    m_dDecouplingValue[i] = m_dResultChValue[0] * m_dDecouplingCoefficient[i][0] + m_dResultChValue[1] * m_dDecouplingCoefficient[i][1]
                            + m_dResultChValue[2] * m_dDecouplingCoefficient[i][2] + m_dResultChValue[3] * m_dDecouplingCoefficient[i][3]
                            + m_dResultChValue[4] * m_dDecouplingCoefficient[i][4] + m_dResultChValue[5] * m_dDecouplingCoefficient[i][5];



    m_dDecouplingValue1[i] = m_dResultChValue1[0] * m_dDecouplingCoefficient[i][0] + m_dResultChValue1[1] * m_dDecouplingCoefficient[i][1]
                            + m_dResultChValue1[2] * m_dDecouplingCoefficient[i][2] + m_dResultChValue1[3] * m_dDecouplingCoefficient[i][3]
                            + m_dResultChValue1[4] * m_dDecouplingCoefficient[i][4] + m_dResultChValue1[5] * m_dDecouplingCoefficient[i][5];

}



ft1->fx=m_dDecouplingValue[0];
ft1->fy=m_dDecouplingValue[1];
ft1->fz=m_dDecouplingValue[2];
ft1->tx=m_dDecouplingValue[3];
ft1->ty=m_dDecouplingValue[4];
ft1->tz=m_dDecouplingValue[5];

ft2->fx=m_dDecouplingValue1[0];
ft2->fy=m_dDecouplingValue1[1];
ft2->fz=m_dDecouplingValue1[2];
ft2->tx=m_dDecouplingValue1[3];
ft2->ty=m_dDecouplingValue1[4];
ft2->tz=m_dDecouplingValue1[5];


//USART_puts(USART3,temp_main);
return 1;
}
 return 0;
}

