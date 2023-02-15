#include "Del_Hal.h"

void Del_Init(void)
{
	printf("---Step Ctrl---\r\n");
	printf("--------Test---\r\n");
	delay(200);
	Led_Init();
	
	MT6816_Init();
	Calibration_Init();
	HW_Elec_Init();
	Motor_Control_Init();
	goal_c_debug = 100;
	goal_v_debug = 2;
	HAL_TIM_Base_Start_IT(&htim3);
	if(Key_B_IsPress)
	{
		printf("\r\nCalibration start...\r\n");
		Red_Led_On();
		encode_cali.trigger = 1;
	}
	else
	{
		Red_Led_Off();
		printf("\r\n Into main loop\r\n");
	}
}

void Del_Update(void)
{
	printf("p_e:%d  op:%d  oi:%d  od:%d  d_out:%d\r\n",dce.p_error,dce.op, dce.oi, dce.od,dce.out);
	Calibration_Loop_Callback();
	delay(100);
	
	if(Key_A_IsPress)
	{
		delay(10);
		if(Key_A_IsPress)
		{
			goal_debug += 25600;
			while(Key_A_IsPress);
		}
	}
	if(Key_B_IsPress)
	{
		delay(10);
		if(Key_B_IsPress)
		{
			goal_debug -= 25600;
			while(Key_B_IsPress);
		}
	}
	
}

int fputc(int ch,FILE *f)
{
	HAL_UART_Transmit(&huart1,(uint8_t *)&ch,1,0xFFFF);
	return (ch);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance == TIM3){
		MT6816_Get_AngleData();
		if(encode_cali.trigger)
		{
			Calibration_Interrupt_Callback();
		}
		else
		{
			Motor_Control_Callback();
		}
	}
}
