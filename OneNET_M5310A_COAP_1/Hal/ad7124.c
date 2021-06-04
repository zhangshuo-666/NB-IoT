#include"stm32f10x_conf.h"
#include "ad7124.h"
#include <stdio.h>
/*
this is ad7124
v1.0
*/




//void RCC_Configuration(void); 
//void GPIO_Configuration(void);
//void NVIC_Configuration(void); 

void Delay(vu32 nCount);
void AD7124_8_Reset(void);
void LED_TEST(void);//AD7124_8? PSW ????LED,??LED??
void read_reg(void);//??????????????,????????conf_reg[3]?mode_reg[3]?
void AD7124_8_INIT(void);
void AD7124_8_INIT1(void);//?????
void AD7124_8_Write(uint8_t Data);
uint32_t AD7124_8_Read(void);//??????,??uint32_t ????
uint8_t AD7124_8_Read_Byte(void);//??????
uint8_t AD7124_8_ID;//????0x12?0x14;




AD7124_INFO voltage_Getvalue(void)
{
	uint32_t Data,i;
  float data_temp,Vol;
  float VolM[4];
  uint8_t DATA_STATUS;
	
	AD7124_INFO ad;

		
	for(i=0;i<4;i++)
	{
		while(AD7124_8_DOUT);
		Data=AD7124_8_Read();
		DATA_STATUS=AD7124_8_Read_Byte()&0x0F;
		printf("%x\r\n",DATA_STATUS);
		data_temp=Data;
		VolM[DATA_STATUS]=(data_temp-0x800000)/0x800000*2.5;
		printf("%x %d %f\n",Data,Data,VolM[DATA_STATUS]);
	}		

	ad.voltagevalue0=VolM[0];
	ad.voltagevalue1=VolM[1];
	ad.voltagevalue2=VolM[2];
	ad.voltagevalue3=VolM[3];
	return ad;
}

	



void AD7124_8_GPIO_INIT(void)
{
  GPIO_InitTypeDef GPIO_InitStructure; 
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOC, &GPIO_InitStructure); 

  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
  GPIO_Init(GPIOC, &GPIO_InitStructure);     
}

