#ifndef DEL_HAL_H
#define DEL_HAL_H

#include "stm32f1xx_hal.h"
#include "usart.h"
#include <stdio.h>

#include "MT6816.h"

void Del_Init(void);
void Del_Update(void);

#endif
