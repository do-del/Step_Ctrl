#ifndef AT8236_H
#define AT8236_H

#include "stm32f1xx_hal.h"
#include "tim.h"

#define D_ABS(x) (x>=0 ? x:-x)

//HW_ELEC
#define HW_ELEC_AP_GPIO_Port			(GPIOA)
#define HW_ELEC_AP_Pin					(GPIO_PIN_5)

#define HW_ELEC_AM_GPIO_Port			(GPIOA)
#define HW_ELEC_AM_Pin					(GPIO_PIN_4)

#define HW_ELEC_BP_GPIO_Port			(GPIOA)
#define HW_ELEC_BP_Pin					(GPIO_PIN_3)

#define HW_ELEC_BM_GPIO_Port			(GPIOA)
#define HW_ELEC_BM_Pin					(GPIO_PIN_2)

//TIM
#define HW_ELEC_APWM_GPIO_Port			(GPIOB)
#define HW_ELEC_APWM_Pin				(GPIO_PIN_11)
#define HW_ELEC_APWM_CHANNEL			(TIM_CHANNEL_4)

#define HW_ELEC_BPWM_GPIO_Port			(GPIOB)
#define HW_ELEC_BPWM_Pin				(GPIO_PIN_10)
#define HW_ELEC_BPWM_CHANNEL			(TIM_CHANNEL_3)

#define HW_ELEC_PWM_Get_TIM				(TIM2)
#define HW_ELEC_PWM_Get_HTIM			(htim2)

//GPIO输出
#define Out_AP_H()		(HW_ELEC_AP_GPIO_Port -> BSRR = HW_ELEC_AP_Pin)
#define Out_AP_L()		(HW_ELEC_AP_GPIO_Port -> BRR  = HW_ELEC_AP_Pin)
#define Out_AM_H()		(HW_ELEC_AM_GPIO_Port -> BSRR = HW_ELEC_AM_Pin)
#define Out_AM_L()		(HW_ELEC_AM_GPIO_Port -> BRR  = HW_ELEC_AM_Pin)
#define Out_BP_H()		(HW_ELEC_BP_GPIO_Port -> BSRR = HW_ELEC_BP_Pin)
#define Out_BP_L()		(HW_ELEC_BP_GPIO_Port -> BRR  = HW_ELEC_BP_Pin)
#define Out_BM_H()		(HW_ELEC_BM_GPIO_Port -> BSRR = HW_ELEC_BM_Pin)
#define Out_BM_L()		(HW_ELEC_BM_GPIO_Port -> BRR  = HW_ELEC_BM_Pin)
//TIM输出
#define Out_PWMtoDAC_A(value) (__HAL_TIM_SET_COMPARE(&HW_ELEC_PWM_Get_HTIM, HW_ELEC_APWM_CHANNEL, value))
#define Out_PWMtoDAC_B(value) (__HAL_TIM_SET_COMPARE(&HW_ELEC_PWM_Get_HTIM, HW_ELEC_BPWM_CHANNEL, value))

/**
  * 线圈定义
**/
typedef struct{
	uint16_t conver;		//SIN数组指针
	int16_t  sin_data;	//SIN换算数值
	uint16_t dac_reg;		//12位DAC数值
}Coil_Typedef;

extern Coil_Typedef		coil_a;	//电流控制
extern Coil_Typedef		coil_b;	//电流控制

void HW_Elec_Init(void);																				//硬件电流控制初始化
void HW_Elec_SetSleep(void);																		//硬件电流设置驱动睡眠
void HW_Elec_SetBrake(void);																		//硬件电流设置驱动刹车
void HW_Elec_SetDivideElec(uint32_t divide, int32_t elec_ma);	//硬件电流设置输出细分电流

#endif
