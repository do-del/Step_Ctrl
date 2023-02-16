#ifndef DEL_CAN_H
#define DEL_CAN_H

#include "stm32f1xx_hal.h"
#include "can.h"

void CANFilter_Config(void);
void CAN_Send_Test(void);
void CAN_Start_Init(void);

#endif
