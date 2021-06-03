/**
	************************************************************
	************************************************************
	************************************************************
	*	�ļ����� 	i2c.c
	*
	*	���ߣ� 		�ż���
	*
	*	���ڣ� 		2016-11-23
	*
	*	�汾�� 		V1.0
	*
	*	˵���� 		���IIC����IO��ʼ������д����
	*
	*	�޸ļ�¼��	
	************************************************************
	************************************************************
	************************************************************
**/
//��Ƭ��ͷ�ļ�
#include "stm32f10x.h"

//Ӳ������
#include "BH1750.h"
#include "delay.h"
#include <stdio.h>

float result_lx=0;
u8 BUF[2]={0};
u16 result=0;


IIC_1750_INFO iic1750Info;



/*
************************************************************
*	�������ƣ�	IIC_1750_SpeedCtl
*
*	�������ܣ�	���IIC�ٶȿ���
*
*	��ڲ�����	speed����ʱ����
*
*	���ز�����	��
*
*	˵����		��λ��΢��
************************************************************
*/
void IIC_1750_SpeedCtl(unsigned short speed)
{

	iic1750Info.speed = speed;

}

/*
************************************************************
*	�������ƣ�	IIC_1750_Init
*
*	�������ܣ�	���IIC����IO��ʼ��
*
*	��ڲ�����	��
*
*	���ز�����	��
*
*	˵����		ʹ�ÿ�©��ʽ���������Բ����л�IO�ڵ������������
************************************************************
*/
void IIC_1750_Init(void)
{

	GPIO_InitTypeDef gpioInitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//��GPIOB��ʱ��
	
	gpioInitStruct.GPIO_Mode = GPIO_Mode_Out_OD;			//��©����������ȥ�л�������뷽��
	gpioInitStruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
	gpioInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpioInitStruct);
	
	IIC_1750_SpeedCtl(2);										//IIC�ٶȿ��ƣ���λ��΢��
	
	SDA1750_H;													//����SDA�ߣ����ڿ���״̬
	SCL1750_H;													//����SCL�ߣ����ڿ���״̬

}

/*
************************************************************
*	�������ƣ�	IIC_1750_Start
*
*	�������ܣ�	���IIC��ʼ�ź�
*
*	��ڲ�����	��
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void IIC_1750_Start(void)
{
	
	SDA1750_H;						//����SDA��
	SCL1750_H;						//����SCL��
	uDelay(iic1750Info.speed);		//��ʱ���ٶȿ���
	
	SDA1750_L;						//��SCL��Ϊ��ʱ��SDA��һ���½��ش���ʼ�ź�
	uDelay(iic1750Info.speed);		//��ʱ���ٶȿ���
	SCL1750_L;						//ǯסSCL�ߣ��Ա㷢������

}

/*
************************************************************
*	�������ƣ�	IIC_1750_Stop
*
*	�������ܣ�	���IICֹͣ�ź�
*
*	��ڲ�����	��
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void IIC_1750_Stop(void)
{

	SDA1750_L;						//����SDA��
	SCL1750_L;						//����SCL��
	uDelay(iic1750Info.speed);		//��ʱ���ٶȿ���
	
	SCL1750_H;						//����SCL��
	SDA1750_H;						//����SDA�ߣ���SCL��Ϊ��ʱ��SDA��һ�������ش���ֹͣ�ź�
	uDelay(iic1750Info.speed);

}

/*
************************************************************
*	�������ƣ�	IIC_1750_WaitAck
*
*	�������ܣ�	���IIC�ȴ�Ӧ��
*
*	��ڲ�����	timeOut����ʱʱ��
*
*	���ز�����	��
*
*	˵����		��λ��΢��
************************************************************
*/
_Bool IIC_1750_WaitAck(unsigned int timeOut)
{
	
	
	SDA1750_H;uDelay(iic1750Info.speed);			//����SDA��
	SCL1750_H;uDelay(iic1750Info.speed);			//����SCL��
	
	while(SDA1750_R)							//�������SDA��Ϊ1����ȴ���Ӧ���ź�Ӧ��0
	{
		if(--timeOut)
		{
			printf("WaitAck TimeOut\r\n");

			IIC_1750_Stop();						//��ʱδ�յ�Ӧ����ֹͣ����
			
			return IIC_1750_Err;					//����ʧ��
		}
		
		uDelay(iic1750Info.speed);
	}
	
	SCL1750_L;									//����SCL�ߣ��Ա�����շ�����
	
	return IIC_1750_OK;							//���سɹ�
	
}

