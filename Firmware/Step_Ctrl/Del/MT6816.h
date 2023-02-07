#ifndef __MT6816_H
#define __MT6816_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"

#define CALI_Encode_Bit ((int32_t)(14)) //(编码器位宽)(14位输出精度)
#define CALI_Encode_Res	((int32_t)((0x00000001U) << CALI_Encode_Bit)) //(编码器分辨率)(2^14 = 16384)(16k分辨率)(占用32k校准空间)
#define CALI_Gather_Encode_Res	((int32_t)(CALI_Encode_Res / Move_Step_NUM))	//(校准每采集步编码器分辨率)

#define MT6816_SPI_CS_H() (GPIOB -> BSRR = GPIO_PIN_12)
#define MT6816_SPI_CS_L() (GPIOB -> BRR  = GPIO_PIN_12)

typedef struct{
	uint16_t sample_data;
	uint16_t angle;
	uint8_t no_mag_flag;
	uint8_t pc_flag;
}MT6816_SPI_Signal_Typedef;

typedef struct{
	uint16_t angle_data;
	uint16_t rectify_angle;
	uint8_t rectify_valid;
	uint8_t state;
	uint8_t pc_state;
}MT6816_Typedef;

extern MT6816_Typedef mt6816;

void MT6816_SPI_Signal_Init(void);
void MT6816_SPI_Get_Angle(void);
void MT6816_Init(void);
void MT6816_Get_AngleData(void);

#ifdef _cplusplus
}
#endif

#endif
