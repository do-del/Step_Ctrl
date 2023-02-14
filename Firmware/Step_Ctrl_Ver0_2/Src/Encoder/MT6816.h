#ifndef MT6816_H
#define MT6816_H

#include "stm32f1xx_hal.h"
#include "Flash_F1xx.h"
#include "Del_Sys.h"
#include "spi.h"

#define MT6816_CS_PORT GPIOB
#define MT6816_CS_PIN GPIO_PIN_12

#define MT6816_SPI_CS_H() GPIO_SET_HIGH(GPIOB,GPIO_PIN_12)
#define MT6816_SPI_CS_L() GPIO_SET_LOW(GPIOB,GPIO_PIN_12)

#define MT6816_SPI hspi1

#define MT6816_Encoder_Bit ((int32_t)(14)) //(编码器位宽)(14位输出精度)
#define MT6816_Encoder_Res	((int32_t)((0x00000001U) << MT6816_Encoder_Bit)) //(编码器分辨率)(2^14 = 16384)(16k分辨率)(占用32k校准空间)

typedef struct{
	//采集数据
	uint16_t	sample_data;	//SPI读取到的数据
	//输出数据
	uint16_t	angle;				//SPI输出的角度
	uint8_t			no_mag_flag;	//磁铁数据有效标志
	uint8_t			pc_flag;			//奇偶校验位
}MT6816_SPI_Signal_Typedef;

//MT6816_SPI
void MT6816_SPI_Signal_Init(void);		//MT6816_SPI采集初始化
void MT6816_SPI_Get_AngleData(void);	//MT6816_SPI采集获取角度数据

typedef struct{
	uint16_t	angle_data;				//角度数据
	uint16_t	rectify_angle;		//校准的角度数据
	uint8_t			rectify_valid;		//校准数据有效标志
}MT6816_Typedef;
extern MT6816_Typedef	mt6816;

//MT6816
void MT6816_Init(void);							//MT6816初始化
void MT6816_Get_AngleData(void);			//MT6816获取角度数据

#endif
