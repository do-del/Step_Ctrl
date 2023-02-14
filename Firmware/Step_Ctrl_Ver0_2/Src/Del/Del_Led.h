#ifndef DEL_LED_H
#define DEL_LED_H

#include "stm32f1xx_hal.h"
#include "Del_Sys.h"

#define Green_Led_Port GPIOC
#define Green_Led_Pin	GPIO_PIN_14
#define Red_Led_Port GPIOC
#define Red_Led_Pin GPIO_PIN_15

#define Green_Led_On() GPIO_SET_LOW(Green_Led_Port,Green_Led_Pin)
#define Green_Led_Off() GPIO_SET_HIGH(Green_Led_Port,Green_Led_Pin)
#define Green_Led_Toggle() GPIO_TOGGLE(Green_Led_Port,Green_Led_Pin)
#define Red_Led_On() GPIO_SET_LOW(Red_Led_Port,Red_Led_Pin)
#define Red_Led_Off() GPIO_SET_HIGH(Red_Led_Port,Red_Led_Pin)
#define Red_Led_Toggle() GPIO_TOGGLE(Red_Led_Port,Red_Led_Pin)

void Led_Init(void);

#endif
