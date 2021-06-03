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
#include "stm32f10x.h"

//LEDͷ�ļ�
#include "led.h"
#include "delay.h"




LED_STATUS ledStatus;


/*
************************************************************
*	�������ƣ�	Led_Init
*
*	�������ܣ�	LED��ʼ��
*
*	��ڲ�����	��
*
*	���ز�����	��
*
*	˵����		LED4-PC7	LED5-PC8	LED6-PA12	LED7-PC10
				�ߵ�ƽ�ص�		�͵�ƽ����
************************************************************
*/
void Led_Init(void)
{
	
	GPIO_InitTypeDef gpioInitStrcut;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);	//��GPIOA��GPIOC��ʱ��
	
	gpioInitStrcut.GPIO_Mode = GPIO_Mode_Out_PP;									//����Ϊ�������ģʽ
	gpioInitStrcut.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 |GPIO_Pin_6 |GPIO_Pin_7;					//��ʼ��Pin7��8��10
	gpioInitStrcut.GPIO_Speed = GPIO_Speed_50MHz;									//���ص����Ƶ��
	GPIO_Init(GPIOA, &gpioInitStrcut);												//��ʼ��GPIOC
	
	Led1_Set(LED_OFF);
    Led2_Set(LED_OFF);
    Led3_Set(LED_OFF);
    Led4_Set(LED_OFF);

}

/*
************************************************************
*	�������ƣ�	Led1_Set
*
*	�������ܣ�	LED1����
*
*	��ڲ�����	status��LED_ON-����	LED_OFF-�ص�
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void Led1_Set(LED_ENUM status)
{

	GPIO_WriteBit(GPIOA, GPIO_Pin_4, status != LED_ON ?Bit_RESET:Bit_SET);	//status���������LED_ON�򷵻�Bit_SET�����򷵻�Bit_RESET����ͬ
	ledStatus.Led1Sta = status;

}

/*
************************************************************
*	�������ƣ�	Led2_Set
*
*	�������ܣ�	LED2����
*
*	��ڲ�����	status��LED_ON-����	LED_OFF-�ص�
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void Led2_Set(LED_ENUM status)
{

	GPIO_WriteBit(GPIOA, GPIO_Pin_5, status != LED_ON ?Bit_RESET:Bit_SET);
	ledStatus.Led2Sta = status;

}

/*
************************************************************
*	�������ƣ�	Led3_Set
*
*	�������ܣ�	LED3����
*
*	��ڲ�����	status��LED_ON-����	LED_OFF-�ص�
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void Led3_Set(LED_ENUM status)
{

	GPIO_WriteBit(GPIOA, GPIO_Pin_6, status != LED_ON ?Bit_RESET:Bit_SET);
	ledStatus.Led3Sta = status;

}

/*
************************************************************
*	�������ƣ�	Led4_Set
*
*	�������ܣ�	LED4����
*
*	��ڲ�����	status��LED_ON-����	LED_OFF-�ص�
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void Led4_Set(LED_ENUM status)
{

	GPIO_WriteBit(GPIOA, GPIO_Pin_7, status != LED_ON ?Bit_RESET:Bit_SET);
	ledStatus.Led4Sta = status;

}

/*
************************************************************
*	�������ƣ�	Led_test
*
*	�������ܣ�	��ˮ��
*
*	��ڲ�����	��ʱʱ��
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void Led_test(int ms)
{

		Led1_Set(LED_OFF);mDelay(ms);Led1_Set(LED_ON);		//����LED4������ʱ500ms��Ȼ��Ϩ��LED4
		
		Led2_Set(LED_OFF);mDelay(ms);Led2_Set(LED_ON);		//����LED5������ʱ500ms��Ȼ��Ϩ��LED5
		
		Led3_Set(LED_OFF);mDelay(ms);Led3_Set(LED_ON);		//����LED6������ʱ500ms��Ȼ��Ϩ��LED6
		
		Led4_Set(LED_OFF);mDelay(ms);Led4_Set(LED_ON);		//����LED7������ʱ500ms��Ȼ��Ϩ��LED7

}
