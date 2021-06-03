/**
	************************************************************
	************************************************************
	************************************************************
	*	�ļ����� 	led.c
	*
	*	���ߣ� 		�ż���
	*
	*	���ڣ� 		2016-11-23
	*
	*	�汾�� 		V1.0
	*
	*	˵���� 		LED��ʼ��������LED
	*
	*	�޸ļ�¼��	
	************************************************************
	************************************************************
	************************************************************
**/

//��Ƭ��ͷ�ļ�
#include <stdio.h>
#include "stm32f10x.h"

//LEDͷ�ļ�
#include "BMP180.h"
#include "delay.h"
#include "BH1750.h"
#include "math.h"



_bmp180 bmp180;


//��BMP180��һ��16λ������
short BMP_ReadTwoByte(uint8_t ReadAddr)
{
	short data;
	uint8_t msb,lsb;
	
	IIC_1750_Start();
	
	IIC_1750_SendByte(0xEE);
	if(IIC_1750_WaitAck(5000))		//�ȴ�Ӧ��
	return IIC_1750_Err;
	
	IIC_1750_SendByte(ReadAddr);
	if(IIC_1750_WaitAck(5000))		//�ȴ�Ӧ��
	return IIC_1750_Err;
	
	IIC_1750_Start();
	
	IIC_1750_SendByte(0xEF);
	if(IIC_1750_WaitAck(5000))		//�ȴ�Ӧ��
	return IIC_1750_Err;
	
	msb = IIC_1750_RecvByte();
	IIC_1750_Ack();			//��ӦACK
	lsb = IIC_1750_RecvByte();
	IIC_1750_NAck();			//���һ��������Ҫ��NOACK
	IIC_1750_Stop();
	
	data = msb*256 + lsb;
	
	return data;
}

//��BMP180�Ļ�ȡ�������
void BMP_ReadCalibrationData(void)
{
	bmp180.AC1 = BMP_ReadTwoByte(0xAA);
	bmp180.AC2 = BMP_ReadTwoByte(0xAC);
	bmp180.AC3 = BMP_ReadTwoByte(0xAE);
	bmp180.AC4 = BMP_ReadTwoByte(0xB0);
	bmp180.AC5 = BMP_ReadTwoByte(0xB2);
	bmp180.AC6 = BMP_ReadTwoByte(0xB4);
	bmp180.B1  = BMP_ReadTwoByte(0xB6);
	bmp180.B2  = BMP_ReadTwoByte(0xB8);
	bmp180.MB  = BMP_ReadTwoByte(0xBA);
	bmp180.MC  = BMP_ReadTwoByte(0xBC);
	bmp180.MD  = BMP_ReadTwoByte(0xBE);
}

//��BMP180��ȡδ�������¶�
long BMP_Read_UT(void)
{
	long temp = 0;
	//BMP_WriteOneByte(0xF4,0x2E);
	I2C_1750_WriteByte(0X77, 0xF4, 0x2E);
	
	mDelay(5);
	temp = (long)BMP_ReadTwoByte(0xF6);
	return temp;
}

//��BMP180��ȡδ�����Ĵ���ѹ
long BMP_Read_UP(void)
{
	long pressure = 0;
	
	//BMP_WriteOneByte(0xF4,0x34);
	I2C_1750_WriteByte(0X77, 0xF4, 0x34);
	mDelay(5);
	
	pressure = (long)BMP_ReadTwoByte(0xF6);
	//pressure = pressure + BMP_ReadOneByte(0xf8);
	pressure &= 0x0000FFFF;
	
	return pressure;
}

//�û�ȡ�Ĳ������¶Ⱥʹ���ѹ���������������㺣��
void BMP_UncompemstatedToTrue(void)
{
	bmp180.UT = BMP_Read_UT();//��һ�ζ�ȡ����
	bmp180.UT = BMP_Read_UT();//���еڶ��ζ�ȡ��������
	bmp180.UP = BMP_Read_UP();
	
	bmp180.X1 = ((bmp180.UT - bmp180.AC6) * bmp180.AC5) >> 15;
	bmp180.X2 = (((long)bmp180.MC) << 11) / (bmp180.X1 + bmp180.MD);
	bmp180.B5 = bmp180.X1 + bmp180.X2;
	bmp180.Temp  = (bmp180.B5 + 8) >> 4;
	
	bmp180.B6 = bmp180.B5 - 4000;
	bmp180.X1 = ((long)bmp180.B2 * (bmp180.B6 * bmp180.B6 >> 12)) >> 11;
	bmp180.X2 = ((long)bmp180.AC2) * bmp180.B6 >> 11;
	bmp180.X3 = bmp180.X1 + bmp180.X2;
	
	bmp180.B3 = ((((long)bmp180.AC1) * 4 + bmp180.X3) + 2) /4;
	bmp180.X1 = ((long)bmp180.AC3) * bmp180.B6 >> 13;
	bmp180.X2 = (((long)bmp180.B1) *(bmp180.B6*bmp180.B6 >> 12)) >>16;
	bmp180.X3 = ((bmp180.X1 + bmp180.X2) + 2) >> 2;
	bmp180.B4 = ((long)bmp180.AC4) * (unsigned long)(bmp180.X3 + 32768) >> 15;
	bmp180.B7 = ((unsigned long)bmp180.UP - bmp180.B3) * 50000;
	
	if(bmp180.B7 < 0x80000000)
	{
		bmp180.p = (bmp180.B7 * 2) / bmp180.B4;		
	}
	else
	{
		bmp180.p = (bmp180.B7 / bmp180.B4) * 2;
	}
	
	bmp180.X1 = (bmp180.p >> 8) * (bmp180.p >>8);
	bmp180.X1 = (((long)bmp180.X1) * 3038) >> 16;
	bmp180.X2 = (-7357 * bmp180.p) >> 16;
	
	bmp180.p = bmp180.p + ((bmp180.X1 + bmp180.X2 + 3791) >> 4);
	
	bmp180.altitude = 44330 * (1-pow(((bmp180.p) / 101325.0),(1.0/5.255)));  
}

void BMP180_test(void)
	
{
   unsigned char ID ;
   
   I2C_1750_ReadByte( 0x77 ,0xD0,&ID);
	 BMP_ReadCalibrationData();
   BMP_UncompemstatedToTrue();
#ifdef HARDWARE_TEST
   printf("BMP180_ID = 0X%X\t  BMP180_temp = %d.%dC\t   BMP180_Pressure = %ldPa\t   BMP180_Altitude = %.5fm\r\n",ID,bmp180.Temp/10,bmp180.Temp%10,bmp180.p,bmp180.altitude);
#endif
}