void AD7124_8_INIT(void)//?????
{
  AD7124_8_CS_L;
  Delay(100);
  AD7124_8_Write(0x09);//???????0  CHANNEL_0  (datasheet Page 86)
  AD7124_8_Write(0x80);//????,??????????,???????;??CONFIG_0???????ADC???AIN0?AIN1
  AD7124_8_Write(0x01);

  AD7124_8_Write(0x19);//???????0 CONFIG_0;(datasheet Page 88)
  AD7124_8_Write(0x09);//??????????????????REF BUF?AIN BUF,???????,?????+-2.5V;
  AD7124_8_Write(0xf0);
  
  AD7124_8_Write(0x21);//??????????,FILTER_0 (datasheet Page 89)
  AD7124_8_Write(0x06);//??SINC4 ???
  AD7124_8_Write(0x07);
  AD7124_8_Write(0x80);//FS=128,???????? = 614.4K/(32*128)= 150Hz
  
  AD7124_8_Write(0x01);//??ADC?????,ADC_CONTROL (datasheet Page 78)
  AD7124_8_Write(0x09);//??????,??????,?????,????614.4kHz???,???????
  AD7124_8_Write(0xC0);
}
void AD7124_8_INIT1(void)//?????
{
  AD7124_8_CS_L;
  Delay(100);
  AD7124_8_Write(0x09);//???????0  CHANNEL_0  (datasheet Page 86)
  AD7124_8_Write(0x80);//????,??????????,???????;??CONFIG_0???????ADC???AIN0?AIN1
  AD7124_8_Write(0x01);
  AD7124_8_Write(0x0A);//???????1  CHANNEL_1 
  AD7124_8_Write(0x80);//????,??????????,???????;??CONFIG_1???????ADC???AIN2?AIN3
  AD7124_8_Write(0x43);
  AD7124_8_Write(0x0B);//???????2  CHANNEL_2 
  AD7124_8_Write(0x80);//????,??????????,???????;??CONFIG_2???????ADC???AIN4?AIN5
  AD7124_8_Write(0x85);
  AD7124_8_Write(0x0C);//???????3  CHANNEL_3
  AD7124_8_Write(0x80);
  AD7124_8_Write(0xC7);



  
  AD7124_8_Write(0x19);//???????0 CONFIG_0;(datasheet Page 88)
  AD7124_8_Write(0x09);//??????????????????REF BUF?AIN BUF,???????,?????+-2.5V;
  AD7124_8_Write(0xf0);
  
  AD7124_8_Write(0x21);//??????????,FILTER_0 (datasheet Page 89)
  AD7124_8_Write(0x06);
  AD7124_8_Write(0x00);
  AD7124_8_Write(0x80);//FS=128,?????????,????????????,???????????
  
  AD7124_8_Write(0x01);//??ADC?????,ADC_CONTROL (datasheet Page 78)
  AD7124_8_Write(0x0D);//??????,??????,?????,????614.4kHz???,??????,?? DATA_STATUS????????,????????
  AD7124_8_Write(0xC0);
}
void LED_TEST(void)//AD7124_8? PSW ????LED,??LED??
{
  AD7124_8_CS_L;
  Delay(40);
  AD7124_8_Write(0x03);//?? IO_CONTROL_1???,????0x000000
  AD7124_8_Write(0x00);//??3?? IO_CONTROL_1????
  AD7124_8_Write(0x80);
  AD7124_8_Write(0x00);
  AD7124_8_CS_H;
  Delay(4000000);//?0.5S??
  AD7124_8_CS_L;
  Delay(40);
  AD7124_8_Write(0x03);
  AD7124_8_Write(0x00);
  AD7124_8_Write(0x00);
  AD7124_8_Write(0x00);
  AD7124_8_CS_H;
  Delay(4000000);
}


void AD7124_8_Write(uint8_t Data)
{
  uint8_t data;
  uint8_t i;
  data=Data;
  for(i=0;i<8;i++)
  {
    AD7124_8_SCLK_L;
    if(data&0x80)
    AD7124_8_DIN_H;
    else
    AD7124_8_DIN_L;
    Delay(100);
    AD7124_8_SCLK_H;
    Delay(100);
    data<<=1;
  }
}


uint32_t AD7124_8_Read(void)
{
  uint32_t DATA;
  uint8_t i;
  DATA=0;
  for(i=0;i<24;i++)
  {
    DATA<<=1;
    AD7124_8_SCLK_L;
    Delay(50);
    if(AD7124_8_DOUT)
    DATA++;
    AD7124_8_SCLK_H;
    Delay(50);
  }
  Delay(100);
  return DATA;
}


uint8_t AD7124_8_Read_Byte(void)
{
  uint8_t DATA;
  uint8_t i;
  DATA=0;
  for(i=0;i<8;i++)
  {
    DATA<<=1;
    AD7124_8_SCLK_L;
    Delay(100);
    if(AD7124_8_DOUT)
    DATA++;
    AD7124_8_SCLK_H;
    Delay(100);
  }
  Delay(100);
  return DATA;
}



void AD7124_8_Reset(void)
{
  uint8_t a;
  AD7124_8_CS_L;
  AD7124_8_DIN_H;
  for(a=0;a<65;a++)//????64????,??AD7124-8
  {
    AD7124_8_SCLK_L;
    Delay(40);
    AD7124_8_SCLK_H;
    Delay(40);
  }
  AD7124_8_CS_H;
}



void Delay(vu32 nCount) 
{ 
  for(; nCount != 0; nCount--); 
} 


#ifdef  DEBUG 
void assert_failed(u8* file, u32 line) 
{  
  while (1) { } 
} 
#endif 