/*
************************************************************
*	�������ƣ�	IIC_1750_Ack
*
*	�������ܣ�	���IIC����һ��Ӧ��
*
*	��ڲ�����	��
*
*	���ز�����	��
*
*	˵����		��SDA��Ϊ��ʱ��SCL��һ�������������һ��Ӧ���ź�
************************************************************
*/
void IIC_1750_Ack(void)
{
	
	SCL1750_L;						//����SCL��
	SDA1750_L;						//����SDA��
	uDelay(iic1750Info.speed);
	SCL1750_H;						//����SCL��
	uDelay(iic1750Info.speed);
	SCL1750_L;						//����SCL��
	
}

/*
************************************************************
*	�������ƣ�	IIC_1750_NAck
*
*	�������ܣ�	���IIC����һ�Ǹ�Ӧ��
*
*	��ڲ�����	��
*
*	���ز�����	��
*
*	˵����		��SDA��Ϊ��ʱ��SCL��һ�������������һ����Ӧ���ź�
************************************************************
*/
void IIC_1750_NAck(void)
{
	
	SCL1750_L;						//����SCL��
	SDA1750_H;						//����SDA��
	uDelay(iic1750Info.speed);
	SCL1750_H;						//����SCL��
	uDelay(iic1750Info.speed);
	SCL1750_L;						//����SCL��
	
}

/*
************************************************************
*	�������ƣ�	IIC_1750_SendByte
*
*	�������ܣ�	���IIC����һ���ֽ�
*
*	��ڲ�����	byte����Ҫ���͵��ֽ�
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void IIC_1750_SendByte(unsigned char byte)
{

	unsigned char count = 0;
	
    SCL1750_L;							//����ʱ�ӿ�ʼ���ݴ���
	
    for(; count < 8; count++)		//ѭ��8�Σ�ÿ�η���һ��bit
    {
		if(byte & 0x80)				//�������λ
			SDA1750_H;
		else
			SDA1750_L;
		
		byte <<= 1;					//byte����1λ
		
		uDelay(iic1750Info.speed);
		SCL1750_H;
		uDelay(iic1750Info.speed);
		SCL1750_L;
    }

}

/*
************************************************************
*	�������ƣ�	IIC_1750_RecvByte
*
*	�������ܣ�	���IIC����һ���ֽ�
*
*	��ڲ�����	��
*
*	���ز�����	���յ����ֽ�����
*
*	˵����		
************************************************************
*/
unsigned char IIC_1750_RecvByte(void)
{
	
	unsigned char count = 0, receive = 0;
	
	SDA1750_H;							//����SDA�ߣ���©״̬�£����������Ա��ȡ����
	
    for(; count < 8; count++ )		//ѭ��8�Σ�ÿ�η���һ��bit
	{
		SCL1750_L;
		uDelay(iic1750Info.speed);
		SCL1750_H;
		
        receive <<= 1;				//����һλ
		
        if(SDA1750_R)					//���SDA��Ϊ1����receive����������ÿ���������Ƕ�bit0��+1��Ȼ����һ��ѭ����������һ��
			receive++;
		
		uDelay(iic1750Info.speed);
    }
	
    return receive;
	
}

/*
************************************************************
*	�������ƣ�	I2C_1750_WriteByte
*
*	�������ܣ�	���IICдһ������
*
*	��ڲ�����	slaveAddr���ӻ���ַ
*				regAddr���Ĵ�����ַ
*				byte����Ҫд�������
*
*	���ز�����	0-д��ɹ�	1-д��ʧ��
*
*	˵����		*byte�ǻ���д�����ݵı����ĵ�ַ����Ϊ��Щ�Ĵ���ֻ��Ҫ�����¼Ĵ�����������Ҫд��ֵ
************************************************************
*/
_Bool I2C_1750_WriteByte(unsigned char slaveAddr, unsigned char regAddr, unsigned char byte)
{

	unsigned char addr = 0;

	addr = slaveAddr << 1;		//IIC��ַ��7bit��������Ҫ����1λ��bit0��1-��	0-д
	
	IIC_1750_Start();				//��ʼ�ź�
	
	IIC_1750_SendByte(addr);			//�����豸��ַ(д)
	if(IIC_1750_WaitAck(5000))		//�ȴ�Ӧ��
		return IIC_1750_Err;
	
	IIC_1750_SendByte(regAddr);		//���ͼĴ�����ַ
	if(IIC_1750_WaitAck(5000))		//�ȴ�Ӧ��
		return IIC_1750_Err;
	
	if(byte)
	{
		IIC_1750_SendByte(byte);	//��������
		if(IIC_1750_WaitAck(5000))	//�ȴ�Ӧ��
			return IIC_1750_Err;
	}
	
	IIC_1750_Stop();					//ֹͣ�ź�
	
	return IIC_1750_OK;

}

