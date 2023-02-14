#ifndef DEL_SYS_H
#define DEL_SYS_H

#include "stm32f1xx_hal.h"

#define GPIO_SET_HIGH(p,i) 	(p->BSRR=i)
#define GPIO_SET_LOW(p,i)		(p->BSRR=(uint32_t)i<<16)
#define GPIO_TOGGLE(p,i) 		(p->ODR ^= i)

#define GPIO_READ_PIN(p,i)			(p->IDR & i)

#endif
