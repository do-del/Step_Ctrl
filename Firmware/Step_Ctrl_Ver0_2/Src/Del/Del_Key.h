#ifndef DEL_KEY_H
#define DEL_KEY_H

#include "stm32f1xx_hal.h"
#include "Del_Sys.h"

#define KEY_A_PORT GPIOA
#define KEY_A_PIN GPIO_PIN_15
#define KEY_B_PORT GPIOB
#define KEY_B_PIN GPIO_PIN_2

#define Key_A_IsPress (!(GPIO_READ_PIN(KEY_A_PORT,KEY_A_PIN)))
#define Key_B_IsPress (!(GPIO_READ_PIN(KEY_B_PORT,KEY_B_PIN)))

#endif