/*
************************************************************
*	�������ƣ�	I2C_1750_ReadByte
*
*	�������ܣ�	���IIC��ȡһ���ֽ�
*
*	��ڲ�����	slaveAddr���ӻ���ַ
*				regAddr���Ĵ�����ַ
*				val����Ҫ��ȡ�����ݻ���
*
*	���ز�����	0-�ɹ�		1-ʧ��
*
*	˵����		val��һ����������ĵ�ַ
************************************************************
*/
_Bool I2C_1750_ReadByte(unsigned char slaveAddr, unsigned char regAddr, unsigned char *val)
{

	unsigned char addr = 0;

    addr = slaveAddr << 1;		//IIC��ַ��7bit��������Ҫ����1λ��bit0��1-��	0-д
	
	IIC_1750_Start();				//��ʼ�ź�
	
	IIC_1750_SendByte(addr);			//�����豸��ַ(д)
	if(IIC_1750_WaitAck(5000))		//�ȴ�Ӧ��
		return IIC_1750_Err;
	
	IIC_1750_SendByte(regAddr);		//���ͼĴ�����ַ
	if(IIC_1750_WaitAck(5000))		//�ȴ�Ӧ��
		return IIC_1750_Err;
	
	IIC_1750_Start();				//�����ź�
	
	IIC_1750_SendByte(addr + 1);		//�����豸��ַ(��)
	if(IIC_1750_WaitAck(5000))		//�ȴ�Ӧ��
		return IIC_1750_Err;
	
	*val = IIC_1750_RecvByte();		//����
	IIC_1750_NAck();					//����һ����Ӧ���źţ������ȡ����
	
	IIC_1750_Stop();					//ֹͣ�ź�
	
	return IIC_1750_OK;

}

/*
************************************************************
*	�������ƣ�	I2C_1750_WriteBytes
*
*	�������ܣ�	���IICд�������
*
*	��ڲ�����	slaveAddr���ӻ���ַ
*				regAddr���Ĵ�����ַ
*				buf����Ҫд������ݻ���
*				num�����ݳ���
*
*	���ز�����	0-д��ɹ�	1-д��ʧ��
*
*	˵����		*buf��һ�������ָ��һ����������ָ��
************************************************************
*/
_Bool I2C_1750_WriteBytes(unsigned char slaveAddr, unsigned char regAddr, unsigned char *buf, unsigned char num)
{

	unsigned char addr = 0;

	addr = slaveAddr << 1;		//IIC��ַ��7bit��������Ҫ����1λ��bit0��1-��	0-д
	
	IIC_1750_Start();				//��ʼ�ź�
	
	IIC_1750_SendByte(addr);			//�����豸��ַ(д)
	if(IIC_1750_WaitAck(5000))		//�ȴ�Ӧ��
		return IIC_1750_Err;
	
	IIC_1750_SendByte(regAddr);		//���ͼĴ�����ַ
	if(IIC_1750_WaitAck(5000))		//�ȴ�Ӧ��
		return IIC_1750_Err;
	
	while(num--)				//ѭ��д������
	{
		IIC_1750_SendByte(*buf);		//��������
		if(IIC_1750_WaitAck(5000))	//�ȴ�Ӧ��
			return IIC_1750_Err;
		
		buf++;					//����ָ��ƫ�Ƶ���һ��
		
		uDelay(10);
	}
	
	IIC_1750_Stop();					//ֹͣ�ź�
	
	return IIC_1750_OK;

}

