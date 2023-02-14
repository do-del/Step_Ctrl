#ifndef __HW_ELEC_H
#define __HW_ELEC_H

#include "stm32f1xx_hal.h"
#include "tim.h"

//HW_ELEC
#define HW_ELEC_AP_GPIO_CLK_ENABLE()	__HAL_RCC_GPIOA_CLK_ENABLE()
#define HW_ELEC_AP_GPIO_Port			(GPIOA)
#define HW_ELEC_AP_Pin					(GPIO_PIN_5)
#define HW_ELEC_AM_GPIO_CLK_ENABLE()	__HAL_RCC_GPIOA_CLK_ENABLE()
#define HW_ELEC_AM_GPIO_Port			(GPIOA)
#define HW_ELEC_AM_Pin					(GPIO_PIN_4)
#define HW_ELEC_BP_GPIO_CLK_ENABLE()	__HAL_RCC_GPIOA_CLK_ENABLE()
#define HW_ELEC_BP_GPIO_Port			(GPIOA)
#define HW_ELEC_BP_Pin					(GPIO_PIN_3)
#define HW_ELEC_BM_GPIO_CLK_ENABLE()	__HAL_RCC_GPIOA_CLK_ENABLE()
#define HW_ELEC_BM_GPIO_Port			(GPIOA)
#define HW_ELEC_BM_Pin					(GPIO_PIN_2)
//TIM
#define HW_ELEC_APWM_GPIO_CLK_ENABLE()	__HAL_RCC_GPIOB_CLK_ENABLE()
#define HW_ELEC_APWM_GPIO_Port			(GPIOB)
#define HW_ELEC_APWM_Pin				(GPIO_PIN_11)
#define HW_ELEC_APWM_CHANNEL			(TIM_CHANNEL_4)
#define HW_ELEC_BPWM_GPIO_CLK_ENABLE()	__HAL_RCC_GPIOB_CLK_ENABLE()
#define HW_ELEC_BPWM_GPIO_Port			(GPIOB)
#define HW_ELEC_BPWM_Pin				(GPIO_PIN_10)
#define HW_ELEC_BPWM_CHANNEL			(TIM_CHANNEL_3)
#define HW_ELEC_PWM_TIM_CLK_ENABLE()	__HAL_RCC_TIM2_CLK_ENABLE()
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

void HW_Elec_Init(void); //硬件电流控制初始化
void HW_Output(float theta,uint8_t effort);	//硬件电流设置输出细分电流

#endif
