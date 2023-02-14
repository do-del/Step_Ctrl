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

#define MT6816_Encoder_Bit ((int32_t)(14)) //(������λ��)(14λ�������)
#define MT6816_Encoder_Res	((int32_t)((0x00000001U) << MT6816_Encoder_Bit)) //(�������ֱ���)(2^14 = 16384)(16k�ֱ���)(ռ��32kУ׼�ռ�)

typedef struct{
	//�ɼ�����
	uint16_t	sample_data;	//SPI��ȡ��������
	//�������
	uint16_t	angle;				//SPI����ĽǶ�
	uint8_t			no_mag_flag;	//����������Ч��־
	uint8_t			pc_flag;			//��żУ��λ
}MT6816_SPI_Signal_Typedef;

//MT6816_SPI
void MT6816_SPI_Signal_Init(void);		//MT6816_SPI�ɼ���ʼ��
void MT6816_SPI_Get_AngleData(void);	//MT6816_SPI�ɼ���ȡ�Ƕ�����

typedef struct{
	uint16_t	angle_data;				//�Ƕ�����
	uint16_t	rectify_angle;		//У׼�ĽǶ�����
	uint8_t			rectify_valid;		//У׼������Ч��־
}MT6816_Typedef;
extern MT6816_Typedef	mt6816;

//MT6816
void MT6816_Init(void);							//MT6816��ʼ��
void MT6816_Get_AngleData(void);			//MT6816��ȡ�Ƕ�����

#endif