/*
************************************************************
*	�������ƣ�	I2C_1750_ReadBytes
*
*	�������ܣ�	���IIC���������
*
*	��ڲ�����	slaveAddr���ӻ���ַ
*				regAddr���Ĵ�����ַ
*				buf����Ҫ��ȡ�����ݻ���
*				num�����ݳ���
*
*	���ز�����	0-д��ɹ�	1-д��ʧ��
*
*	˵����		*buf��һ�������ָ��һ����������ָ��
************************************************************
*/
_Bool I2C_1750_ReadBytes(unsigned char slaveAddr, unsigned char regAddr, unsigned char *buf, unsigned char num)
{

	unsigned short addr = 0;

    addr = slaveAddr << 1;		//IIC��ַ��7bit��������Ҫ����1λ��bit0��1-��	0-д
	
	IIC_1750_Start();				//��ʼ�ź�
	
	IIC_1750_SendByte(addr);			//�����豸��ַ(д)
	if(IIC_1750_WaitAck(5000))		//�ȴ�Ӧ��
		return IIC_1750_Err;
	
	IIC_1750_SendByte(regAddr);		//���ͼĴ�����ַ
	if(IIC_1750_WaitAck(5000))		//�ȴ�Ӧ��
		return IIC_1750_Err;
	
	IIC_1750_Start();				//�����ź�
	
	IIC_1750_SendByte(addr + 1);		//�����豸��ַ(��)
	if(IIC_1750_WaitAck(5000))		//�ȴ�Ӧ��
		return IIC_1750_Err;
	
	while(num--)
	{
		*buf = IIC_1750_RecvByte();
		buf++;					//ƫ�Ƶ���һ�����ݴ洢��ַ
		
		if(num == 0)
        {
           IIC_1750_NAck();			//���һ��������Ҫ��NOACK
        }
        else
        {
          IIC_1750_Ack();			//��ӦACK
		}
	}
	
	IIC_1750_Stop();
	
	return IIC_1750_OK;

}


_Bool Cmd_Write_BH1750(unsigned int cmd)
{
    IIC_1750_Start();                  //��ʼ�ź�
    IIC_1750_SendByte(BH1750_Addr+0);   //�����豸��ַ+д�ź�
	if(IIC_1750_WaitAck(5000))		//�ȴ�Ӧ��
	return IIC_1750_Err;
    IIC_1750_SendByte(cmd);    //�ڲ��Ĵ�����ַ
	if(IIC_1750_WaitAck(5000))		//�ȴ�Ӧ��
	return IIC_1750_Err;
    //BH1750_SendByte(REG_data);       //�ڲ��Ĵ������ݣ�
    IIC_1750_Stop();                   //����ֹͣ�ź�
	uDelay(5000);
	return IIC_1750_OK;

}
void Start_BH1750(void)
{
	Cmd_Write_BH1750(BH1750_ON);	 //power on
	Cmd_Write_BH1750(BH1750_RSET);	//clear
	Cmd_Write_BH1750(BH1750_ONE);  //һ��H�ֱ���ģʽ������120ms��֮���Զ��ϵ�ģʽ  
}
_Bool Read_BH1750(void)
{   	
    IIC_1750_Start();                          //��ʼ�ź�
    IIC_1750_SendByte(BH1750_Addr+1);         //�����豸��ַ+���ź�
	if(IIC_1750_WaitAck(5000))		//�ȴ�Ӧ��
	return IIC_1750_Err;
	BUF[0]=IIC_1750_RecvByte();  //����ACK
	IIC_1750_Ack();			//��ӦACK
	BUF[1]=IIC_1750_RecvByte();  //����NACK
    IIC_1750_NAck();			//���һ��������Ҫ��NOACK
    IIC_1750_Stop();                          //ֹͣ�ź�
    uDelay(5000);
	return IIC_1750_OK;
}
void Convert_BH1750(void)
{
	
	result=BUF[0];
	result=(result<<8)+BUF[1];  //�ϳ����ݣ�����������
	
	result_lx=(float)result/1.2;
#ifdef HARDWARE_TEST	
	printf("BH1750 DATA is: %f LUX\r\n",result_lx);
#endif
}

void BH1750_test(void)
{
  Start_BH1750();
	mDelay(200);
	Read_BH1750();
	Convert_BH1750();
	
}



