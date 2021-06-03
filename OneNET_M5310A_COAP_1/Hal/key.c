
#include "key.h"
#include "delay.h"
#include "beep.h"
#include "sht20.h"
#include "stdio.h"


/*
************************************************************
*	????:	Key_Init
*
*	????:	??IO???
*
*	????:	?
*
*	????:	?
*
*	??:		SW2-PD2		SW3-PC11	SW4-PC12	SW5-PC13	
*				??????		??????
************************************************************
*/
void Key_Init(void)
{

	GPIO_InitTypeDef gpioInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE);	//??GPIOC?GPIOD???
	
	gpioInitStructure.GPIO_Mode = GPIO_Mode_IPU;									//?????????
	gpioInitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;			//???Pin11?12?13
	gpioInitStructure.GPIO_Speed = GPIO_Speed_50MHz;								//???????
	GPIO_Init(GPIOC, &gpioInitStructure);											//???GPIOC
	

}

/*
************************************************************
*	????:	KeyScan
*
*	????:	??????
*
*	????:	GPIOX:?????GPIO?	NUM:?GPIO?????
*
*	????:	IO????
*
*	??:		
************************************************************
*/
_Bool KeyScan(GPIO_TypeDef* GPIOX, unsigned int NUM)
{
	
	if(GPIOX == GPIOC)
	{
		if(!GPIO_ReadInputDataBit(GPIOC, NUM))	//??  ??
		{
			return KEYDOWN;
		}
		else									//??  ??
		{
			return KEYUP;
		}
	}

		
	return KEYUP;								//????????
	
}

/*
************************************************************
*	????:	Keyboard
*
*	????:	??????
*
*	????:	
*
*	????:	IO????
*
*	??:		
************************************************************
*/
unsigned char Keyboard(void)
{
	
	if(KeyScan(GPIOC, KEY0) == KEYDOWN)
	{
		while(KeyScan(GPIOC, KEY0) != KEYUP);		//????
		return KEY0DOWN;
	}
	
	if(KeyScan(GPIOC, KEY1) == KEYDOWN)
	{
		while(KeyScan(GPIOC, KEY1) != KEYUP);		//????
		return KEY1DOWN;
	}
	
	if(KeyScan(GPIOC, KEY2) == KEYDOWN)
	{
		while(KeyScan(GPIOC, KEY2) != KEYUP);		//????
		return KEY2DOWN;
	}
	
	if(KeyScan(GPIOC, KEY3) == KEYDOWN)
	{
		while(KeyScan(GPIOC, KEY3) != KEYUP);		//????
		return KEY3DOWN;
	}
	
	return KEYNONE;
	
}

/*
************************************************************
*	????:	Key_test
*
*	????:	????
*
*	????:	?
*
*	????:	?
*
*	??:		SW0-PC0		SW1-PC1	SW2-PC2	SW3-PC3	
*				??????		??????
************************************************************
*/
void Key_test(void)
{
	char stat=1;
	    printf("�����ΰ��¼�1��4:\r\n");
  while(1){
    if(stat==5)
			break;
		switch(Keyboard())
		{
			case KEY0DOWN:
				printf("����1�Ѿ����£��밴�¼�2\r\n");
			  stat=2;
        break;
			case KEY1DOWN:
				printf("����2�Ѿ����£��밴�¼�3\r\n");
			  stat=3;
			  break;		
			case KEY2DOWN:
				printf("����3�Ѿ����£��밴�¼�4\r\n");
			  stat=4;
			  break;
			case KEY3DOWN:
  			printf("����4�Ѿ����£������������\r\n");
			  stat=5;
		  	break;
			default: break;
		}
	}	
}
