#ifndef DEL_HAL_H
#define DEL_HAL_H

#include "stm32f1xx_hal.h"
#include "usart.h"
#include <stdio.h>
#include "tim.h"
#include "math_sin.h"
#include "hw_elec.h"
#include "MT6816.h"
#include "flash_cb.h"
#include "led.h"

void Del_Init(void);
void Del_Update(void);

#endif
