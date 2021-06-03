#define AD7124_8_CS_H GPIO_SetBits(GPIOC,GPIO_Pin_6)
#define AD7124_8_CS_L GPIO_ResetBits(GPIOC,GPIO_Pin_6)
#define AD7124_8_SCLK_H GPIO_SetBits(GPIOC,GPIO_Pin_8)
#define AD7124_8_SCLK_L GPIO_ResetBits(GPIOC,GPIO_Pin_8)
#define AD7124_8_DIN_H GPIO_SetBits(GPIOC,GPIO_Pin_7)
#define AD7124_8_DIN_L GPIO_ResetBits(GPIOC,GPIO_Pin_7)
#define AD7124_8_DOUT GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9)

void AD7124_8_GPIO_INIT(void); 
typedef struct
{

	float voltagevalue0;
	float voltagevalue1;
	float voltagevalue2;
	float voltagevalue3;
	

} AD7124_INFO;

AD7124_INFO voltage_Getvalue(void);






