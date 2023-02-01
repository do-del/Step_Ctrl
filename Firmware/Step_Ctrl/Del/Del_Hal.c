#include "Del_Hal.h"

void Del_Init(void)
{
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_15,GPIO_PIN_RESET);
}

void Del_Update(void)
{
	printf("hello world\r\n");
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_14,GPIO_PIN_SET);
	HAL_Delay(1000);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_14,GPIO_PIN_RESET);
	HAL_Delay(1000);
}

int fputc(int ch,FILE *f)
{
	HAL_UART_Transmit(&huart1,(uint8_t *)&ch,1,0xFFFF);
	return (ch);
}
