#include "Del_Hal.h"

void Del_Init(void)
{
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_15,GPIO_PIN_RESET);
	
	MT6816_Init();
}

void Del_Update(void)
{
	MT6816_Get_AngleData();
	printf("Angle:%d\r\n",mt6816.angle_data);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_14,GPIO_PIN_SET);
	HAL_Delay(200);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_14,GPIO_PIN_RESET);
	HAL_Delay(200);
}

int fputc(int ch,FILE *f)
{
	HAL_UART_Transmit(&huart1,(uint8_t *)&ch,1,0xFFFF);
	return (ch);
}
