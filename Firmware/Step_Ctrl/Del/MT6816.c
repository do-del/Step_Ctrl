#include "MT6816.h"
#include "spi.h"
#include <stdio.h>
#include "stdbool.h"

MT6816_SPI_Signal_Typedef mt6816_spi;
MT6816_Typedef	mt6816;

/**
  * @brief  MT6816_SPI�ɼ���ʼ��
  * @param  NULL
  * @retval NULL
**/
void MT6816_SPI_Signal_Init(void)
{
	//�ɼ�����
	mt6816_spi.sample_data = 0;
	//�������
	mt6816_spi.angle = 0;
	
}

/**
  * @brief  MT6816_SPI�ɼ���ȡ�Ƕ�����
  * @param  NULL
  * @retval NULL
**/
void MT6816_SPI_Get_AngleData(void)
{
	uint16_t data_t[2];
	uint16_t data_r[2];
	uint8_t h_count;
	
	data_t[0] = (0x80 | 0x03) << 8;
	data_t[1] = (0x80 | 0x04) << 8;
	
	for(uint8_t i=0; i<3; i++){
		//��ȡSPI����
		MT6816_SPI_CS_L();
		HAL_SPI_TransmitReceive(&hspi1, (uint8_t*)&data_t[0], (uint8_t*)&data_r[0], 1, HAL_MAX_DELAY);
		MT6816_SPI_CS_H();
		MT6816_SPI_CS_L();
		HAL_SPI_TransmitReceive(&hspi1, (uint8_t*)&data_t[1], (uint8_t*)&data_r[1], 1, HAL_MAX_DELAY);
		//MT6816_SPI_CS_H();
		mt6816_spi.sample_data = ((data_r[0] & 0x00FF) << 8) | (data_r[1] & 0x00FF);
		
		//��żУ��
		h_count = 0;
		for(uint8_t j=0; j<16; j++){
			if(mt6816_spi.sample_data & (0x0001 << j))
				h_count++;
		}
		if(h_count & 0x01){
			mt6816_spi.pc_flag = false;
		}
		else{
			mt6816_spi.pc_flag = true;
			break;
		}
	}
	
	if(mt6816_spi.pc_flag){
		mt6816_spi.angle = mt6816_spi.sample_data >> 2;
		mt6816_spi.no_mag_flag = (uint8_t)(mt6816_spi.sample_data & (0x0001 << 1));
	}
	
}

void MT6816_Init(void)
{
	MT6816_SPI_Signal_Init();		//��ʼ��SPI�ӿ�
	//��ʼ���׶λ�ȡһ�νǶ�����(���˴�������)(��δ������λ���һ�ζ�ȡ���ݶ�ʧ��ԭ��)
	MT6816_Get_AngleData();
	
}

/**
  * @brief  MT6816��ȡ�Ƕ�����
  * @param  NULL
  * @retval NULL
**/
void MT6816_Get_AngleData(void)
{
	MT6816_SPI_Get_AngleData();
	mt6816.angle_data = mt6816_spi.angle;
	mt6816.state = mt6816_spi.no_mag_flag ? false : true;
	mt6816.pc_state = mt6816_spi.pc_flag;
}
