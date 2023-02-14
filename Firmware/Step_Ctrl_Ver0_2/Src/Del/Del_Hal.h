#ifndef DEL_HAL_H
#define DEL_HAL_H

#include "stm32f1xx_hal.h"
#include "usart.h"
#include "tim.h"
#include "stdio.h"
#include "Del_Sys.h"
#include "Del_Led.h"
#include "Del_Key.h"
#include "Flash_F1xx.h"
#include "MT6816.h"
#include "AT8236.h"
#include "Encoder_Cali.h"
#include "Motor_Control.h"

void Del_Init(void);
void Del_Update(void);

#endif
